#include <Az/Az.hpp>
#ifndef AZ_DISABLE
#include "az_impl_dx11.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <stdio.h>
#ifdef _MSC_VER
#pragma comment(lib, "d3dcompiler")
#endif

struct Az_ImplDX11_Texture
{
    ID3D11Texture2D* pTexture;
    ID3D11ShaderResourceView* pTextureView;
};

struct Az_ImplDX11_Data
{
    ID3D11Device* pd3dDevice;
    ID3D11DeviceContext* pd3dDeviceContext;
    IDXGIFactory* pFactory;
    ID3D11Buffer* pVB;
    ID3D11Buffer* pIB;
    ID3D11VertexShader* pVertexShader;
    ID3D11InputLayout* pInputLayout;
    ID3D11Buffer* pVertexConstantBuffer;
    ID3D11PixelShader* pPixelShader;
    ID3D11SamplerState* pFontSampler;
    ID3D11RasterizerState* pRasterizerState;
    ID3D11BlendState* pBlendState;
    ID3D11DepthStencilState* pDepthStencilState;
    int VertexBufferSize;
    int IndexBufferSize;
    AzVector<DXGI_SWAP_CHAIN_DESC> SwapChainDescsForViewports;

    Az_ImplDX11_Data()
    {
        memset((void*)this, 0, sizeof(*this));
        VertexBufferSize = 5000;
        IndexBufferSize = 10000;
    }
};

struct VERTEX_CONSTANT_BUFFER_DX11
{
    float mvp[4][4];
};

static Az_ImplDX11_Data* Az_ImplDX11_GetBackendData()
{
    return Az::GetCurrentContext()
                   ? (Az_ImplDX11_Data*)Az::GetIO().BackendRendererUserData
                   : nullptr;
}

static void Az_ImplDX11_InitMultiViewportSupport();
static void Az_ImplDX11_ShutdownMultiViewportSupport();

