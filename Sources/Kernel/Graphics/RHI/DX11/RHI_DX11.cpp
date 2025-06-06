#include "RHI_DX11.hpp"

#include "RHI_DX11_GPUBuffer.hpp"
#include "RHI_DX11_GPUShader.hpp"
#include "RHI_DX11_GPUTexture.hpp"

#include <ioatream>
#include <exception>

#define DX11_MAX_BOUND_RENDTARGETS_VIEWS 8
#define DX11_MAX_BOUND_SHADRESOURCE_VIEWS 64
#define DX11_MAX_BOUND_UNORDERED_ACCESS_VIEWS 8
#define DX11_MAX_BOUND_CONSTANT_BUFFERS 8
#define DX11_MAX_BOUND_SAMPLERS 8

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace RHI
        {
            RHI_DX11::RHI_DX11()
            {}

            RHI_DX11::~RHI_DX11()
            {
                ReleaseObject(main_render_target_view_);
                ReleaseObject(main_depth_stencil_view_);
                ReleaseObject(swap_chain_);
                ReleaseObject(depth_stencil_buffer_);

                ReleaseObject(bilinear_wrap_ss_);
                ReleaseObject(bilinear_mirror_ss_);
                ReleaseObject(bilinear_clamp_ss_);
                ReleaseObject(bilinear_border_ss_);
                ReleaseObject(trilinear_wrap_ss_);
                ReleaseObject(trilinear_mirror_ss_);
                ReleaseObject(trilinear_clamp_ss_);
                ReleaseObject(trilinear_border_ss_);
                ReleaseObject(anisotropic_wrap_ss_);
                ReleaseObject(anisotropic_mirror_ss_);
                ReleaseObject(anisotropic_clamp_ss_);
                ReleaseObject(anisotropic_border_ss_);
                ReleaseObject(shadow_ss_);

                ReleaseObject(no_blend_state_);
                ReleaseObject(alpha_to_coverage_state_);
                ReleaseObject(alpha_blend_state_);

                ReleaseObject(back_culling_rs_);
                ReleaseObject(front_culling_rs_);
                ReleaseObject(no_culling_rs_);
                ReleaseObject(wireframe_rs_);
                ReleaseObject(no_culling_no_depth_enabled_scissor_rs_);
                ReleaseObject(shadow_rs_);

                ReleaseObject(depth_only_read_comparison_never_ds_);
                ReleaseObject(depth_only_read_comparison_less_ds_);
                ReleaseObject(depth_only_read_comparison_equal_ds_);
                ReleaseObject(depth_only_read_comparison_less_equal_ds_);
                ReleaseObject(depth_only_read_comparison_greater_ds_);
                ReleaseObject(depth_only_read_comparison_not_equal_ds_);
                ReleaseObject(depth_only_read_comparison_greater_equal_ds_);
                ReleaseObject(depth_only_read_comparison_always_ds_);
                ReleaseObject(depth_only_write_comparison_never_ds_);
                ReleaseObject(depth_only_write_comparison_less_ds_);
                ReleaseObject(depth_only_write_comparison_equal_ds_);
                ReleaseObject(depth_only_write_comparisonLess_equal_ds_);
                ReleaseObject(depth_only_write_comparison_greater_ds_);
                ReleaseObject(depth_only_write_comparison_not_equal_ds_);
                ReleaseObject(depth_only_write_comparison_greater_equal_ds_);
                ReleaseObject(depth_only_write_comparison_always_ds_);

                if (direct3d_device_context_)
                    direct3d_device_context_->ClearState();

                ReleaseObject(direct3d_device_context_);
                ReleaseObject(direct3d_device_);
                ReleaseObject(user_defined_annotation_);
            }

            bool RHI_DX11::Initialize(void* window_handle_a,
                                      unsigned int width_a,
                                      unsigned int height_a,
                                      bool is_fullscreen_a,
                                      bool is_reset_a)
            {
                api_ = GRAPHICS_API::DX11;
                assert(width_a > 0 && height_a > 0);

                HRESULT hr_;
                unsigned int create_device_flags_ = 0;

#if defined(DEBUG) || defined(_DEBUG)
                create_device_flags_ |= D3D11_CREATE_DEVICE_DEBUG;
#endif

                D3D_FEATURE_LEVEL feature_levels_[] = {D3D_FEATURE_LEVEL_11_1,
                                                       D3D_FEATURE_LEVEL_11_0,
                                                       D3D_FEATURE_LEVEL_10_1,
                                                       D3D_FEATURE_LEVEL_10_0};

                ID3D11Device* direct3d_device_ = nullptr;
                ID3D11DeviceContext* direct3d_device_context_ = nullptr;
                if (FAILED(hr_ = D3D11CreateDevice(NULL,
                                                   D3D_DRIVTYPE_HARDWARE,
                                                   NULL,
                                                   create_device_flags_,
                                                   feature_levels_,
                                                   ARRAYSIZE(feature_levels_),
                                                   D3D11_SDK_VERSION,
                                                   &direct3d_device_,
                                                   &feature_level_,
                                                   &direct3d_device_context_)))
                    throw std::exception("RHI_DX11: D3D11CreateDevice() failed",
                                         hr_);

                if (FAILED(hr_ = direct3d_device_->QueryInterface(
                                   __uuidof(ID3D11Device1),
                                   reinterpret_cast<void**>(
                                           &direct3d_device_))))
                    throw std::exception(
                            "RHI_DX11: ID3D11Device::QueryInterface() failed",
                            hr_);

                if (FAILED(hr_ = direct3d_device_context_->QueryInterface(
                                   __uuidof(ID3D11DeviceContext1),
                                   reinterpret_cast<void**>(
                                           &direct3d_device_context_))))
                    throw std::exception(
                            "RHI_DX11: ID3D11Device::QueryInterface() failed",
                            hr_);

                if (FAILED(hr_ = direct3d_device_context_->QueryInterface(
                                   __uuidof(ID3DUserDefinedAnnotation),
                                   reinterpret_cast<void**>(
                                           &user_defined_annotation_))))
                    throw std::exception(
                            "RHI_DX11: ID3D11Device::QueryInterface() failed",
                            hr_);

                ReleaseObject(direct3d_device_);
                ReleaseObject(direct3d_device_context_);

                DXGI_SWAP_CHAIN_DESC1 swap_chain_desc_;
                ZeroMemory(&swap_chain_desc_, sizeof(swap_chain_desc_));
                swap_chain_desc_.Width = width_a;
                swap_chain_desc_.Height = height_a;
                swap_chain_desc_.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
                swap_chain_desc_.SampleDesc.Count = 1;
                swap_chain_desc_.SampleDesc.Quality = 0;
                swap_chain_desc_.BufferUsage = DXGI_USAGE_RENDTARGET_OUTPUT;
                swap_chain_desc_.BufferCount = 1;
                swap_chain_desc_.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

                IDXGIDevice* dxgi_device_ = nullptr;
                if (FAILED(hr_ = direct3d_device_->QueryInterface(
                                   __uuidof(IDXGIDevice),
                                   reinterpret_cast<void**>(&dxgi_device_))))
                {
                    throw std::exception(
                            "RHI_DX11: ID3D11Device::QueryInterface() "
                            "failed",
                            hr_);
                }

                IDXGIAdapter* dxgi_adapter_ = nullptr;
                if (FAILED(hr_ = dxgi_device_->GetParent(__uuidof(IDXGIAdapter),
                                                      reinterpret_cast<void**>(
                                                              &dxgi_adapter_))))
                {
                    ReleaseObject(dxgi_device_);
                    throw std::exception(
                            "RHI_DX11: IDXGIDevice::GetParent() failed "
                            "retrieving "
                            "adapter.",
                            hr_);
                }

                IDXGIFactory2* dxgi_factory_ = nullptr;
                if (FAILED(hr_ = dxgi_adapter_->GetParent(__uuidof(IDXGIFactory2),
                                                       reinterpret_cast<void**>(
                                                               &dxgi_factory_))))
                {
                    ReleaseObject(dxgi_device_);
                    ReleaseObject(dxgi_adapter_);
                    throw std::exception(
                            "RHI_DX11: IDXGIAdapter::GetParent() failed "
                            "retrieving "
                            "factory.",
                            hr_);
                }

                DXGI_SWAP_CHAIN_FULLSCREEN_DESC full_screen_desc_;
                ZeroMemory(&full_screen_desc_, sizeof(full_screen_desc_));
                full_screen_desc_.RefreshRate.Numerator = DefaultFrameRate;
                full_screen_desc_.RefreshRate.Denominator = 1;
                full_screen_desc_.Windowed = !is_fullscreen_;

                if (FAILED(hr_ = dxgi_factory_->CreateSwapChainForHwnd(
                                   dxgi_device_,
                                   window_handle_a,
                                   &swap_chain_desc_,
                                   &full_screen_desc_,
                                   nullptr,
                                   &swap_chain_)))
                {
                    ReleaseObject(dxgi_device_);
                    ReleaseObject(dxgi_adapter_);
                    ReleaseObject(dxgi_factory_);
                    throw std::exception(
                            "RHI_DX11: "
                            "IDXGIDevice::CreateSwapChainForHwnd() "
                            "failed.",
                            hr_);
                }

                ReleaseObject(dxgi_device_);
                ReleaseObject(dxgi_adapter_);
                ReleaseObject(dxgi_factory_);

                ID3D11Texture2D* back_buffer_;
                if (FAILED(hr_ = swap_chain_->GetBuffer(0,
                                                      __uuidof(ID3D11Texture2D),
                                                      reinterpret_cast<void**>(
                                                              &back_buffer_))))
                {
                    throw std::exception(
                            "RHI_DX11: IDXGISwapChain::GetBuffer() failed.",
                            hr_);
                }

                back_buffer_->GetDesc(&back_buffer_desc_);

                if (FAILED(hr_ = direct3d_device_->CreateRenderTargetView(
                                   back_buffer_,
                                   nullptr,
                                   &main_render_target_view_)))
                {
                    ReleaseObject(back_buffer_);
                    throw std::exception(
                            "RHI_DX11: Failed to create main "
                            "RenderTargetView.",
                            hr_);
                }

                ReleaseObject(back_buffer_);

                {
                    D3D11_TEXTURE2D_DESC depth_stencil_desc_;
                    ZeroMemory(&depth_stencil_desc_, sizeof(depth_stencil_desc_));
                    depth_stencil_desc_.Width = width_a;
                    depth_stencil-desc_.Height = height_a;
                    depth_stencil_desc_.MipLevels = 1;
                    depth_stencil_sesc_.ArraySize = 1;
                    depth_stencil_desc_.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
                    depth_stencil_desc_.BindFlags = D3D11_BIND_DEPTH_STENCIL;
                    depth_stencil_desc_.Usage = D3D11_USAGE_DEFAULT;
                    depth_stencil_desc_.SampleDesc.Count = 1;
                    depth_stencil_desc_.SampleDesc.Quality = 0;

                    if (FAILED(hr_ = direct3d_device_->CreateTexture2D(
                                       &depth_stencil_desc_,
                                       nullptr,
                                       &depth_stencil_buffer_)))
                    {
                        throw std::exception(
                                "RHI_DX11: Failed to create main "
                                "DepthStencil "
                                "Texture2D.",
                                hr_);
                    }

                    if (FAILED(hr_ = direct3d_device_->CreateDepthStencilView(
                                       depth_stencil_buffer_,
                                       nullptr,
                                       &main_depth_stencil_view_)))
                    {
                        throw std::exception(
                                "RHI_DX11: Failed to create main "
                                "DepthStencilView.",
                                hr_);
                    }
                }

                main_viewport_.top_left_x_ = 0.0f;
                main_viewport_.top_left_y_ = 0.0f;
                main_viewport_.width_ = static_cast<float>(width_a);
                main_viewport_.height_ = static_cast<float>(height_a);
                main_viewport_.min_depth_ = 0.0f;
                main_viewport_.max_depth_ = 1.0f;

                D3D11_VIEWPORT viewport_;
                viewport_.top_left_x_ = main_viewport_.top_left_x_;
                viewport_.top_left_y_ = main_viewport_.top_left_y_;
                viewport_.width_ = main_viewport_.width_;
                viewport_.height_ = main_viewport.height_;
                viewport_.min_depth_ = 0.0f;
                viewport_.max_depth_ = 1.0f;

                mDirect3DDeviceContext->OMSetRenderTargets(
                        1,
                        &main_render_target_view_,
                        main_depth_stencil_view_);
                direct3d_device_context_->RSSetViewports(1, &viewport_);

                CreateSamplerStates();
                CreateRasterizerStates();
                CreateDepthStencilStates();
                CreateBlendStates();

                return true;
            }

            void RHI_DX11::ClearMainRenderTarget(float colors_a[4])
            {
                mDirect3DDeviceContext
                        ->ClearRenderTargetView(main_render_target_view_, colors_a);
            }

            void RHI_DX11::ClearMainDepthStencilTarget(float depth_a,
                                                       unsigned int stencil_a)
            {
                mDirect3DDeviceContext->ClearDepthStencilView(
                        main_depth_stencil_view_,
                        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                        1.0f,
                        0);
            }

            void RHI_DX11::ClearRenderTarget(RHI_GPUTexture* render_target_a,
                                             float colors_a[4],
                                             int rtv_array_index_a)
            {
                assert(render_target_a);

                ID3D11RenderTargetView* rtv_;
                if (rtv_array_index_a > 0)
                    rtv_ = static_cast<ID3D11RenderTargetView*>(
                             render_target_->GetRTV(rtv_array_index_a));
                else
                    rtv_ = static_cast<ID3D11RenderTargetView*>(
                             render_target_->GetRTV());
                direct3d_device_context_->ClearRenderTargetView(rtv_, colors_a);
            }

            void RHI_DX11::ClearDepthStencilTarget(RHI_GPUTexture* depth_target_a,
                                                   float depth_a,
                                                   unsigned int stencil_a)
            {
                assert(depth_target_a);
                ID3D11DepthStencilView* depth_stencil_view_ =
                        static_cast<ID3D11DepthStencilView*>(
                                depth_target_a->GetDSV());
                
                assert(depth_stencil_view_);
                direct3d_device_context_->ClearDepthStencilView(
                        depth_stencil_view_,
                        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                        depth_a,
                        stencil_a);
            }

            void RHI_DX11::ClearUAV(RHI_GPUResource* render_target_a,
                                    float colors_a[4])
            {
                assert(render_target_a);
                ID3D11UnorderedAccessView* unordered_access_view_ =
                        static_cast<ID3D11UnorderedAccessView*>(
                                render_target_a->GetUAV());
                
                assert(unordered_access_view_);
                direct3d_device_context_->ClearUnorderedAccessViewFloat(
                        unordered_access_view_,
                        colors_a);
            }

            void RHI_DX11::ClearUAV(RHI_GPUBuffer* buffer_a, unsigned int clear_a)
            {
                assert(buffer_a);
                ID3D11UnorderedAccessView* unordered_access_view_ =
                        static_cast<ID3D11UnorderedAccessView*>(
                                buffer_a->GetUAV());
                
                assert(unordered_access_view_);
                unsigned int clears_[4] = {clear_a, clear_a, clear_a, clear_a};
                mDirect3DDeviceContext->ClearUnorderedAccessViewUint(
                        unordered_access_view_,
                        clears_);
            }

            void RHI_DX11::CreateInputLayout(
                    RHI_InputLayout* out_input_layout_a,
                    RHI_INPUT_ELEMENT_DESC* input_element_descriptions_a,
                    unsigned int input_element_description_count_a,
                    const void* shader_byte_code_with_input_signature_a,
                    unsigned int byte_code_length_a)
            {
                assert(input_element_descriptions_a);
                assert(out_input_layout_a);

                D3D11_INPUT_ELEMENT_DESC* descriptions_ =
                        new D3D11_INPUT_ELEMENT_DESC [input_element_description_count_];

                for (unsigned int i = 0; i < input_element_description_count_; i++)
                {
                    descriptions_[i].SemanticName =
                            input_element_descriptions_a[i].semantic_name_;
                    descriptions_[i].SemanticIndex =
                            input_element_descriptions_a[i].semantic_index_;
                    descriptions_[i].Format = GetFormat(
                            input_element_descriptions_a[i].format_);
                    descriptions_[i].InputSlot =
                            input_element_descriptions_a[i].input_slot_;
                    descriptions_[i].AlignedByteOffset =
                            input_element_descriptions_a[i].aligned_byte_offset_;
                    descriptions_[i].InputSlotClass =
                            input_element_descriptions_a[i].is_per_vertex_
                                    ? D3D11_INPUT_PVERTEX_DATA
                                    : D3D11_INPUT_PINSTANCE_DATA;
                    descriptions_[i].InstanceDataStepRate =
                            input_element_descriptions_a[i].instance_data_step_rate_;
                }

                direct3d_device_->CreateInputLayout(
                        descriptions_a,
                        input_element_description_count_a,
                        shader_byte_code_with_input_signature_a,
                        byte_code_length_a,
                        &(static_cast<RHI_DX11_InputLayout*>(out_input_layout_a)
                                  ->input_layout_));
                DeleteObject(descriptions_);
            }

            RHI_InputLayout* RHI_DX11::CreateInputLayout(
                    RHI_INPUT_ELEMENT_DESC* input_element_descriptions_a,
                    unsigned int input_element_description_count_a)
            {
                return new RHI_DX11_InputLayout(input_element_descriptions_a,
                                                input_element_description_count_a);
            }

            RHI_GPUShader* RHI_DX11::CreateGPUShader()
            {
                return new RHI_DX11_GPUShader();
            }

            RHI_GPUBuffer* RHI_DX11::CreateGPUBuffer(
                    const std::string& debug_name_a)
            {
                return new RHI_DX11_GPUBuffer(debug_name_a);
            }

            RHI_GPUTexture* RHI_DX11::CreateGPUTexture(
                    const std::wstring& debug_name_a)
            {
                return new RHI_DX11_GPUTexture(debug_name_a);
            }

            void RHI_DX11::CreateTexture(
                    RHI_GPUTexture* out_texture_a,
                    unsigned int width_a,
                    unsigned int height_a,
                    unsigned int samples_a,
                    RHI_FORMAT format_a,
                    RHI_BIND_FLAG bind_flags_a,
                    int mip_a,
                    int depth_a,
                    int array_size_a,
                    bool is_cubemap_a,
                    int cubemap_array_size_a)
            {
                assert(out_texture_a);
                out_texture_a->CreateGPUTextureResource(this,
                                                      width_a,
                                                      height_a,
                                                      samples_a,
                                                      format_a,
                                                      bind_flags_a,
                                                      mip_a,
                                                      depth_a,
                                                      array_size_a,
                                                      is_cubemap_a,
                                                      cubemap_array_size_a);
            }

            void RHI_DX11::CreateTexture(RHI_GPUTexture* out_texture_a,
                                         const std::string& path_a,
                                         bool is_full_path_a)
            {
                assert(out_texture_a);
                out_texture_a->CreateGPUTextureResource(this, path_a, is_full_path_a);
            }

            void RHI_DX11::CreateTexture(RHI_GPUTexture* out_texture_a,
                                         const std::wstring& path_a,
                                         bool is_full_path_a)
            {
                assert(out_texture_a);
                out_texture_a->CreateGPUTextureResource(this, path_a, is_full_path_a);
            }

            void RHI_DX11::CreateBuffer(
                    RHI_GPUBuffer* out_buffer_a,
                    void* data_a,
                    unsigned int objects_count_a,
                    unsigned int byte_stride_a,
                    bool is_dynamic_a,
                    RHI_BIND_FLAG bind_flags_a,
                    unsigned int cpu_access_flags_a,
                    RHI_RESOURCE_MISC_FLAG misc_flags_a,
                    RHI_FORMAT format_a)
            {
                assert(out_buffer_a);
                out_buffer_a->CreateGPUBufferResource(this,
                                                    data_a,
                                                    objects_count_a,
                                                    byte_stride_a,
                                                    is_dynamic_a,
                                                    bind_flags_a,
                                                    cpu_access_flags_a,
                                                    misc_flags_a,
                                                    format_a);
            }

            void RHI_DX11::CopyBuffer(RHI_GPUBuffer* dest_buffer_a,
                                      RHI_GPUBuffer* src_buffer_a,
                                      int cmd_list_index_a,
                                      bool is_in_copy_queue_a)
            {
                assert(dest_buffer_a);
                assert(src_buffer_a);

                ID3D11Resource* dst_resource_ = static_cast<ID3D11Resource*>(
                        dest_buffer_a->GetBuffer());
                ID3D11Resource* src_resource_ = static_cast<ID3D11Resource*>(
                        src_buffer_a->GetBuffer());

                assert(dst_resource_);
                assert(src_resource_);

                direct3d_device_context_->CopyResource(dst_resource_, src_resource_);
            }

            void RHI_DX11::BeginBufferRead(RHI_GPUBuffer* buffer_a,
                                           void** output_a)
            {
                assert(buffer_a);
                assert(!is_context_reading_buffer_);

                RHI_DX11_GPUBuffer* buffer_ = static_cast<RHI_DX11_GPUBuffer*>(
                        buffer_a);
                assert(buffer_);

                D3D11_MAPPED_SUBRESOURCE mapped_resource_;
                buffer_->Map(this, D3D11_MAP_READ, &mapped_resource_);
                *output_a = mapped_resource_.pData;
                is_context_reading_buffer_ = true;
            }

            void RHI_DX11::EndBufferRead(RHI_GPUBuffer* buffer_a)
            {
                RHI_DX11_GPUBuffer* buffer_ = static_cast<RHI_DX11_GPUBuffer*>(
                        buffer_a);
                assert(buffer_);

                buffer_->Unmap(this);

                is_context_reading_buffer_ = false;
            }

            void RHI_DX11::CopyGPUTextureSubresourceRegion(
                    RHI_GPUResource* dest_buffer_a,
                    unsigned int dst_subresource_a,
                    unsigned int dst_x_a,
                    unsigned int dst_y_a,
                    unsigned int dst_z_a,
                    RHI_GPUResource* src_buffer_a,
                    unsigned int src_subresource_a,
                    bool is_in_copy_queue_or_skip_transitions_a)
            {
                assert(dest_buffer_a);
                assert(src_buffer_a);

                RHI_DX11_GPUTexture* dst_buffer_ =
                        static_cast<RHI_DX11_GPUTexture*>(dest_buffer_a);
                assert(dst_buffer_);
                
                RHI_DX11_GPUTexture* src_buffer_ =
                        static_cast<RHI_DX11_GPUTexture*>(src_buffer_a);
                assert(src_buffer_);

                if (dst_buffer_->GetTexture2D() && src_buffer_->GetTexture2D())
                    direct3d_device_context_->CopySubresourceRegion(
                            dst_buffer_->GetTexture2D(),
                            dst_subresource_,
                            dst_x_a,
                            dst_y_a,
                            dst_z_a,
                            src_buffer_->GetTexture2D(),
                            src_subresource_,
                            NULL);
                
                else if (dst_buffer_->GetTexture3D() && src_buffer_->GetTexture3D())
                    direct3d_device_context_->CopySubresourceRegion(
                            dst_buffer_->GetTexture3D(),
                            dst_subresource_,
                            dst_x_,
                            dst_y_,
                            dst_z_,
                            src_buffer_->GetTexture3D(),
                            src_subresource_,
                            NULL);
                else
                    throw std::exception(
                            "RHI_DX11:: One of the resources is NULL during "
                            "CopyGPUTextureSubresourceRegion()");
            }

            void RHI_DX11::Draw(unsigned int vertex_count_a)
            {
                assert(vertex_count_a > 0);
                direct3d_device_context_->DrawInstanced(vertex_count_a, 1, 0, 0);
            }

            void RHI_DX11::DrawIndexed(unsigned int index_count_a)
            {
                assert(index_count_a > 0);
                direct3d_device_context_->DrawIndexedInstanced(index_count_a,
                                                             1,
                                                             0,
                                                             0,
                                                             0);
            }

            void RHI_DX11::DrawInstanced(unsigned int vertex_count_per_instance_a,
                                         unsigned int instance_count_a,
                                         unsigned int start_vertex_location_a,
                                         unsigned int start_instance_location_a)
            {
                assert(vertex_count_per_instance_a > 0);
                assert(instance_count_a > 0);

                direct3d_device_context_->DrawInstanced(vertex_count_per_instance_a,
                                                      instance_count_a,
                                                      start_vertex_location_a,
                                                      start_instance_location_a);
            }

            void RHI_DX11::DrawIndexedInstanced(unsigned int index_count_per_instance_a,
                                                unsigned int instance_count_a,
                                                unsigned int start_index_location_a,
                                                int base_vertex_location_a,
                                                unsigned int start_instance_location_a)
            {
                assert(index_count_per_instance_a > 0);
                assert(instance_count_a > 0);

                direct3d_device_context_->DrawIndexedInstanced(
                        index_count_per_instance_a,
                        instance_count_a,
                        start_index_location_a,
                        base_vertex_location_a,
                        start_instance_location_a);
            }

            void RHI_DX11::DrawIndexedInstancedIndirect(
                    RHI_GPUBuffer* an_args_buffer_a,
                    unsigned int aligned_byte_offset_a)
            {
                assert(an_args_buffer_a);

                RHI_DX11_GPUBuffer* dx11_buffer_ =
                        static_cast<RHI_DX11_GPUBuffer*>(an_args_buffer_a);
                direct3d_device_context_->DrawIndexedInstancedIndirect(
                        static_cast<ID3D11Buffer*>(dx11_buffer_->GetBuffer()),
                        aligned_byte_offset_a);
            }

            void RHI_DX11::Dispatch(unsigned int thread_group_count_x_a,
                                    unsigned int thread_group_count_y_a,
                                    unsigned int thread_group_count_z_a)
            {
                direct3d_device_context_->Dispatch(thread_group_count_x_a,
                                                   thread_group_count_y_a,
                                                   thread_group_count_z_a);
            }

            void RHI_DX11::GenerateMips(RHI_GPUTexture* texture_a,
                                        RHI_GPUTexture* srgb_texture_a)
            {
                assert(texture_a);
                ID3D11ShaderResourceView* shader_resource_view_ =
                        static_cast<ID3D11ShaderResourceView*>(
                                texture_a->GetSRV());
                
                assert(shader_resource_view_);
                direct3d_device_context_->GenerateMips(shader_resource_view_);
            }

            void RHI_DX11::PresentGraphics()
            {
                HRESULT hr_ = swap_chain_->Present(0, 0);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: IDXGISwapChain::Present() failed.",
                            hr_);
            }

            bool RHI_DX11::ProjectCubemapToSH(RHI_GPUTexture* texture_a,
                                              unsigned int order_a,
                                              float* result_r_a,
                                              float* result_g_a,
                                              float* result_b_a)
            {
                assert(texture_a);

                RHI_DX11_GPUTexture* tex_ = static_cast<RHI_DX11_GPUTexture*>(
                        texture_a);
                assert(tex_);

                return !(
                        FAILED(DirectX::SHProjectCubeMap(direct3d_device_context_,
                                                         order_a,
                                                         tex_->GetTexture2D(),
                                                         result_r_a,
                                                         result_g_a,
                                                         result_b_a)));
            }

            void RHI_DX11::SaveGPUTextureToFile(RHI_GPUTexture* texture_a,
                                                const std::wstring& path_name_a)
            {
                assert(texture_a);

                RHI_DX11_GPUTexture* tex_ = static_cast<RHI_DX11_GPUTexture*>(
                        texture_a);
                assert(tex_);

                DirectX::ScratchImage temp_image_;
                HRESULT res_ = DirectX::CaptureTexture(direct3d_device_,
                                                       direct3d_device_context_,
                                                       tex_->GetTexture2D(),
                                                       temp_image_);
                if (FAILED(res_))
                    throw std::exception(
                            "Failed to capture a probe texture when saving!",
                            res_);

                res_ = DirectX::SaveToDDSFile(temp_image_.GetImages(),
                                              temp_image_.GetImageCount(),
                                              temp_image_.GetMetadata(),
                                              DDS_FLAGS_NONE,
                                              path_name_a.c_str());
                if (FAILED(res_))
                {
                    throw std::exception("Failed to save a texture to a file: ");
                }
            }

            void RHI_DX11::SetMainRenderTargets(int cmd_list_index_a)
            {
                direct3d_device_context_
                        ->OMSetRenderTargets(1, &main_render_target_view_, NULL);
            }

            void RHI_DX11::SetRenderTargets(
                    const std::vector<RHI_GPUTexture*>& render_targets_a,
                    RHI_GPUTexture* depth_target_a,
                    RHI_GPUTexture* uav_a,
                    int rtv_array_index_a)
            {
                if (!uav_a)
                {
                    if (rtv_array_index_a > 0)
                        assert(render_targets_a.size() == 1);

                    assert(render_targets_a.size() > 0);
                    assert(render_targets_a.size()
                           <= DX11_MAX_BOUND_RENDTARGETS_VIEWS);

                    ID3D11RenderTargetView*
                            rtvs_[DX11_MAX_BOUND_RENDTARGETS_VIEWS] = {};
                    unsigned int rt_count_ = static_cast<unsigned int>(render_targets_a.size());
                    for (unsigned int i = 0; i < rt_count_; i++)
                    {
                        assert(render_targets_a[i]);
                        if (rtv_array_index > 0)
                            rtvs_[i] = static_cast<ID3D11RenderTargetView*>(
                                    render_targets_a[i]->GetRTV(rtv_array_index_));
                        else
                            rtvs_[i] = static_cast<ID3D11RenderTargetView*>(
                                    render_targets_a[i]->GetRTV());
                        assert(rtvs_[i]);
                    }

                    if (depth_target_)
                    {
                        ID3D11DepthStencilView* dsv_ =
                                static_cast<ID3D11DepthStencilView*>(
                                        depth_target_->GetDSV());
                        direct3d_device_context_->OMSetRenderTargets(rt_count_,
                                                                   rtvs_,
                                                                   dsv_);
                    } 
                    else
                        direct3d_device_context_->OMSetRenderTargets(rt_count_,
                                                                   rtvs_,
                                                                   NULL);

                } 

                else
                {
                    ID3D11RenderTargetView* nullrtvs_[1] = {NULL};
                    ID3D11UnorderedAccessView* uavs_[1] = {
                            static_cast<ID3D11UnorderedAccessView*>(
                                    uav_a->GetUAV())};
                    assert(uavs_[0]);
                    if (depth_target_)
                    {
                        ID3D11DepthStencilView* dsv_ =
                                static_cast<ID3D11DepthStencilView*>(
                                        depth_target_->GetDSV());
                        assert(dsv_);
                        mDirect3DDeviceContext
                                ->OMSetRenderTargetsAndUnorderedAccessViews(
                                        0,
                                        nullrtvs_,
                                        dsv_,
                                        0,
                                        1,
                                        uavs_,
                                        NULL);
                    } else
                        direct3d_device_context_
                                ->OMSetRenderTargetsAndUnorderedAccessViews(
                                        0,
                                        nullrtvs_,
                                        NULL,
                                        0,
                                        1,
                                        uavs_,
                                        NULL);
                }
            }

            void RHI_DX11::SetDepthTarget(RHI_GPUTexture* depth_target_a)
            {
                ID3D11RenderTargetView* nullrtvs_[1] = {NULL};

                assert(depth_target_a);
                ID3D11DepthStencilView* dsv_ =
                        static_cast<ID3D11DepthStencilView*>(
                                depth_target_a->GetDSV());
                assert(dsv_);

                direct3d_device_context_->OMSetRenderTargets(1, nullrtvs_, dsv_);
            }

            void RHI_DX11::SetDepthStencilState(RHI_DEPTH_STENCIL_STATE ds_a,
                                                unsigned int stencil_ref_a)
            {
                if (ds_a == DISABLED)
                {
                    direct3d_device_context_->OMSetDepthStencilState(nullptr,
                                                                   0xffffffff);
                    return;
                }

                auto it_ = depth_states_.find(ds_a);
                if (it_ != depth_states_.end())
                    direct3d_device_context_->OMSetDepthStencilState(it_->second,
                                                                     stencil_ref_a);
                else
                    throw std::exception(
                            "RHI_DX11: DepthStencil state is not found.");
            }

            void RHI_DX11::SetBlendState(RHI_BLEND_STATE bs_a,
                                         const float blend_factor_a[4],
                                         unsigned int sample_mask_a)
            {
                if (bs_a == RHI_BLEND_STATE::NO_BLEND)
                {
                    direct3d_device_context_->OMSetBlendState(nullptr,
                                                            NULL,
                                                            0xffffffff);
                    return;
                }

                auto it_ = blend_states_.find(bs_a);
                if (it_ != blend_states_.end())
                {
                    current_bs_ = bs_a;
                    direct3d_device_context_->OMSetBlendState(it_->second,
                                                            blend_factor_,
                                                            sample_mask_);
                } else
                    throw std::exception("RHI_DX11: Blend state is not found.");
            }

            void RHI_DX11::SetRasterizerState(RHI_RASTERIZSTATE rs_a)
            {
                auto it_ = rasterizer_states_.find(rs_a);
                if (it_ != rasterizer_states_.end())
                {
                    current_rs_ = rs_a;
                    direct3d_device_context_->RSSetState(it_->second);
                } else
                    throw std::exception(
                            "RHI_DX11: Rasterizer state is not found.");
            }

            void RHI_DX11::SetViewport(const RHI_Viewport& viewport_a)
            {
                D3D11_VIEWPORT viewport_;
                viewport_a.top_left_x_ = viewport_a.top_left_x_;
                viewport_a.top_left_y_ = viewport_a.top_left_y_;
                viewport_a.width_ = viewport_a.width_;
                viewport_a.height_ = viewport_a.height_;
                viewport_a.min_depth_ = viewport_a.min_depth_;
                viewport_a.max_depth_ = viewport_a.max_depth_;

                current_viewport_ = viewport_a;

                direct3d_device_context_->RSSetViewports(1, &viewport_a);
            }

            void RHI_DX11::SetRect(const RHI_Rect& rect_a)
            {
                D3D11_RECT current_rect_ = {rect_a.left_,
                                            rect_a.top_,
                                            rect_a.right_,
                                            rect_a.bottom_};
                direct3d_device_context_->RSSetScissorRects(1, &current_rect_);
            }

            void RHI_DX11::SetShaderResources(
                    RHI_SHADTYPE shader_type_a,
                    const std::vector<RHI_GPUResource*>& srvs_a,
                    unsigned int start_slot_a,
                    RHI_GPURootSignature* rs_a,
                    int root_param_index_a,
                    bool is_compute_rs_a,
                    bool skip_automatic_transition_a)
            {
                assert(srvs_a.size() > 0);
                assert(srvs_a.size() <= DX11_MAX_BOUND_SHADRESOURCE_VIEWS);

                ID3D11ShaderResourceView*
                        srs_[DX11_MAX_BOUND_SHADRESOURCE_VIEWS] = {};
                unsigned int sr_count_ = static_cast<unsigned int>(srvs_a.size());
                for (unsigned int i = 0; i < sr_count_; i++)
                {
                    if (!srvs_a[i])
                        srs_[i] = nullptr;
                    else
                    {
                        srs_[i] = static_cast<ID3D11ShaderResourceView*>(
                                srvs_a[i]->GetSRV());
                        assert(srs_[i]);
                    }
                }

                switch (shader_type_a)
                {
                case RHI_SHADTYPE::VERTEX:
                    direct3d_device_context_->VSSetShaderResources(start_slot_a,
                                                                   sr_count_a,
                                                                   srs_a);
                    break;
                case RHI_SHADTYPE::GEOMETRY:
                    direct3d_device_context_->GSSetShaderResources(start_slot_a,
                                                                 sr_count_a,
                                                                 srs_a);
                    break;
                case RHI_SHADTYPE::TESSELLATION_HULL:
                    direct3d_device_context_->HSSetShaderResources(start_slot_a,
                                                                 sr_count_a,
                                                                 srs_a);
                    break;
                case RHI_SHADTYPE::TESSELLATION_DOMAIN:
                    direct3d_device_context_->DSSetShaderResources(start_slot_a,
                                                                 sr_count_a,
                                                                 srs_a);
                    break;
                case RHI_SHADTYPE::PIXEL:
                    direct3d_device_context_->PSSetShaderResources(start_slot_a,
                                                                 sr_count_a,
                                                                 srs_a);
                    break;
                case RHI_SHADTYPE::COMPUTE:
                    direct3d_device_context_->CSSetShaderResources(start_slot_a,
                                                                 sr_count_a,
                                                                 srs_a);
                    break;
                }
            }

            void RHI_DX11::SetUnorderedAccessResources(
                    RHI_SHADTYPE shader_type_a,
                    const std::vector<RHI_GPUResource*>& uavs_a,
                    unsigned int start_slot_a,
                    RHI_GPURootSignature* rs_a,
                    int root_param_index_a,
                    bool is_compute_rs_a,
                    bool skip_automatic_transition_a)
            {
                assert(uavs_a.size() > 0);
                assert(uavs_a.size() <= DX11_MAX_BOUND_UNORDERED_ACCESS_VIEWS);

                ID3D11UnorderedAccessView*
                        uavs_[DX11_MAX_BOUND_UNORDERED_ACCESS_VIEWS] = {};
                
                unsigned int uav_count_ = static_cast<unsigned int>(uavs_a.size());
                for (unsigned int i = 0; i < uav_count_; i++)
                {
                    assert(uavs_a[i]);
                    uavs_a[i] = static_cast<ID3D11UnorderedAccessView*>(
                            uavs_a[i]->GetUAV());
                    assert(uavs_a[i]);
                }

                switch (shader_type_)
                {
                case RHI_SHADTYPE::VERTEX:
                    throw std::exception(
                            "RHI_DX11: Binding UAV to this shader stage is "
                            "not "
                            "possible.");
                    break;
                case RHI_SHADTYPE::GEOMETRY:
                    throw std::exception(
                            "RHI_DX11: Binding UAV to this shader stage is "
                            "not "
                            "possible.");
                    break;
                case RHI_SHADTYPE::TESSELLATION_HULL:
                case RHI_SHADTYPE::TESSELLATION_DOMAIN:
                    throw std::exception(
                            "RHI_DX11: Binding UAV to this shader stage is "
                            "not "
                            "possible.");
                    break;
                case RHI_SHADTYPE::PIXEL:
                    throw std::exception(
                            "RHI_DX11: Binding UAV to this shader stage is "
                            "not "
                            "possible.");
                    break;
                case RHI_SHADTYPE::COMPUTE:
                    direct3d_device_context_->CSSetUnorderedAccessViews(start_slot_a,
                                                                      uav_count_a,
                                                                      uavs_a,
                                                                      NULL);
                    break;
                }
            }

            void RHI_DX11::SetConstantBuffers(
                    RHI_SHADTYPE shader_type_a,
                    const std::vector<RHI_GPUBuffer*>& cbs_a,
                    unsigned int start_slot_a,
                    RHI_GPURootSignature* rs_a,
                    int root_param_index_a,
                    bool is_compute_rs_)
            {
                assert(cbs_a.size() > 0);
                assert(cbs_a.size() <= DX11_MAX_BOUND_CONSTANT_BUFFERS);

                ID3D11Buffer* cbs_[DX11_MAX_BOUND_CONSTANT_BUFFERS] = {};
                unsigned int cbs_count_ = static_cast<unsigned int>(cbs_.size());
                for (unsigned int i = 0; i < cbs_count_; i++)
                {
                    assert(cbs_a[i]);
                    cbs_[i] = static_cast<ID3D11Buffer*>(cbs_a[i]->GetBuffer());
                    assert(cbs_[i]);
                }

                switch (shader_type_a)
                {
                case RHI_SHADTYPE::VERTEX:
                    direct3d_device_context_->VSSetConstantBuffers(start_slot_a,
                                                                 cbs_count_a,
                                                                 cbs_a);
                    break;
                case RHI_SHADTYPE::GEOMETRY:
                    direct3d_device_context_->GSSetConstantBuffers(start_slot_a,
                                                                 cbs_count_a,
                                                                 cbs_a);
                    break;
                case RHI_SHADTYPE::TESSELLATION_HULL:
                    direct3d_device_context_->HSSetConstantBuffers(start_slot_a,
                                                                 cbs_count_a,
                                                                 cbs_a);
                    break;
                case RHI_SHADTYPE::TESSELLATION_DOMAIN:
                    direct3d_device_context_->DSSetConstantBuffers(start_slot_a,
                                                                 cbs_count_a,
                                                                 cbs_a);
                    break;
                case RHI_SHADTYPE::PIXEL:
                    direct3d_device_context_->PSSetConstantBuffers(start_slot_a,
                                                                 cbs_count_a,
                                                                 cbs_a);
                    break;
                case RHI_SHADTYPE::COMPUTE:
                    direct3d_device_context_->CSSetConstantBuffers(start_slot_a,
                                                                 cbs_count_a,
                                                                 cbs_a);
                    break;
                }
            }

            void RHI_DX11::SetShader(RHI_GPUShader* shader_a)
            {
                assert(shader_a);

                switch (shader_a->shader_type_)
                {
                case RHI_SHADTYPE::VERTEX: {
                    ID3D11VertexShader* vs_ = static_cast<ID3D11VertexShader*>(
                            shader_a->GetShaderObject());
                    assert(vs_);
                    direct3d_device_context_->VSSetShader(vs_, NULL, NULL);
                }
                break;
                case RHI_SHADTYPE::GEOMETRY: {
                    ID3D11GeometryShader* gs_ =
                            static_cast<ID3D11GeometryShader*>(
                                    shader_a->GetShaderObject());
                    assert(gs_);
                    direct3d_device_context_->GSSetShader(gs_, NULL, NULL);
                }
                break;
                case RHI_SHADTYPE::TESSELLATION_HULL: {
                    ID3D11HullShader* hs_ = static_cast<ID3D11HullShader*>(
                            shader_a->GetShaderObject());
                    
                    assert(hs_);
                    direct3d_device_context_->HSSetShader(hs_, NULL, NULL);
                }
                break;
                case RHI_SHADTYPE::TESSELLATION_DOMAIN: {
                    ID3D11DomainShader* ds_ = static_cast<ID3D11DomainShader*>(
                            shader_a->GetShaderObject());
                    assert(ds_);
                    direct3d_device_context_->DSSetShader(ds_, NULL, NULL);
                }
                break;
                case RHI_SHADTYPE::PIXEL: {
                    ID3D11PixelShader* ps_ = static_cast<ID3D11PixelShader*>(
                            shader_a->GetShaderObject());
                    assert(ps_);
                    direct3d_device_context_->PSSetShader(ps_, NULL, NULL);
                }
                break;
                case RHI_SHADTYPE::COMPUTE: {
                    ID3D11ComputeShader* cs_ = static_cast<ID3D11ComputeShader*>(
                            shader_a->GetShaderObject());
                    
                    assert(cs_);
                    direct3d_device_context_->CSSetShader(cs_, NULL, NULL);
                }
                break;
                }
            }

            void RHI_DX11::SetSamplers(
                    RHI_SHADTYPE shader_type_a,
                    const std::vector<RHI_SAMPLSTATE>& samplers_a,
                    unsigned int start_slot_a,
                    RHI_GPURootSignature* rs_a)
            {
                assert(samplers_a.size() > 0);
                assert(samplers_a.size() <= DX11_MAX_BOUND_SAMPLERS);

                ID3D11SamplerState* ss_[DX11_MAX_BOUND_SAMPLERS] = {};
                unsigned int ss_count_ = static_cast<unsigned int>(samplers_a.size());
                for (unsigned int i = 0; i < ss_count_; i++)
                {
                    auto it_ = sampler_states_.find(samplers_a[i]);
                    if (it_ == sampler_states_.end())
                        throw std::exception(
                                "RHI_DX11: Could not find a sampler state "
                                "that you "
                                "want to bind to the shader. It is probably "
                                "not "
                                "implemented in your graphics API");
                    else
                        ss_[i] = it_->second;
                }

                switch (shader_type_a)
                {
                case RHI_SHADTYPE::VERTEX:
                    direct3d_device_context_->VSSetSamplers(start_slot_a,
                                                          ss_count_a,
                                                          ss_a);
                    break;
                case RHI_SHADTYPE::GEOMETRY:
                    direct3d_device_context_->GSSetSamplers(start_slot_a,
                                                          ss_count_a,
                                                          ss_a);
                    break;
                case RHI_SHADTYPE::TESSELLATION_HULL:
                    direct3d_device_context_->HSSetSamplers(start_slot_a,
                                                          ss_count_a,
                                                          ss_a);
                    break;
                case RHI_SHADTYPE::TESSELLATION_DOMAIN:
                    direct3d_device_context_->DSSetSamplers(start_slot_a,
                                                          ss_count_a,
                                                          ss_a);
                    break;
                case RHI_SHADTYPE::PIXEL:
                    direct3d_device_context_->PSSetSamplers(start_slot_a,
                                                          ss_count_a,
                                                          ss_a);
                    break;
                case RHI_SHADTYPE::COMPUTE:
                    direct3d_device_context_->CSSetSamplers(start_slot_a,
                                                          ss_count_a,
                                                          ss_a);
                    break;
                }
            }

            void RHI_DX11::SetInputLayout(RHI_InputLayout* il_a)
            {
                RHI_DX11_InputLayout* dx11_il_ =
                        static_cast<RHI_DX11_InputLayout*>(il_a);
                assert(dx11_il_);
                assert(dx11_il_->input_layout_);
                mDirect3DDeviceContext->IASetInputLayout(
                        dx11_il_->input_layout_);
            }

            void RHI_DX11::SetEmptyInputLayout()
            {
                direct3d_device_context_->IASetInputLayout(nullptr);
            }

            void RHI_DX11::SetIndexBuffer(RHI_GPUBuffer* buffer_a,
                                          unsigned int offset_a)
            {
                assert(buffer_a);
                ID3D11Buffer* buf_ = static_cast<ID3D11Buffer*>(
                        buffer_a->GetBuffer());
                assert(buf_);
                direct3d_device_context_->IASetIndexBuffer(
                        buf_,
                        GetFormat(buffer_->GetFormatRhi()),
                        offset_a);
            }

            void RHI_DX11::SetVertexBuffers(
                    const std::vector<RHI_GPUBuffer*>& vertex_buffers_a)
            {
                assert(vertex_buffers_a.size() > 0
                       && vertex_buffers_a.size()
                                  <= RHI_MAX_BOUND_VERTEX_BUFFERS);
                if (vertex_buffers_a.size() == 1)
                {
                    unsigned int stride_ = vertex_buffers_a[0]->GetStride();
                    unsigned int offset_ = 0;
                    assert(vertex_buffers_a[0]);
                    ID3D11Buffer* buffer_pointers_[1] = {
                            static_cast<ID3D11Buffer*>(
                                    vertex_buffers_a[0]->GetBuffer())};
                    assert(buffer_pointers_[0]);
                    direct3d_device_context_->IASetVertexBuffers(0,
                                                               1,
                                                               buffer_pointers_,
                                                               &stride_,
                                                               &offset_);
                } 

                else
                {
                    unsigned int strides_[2] = {vertex_buffers_a[0]->GetStride(),
                                                vertex_buffers_a[1]->GetStride()};
                    unsigned int offsets_[2] = {0, 0};

                    assert(vertex_buffers_a[0]);
                    assert(vertex_buffers_a[1]);
                    ID3D11Buffer* buffer_pointers_[2] = {
                            static_cast<ID3D11Buffer*>(
                                    vertex_buffers_a[0]->GetBuffer()),
                            static_cast<ID3D11Buffer*>(
                                    vertex_buffers_a[1]->GetBuffer())};
                    
                    assert(buffer_pointers_[0]);
                    assert(buffer_pointers_[1]);
                    direct3d_device_context_->IASetVertexBuffers(0,
                                                               2,
                                                               buffer_pointers_,
                                                               strides_,
                                                               offsets_);
                }
            }

            void RHI_DX11::SetTopologyType(RHI_PRIMITIVE_TYPE type_a)
            {
                direct3d_device_context_->IASetPrimitiveTopology(
                        GetTopologyType(type_a));
            }

            RHI_PRIMITIVE_TYPE RHI_DX11::GetCurrentTopologyType()
            {
                D3D11_PRIMITIVE_TOPOLOGY current_topology_;
                direct3d_device_context_->IAGetPrimitiveTopology(
                        &current_topology_);
                return GetTopologyType(current_topology_);
            }

            void RHI_DX11::UnbindRenderTargets()
            {
                ID3D11RenderTargetView* nullrtvs_[1] = {NULL};
                direct3d_device_context_->OMSetRenderTargets(1,
                                                           nullrtvs_,
                                                           nullptr);
            }

            void RHI_DX11::UnbindResourcesFromShader(RHI_SHADTYPE shader_type_a,
                                                     bool unbind_shader_a)
            {
                auto context_ = GetContext();

                ID3D11ShaderResourceView*
                        nullsrv_[DX11_MAX_BOUND_SHADRESOURCE_VIEWS] = {NULL};
                
                ID3D11Buffer* nullcbs_[DX11_MAX_BOUND_CONSTANT_BUFFERS] = {NULL};
                ID3D11SamplerState* nullsss_[DX11_MAX_BOUND_SAMPLERS] = {NULL};
                ID3D11UnorderedAccessView*
                        nulluav_[DX11_MAX_BOUND_UNORDERED_ACCESS_VIEWS] = {NULL};

                switch (shader_type_)
                {
                case RHI_SHADTYPE::VERTEX: {
                    if (unbind_shader_a)
                        context_->VSSetShader(NULL, NULL, NULL);
                    context_->VSSetShaderResources(
                            0,
                            DX11_MAX_BOUND_SHADRESOURCE_VIEWS,
                            nullsrv_);
                    context_->VSSetConstantBuffers(
                            0,
                            DX11_MAX_BOUND_CONSTANT_BUFFERS,
                            nullcbs_);
                }
                break;
                case RHI_SHADTYPE::GEOMETRY: {
                    if (unbind_shader_)
                        context_->GSSetShader(NULL, NULL, NULL);
                    context_->GSSetShaderResources(
                            0,
                            DX11_MAX_BOUND_SHADRESOURCE_VIEWS,
                            nullsrv_);
                    context_->GSSetConstantBuffers(
                            0,
                            DX11_MAX_BOUND_CONSTANT_BUFFERS,
                            nullcbs_);
                    context_->GSSetSamplers(0, DX11_MAX_BOUND_SAMPLERS, nullsss_);
                }
                break;
                case RHI_SHADTYPE::TESSELLATION_HULL: {
                    if (unbind_shader_)
                        context_->HSSetShader(NULL, NULL, NULL);
                    context_->HSSetShaderResources(
                            0,
                            DX11_MAX_BOUND_SHADRESOURCE_VIEWS,
                            nullsrv_);
                    context_->HSSetConstantBuffers(
                            0,
                            DX11_MAX_BOUND_CONSTANT_BUFFERS,
                            nullcbs_);
                    context_->HSSetSamplers(0, DX11_MAX_BOUND_SAMPLERS, nullsss_);
                }
                break;
                case RHI_SHADTYPE::TESSELLATION_DOMAIN: {
                    if (unbind_shader_)
                        context_->DSSetShader(NULL, NULL, NULL);
                    context_->DSSetShaderResources(
                            0,
                            DX11_MAX_BOUND_SHADRESOURCE_VIEWS,
                            nullsrv_);
                    context_->DSSetConstantBuffers(
                            0,
                            DX11_MAX_BOUND_CONSTANT_BUFFERS,
                            nullcbs_);
                    context_->DSSetSamplers(0, DX11_MAX_BOUND_SAMPLERS, nullsss_);
                }
                break;
                case RHI_SHADTYPE::PIXEL: {
                    if (unbind_shader_)
                        context_->PSSetShader(NULL, NULL, NULL);
                    context_->PSSetShaderResources(
                            0,
                            DX11_MAX_BOUND_SHADRESOURCE_VIEWS,
                            nullsrv_);
                    context_->PSSetConstantBuffers(
                            0,
                            DX11_MAX_BOUND_CONSTANT_BUFFERS,
                            nullcbs_);
                    context_->PSSetSamplers(0, DX11_MAX_BOUND_SAMPLERS, nullsss_);
                }
                break;
                case RHI_SHADTYPE::COMPUTE: {
                    if (unbind_shader_)
                        context_->CSSetShader(NULL, NULL, NULL);
                    context_->CSSetShaderResources(
                            0,
                            DX11_MAX_BOUND_SHADRESOURCE_VIEWS,
                            nullsrv_);
                    context_->CSSetConstantBuffers(
                            0,
                            DX11_MAX_BOUND_CONSTANT_BUFFERS,
                            nullcbs_);
                    context_->CSSetSamplers(0, DX11_MAX_BOUND_SAMPLERS, nullsss_);
                    context_->CSSetUnorderedAccessViews(
                            0,
                            DX11_MAX_BOUND_UNORDERED_ACCESS_VIEWS,
                            nulluav_,
                            0);
                }
                break;
                }
            }

            void RHI_DX11::UpdateBuffer(RHI_GPUBuffer* buffer_a,
                                        void* data_a,
                                        int data_size_a,
                                        bool update_for_all_back_buffers_a)
            {
                assert(buffer_a->GetSize() >= data_size_a);

                RHI_DX11_GPUBuffer* buffer_ = static_cast<RHI_DX11_GPUBuffer*>(buffer_a);
                assert(buffer_);

                D3D11_MAPPED_SUBRESOURCE mapped_resource_;
                ZeroMemory(&mapped_resource_, sizeof(D3D11_MAPPED_SUBRESOURCE));
                buffer_->Map(this, D3D11_MAP_WRITE_DISCARD, &mapped_resource_);
                memcpy(mapped_resource_.pData, data_a, data_size_a);
                buffer_->Unmap(this);
            }

            void RHI_DX11::BeginEventTag(const std::string& name_a,
                                         bool is_compute_queue_a)
            {
                mUserDefinedAnnotation->BeginEvent(
                        Utility::ToWideString(name_a).c_str());
            }

            void RHI_DX11::EndEventTag(bool isComputeQueue)
            {
                user_defined_annotation->EndEvent();
            }

            DXGI_FORMAT RHI_DX11::GetFormat(RHI_FORMAT format_a)
            {
                switch (format_a)
                {
                case RHI_FORMAT::FORMAT_R32G32B32A32_TYPELESS:
                    return DXGI_FORMAT_R32G32B32A32_TYPELESS;
                case RHI_FORMAT::FORMAT_R32G32B32A32_FLOAT:
                    return DXGI_FORMAT_R32G32B32A32_FLOAT;
                case RHI_FORMAT::FORMAT_R32G32B32A32_UINT:
                    return DXGI_FORMAT_R32G32B32A32_UINT;
                case RHI_FORMAT::FORMAT_R32G32B32_TYPELESS:
                    return DXGI_FORMAT_R32G32B32_TYPELESS;
                case RHI_FORMAT::FORMAT_R32G32B32_FLOAT:
                    return DXGI_FORMAT_R32G32B32_FLOAT;
                case RHI_FORMAT::FORMAT_R32G32B32_UINT:
                    return DXGI_FORMAT_R32G32B32_UINT;
                case RHI_FORMAT::FORMAT_R16G16B16A16_TYPELESS:
                    return DXGI_FORMAT_R16G16B16A16_TYPELESS;
                case RHI_FORMAT::FORMAT_R16G16B16A16_FLOAT:
                    return DXGI_FORMAT_R16G16B16A16_FLOAT;
                case RHI_FORMAT::FORMAT_R16G16B16A16_UNORM:
                    return DXGI_FORMAT_R16G16B16A16_UNORM;
                case RHI_FORMAT::FORMAT_R16G16B16A16_UINT:
                    return DXGI_FORMAT_R16G16B16A16_UINT;
                case RHI_FORMAT::FORMAT_R32G32_TYPELESS:
                    return DXGI_FORMAT_R32G32_TYPELESS;
                case RHI_FORMAT::FORMAT_R32G32_FLOAT:
                    return DXGI_FORMAT_R32G32_FLOAT;
                case RHI_FORMAT::FORMAT_R32G32_UINT:
                    return DXGI_FORMAT_R32G32_UINT;
                case RHI_FORMAT::FORMAT_R10G10B10A2_TYPELESS:
                    return DXGI_FORMAT_R10G10B10A2_TYPELESS;
                case RHI_FORMAT::FORMAT_R10G10B10A2_UNORM:
                    return DXGI_FORMAT_R10G10B10A2_UNORM;
                case RHI_FORMAT::FORMAT_R10G10B10A2_UINT:
                    return DXGI_FORMAT_R10G10B10A2_UINT;
                case RHI_FORMAT::FORMAT_R11G11B10_FLOAT:
                    return DXGI_FORMAT_R11G11B10_FLOAT;
                case RHI_FORMAT::FORMAT_R8G8B8A8_TYPELESS:
                    return DXGI_FORMAT_R8G8B8A8_TYPELESS;
                case RHI_FORMAT::FORMAT_R8G8B8A8_UNORM:
                    return DXGI_FORMAT_R8G8B8A8_UNORM;
                case RHI_FORMAT::FORMAT_R8G8B8A8_UINT:
                    return DXGI_FORMAT_R8G8B8A8_UINT;
                case RHI_FORMAT::FORMAT_R16G16_TYPELESS:
                    return DXGI_FORMAT_R16G16_TYPELESS;
                case RHI_FORMAT::FORMAT_R16G16_FLOAT:
                    return DXGI_FORMAT_R16G16_FLOAT;
                case RHI_FORMAT::FORMAT_R16G16_UNORM:
                    return DXGI_FORMAT_R16G16_UNORM;
                case RHI_FORMAT::FORMAT_R16G16_UINT:
                    return DXGI_FORMAT_R16G16_UINT;
                case RHI_FORMAT::FORMAT_R32_TYPELESS:
                    return DXGI_FORMAT_R32_TYPELESS;
                case RHI_FORMAT::FORMAT_D32_FLOAT:
                    return DXGI_FORMAT_D32_FLOAT;
                case RHI_FORMAT::FORMAT_R32_FLOAT:
                    return DXGI_FORMAT_R32_FLOAT;
                case RHI_FORMAT::FORMAT_R32_UINT:
                    return DXGI_FORMAT_R32_UINT;
                case RHI_FORMAT::FORMAT_D24_UNORM_S8_UINT:
                    return DXGI_FORMAT_D24_UNORM_S8_UINT;
                case RHI_FORMAT::FORMAT_R8G8_TYPELESS:
                    return DXGI_FORMAT_R8G8_TYPELESS;
                case RHI_FORMAT::FORMAT_R8G8_UNORM:
                    return DXGI_FORMAT_R8G8_UNORM;
                case RHI_FORMAT::FORMAT_R8G8_UINT:
                    return DXGI_FORMAT_R8G8_UINT;
                case RHI_FORMAT::FORMAT_R16_TYPELESS:
                    return DXGI_FORMAT_R16_TYPELESS;
                case RHI_FORMAT::FORMAT_D16_UNORM:
                    return DXGI_FORMAT_D16_UNORM;
                case RHI_FORMAT::FORMAT_R16_FLOAT:
                    return DXGI_FORMAT_R16_FLOAT;
                case RHI_FORMAT::FORMAT_R16_UNORM:
                    return DXGI_FORMAT_R16_UNORM;
                case RHI_FORMAT::FORMAT_R16_UINT:
                    return DXGI_FORMAT_R16_UINT;
                case RHI_FORMAT::FORMAT_R8_TYPELESS:
                    return DXGI_FORMAT_R8_TYPELESS;
                case RHI_FORMAT::FORMAT_R8_UNORM:
                    return DXGI_FORMAT_R8_UNORM;
                case RHI_FORMAT::FORMAT_R8_UINT:
                    return DXGI_FORMAT_R8_UINT;
                default:
                    return DXGI_FORMAT_UNKNOWN;
                }
            }

            D3D11_PRIMITIVE_TOPOLOGY RHI_DX11::GetTopologyType(
                    RHI_PRIMITIVE_TYPE type_a)
            {
                switch (type_a)
                {
                case RHI_PRIMITIVE_TYPE::PRIMITIVE_TOPOLOGY_POINTLIST:
                    return D3D11_PRIMITIVE_TOPOLOGY::
                            D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
                case RHI_PRIMITIVE_TYPE::PRIMITIVE_TOPOLOGY_LINELIST:
                    return D3D11_PRIMITIVE_TOPOLOGY::
                            D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
                case RHI_PRIMITIVE_TYPE::PRIMITIVE_TOPOLOGY_TRIANGLELIST:
                    return D3D11_PRIMITIVE_TOPOLOGY::
                            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                case RHI_PRIMITIVE_TYPE::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
                    return D3D11_PRIMITIVE_TOPOLOGY::
                            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
                case RHI_PRIMITIVE_TYPE::
                        PRIMITIVE_TOPOLOGY_CONTROL_POINT_PATCHLIST:
                    return D3D11_PRIMITIVE_TOPOLOGY::
                            D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
                default: {
                    assert(0);
                    return D3D11_PRIMITIVE_TOPOLOGY::
                            D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
                }
                }
            }

            RHI_PRIMITIVE_TYPE RHI_DX11::GetTopologyType(
                    D3D11_PRIMITIVE_TOPOLOGY type_a)
            {
                switch (type_a)
                {
                case D3D11_PRIMITIVE_TOPOLOGY::
                        D3D11_PRIMITIVE_TOPOLOGY_POINTLIST:
                    return RHI_PRIMITIVE_TYPE::PRIMITIVE_TOPOLOGY_POINTLIST;
                case D3D11_PRIMITIVE_TOPOLOGY::
                        D3D11_PRIMITIVE_TOPOLOGY_LINELIST:
                    return RHI_PRIMITIVE_TYPE::PRIMITIVE_TOPOLOGY_LINELIST;
                case D3D11_PRIMITIVE_TOPOLOGY::
                        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
                    return RHI_PRIMITIVE_TYPE::PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                case D3D11_PRIMITIVE_TOPOLOGY::
                        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
                    return RHI_PRIMITIVE_TYPE::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
                case D3D11_PRIMITIVE_TOPOLOGY::
                        D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST:
                    return RHI_PRIMITIVE_TYPE::
                            PRIMITIVE_TOPOLOGY_CONTROL_POINT_PATCHLIST;
                default: {
                    assert(0);
                    return RHI_PRIMITIVE_TYPE::PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                }
                }
            }

            void RHI_DX11::CreateSamplerStates()
            {
                float white_[4] = {1.0f, 1.0f, 1.0f, 1.0f};
                float black_[4] = {0.0f, 0.0f, 0.0f, 0.0f};

                auto direct3d_device_ = direct3d_device_;

                D3D11_SAMPLDESC sampler_state_desc_;
                ZeroMemory(&sampler_state_desc_, sizeof(sampler_state_desc_));
                sampler_state_desc_.Filter = D3D11_FILTMIN_MAG_MIP_LINEAR;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
                sampler_state_desc_.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
                sampler_state_desc_.MipLODBias = 0;
                sampler_state_desc_.MaxAnisotropy = 1;
                sampler_state_desc_.MinLOD = -1000.0f;
                sampler_state_desc_.MaxLOD = 1000.0f;
                HRESULT hr_ =
                        direct3d_device_->CreateSamplerState(&sampler_state_desc_,
                                                           &trilinear_wrap_ss_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: Could not create TrilinearWrapSS",
                            hr_);
                sampler_states_.insert(
                        std::make_pair(RHI_SAMPLSTATE::TRILINEAR_WRAP,
                                       trilinear_wrap_ss_));

                ZeroMemory(&sampler_state_desc_, sizeof(sampler_state_desc_));
                sampler_state_desc_.Filter = D3D11_FILTMIN_MAG_MIP_LINEAR;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
                sampler_State_desc_.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
                hr_ = direct3d_device_->CreateSamplerState(&sampler_state_desc_,
                                                        &trilinear_mirror_ss_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: Could not create TrilinearMirrorSS",
                            hr_);
                sampler_states_.insert(
                        std::make_pair(RHI_SAMPLSTATE::TRILINEAR_MIRROR,
                                       trilinear_mirror_ss_));

                ZeroMemory(&sampler_state_desc_, sizeof(sampler_state_desc_));
                sampler_state_desc_.Filter = D3D11_FILTMIN_MAG_MIP_LINEAR;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
                sampler_state_desc_.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
                hr_ = direct3d_device_->CreateSamplerState(&sampler_state_desc_,
                                                        &TrilinearClampSS);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: Could not create TrilinearClampSS",
                            hr_);
                sampler_states.insert(
                        std::make_pair(RHI_SAMPLSTATE::TRILINEAR_CLAMP,
                                       trilinear_clamp_ss_));

                ZeroMemory(&sampler_state_desc_, sizeof(sampler_state_desc_));
                sampler_state_desc_.Filter = D3D11_FILTMIN_MAG_MIP_LINEAR;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
                sampler_state_desc_.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
                memcpy(sampler_state_desc_.BorderColor,
                       reinterpret_cast<float*>(&black_),
                       sizeof(float) * 4);
                
                hr_ = direct3d_device_->CreateSamplerState(&sampler_state_desc_,
                                                        &trilinear_border_ss_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: Could not create TrilinearBorderSS",
                            hr_);
                sampler_states_.insert(
                        std::make_pair(RHI_SAMPLSTATE::TRILINEAR_BORDER,
                                       trilinear_border_ss_));

                ZeroMemory(&sampler_state_desc_, sizeof(sampler_state_desc_));
                sampler_state_desc_.Filter = D3D11_FILTMIN_MAG_LINEAR_MIP_POINT;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
                sampler_state_desc_.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
                hr_ = direct3d_device_->CreateSamplerState(&sampler_state_desc_,
                                                        &bilinear_wrap_ss_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: Could not create BilinearWrapSS",
                            hr_);
                sampler_states_.insert(
                        std::make_pair(RHI_SAMPLSTATE::BILINEAR_WRAP,
                                       bilinear_wrap_ss_));

                ZeroMemory(&sampler_state_desc_, sizeof(sampler_state_desc_));
                sampler_state_desc_.Filter = D3D11_FILTMIN_MAG_LINEAR_MIP_POINT;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
                sampler_state_desc_.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
                hr_ = direct3d_device_->CreateSamplerState(&sampler_state_desc_,
                                                        &bilinear_mirror_ss_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: Could not create BilinearMirrorSS",
                            hr_);
                sampler_states_.insert(
                        std::make_pair(RHI_SAMPLSTATE::BILINEAR_MIRROR,
                                       bilinear_mirror_ss_));

                ZeroMemory(&sampler_state_desc_, sizeof(sampler_state_desc_));
                sampler_state_desc_.Filter = D3D11_FILTMIN_MAG_LINEAR_MIP_POINT;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
                sampler_state_desc_.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
                hr_ = direct3DDevice->CreateSamplerState(&sampler_state_desc_,
                                                         &bilinear_clamp_ss_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: Could not create BilinearClampSS",
                            hr_);
                sampler_states_.insert(
                        std::make_pair(RHI_SAMPLSTATE::BILINEAR_CLAMP,
                                       bilinear_clamp_ss_));

                ZeroMemory(&sampler_state_desc_, sizeof(sampler_state_desc_));
                sampler_state_desc_.Filter = D3D11_FILTMIN_MAG_LINEAR_MIP_POINT;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
                sampler_state_desc_.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
                memcpy(sampler_state_desc_.BorderColor,
                       reinterpret_cast<FLOAT*>(&black_),
                       sizeof(float) * 4);
                hr_ = direct3d_device_->CreateSamplerState(&sampler_state_desc_,
                                                           &bilinear_border_ss_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: Could not create BilinerBorderSS",
                            hr_);
                sampler_states_.insert(
                        std::make_pair(RHI_SAMPLSTATE::BILINEAR_BORDER,
                                       bilinear_border_ss_));

                ZeroMemory(&sampler_state_desc_, sizeof(sampler_state_desc_));
                sampler_state_desc_.Filter = D3D11_FILTANISOTROPIC;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
                sampler_state_desc_.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
                hr_ = direct3d_device_->CreateSamplerState(&sampler_state_desc_,
                                                           &anisotropic_wrap_ss_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: Could not create AnisotropicWrapSS",
                            hr_);
                sampler_states_.insert(
                        std::make_pair(RHI_SAMPLSTATE::ANISOTROPIC_WRAP,
                                       anisotropic_wrap_ss_));

                ZeroMemory(&sampler_state_desc_, sizeof(sampler_state_desc_));
                sampler_state_desc_.Filter = D3D11_FILTANISOTROPIC;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
                sampler_state_desc_.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
                hr_ = direct3d_device_->CreateSamplerState(&sampler_state_desc_,
                                                        &anisotropic_mirror_ss_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: Could not create AnisotropicMirrorSS",
                            hr_);
                sampler_states_.insert(
                        std::make_pair(RHI_SAMPLSTATE::ANISOTROPIC_MIRROR,
                                       anisotropic_mirror_ss_));

                ZeroMemory(&sampler_state_desc_, sizeof(sampler_state_desc_));
                sampler_state_desc_.Filter = D3D11_FILTANISOTROPIC;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
                sampler_state_desc_.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
                hr = direct3DDevice->CreateSamplerState(&sampler_state_desc_,
                                                        &anisotropic_clamp_ss_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: Could not create AnisotropicClampSS",
                            hr_);
                sampler_states_.insert(
                        std::make_pair(RHI_SAMPLSTATE::ANISOTROPIC_CLAMP,
                                       anisotropic_clamp_ss_));

                ZeroMemory(&sampler_state_desc_, sizeof(sampler_state_desc_));
                sampler_state_desc_.Filter = D3D11_FILTANISOTROPIC;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
                sampler_state_desc_.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
                memcpy(sampler_state_desc_.BorderColor,
                       reinterpret_cast<float*>(&black_),
                       sizeof(float) * 4);
                hr_ = direct3d_device_->CreateSamplerState(&sampler_state_desc_,
                                                           &anisotropic_border_ss_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: Could not create AnisotropicBorderSS",
                            hr_);
                sampler_states_.insert(
                        std::make_pair(RHI_SAMPLSTATE::ANISOTROPIC_BORDER,
                                       anisotropic_border_ss_));

                sampler_state_desc_.Filter =
                        D3D11_FILTCOMPARISON_MIN_MAG_LINEAR_MIP_POINT;
                sampler_state_desc_.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
                sampler_state_desc_.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
                sampler_state_desc_.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
                memcpy(sampler_state_desc_.BorderColor,
                       reinterpret_cast<float*>(&white_),
                       sizeof(float) * 4);
                sampler_state_desc_.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
                if (FAILED(direct3d_device_->CreateSamplerState(&sampler_state_desc_,
                                                              &shadow_ss_)))
                    throw std::esception("RHI_DX11: Could not create ShadowSS!");
                        sampler_states_.insert(
                        std::make_pair(RHI_SAMPLSTATE::SHADOW_SS, shadow_ss_));
            }

            void RHI_DX11::CreateBlendStates()
            {
                D3D11_BLEND_DESC blend_state_description_;
                ZeroMemory(&blend_state_description_, sizeof(D3D11_BLEND_DESC));

                blend_state_description_.RenderTarget[0].BlendEnable = TRUE;
                blend_state_description_.RenderTarget[0].SrcBlend =
                        D3D11_BLEND_SRC_ALPHA;
                blend_state_description_.RenderTarget[0].DestBlend =
                        D3D11_BLEND_INV_SRC_ALPHA;
                blend_state_description_.RenderTarget[0].BlendOp =
                        D3D11_BLEND_OP_ADD;
                blend_state_description_.RenderTarget[0].SrcBlendAlpha =
                        D3D11_BLEND_SRC_ALPHA;
                blend_state_description_.RenderTarget[0].DestBlendAlpha =
                        D3D11_BLEND_INV_SRC_ALPHA;
                blend_state_description_.RenderTarget[0].BlendOpAlpha =
                        D3D11_BLEND_OP_ADD;
                blend_state_description_.RenderTarget[0].RenderTargetWriteMask =
                        D3D11_COLOR_WRITE_ENABLE_RED
                        | D3D11_COLOR_WRITE_ENABLE_GREEN
                        | D3D11_COLOR_WRITE_ENABLE_BLUE;
                if (FAILED(direct3d_device_->CreateBlendState(
                            &blend_state_description_,
                            &alpha_blend_state_)))
                    throw std::exception(
                            "RHI_DX11: ID3D11Device::CreateBlendState() "
                            "failed "
                            "while create alpha blend state.");
                blend_states_.insert(std::make_pair(RHI_BLEND_STATE::ALPHA_BLEND,
                                                    alpha_blend_state_));

                blend_state_description_.RenderTarget[0].RenderTargetWriteMask =
                        D3D11_COLOR_WRITE_ENABLE_ALL;
                blend_state_description_.AlphaToCoverageEnable = TRUE;
                blend_state_description_.RenderTarget[0].SrcBlend =
                        D3D11_BLEND_ONE;
                blend_state_description_.RenderTarget[0].SrcBlendAlpha =
                        D3D11_BLEND_ONE;
                blend_state_description_.RenderTarget[0].DestBlendAlpha =
                        D3D11_BLEND_ZERO;
                if (FAILED(direct3d_device_->CreateBlendState(
                            &blend_state_description_,
                            &alpha_to_coverage_state_)))
                    throw std::exception(
                            "RHI_DX11: ID3D11Device::CreateBlendState() "
                            "failed "
                            "while create alpha-to-coverage blend state.");
                blend_states_.insert(
                        std::make_pair(RHI_BLEND_STATE::ALPHA_TO_COVERAGE,
                                       alpha_to_coverage_state_));

                {
                    D3D11_BLEND_DESC blend_state_description_multi_target_ =
                            blend_state_description_;
                    blend_state_description_multi_target_.IndependentBlendEnable =
                            TRUE;
                    int i;
                    for (i = 0; i < 4; i++)
                        blend_state_description_multi_target_.RenderTarget[i] =
                                blend_state_description_MultiTarget
                                        .RenderTarget[0];
                    for (i = 4; i < D3D11_SIMULTANEOUS_RENDTARGET_COUNT; i++)
                    {
                        blend_state_description_multi_target_.RenderTarget[i]
                                .BlendEnable = FALSE;
                        blend_state_description_multi_target_.RenderTarget[i]
                                .RenderTargetWriteMask =
                                D3D11_COLOR_WRITE_ENABLE_RED
                                | D3D11_COLOR_WRITE_ENABLE_GREEN
                                | D3D11_COLOR_WRITE_ENABLE_BLUE;
                    }

                    if (FAILED(direct3d_device_->CreateBlendState(
                                &blend_state_description_multi_target_,
                                &alpha_to_coverage_state_4_targets_)))
                        throw std::exception(
                                "RHI_DX11: ID3D11Device::CreateBlendState() "
                                "failed "
                                "while create alpha-to-coverage 4 targets "
                                "blend "
                                "state.");
                    blend_states_.insert(std::make_pair(
                            RHI_BLEND_STATE::ALPHA_TO_COVERAGE_4_TARGETS,
                            alpha_to_coverage_state_4_targets_));
                }

                blend_state_description_.RenderTarget[0].BlendEnable = FALSE;
                blend_state_description_.AlphaToCoverageEnable = FALSE;
                if (FAILED(direct3d_device_->CreateBlendState(
                            &blend_state_description_,
                            &no_blend_state_)))
                    throw std::exception(
                            "RHI_DX11: ID3D11Device::CreateBlendState() "
                            "failed "
                            "while create no blend state.");
                blend_states_.insert(std::make_pair(RHI_BLEND_STATE::NO_BLEND,
                                                    no_blend_state_));
            }

            void RHI_DX11::CreateRasterizerStates()
            {
                D3D11_RASTERIZDESC rasterizer_state_desc_;
                ZeroMemory(&rasterizer_state_desc_, sizeof(rasterizer_state_desc_));
                rasterizer_state_desc_.FillMode = D3D11_FILL_SOLID;
                rasterizer_state_desc_.CullMode = D3D11_CULL_BACK;
                rasterizer_state_desc_.DepthClipEnable = true;
                HRESULT hr_ = direct3d_device_->CreateRasterizerState(
                        &rasterizer_state_desc_,
                        &back_culling_rs_);
                
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: CreateRasterizerStates() failed when "
                            "creating BackCullingRS.",
                            hr_);
                rasterizer_states_.insert(
                        std::make_pair(RHI_RASTERIZSTATE::BACK_CULLING,
                                       back_culling_rs_));

                ZeroMemory(&rasterizer_state_desc_, sizeof(rasterizer_state_desc_));
                rasterizer_state_desc_.FillMode = D3D11_FILL_SOLID;
                rasterizer_state_desc_.CullMode = D3D11_CULL_BACK;
                rasterizer_state_desc_.FrontCounterClockwise = true;
                rasterizer_state_desc_.DepthClipEnable = true;
                hr_ = direct3d_device_->CreateRasterizerState(
                        &rasterizer_state_desc_,
                        &front_culling_rs_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: CreateRasterizerStates() failed when "
                            "creating FrontCullingRS.",
                            hr_);
                rasterizer_states_.insert(
                        std::make_pair(RHI_RASTERIZSTATE::FRONT_CULLING,
                                       front_culling_rs_));

                ZeroMemory(&rasterizer_state_desc_, sizeof(rasterizer_state_desc_));
                rasterizer_state_desc_.FillMode = D3D11_FILL_SOLID;
                rasterizer_state_desc_.CullMode = D3D11_CULL_NONE;
                rasterizer_state_desc_.DepthClipEnable = true;
                hr_ = direct3d_device_->CreateRasterizerState(
                        &rasterizer_state_desc_,
                        &no_culling_rs_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: CreateRasterizerStates() failed when "
                            "creating NoCullingRS.",
                            hr_);
                
                rasterizer_states_.insert(
                        std::make_pair(RHI_RASTERIZSTATE::NO_CULLING,
                                       no_culling_rs_));

                ZeroMemory(&rasterizer_state_desc_, sizeof(rasterizer_state_desc_));
                rasterizer_state_desc_.FillMode = D3D11_FILL_WIREFRAME;
                rasterizer_state_desc_.CullMode = D3D11_CULL_NONE;
                rasterizer_state_desc_.DepthClipEnable = true;
                hr_ = direct3d_device_->CreateRasterizerState(
                        &rasterizer_state_desc_,
                        &wireframe_rs_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: CreateRasterizerStates() failed when "
                            "creating WireframeRS.",
                            hr_);
                rasterizer_states_.insert(
                        std::make_pair(RHI_RASTERIZSTATE::WIREFRAME,
                                       wireframe_rs_));

                ZeroMemory(&rasterizer_state_desc_, sizeof(rasterizer_state_desc_));
                rasterizer_state_desc_.FillMode = D3D11_FILL_SOLID;
                rasterizer_state_desc_.CullMode = D3D11_CULL_NONE;
                rasterizer_state_desc_.DepthClipEnable = false;
                rasterizer_state_desc_.ScissorEnable = true;
                hr_ = direct3d_device_->CreateRasterizerState(
                        &rasterizer_state_desc_,
                        &no_culling_no_depth_enabled_scissor_rs_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: CreateRasterizerStates() failed when "
                            "creating NoCullingNoDepthEnabledScissorRS.",
                            hr_);
                rasterizer_states_.insert(std::make_pair(
                        RHI_RASTERIZSTATE::NO_CULLING_NO_DEPTH_SCISSOR_ENABLED,
                        no_culling_no_depth_enabled_scissor_rs_));

                ZeroMemory(&rasterizer_state_desc_, sizeof(rasterizer_state_desc_));
                rasterizer_state_desc_.FillMode = D3D11_FILL_SOLID;
                rasterizer_state_desc_.CullMode = D3D11_CULL_BACK;
                rasterizer_state_desc_.DepthClipEnable = false;
                rasterizer_state_desc_.DepthBias = 0; // 0.05f
                rasterizer_state_desc_.SlopeScaledDepthBias = 3.0f;
                rasterizer_state_desc_.FrontCounterClockwise = false;
                hr_ = direct3d_device_->CreateRasterizerState(
                        &rasterizer_state_desc_,
                        &shadow_rs_);
                if (FAILED(hr_))
                    throw std::exception(
                            "RHI_DX11: CreateRasterizerStates() failed when "
                            "creating ShadowRS.",
                            hr_);
                rasterizer_states_.insert(
                        std::make_pair(RHI_RASTERIZSTATE::SHADOW_RS, shadow_rs_));
            }

            void RHI_DX11::CreateDepthStencilStates()
            {
                D3D11_DEPTH_STENCIL_DESC depth_stencil_state_desc_;
                depth_stencil_state_desc_.DepthEnable = TRUE;
                depth_stencil_state_desc_.DepthWriteMask =
                        D3D11_DEPTH_WRITE_MASK_ZERO;
                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_NEVER;
                depth_stencil_state_desc_.StencilEnable = FALSE;
                HRESULT hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_read_comparison_never_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyReadComparisonNeverDS.",
                            hr_);
                depth_states_.insert(
                        std::make_pair(RHI_DEPTH_STENCIL_STATE::
                                               DEPTH_ONLY_READ_COMPARISON_NEVER,
                                       depth_only_read_comparison_never_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_LESS;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_read_comparison_less_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyReadComparisonLessDS.",
                            hr_);
                depth_states_.insert(
                        std::make_pair(RHI_DEPTH_STENCIL_STATE::
                                               DEPTH_ONLY_READ_COMPARISON_LESS,
                                       depth_only_read_comparison_less_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_EQUAL;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_read_comparison_equal_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyReadComparisonEqualDS.",
                            hr_);
                depth_states_.insert(
                        std::make_pair(RHI_DEPTH_STENCIL_STATE::
                                               DEPTH_ONLY_READ_COMPARISON_EQUAL,
                                       depth_only_read_comparison_equal_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_read_comparison_less_equal_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyReadComparisonLessEqualDS.",
                            hr_);
                depth_states_.insert(std::make_pair(
                        RHI_DEPTH_STENCIL_STATE::
                                DEPTH_ONLY_READ_COMPARISON_LESS_EQUAL,
                        depth_only_read_comparison_less_equal_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_GREATER;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_read_comparison_greater_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyReadComparisonGreaterDS.",
                            hr_);
                depth_states_.insert(std::make_pair(
                        RHI_DEPTH_STENCIL_STATE::
                                DEPTH_ONLY_READ_COMPARISON_GREATER,
                        depth_only_read_comparison_greater_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_read_comparison_not_equal_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyReadComparisonNotEqualDS.",
                            hr_);
                depth_states_.insert(std::make_pair(
                        RHI_DEPTH_STENCIL_STATE::
                                DEPTH_ONLY_READ_COMPARISON_NOT_EQUAL,
                        depth_only_read_comparison_not_equal_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_GREATEQUAL;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_read_comparison_greater_equal_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyReadComparisonGreaterEqualDS.",
                            hr_);
                depth_states_.insert(std::make_pair(
                        RHI_DEPTH_STENCIL_STATE::
                                DEPTH_ONLY_READ_COMPARISON_GREATEQUAL,
                        depth_only_read_comparison_greater_equal_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_ALWAYS;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_read_comparison_always_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyReadComparisonAlwaysDS.",
                            hr_);
                depth_states_.insert(std::make_pair(
                        RHI_DEPTH_STENCIL_STATE::
                                DEPTH_ONLY_READ_COMPARISON_ALWAYS,
                        depth_only_read_comparison_always_ds_));

                depth_stencil_state_desc_.DepthWriteMask =
                        D3D11_DEPTH_WRITE_MASK_ALL;
                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_LESS;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_write_comparison_less_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyWriteComparisonLessDS.",
                            hr_);
                depth_states_.insert(
                        std::make_pair(RHI_DEPTH_STENCIL_STATE::
                                               DEPTH_ONLY_WRITE_COMPARISON_LESS,
                                       depth_only_write_comparison_less_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_EQUAL;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_write_comparison_equal_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyWriteComparisonEqualDS.",
                            hr_);
                depth_states_.insert(std::make_pair(
                        RHI_DEPTH_STENCIL_STATE::
                                DEPTH_ONLY_WRITE_COMPARISON_EQUAL,
                        depth_only_write_comparison_equal_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_write_comparison_less_equal_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyWriteComparisonLessEqualDS.",
                            hr_);
                depth_states_.insert(std::make_pair(
                        RHI_DEPTH_STENCIL_STATE::
                                DEPTH_ONLY_WRITE_COMPARISON_LESS_EQUAL,
                        depth_only_write_comparison_less_equal_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_GREATER;
                hr = mDirect3DDevice->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_write_comparison_greater_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyWriteComparisonGreaterDS.",
                            hr_);
                depth_states_.insert(std::make_pair(
                        RHI_DEPTH_STENCIL_STATE::
                                DEPTH_ONLY_WRITE_COMPARISON_GREATER,
                        depth_only_write_comparison_greater_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_write_comparison_not_equal_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyWriteComparisonNotEqualDS.",
                            hr_);
                depth_states_.insert(std::make_pair(
                        RHI_DEPTH_STENCIL_STATE::
                                DEPTH_ONLY_WRITE_COMPARISON_NOT_EQUAL,
                        depth_only_write_comparison_not_equal_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_GREATEQUAL;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_write_comparison_greater_equal_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyWriteComparisonGreaterEqualDS.",
                            hr_);
                depth_states_.insert(std::make_pair(
                        RHI_DEPTH_STENCIL_STATE::
                                DEPTH_ONLY_WRITE_COMPARISON_GREATEQUAL,
                        depth_only_write_comparison_greater_equal_ds_));

                depth_stencil_state_desc_.DepthFunc = D3D11_COMPARISON_ALWAYS;
                hr_ = direct3d_device_->CreateDepthStencilState(
                        &depth_stencil_state_desc_,
                        &depth_only_write_comparison_always_ds_);
                if (FAILED(hr_))
                    throw std::exception(
                            "CreateDepthStencilState() failed when creating "
                            "DepthOnlyWriteComparisonAlwaysDS.",
                            hr_);
                depth_states_.insert(std::make_pair(
                        RHI_DEPTH_STENCIL_STATE::
                                DEPTH_ONLY_WRITE_COMPARISON_ALWAYS,
                        depth_only_write_comparison_always_ds_));
            }
        } // namespace RHI
    } // namespace Graphics
} // namespace CE_Kernel
