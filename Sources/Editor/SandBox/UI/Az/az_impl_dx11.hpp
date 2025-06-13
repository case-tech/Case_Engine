// Copyright (c) 2025 Case Technologies

#pragma once
#include <Az/Az.hpp>      
#ifndef AZ_DISABLE

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11Buffer;

AZ_IMPL_API bool     Az_ImplDX11_Init(ID3D11Device* device, ID3D11DeviceContext* device_context);
AZ_IMPL_API void     Az_ImplDX11_Shutdown();
AZ_IMPL_API void     Az_ImplDX11_NewFrame();
AZ_IMPL_API void     Az_ImplDX11_RenderDrawData(AzDrawData* draw_data);

AZ_IMPL_API bool     Az_ImplDX11_CreateDeviceObjects();
AZ_IMPL_API void     Az_ImplDX11_InvalidateDeviceObjects();

AZ_IMPL_API void     Az_ImplDX11_UpdateTexture(AzTextureData* tex);

struct Az_AzplDX11_RenderState
{
    ID3D11Device*           Device;
    ID3D11DeviceContext*    DeviceContext;
    ID3D11SamplerState*     SamplerDefault;
    ID3D11Buffer*           VertexConstantBuffer;
};

#endif 