static void Az_ImplDX11_SetupRenderState(AzDrawData* draw_data,
                                         ID3D11DeviceContext* device_ctx)
{
    Az_ImplDX11_Data* bd = Az_ImplDX11_GetBackendData();

    D3D11_VIEWPORT vp = {};
    vp.Width = draw_data->DisplaySize.x * draw_data->FramebufferScale.x;
    vp.Height = draw_data->DisplaySize.y * draw_data->FramebufferScale.y;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = vp.TopLeftY = 0;
    device_ctx->RSSetViewports(1, &vp);

    D3D11_MAPPED_SUBRESOURCE mapped_resource;
    if (device_ctx->Map(bd->pVertexConstantBuffer,
                        0,
                        D3D11_MAP_WRITE_DISCARD,
                        0,
                        &mapped_resource)
        == S_OK)
    {
        VERTEX_CONSTANT_BUFFER_DX11* constant_buffer =
                (VERTEX_CONSTANT_BUFFER_DX11*)mapped_resource.pData;
        float L = draw_data->DisplayPos.x;
        float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
        float T = draw_data->DisplayPos.y;
        float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
        float mvp[4][4] = {
                {2.0f / (R - L), 0.0f, 0.0f, 0.0f},
                {0.0f, 2.0f / (T - B), 0.0f, 0.0f},
                {0.0f, 0.0f, 0.5f, 0.0f},
                {(R + L) / (L - R), (T + B) / (B - T), 0.5f, 1.0f},
        };
        memcpy(&constant_buffer->mvp, mvp, sizeof(mvp));
        device_ctx->Unmap(bd->pVertexConstantBuffer, 0);
    }

    unsigned int stride = sizeof(AzDrawVert);
    unsigned int offset = 0;
    device_ctx->IASetInputLayout(bd->pInputLayout);
    device_ctx->IASetVertexBuffers(0, 1, &bd->pVB, &stride, &offset);
    device_ctx->IASetIndexBuffer(bd->pIB,
                                 sizeof(AzDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT
                                                        : DXGI_FORMAT_R32_UINT,
                                 0);
    device_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    device_ctx->VSSetShader(bd->pVertexShader, nullptr, 0);
    device_ctx->VSSetConstantBuffers(0, 1, &bd->pVertexConstantBuffer);
    device_ctx->PSSetShader(bd->pPixelShader, nullptr, 0);
    device_ctx->PSSetSamplers(0, 1, &bd->pFontSampler);
    device_ctx->GSSetShader(nullptr, nullptr, 0);
    device_ctx->HSSetShader(nullptr, nullptr, 0);
    device_ctx->DSSetShader(nullptr, nullptr, 0);
    device_ctx->CSSetShader(nullptr, nullptr, 0);

    const float blend_factor[4] = {0.f, 0.f, 0.f, 0.f};
    device_ctx->OMSetBlendState(bd->pBlendState, blend_factor, 0xffffffff);
    device_ctx->OMSetDepthStencilState(bd->pDepthStencilState, 0);
    device_ctx->RSSetState(bd->pRasterizerState);
}

void Az_ImplDX11_RenderDrawData(AzDrawData* draw_data)
{
    if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f)
        return;

    Az_ImplDX11_Data* bd = Az_ImplDX11_GetBackendData();
    ID3D11DeviceContext* device = bd->pd3dDeviceContext;

    if (draw_data->Textures != nullptr)
        for (AzTextureData* tex : *draw_data->Textures)
            if (tex->Status != AzTextureStatus_OK)
                Az_ImplDX11_UpdateTexture(tex);

    if (!bd->pVB || bd->VertexBufferSize < draw_data->TotalVtxCount)
    {
        if (bd->pVB)
        {
            bd->pVB->Release();
            bd->pVB = nullptr;
        }
        bd->VertexBufferSize = draw_data->TotalVtxCount + 5000;
        D3D11_BUFFER_DESC desc = {};
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = bd->VertexBufferSize * sizeof(AzDrawVert);
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        if (bd->pd3dDevice->CreateBuffer(&desc, nullptr, &bd->pVB) < 0)
            return;
    }
    if (!bd->pIB || bd->IndexBufferSize < draw_data->TotalIdxCount)
    {
        if (bd->pIB)
        {
            bd->pIB->Release();
            bd->pIB = nullptr;
        }
        bd->IndexBufferSize = draw_data->TotalIdxCount + 10000;
        D3D11_BUFFER_DESC desc = {};
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = bd->IndexBufferSize * sizeof(AzDrawIdx);
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        if (bd->pd3dDevice->CreateBuffer(&desc, nullptr, &bd->pIB) < 0)
            return;
    }

    D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
    if (device->Map(bd->pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource)
        != S_OK)
        return;
    if (device->Map(bd->pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource)
        != S_OK)
        return;
    AzDrawVert* vtx_dst = (AzDrawVert*)vtx_resource.pData;
    AzDrawIdx* idx_dst = (AzDrawIdx*)idx_resource.pData;
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const AzDrawList* draw_list = draw_data->CmdLists[n];
        memcpy(vtx_dst,
               draw_list->VtxBuffer.Data,
               draw_list->VtxBuffer.Size * sizeof(AzDrawVert));
        memcpy(idx_dst,
               draw_list->IdxBuffer.Data,
               draw_list->IdxBuffer.Size * sizeof(AzDrawIdx));
        vtx_dst += draw_list->VtxBuffer.Size;
        idx_dst += draw_list->IdxBuffer.Size;
    }
    device->Unmap(bd->pVB, 0);
    device->Unmap(bd->pIB, 0);

    struct BACKUP_DX11_STATE
    {
        UINT ScissorRectsCount, ViewportsCount;
        D3D11_RECT ScissorRects
                [D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
        D3D11_VIEWPORT Viewports
                [D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
        ID3D11RasterizerState* RS;
        ID3D11BlendState* BlendState;
        FLOAT BlendFactor[4];
        UINT SampleMask;
        UINT StencilRef;
        ID3D11DepthStencilState* DepthStencilState;
        ID3D11ShaderResourceView* PSShaderResource;
        ID3D11SamplerState* PSSampler;
        ID3D11PixelShader* PS;
        ID3D11VertexShader* VS;
        ID3D11GeometryShader* GS;
        UINT PSInstancesCount, VSInstancesCount, GSInstancesCount;
        ID3D11ClassInstance *PSInstances[256], *VSInstances[256],
                *GSInstances[256];
        D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology;
        ID3D11Buffer *IndexBuffer, *VertexBuffer, *VSConstantBuffer;
        UINT IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
        DXGI_FORMAT IndexBufferFormat;
        ID3D11InputLayout* InputLayout;
    };
    BACKUP_DX11_STATE old = {};
    old.ScissorRectsCount = old.ViewportsCount =
            D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    device->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
    device->RSGetViewports(&old.ViewportsCount, old.Viewports);
    device->RSGetState(&old.RS);
    device->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
    device->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
    device->PSGetShaderResources(0, 1, &old.PSShaderResource);
    device->PSGetSamplers(0, 1, &old.PSSampler);
    old.PSInstancesCount = old.VSInstancesCount = old.GSInstancesCount = 256;
    device->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
    device->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
    device->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
    device->GSGetShader(&old.GS, old.GSInstances, &old.GSInstancesCount);

    device->IAGetPrimitiveTopology(&old.PrimitiveTopology);
    device->IAGetIndexBuffer(&old.IndexBuffer,
                             &old.IndexBufferFormat,
                             &old.IndexBufferOffset);
    device->IAGetVertexBuffers(0,
                               1,
                               &old.VertexBuffer,
                               &old.VertexBufferStride,
                               &old.VertexBufferOffset);
    device->IAGetInputLayout(&old.InputLayout);

    Az_ImplDX11_SetupRenderState(draw_data, device);

    AzPlatformIO& platform_io = Az::GetPlatformIO();
    Az_AzplDX11_RenderState render_state;
    render_state.Device = bd->pd3dDevice;
    render_state.DeviceContext = bd->pd3dDeviceContext;
    render_state.SamplerDefault = bd->pFontSampler;
    render_state.VertexConstantBuffer = bd->pVertexConstantBuffer;
    platform_io.Renderer_RenderState = &render_state;

    int global_idx_offset = 0;
    int global_vtx_offset = 0;
    AzVec2 clip_off = draw_data->DisplayPos;
    AzVec2 clip_scale = draw_data->FramebufferScale;
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const AzDrawList* draw_list = draw_data->CmdLists[n];
        for (int cmd_i = 0; cmd_i < draw_list->CmdBuffer.Size; cmd_i++)
        {
            const AzDrawCmd* pcmd = &draw_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != nullptr)
            {
                if (pcmd->UserCallback == AzDrawCallback_ResetRenderState)
                    Az_ImplDX11_SetupRenderState(draw_data, device);
                else
                    pcmd->UserCallback(draw_list, pcmd);
            } else
            {
                AzVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x,
                                (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                AzVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x,
                                (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    continue;

                const D3D11_RECT r = {(LONG)clip_min.x,
                                      (LONG)clip_min.y,
                                      (LONG)clip_max.x,
                                      (LONG)clip_max.y};
                device->RSSetScissorRects(1, &r);

                ID3D11ShaderResourceView* texture_srv =
                        (ID3D11ShaderResourceView*)pcmd->GetTexID();
                device->PSSetShaderResources(0, 1, &texture_srv);
                device->DrawIndexed(pcmd->ElemCount,
                                    pcmd->IdxOffset + global_idx_offset,
                                    pcmd->VtxOffset + global_vtx_offset);
            }
        }
        global_idx_offset += draw_list->IdxBuffer.Size;
        global_vtx_offset += draw_list->VtxBuffer.Size;
    }
    platform_io.Renderer_RenderState = nullptr;

    device->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
    device->RSSetViewports(old.ViewportsCount, old.Viewports);
    device->RSSetState(old.RS);
    if (old.RS)
        old.RS->Release();
    device->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask);
    if (old.BlendState)
        old.BlendState->Release();
    device->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef);
    if (old.DepthStencilState)
        old.DepthStencilState->Release();
    device->PSSetShaderResources(0, 1, &old.PSShaderResource);
    if (old.PSShaderResource)
        old.PSShaderResource->Release();
    device->PSSetSamplers(0, 1, &old.PSSampler);
    if (old.PSSampler)
        old.PSSampler->Release();
    device->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount);
    if (old.PS)
        old.PS->Release();
    for (UINT i = 0; i < old.PSInstancesCount; i++)
        if (old.PSInstances[i])
            old.PSInstances[i]->Release();
    device->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount);
    if (old.VS)
        old.VS->Release();
    device->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer);
    if (old.VSConstantBuffer)
        old.VSConstantBuffer->Release();
    device->GSSetShader(old.GS, old.GSInstances, old.GSInstancesCount);
    if (old.GS)
        old.GS->Release();
    for (UINT i = 0; i < old.VSInstancesCount; i++)
        if (old.VSInstances[i])
            old.VSInstances[i]->Release();
    device->IASetPrimitiveTopology(old.PrimitiveTopology);
    device->IASetIndexBuffer(old.IndexBuffer,
                             old.IndexBufferFormat,
                             old.IndexBufferOffset);
    if (old.IndexBuffer)
        old.IndexBuffer->Release();
    device->IASetVertexBuffers(0,
                               1,
                               &old.VertexBuffer,
                               &old.VertexBufferStride,
                               &old.VertexBufferOffset);
    if (old.VertexBuffer)
        old.VertexBuffer->Release();
    device->IASetInputLayout(old.InputLayout);
    if (old.InputLayout)
        old.InputLayout->Release();
}

static void Az_ImplDX11_DestroyTexture(AzTextureData* tex)
{
    Az_ImplDX11_Texture* backend_tex = (Az_ImplDX11_Texture*)
                                               tex->BackendUserData;
    if (backend_tex == nullptr)
        return;
    AZ_ASSERT(backend_tex->pTextureView
              == (ID3D11ShaderResourceView*)(intptr_t)tex->TexID);
    backend_tex->pTextureView->Release();
    backend_tex->pTexture->Release();
    AZ_DELETE(backend_tex);

    tex->SetTexID(AzTextureID_Invalid);
    tex->SetStatus(AzTextureStatus_Destroyed);
    tex->BackendUserData = nullptr;
}

void Az_ImplDX11_UpdateTexture(AzTextureData* tex)
{
    Az_ImplDX11_Data* bd = Az_ImplDX11_GetBackendData();
    if (tex->Status == AzTextureStatus_WantCreate)
    {
        AZ_ASSERT(tex->TexID == AzTextureID_Invalid
                  && tex->BackendUserData == nullptr);
        AZ_ASSERT(tex->Format == AzTextureFormat_RGBA32);
        unsigned int* pixels = (unsigned int*)tex->GetPixels();
        Az_ImplDX11_Texture* backend_tex = AZ_NEW(Az_ImplDX11_Texture)();

        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = (UINT)tex->Width;
        desc.Height = (UINT)tex->Height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = pixels;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        bd->pd3dDevice->CreateTexture2D(&desc,
                                        &subResource,
                                        &backend_tex->pTexture);
        AZ_ASSERT(backend_tex->pTexture != nullptr
                  && "Backend failed to create texture!");

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = desc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        bd->pd3dDevice->CreateShaderResourceView(backend_tex->pTexture,
                                                 &srvDesc,
                                                 &backend_tex->pTextureView);
        AZ_ASSERT(backend_tex->pTextureView != nullptr
                  && "Backend failed to create texture!");

        tex->SetTexID((AzTextureID)(intptr_t)backend_tex->pTextureView);
        tex->SetStatus(AzTextureStatus_OK);
        tex->BackendUserData = backend_tex;
    } else if (tex->Status == AzTextureStatus_WantUpdates)
    {
        Az_ImplDX11_Texture* backend_tex = (Az_ImplDX11_Texture*)
                                                   tex->BackendUserData;
        AZ_ASSERT(backend_tex->pTextureView
                  == (ID3D11ShaderResourceView*)(intptr_t)tex->TexID);
        for (AzTextureRect& r : tex->Updates)
        {
            D3D11_BOX box = {(UINT)r.x,
                             (UINT)r.y,
                             (UINT)0,
                             (UINT)(r.x + r.w),
                             (UINT)(r.y + r.h),
                             (UINT)1};
            bd->pd3dDeviceContext->UpdateSubresource(backend_tex->pTexture,
                                                     0,
                                                     &box,
                                                     tex->GetPixelsAt(r.x, r.y),
                                                     (UINT)tex->GetPitch(),
                                                     0);
        }
        tex->SetStatus(AzTextureStatus_OK);
    }
    if (tex->Status == AzTextureStatus_WantDestroy && tex->UnusedFrames > 0)
        Az_ImplDX11_DestroyTexture(tex);
}

bool Az_ImplDX11_CreateDeviceObjects()
{
    Az_ImplDX11_Data* bd = Az_ImplDX11_GetBackendData();
    if (!bd->pd3dDevice)
        return false;
    Az_ImplDX11_InvalidateDeviceObjects();

    {
        static const char* vertexShader =
                "cbuffer vertexBuffer : register(b0) \
            {\
              float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
              float2 pos : POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
              PS_INPUT output;\
              output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
              output.col = input.col;\
              output.uv  = input.uv;\
              return output;\
            }";

        ID3DBlob* vertexShaderBlob;
        if (FAILED(D3DCompile(vertexShader,
                              strlen(vertexShader),
                              nullptr,
                              nullptr,
                              nullptr,
                              "main",
                              "vs_4_0",
                              0,
                              0,
                              &vertexShaderBlob,
                              nullptr)))
            return false;
        if (bd->pd3dDevice->CreateVertexShader(
                    vertexShaderBlob->GetBufferPointer(),
                    vertexShaderBlob->GetBufferSize(),
                    nullptr,
                    &bd->pVertexShader)
            != S_OK)
        {
            vertexShaderBlob->Release();
            return false;
        }

        D3D11_INPUT_ELEMENT_DESC local_layout[] = {
                {"POSITION",
                 0,
                 DXGI_FORMAT_R32G32_FLOAT,
                 0,
                 (UINT)offsetof(AzDrawVert, pos),
                 D3D11_INPUT_PER_VERTEX_DATA,
                 0},
                {"TEXCOORD",
                 0,
                 DXGI_FORMAT_R32G32_FLOAT,
                 0,
                 (UINT)offsetof(AzDrawVert, uv),
                 D3D11_INPUT_PER_VERTEX_DATA,
                 0},
                {"COLOR",
                 0,
                 DXGI_FORMAT_R8G8B8A8_UNORM,
                 0,
                 (UINT)offsetof(AzDrawVert, col),
                 D3D11_INPUT_PER_VERTEX_DATA,
                 0},
        };
        if (bd->pd3dDevice->CreateInputLayout(
                    local_layout,
                    3,
                    vertexShaderBlob->GetBufferPointer(),
                    vertexShaderBlob->GetBufferSize(),
                    &bd->pInputLayout)
            != S_OK)
        {
            vertexShaderBlob->Release();
            return false;
        }
        vertexShaderBlob->Release();

        {
            D3D11_BUFFER_DESC desc = {};
            desc.ByteWidth = sizeof(VERTEX_CONSTANT_BUFFER_DX11);
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            bd->pd3dDevice->CreateBuffer(&desc,
                                         nullptr,
                                         &bd->pVertexConstantBuffer);
        }
    }

    {
        static const char* pixelShader =
                "struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            sampler sampler0;\
            Texture2D texture0;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
            float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
            return out_col; \
            }";

        ID3DBlob* pixelShaderBlob;
        if (FAILED(D3DCompile(pixelShader,
                              strlen(pixelShader),
                              nullptr,
                              nullptr,
                              nullptr,
                              "main",
                              "ps_4_0",
                              0,
                              0,
                              &pixelShaderBlob,
                              nullptr)))
            return false;
        if (bd->pd3dDevice->CreatePixelShader(
                    pixelShaderBlob->GetBufferPointer(),
                    pixelShaderBlob->GetBufferSize(),
                    nullptr,
                    &bd->pPixelShader)
            != S_OK)
        {
            pixelShaderBlob->Release();
            return false;
        }
        pixelShaderBlob->Release();
    }

    {
        D3D11_BLEND_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.AlphaToCoverageEnable = false;
        desc.RenderTarget[0].BlendEnable = true;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].RenderTargetWriteMask =
                D3D11_COLOR_WRITE_ENABLE_ALL;
        bd->pd3dDevice->CreateBlendState(&desc, &bd->pBlendState);
    }

    {
        D3D11_RASTERIZER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.FillMode = D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_NONE;
        desc.ScissorEnable = true;
        desc.DepthClipEnable = true;
        bd->pd3dDevice->CreateRasterizerState(&desc, &bd->pRasterizerState);
    }

    {
        D3D11_DEPTH_STENCIL_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.DepthEnable = false;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
        desc.StencilEnable = false;
        desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp =
                desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        desc.BackFace = desc.FrontFace;
        bd->pd3dDevice->CreateDepthStencilState(&desc, &bd->pDepthStencilState);
    }

    {
        D3D11_SAMPLER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.MipLODBias = 0.f;
        desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        desc.MinLOD = 0.f;
        desc.MaxLOD = 0.f;
        bd->pd3dDevice->CreateSamplerState(&desc, &bd->pFontSampler);
    }

    return true;
}

void Az_ImplDX11_InvalidateDeviceObjects()
{
    Az_ImplDX11_Data* bd = Az_ImplDX11_GetBackendData();
    if (!bd->pd3dDevice)
        return;

    for (AzTextureData* tex : Az::GetPlatformIO().Textures)
        if (tex->RefCount == 1)
            Az_ImplDX11_DestroyTexture(tex);

    if (bd->pFontSampler)
    {
        bd->pFontSampler->Release();
        bd->pFontSampler = nullptr;
    }
    if (bd->pIB)
    {
        bd->pIB->Release();
        bd->pIB = nullptr;
    }
    if (bd->pVB)
    {
        bd->pVB->Release();
        bd->pVB = nullptr;
    }
    if (bd->pBlendState)
    {
        bd->pBlendState->Release();
        bd->pBlendState = nullptr;
    }
    if (bd->pDepthStencilState)
    {
        bd->pDepthStencilState->Release();
        bd->pDepthStencilState = nullptr;
    }
    if (bd->pRasterizerState)
    {
        bd->pRasterizerState->Release();
        bd->pRasterizerState = nullptr;
    }
    if (bd->pPixelShader)
    {
        bd->pPixelShader->Release();
        bd->pPixelShader = nullptr;
    }
    if (bd->pVertexConstantBuffer)
    {
        bd->pVertexConstantBuffer->Release();
        bd->pVertexConstantBuffer = nullptr;
    }
    if (bd->pInputLayout)
    {
        bd->pInputLayout->Release();
        bd->pInputLayout = nullptr;
    }
    if (bd->pVertexShader)
    {
        bd->pVertexShader->Release();
        bd->pVertexShader = nullptr;
    }
}

bool Az_ImplDX11_Init(ID3D11Device* device, ID3D11DeviceContext* device_context)
{
    AzIO& io = Az::GetIO();
    AZ_CHECKVERSION();
    AZ_ASSERT(io.BackendRendererUserData == nullptr
              && "Already initialized a renderer backend!");

    Az_ImplDX11_Data* bd = AZ_NEW(Az_ImplDX11_Data)();
    io.BackendRendererUserData = (void*)bd;
    io.BackendRendererName = "imgui_impl_dx11";
    io.BackendFlags |= AzBackendFlags_RendererHasVtxOffset;
    io.BackendFlags |= AzBackendFlags_RendererHasTextures;
    io.BackendFlags |= AzBackendFlags_RendererHasViewports;

    AzPlatformIO& platform_io = Az::GetPlatformIO();
    platform_io.Renderer_TextureMaxWidth =
            platform_io.Renderer_TextureMaxHeight =
                    D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;

    IDXGIDevice* pDXGIDevice = nullptr;
    IDXGIAdapter* pDXGIAdapter = nullptr;
    IDXGIFactory* pFactory = nullptr;

    if (device->QueryInterface(IID_PPV_ARGS(&pDXGIDevice)) == S_OK)
        if (pDXGIDevice->GetParent(IID_PPV_ARGS(&pDXGIAdapter)) == S_OK)
            if (pDXGIAdapter->GetParent(IID_PPV_ARGS(&pFactory)) == S_OK)
            {
                bd->pd3dDevice = device;
                bd->pd3dDeviceContext = device_context;
                bd->pFactory = pFactory;
            }
    if (pDXGIDevice)
        pDXGIDevice->Release();
    if (pDXGIAdapter)
        pDXGIAdapter->Release();
    bd->pd3dDevice->AddRef();
    bd->pd3dDeviceContext->AddRef();

    Az_ImplDX11_InitMultiViewportSupport();

    return true;
}

void Az_ImplDX11_Shutdown()
{
    Az_ImplDX11_Data* bd = Az_ImplDX11_GetBackendData();
    AZ_ASSERT(bd != nullptr
              && "No renderer backend to shutdown, or already shutdown?");
    AzIO& io = Az::GetIO();

    Az_ImplDX11_ShutdownMultiViewportSupport();
    Az_ImplDX11_InvalidateDeviceObjects();
    if (bd->pFactory)
    {
        bd->pFactory->Release();
    }
    if (bd->pd3dDevice)
    {
        bd->pd3dDevice->Release();
    }
    if (bd->pd3dDeviceContext)
    {
        bd->pd3dDeviceContext->Release();
    }
    io.BackendRendererName = nullptr;
    io.BackendRendererUserData = nullptr;
    io.BackendFlags &= ~(AzBackendFlags_RendererHasVtxOffset
                         | AzBackendFlags_RendererHasTextures
                         | AzBackendFlags_RendererHasViewports);
    AZ_DELETE(bd);
}

void Az_ImplDX11_NewFrame()
{
    Az_ImplDX11_Data* bd = Az_ImplDX11_GetBackendData();
    AZ_ASSERT(bd != nullptr && "Context or backend not initialized! Did you call Az_AzplDX11_Init()?");

    if (!bd->pVertexShader)
        Az_ImplDX11_CreateDeviceObjects();
}

struct Az_ImplDX11_ViewportData
{
    IDXGISwapChain* SwapChain;
    ID3D11RenderTargetView* RTView;

    Az_ImplDX11_ViewportData()
    {
        SwapChain = nullptr;
        RTView = nullptr;
    }
    ~Az_ImplDX11_ViewportData()
    {
        AZ_ASSERT(SwapChain == nullptr && RTView == nullptr);
    }
};

void Az_ImplDX11_SetSwapChainDescs(const DXGI_SWAP_CHAIN_DESC* desc_templates,
                                   int desc_templates_count);
void Az_ImplDX11_SetSwapChainDescs(const DXGI_SWAP_CHAIN_DESC* desc_templates,
                                   int desc_templates_count)
{
    Az_ImplDX11_Data* bd = Az_ImplDX11_GetBackendData();
    bd->SwapChainDescsForViewports.resize(desc_templates_count);
    memcpy(bd->SwapChainDescsForViewports.Data,
           desc_templates,
           sizeof(DXGI_SWAP_CHAIN_DESC));
}

static void Az_ImplDX11_CreateWindow(AzViewport* viewport)
{
    Az_ImplDX11_Data* bd = Az_ImplDX11_GetBackendData();
    Az_ImplDX11_ViewportData* vd = AZ_NEW(Az_ImplDX11_ViewportData)();
    viewport->RendererUserData = vd;

    HWND hwnd = viewport->PlatformHandleRaw ? (HWND)viewport->PlatformHandleRaw
                                            : (HWND)viewport->PlatformHandle;
    AZ_ASSERT(hwnd != 0);
    AZ_ASSERT(vd->SwapChain == nullptr && vd->RTView == nullptr);

    HRESULT hr = DXGI_ERROR_UNSUPPORTED;
    for (const DXGI_SWAP_CHAIN_DESC& sd_template :
         bd->SwapChainDescsForViewports)
    {
        AZ_ASSERT(sd_template.BufferDesc.Width == 0
                  && sd_template.BufferDesc.Height == 0
                  && sd_template.OutputWindow == nullptr);
        DXGI_SWAP_CHAIN_DESC sd = sd_template;
        sd.BufferDesc.Width = (UINT)viewport->Size.x;
        sd.BufferDesc.Height = (UINT)viewport->Size.y;
        sd.OutputWindow = hwnd;
        hr = bd->pFactory->CreateSwapChain(bd->pd3dDevice, &sd, &vd->SwapChain);
        if (SUCCEEDED(hr))
            break;
    }
    AZ_ASSERT(SUCCEEDED(hr));

    if (vd->SwapChain != nullptr)
    {
        ID3D11Texture2D* pBackBuffer;
        vd->SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        bd->pd3dDevice->CreateRenderTargetView(pBackBuffer,
                                               nullptr,
                                               &vd->RTView);
        pBackBuffer->Release();
    }
}

static void Az_ImplDX11_DestroyWindow(AzViewport* viewport)
{
    if (Az_ImplDX11_ViewportData* vd = (Az_ImplDX11_ViewportData*)
                                               viewport->RendererUserData)
    {
        if (vd->SwapChain)
            vd->SwapChain->Release();
        vd->SwapChain = nullptr;
        if (vd->RTView)
            vd->RTView->Release();
        vd->RTView = nullptr;
        AZ_DELETE(vd);
    }
    viewport->RendererUserData = nullptr;
}

static void Az_ImplDX11_SetWindowSize(AzViewport* viewport, AzVec2 size)
{
    Az_ImplDX11_Data* bd = Az_ImplDX11_GetBackendData();
    Az_ImplDX11_ViewportData* vd = (Az_ImplDX11_ViewportData*)
                                           viewport->RendererUserData;
    if (vd->RTView)
    {
        vd->RTView->Release();
        vd->RTView = nullptr;
    }
    if (vd->SwapChain)
    {
        ID3D11Texture2D* pBackBuffer = nullptr;
        vd->SwapChain->ResizeBuffers(0,
                                     (UINT)size.x,
                                     (UINT)size.y,
                                     DXGI_FORMAT_UNKNOWN,
                                     0);
        vd->SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        if (pBackBuffer == nullptr)
        {
            fprintf(stderr,
                    "Az_AzplDX11_SetWindowSize() failed creating buffers.\n");
            return;
        }
        bd->pd3dDevice->CreateRenderTargetView(pBackBuffer,
                                               nullptr,
                                               &vd->RTView);
        pBackBuffer->Release();
    }
}

static void Az_ImplDX11_RenderWindow(AzViewport* viewport, void*)
{
    Az_ImplDX11_Data* bd = Az_ImplDX11_GetBackendData();
    Az_ImplDX11_ViewportData* vd = (Az_ImplDX11_ViewportData*)
                                           viewport->RendererUserData;
    AzVec4 clear_color = AzVec4(0.0f, 0.0f, 0.0f, 1.0f);
    bd->pd3dDeviceContext->OMSetRenderTargets(1, &vd->RTView, nullptr);
    if (!(viewport->Flags & AzViewportFlags_NoRendererClear))
        bd->pd3dDeviceContext->ClearRenderTargetView(vd->RTView,
                                                     (float*)&clear_color);
    Az_ImplDX11_RenderDrawData(viewport->DrawData);
}

static void Az_ImplDX11_SwapBuffers(AzViewport* viewport, void*)
{
    Az_ImplDX11_ViewportData* vd = (Az_ImplDX11_ViewportData*)
                                           viewport->RendererUserData;
    vd->SwapChain->Present(0, 0);
}

static void Az_ImplDX11_InitMultiViewportSupport()
{
    AzPlatformIO& platform_io = Az::GetPlatformIO();
    platform_io.Renderer_CreateWindow = Az_ImplDX11_CreateWindow;
    platform_io.Renderer_DestroyWindow = Az_ImplDX11_DestroyWindow;
    platform_io.Renderer_SetWindowSize = Az_ImplDX11_SetWindowSize;
    platform_io.Renderer_RenderWindow = Az_ImplDX11_RenderWindow;
    platform_io.Renderer_SwapBuffers = Az_ImplDX11_SwapBuffers;

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;
    Az_ImplDX11_SetSwapChainDescs(&sd, 1);
}

static void Az_ImplDX11_ShutdownMultiViewportSupport()
{
    Az::DestroyPlatformWindows();
}
#endif
