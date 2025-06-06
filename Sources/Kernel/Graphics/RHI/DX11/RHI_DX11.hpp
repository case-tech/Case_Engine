// Copyright (c) 2025 Case Technologies

#pragma once
#include "..\RHI.hpp"

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace RHI
        {
            class RHI_DX11_InputLayout : public RHI_InputLayout
            {
            public:
                RHI_DX11_InputLayout(
                        RHI_INPUT_ELEMENT_DESC* input_element_descriptions_a,
                        unsigned int input_element_description_count_a)
                    : RHI_InputLayout(input_element_descriptions_a,
                                      input_element_description_count_a) {}
                
                virtual ~RHI_DX11_InputLayout()
                {
                    ReleaseObject(input_layout_);
                };
                
                ID3D11InputLayout* input_layout_ = nullptr;
            };

            class RHI_DX11 : public RHI
            {
            public:
                RHI_DX11();
                virtual ~RHI_DX11();

                virtual bool Initialize(void* window_handle_a,
                                        unsigned int width_a,
                                        unsigned int height_a,
                                        bool is_fullscreen_a,
                                        bool is_reset_a = false) override;

                virtual void BeginGraphicsCommandList(int index_a = 0) override {};
                virtual void EndGraphicsCommandList(int index_a = 0) override {}; 

                virtual void BeginComputeCommandList(int index_a = 0) override {};
                virtual void EndComputeCommandList(int index_a = 0) override {};

                virtual void BeginCopyCommandList(int index_a = 0) override {};
                virtual void EndCopyCommandList(int index_a = 0) override {};

                virtual void ClearMainRenderTarget(float colors_a[4]) override;
                virtual void ClearMainDepthStencilTarget(
                        float depth_a,
                        unsigned int stencil_a = 0) override;
                
                virtual void ClearRenderTarget(RHI_GPUTexture* render_target_a,
                                               float colors_a[4],
                                               int rtv_array_index_a = -1) override;
                
                virtual void ClearDepthStencilTarget(
                        RHI_GPUTexture* depth_target_a,
                        float depth_a,
                        unsigned int stencil_a = 0) override;
                
                virtual void ClearUAV(RHI_GPUResource* render_target_a,
                                      float colors_a[4]) override;
                
                virtual void ClearUAV(RHI_GPUBuffer* buffer_a,
                                      unsigned int clear_a) override;

                virtual RHI_GPUShader* CreateGPUShader() override;
                virtual RHI_GPUBuffer* CreateGPUBuffer(
                        const std::string& debug_name_a) override;
                
                virtual RHI_GPUTexture* CreateGPUTexture(
                        const std::wstring& debug_name_a) override;
                
                virtual RHI_GPURootSignature* CreateRootSignature(
                        unsigned int num_root_params_a = 0,
                        unsigned int num_static_samplers_a = 0) override
                {
                    return nullptr;
                }
                
                virtual RHI_InputLayout* CreateInputLayout(
                        RHI_INPUT_ELEMENT_DESC* input_element_descriptions_a,
                        unsigned int input_element_description_count_a) override;
                
                void CreateInputLayout(
                        RHI_InputLayout* out_input_layout_a,
                        RHI_INPUT_ELEMENT_DESC* input_element_descriptions_a,
                        unsigned int input_element_description_count_a,
                        const void* shader_byte_code_with_input_signature_a,
                        unsigned int byte_code_length_a);

                virtual void CreateTexture(
                        RHI_GPUTexture* out_texture_a,
                        unsigned int width_a,
                        unsigned int height_a,
                        unsigned int samples_a,
                        RHI_FORMAT format_a,
                        RHI_BIND_FLAG bind_flags_a = BIND_NONE,
                        int mip_a = 1,
                        int depth_a = -1,
                        int array_size_a = 1,
                        bool is_cubemap_a = false,
                        int cubemap_array_size_a = -1) override;
                
                virtual void CreateTexture(RHI_GPUTexture* out_texture_a,
                                           const std::string& path_a,
                                           bool is_full_path_a = false) override;
                
                virtual void CreateTexture(RHI_GPUTexture* out_texture_a,
                                           const std::wstring& path_a,
                                           bool is_full_path_a = false) override;

                virtual void CreateBuffer(
                        RHI_GPUBuffer* out_buffer_a,
                        void* data_a,
                        UINT objects_count_a,
                        UINT byte_stride_a,
                        bool is_dynamic_a = false,
                        RHI_BIND_FLAG bind_flags_a = BIND_NONE,
                        UINT cpu_access_flags_a = 0,
                        RHI_RESOURCE_MISC_FLAG misc_flags_a =
                                RESOURCE_MISC_NONE,
                        RHI_FORMAT format_a = FORMAT_UNKNOWN) override;
                
                virtual void CopyBuffer(RHI_GPUBuffer* dest_buffer_a,
                                        RHI_GPUBuffer* src_buffer_a,
                                        int cmd_list_index_a,
                                        bool is_in_copy_queue_a = false) override;
                
                virtual void BeginBufferRead(RHI_GPUBuffer* buffer_a,
                                             void** output_a) override;
                
                virtual void EndBufferRead(RHI_GPUBuffer* buffer_a) override;

                virtual void CopyGPUTextureSubresourceRegion(
                        RHI_GPUResource* dest_buffer_a,
                        unsigned int dst_subresource_a,
                        unsigned int dst_x_a,
                        unsigned int dst_y_a,
                        unsigned int dst_z_a,
                        RHI_GPUResource* src_buffer_a,
                        unsigned int src_subresource_a,
                        bool is_in_copy_queue_or_skip_transitions_a = false) override;

                virtual void Draw(unsigned int vertex_count_a) override;
                virtual void DrawIndexed(unsigned int index_count_a) override;
                virtual void DrawInstanced(unsigned int vertex_count_per_instance_a,
                                           unsigned int instance_count_a,
                                           unsigned int start_vertex_location_a,
                                           unsigned int start_instance_location_a) override;
                
                virtual void DrawIndexedInstanced(
                        unsigned int index_count_per_instance_a,
                        unsigned int instance_count_a,
                        unsigned int start_index_location_a,
                        int base_vertex_location_a,
                        unsigned int start_instance_location_a) override;
                
                virtual void DrawIndexedInstancedIndirect(
                        RHI_GPUBuffer* an_args_buffer_a,
                        UINT aligned_byte_offset_a) override;

                virtual void Dispatch(unsigned int thread_group_count_x_a,
                                      unsigned int thread_group_count_y_a,
                                      unsigned int thread_group_count_z_a) override;

                virtual void ExecuteCommandLists(int command_list_index_a= 0,
                                                 bool is_compute_a = false) override {};
                
                virtual void ExecuteCopyCommandList() override {};

                virtual void GenerateMips(
                        RHI_GPUTexture* texture_a,
                        RHI_GPUTexture* srgb_texture_a = nullptr) override;
                
                virtual void GenerateMipsWithTextureReplacement(
                        RHI_GPUTexture** texture_a,
                        std::function<void(RHI_GPUTexture**)>
                                aReplacementCallback) override {};
                
                virtual void ReplaceOriginalTexturesWithMipped() override {};

                virtual void PresentGraphics() override;
                virtual void PresentCompute() override {};

                virtual bool ProjectCubemapToSH(RHI_GPUTexture* texture_a,
                                                unsigned int order_a,
                                                float* result_r_a,
                                                float* result_g_a,
                                                float* result_b_a) override;

                virtual void SaveGPUTextureToFile(
                        RHI_GPUTexture* texture_a,
                        const std::wstring& path_name_a) override;

                virtual void SetMainRenderTargets(
                        int cmd_list_index_a = 0) override;
                
                virtual void SetRenderTargets(
                        const std::vector<RHI_GPUTexture*>& render_targets_a,
                        RHI_GPUTexture* depth_target_a = nullptr,
                        RHI_GPUTexture* uav_a = nullptr,
                        int rtv_array_index_a = -1) override;
                
                virtual void SetDepthTarget(
                        RHI_GPUTexture* depth_target_a) override;
                
                virtual void SetRenderTargetFormats(
                        const std::vector<RHI_GPUTexture*>& render_targets_a,
                        RHI_GPUTexture* depth_target_a = nullptr) override {};
                
                virtual void SetMainRenderTargetFormats() override {};

                virtual void SetDepthStencilState(
                        RHI_DEPTH_STENCIL_STATE ds_a,
                        unsigned int stencil_ref_a = 0xffffffff) override;
                
                virtual void SetBlendState(
                        RHI_BLEND_STATE bs_a,
                        const float blend_factor_a[4] = nullptr,
                        unsigned int sample_mask_a = 0xffffffff) override;
                
                virtual void SetRasterizerState(
                        RHI_RASTERIZSTATE rs_a) override;

                virtual void SetViewport(
                        const RHI_Viewport& viewport_a) override;
                
                virtual void SetRect(const RHI_Rect& rect_a) override;

                virtual void SetShaderResources(
                        RHI_SHADTYPE shader_type_a,
                        const std::vector<RHI_GPUResource*>& srvs_a,
                        unsigned int start_slot_a = 0,
                        RHI_GPURootSignature* rs_a = nullptr,
                        int root_param_index_a = -1,
                        bool is_compute_rs_a = false,
                        bool skip_automatic_transition_a = false) override;
                
                virtual void SetUnorderedAccessResources(
                        RHI_SHADTYPE shader_type_a,
                        const std::vector<RHI_GPUResource*>& uavs_a,
                        unsigned int start_slot_a = 0,
                        RHI_GPURootSignature* rs_a = nullptr,
                        int root_param_index_a = -1,
                        bool is_compute_rs_a = false,
                        bool skip_automatic_transition_a = false) override;
                
                virtual void SetConstantBuffers(
                        RHI_SHADTYPE shader_type_a,
                        const std::vector<RHI_GPUBuffer*>& cbs_a,
                        unsigned int start_slot_a = 0,
                        RHI_GPURootSignature* rs_a = nullptr,
                        int root_param_index_a = -1,
                        bool is_compute_rs_a = false) override;
                
                virtual void SetSamplers(
                        RHI_SHADTYPE shader_type_a,
                        const std::vector<RHI_SAMPLSTATE>& samplers_a,
                        unsigned int start_slot_a = 0,
                        RHI_GPURootSignature* rs_a = nullptr) override;

                virtual void SetRootSignature(RHI_GPURootSignature* rs_a,
                                              bool is_compute_a = false) override {};

                virtual void SetRootConstant(unsigned int constant_a,
                                             unsigned int root_index_a,
                                             unsigned int an_offset_a = 0,
                                             bool is_compute_a = false) override {};

                virtual void SetShader(RHI_GPUShader* shader_a) override;
                virtual void SetInputLayout(RHI_InputLayout* il_a) override;
                virtual void SetEmptyInputLayout() override;
                virtual void SetIndexBuffer(RHI_GPUBuffer* buffer_a,
                                            unsigned int offset_a = 0) override;
                
                virtual void SetVertexBuffers(
                        const std::vector<RHI_GPUBuffer*>& vertex_buffers_a) override;

                virtual void SetTopologyType(
                        RHI_PRIMITIVE_TYPE type_a) override;
                virtual RHI_PRIMITIVE_TYPE GetCurrentTopologyType() override;

                virtual void SetGPUDescriptorHeap(
                        RHI_DESCRIPTOR_HEAP_TYPE type_a,
                        bool reset_a) override {};

                virtual void TransitionResources(
                        const std::vector<RHI_GPUResource*>& resources_a,
                        const std::vector<RHI_RESOURCE_STATE>& states_a,
                        int cmd_list_index_a = 0,
                        bool is_copy_queue_a = false,
                        int subresource_index_a = -1) override {};
                
                virtual void TransitionResources(
                        const std::vector<RHI_GPUResource*>& resources_a,
                        RHI_RESOURCE_STATE state_a,
                        int cmd_list_index_a = 0,
                        bool is_copy_queue_a = false,
                        int subresource_index_a = -1) override {};
                
                virtual void TransitionMainRenderTargetToPresent(
                        int cmd_list_index_a = 0) override {};

                virtual bool IsPSOReady(const std::string& name_a,
                                        bool is_compute_a = false) override
                {
                    return false;
                }
                
                virtual void InitializePSO(const std::string& name_a,
                                           bool is_compute_a = false) override {};
                
                virtual void SetRootSignatureToPSO(const std::string& name_a,
                                                   RHI_GPURootSignature* rs_a,
                                                   bool is_compute_a = false) override {};
                
                virtual void SetTopologyTypeToPSO(const std::string& name_a,
                                                  RHI_PRIMITIVE_TYPE type_a) override {};
                
                virtual void FinalizePSO(const std::string& name_a,
                                         bool is_compute_a = false) override {};
                
                virtual void SetPSO(const std::string& name_a,
                                    bool is_compute_a = false) override {};
                
                virtual void UnsetPSO() override {};

                virtual void UnbindRenderTargets() override;
                virtual void UnbindResourcesFromShader(
                        RHI_SHADTYPE shader_type_a,
                        bool unbind_shader_a = true) override;

                virtual void UpdateBuffer(
                        RHI_GPUBuffer* buffer_a,
                        void* data_a,
                        int data_size_a,
                        bool update_for_all_back_buffers_a = false) override;

                virtual bool IsHardwareRaytracingSupported() override
                {
                    return false;
                }
                
                virtual bool IsRootConstantSupported() override
                {
                    return false;
                }

                virtual void OnWindowSizeChanged(int width_a,
                                                 int height_a) override {};

                virtual void WaitForGpuOnGraphicsFence() override {};
                virtual void WaitForGpuOnComputeFence() override {};
                virtual void WaitForGpuOnCopyFence() override {};

                virtual void ResetReplacementMippedTexturesPool() override {};
                virtual void ResetDescriptorManager() override {};
                virtual void ResetRHI(int width_a,
                                      int height_a,
                                      bool is_fullscreen_a) override {};

                virtual void BeginEventTag(
                        const std::string& name_a,
                        bool is_compute_queue_a = false) override;
                virtual void EndEventTag(bool is_comput_queue_a = false) override;

                ID3D11Device1* GetDevice()
                {
                    return direct3d_device_;
                }
                
                ID3D11DeviceContext1* GetContext()
                {
                    return direct3d_device_context_;
                }
                
                DXGI_FORMAT GetFormat(RHI_FORMAT format_a);

                GRAPHICS_API GetAPI()
                {
                    return api_;
                }

            private:
                D3D11_PRIMITIVE_TOPOLOGY GetTopologyType(
                        RHI_PRIMITIVE_TYPE type_a);
                
                RHI_PRIMITIVE_TYPE GetTopologyType(
                        D3D11_PRIMITIVE_TOPOLOGY type_a);

                void CreateSamplerStates();
                void CreateBlendStates();
                void CreateRasterizerStates();
                void CreateDepthStencilStates();

                D3D_FEATURE_LEVEL feature_level_ = D3D_FEATURE_LEVEL_11_1;
                ID3D11Device1* direct3d_device_ = nullptr;
                ID3D11DeviceContext1* direct3d_device_context_ = nullptr;
                IDXGISwapChain1* swap_chain_ = nullptr;
                ID3DUserDefinedAnnotation* user_defined_annotation_ = nullptr;

                ID3D11Texture2D* depth_stencil_buffer_ = nullptr;
                D3D11_TEXTURE2D_DESC back_buffer_desc_;
                ID3D11RenderTargetView* main_render_target_view_ = nullptr;
                ID3D11DepthStencilView* main_depth_stencil_view_ = nullptr;

                ID3D11SamplerState* bilinear_wrap_ss_ = nullptr;
                ID3D11SamplerState* bilinear_mirror_ss_ = nullptr;
                ID3D11SamplerState* bilinear_clamp_ss_ = nullptr;
                ID3D11SamplerState* bilinear_border_ss_ = nullptr;
                ID3D11SamplerState* trilinear_wrap_ss_ = nullptr;
                ID3D11SamplerState* trilinear_mirror_ss_ = nullptr;
                ID3D11SamplerState* trilinear_clamp_ss_ = nullptr;
                ID3D11SamplerState* trilinear_border_ss_ = nullptr;
                ID3D11SamplerState* anisotropic_wrap_ss_ = nullptr;
                ID3D11SamplerState* anisotropic_mirror_ss_ = nullptr;
                ID3D11SamplerState* anisotropic_clamp_ss_ = nullptr;
                ID3D11SamplerState* anisotropic_border_ss_ = nullptr;
                ID3D11SamplerState* shadow_ss_ = nullptr;
                std::map<RHI_SAMPLSTATE, ID3D11SamplerState*>
                        sampler_states_;

                ID3D11BlendState* no_blend_state_ = nullptr;
                ID3D11BlendState* alpha_to_coverage_state_ = nullptr;
                ID3D11BlendState* alpha_to_coverage_state_4_targets_ = nullptr;
                ID3D11BlendState* alpha_blend_state_ = nullptr;
                std::map<RHI_BLEND_STATE, ID3D11BlendState*> blend_states_;

                ID3D11RasterizerState* back_culling_rs_ = nullptr;
                ID3D11RasterizerState* front_culling_rs_ = nullptr;
                ID3D11RasterizerState* no_culling_rs_ = nullptr;
                ID3D11RasterizerState* wireframe_rs_ = nullptr;
                ID3D11RasterizerState* no_culling_no_depth_enabled_scissor_rs_ = nullptr;
                ID3D11RasterizerState* shadow_rs_ = nullptr;
                std::map<RHI_RASTERIZSTATE, ID3D11RasterizerState*> rasterizer_states_;

                ID3D11DepthStencilState* disabled_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_read_comparison_never_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_read_comparison_less_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_read_comparison_equal_ds_ = nullptr;
                ID3D11DepthStencilState* depth_Only_read_comparison_less_equal_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_read_comparison_greater_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_read_comparison_not_equal_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_read_comparison_greater_equal_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_read_comparison_always_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_write_comparison_never_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_write_comparison_less_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_write_comparison_equal_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_write_comparison_less_equal_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_write_comparison_greater_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_write_comparison_not_equal_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_write_comparison_greater_equal_ds_ = nullptr;
                ID3D11DepthStencilState* depth_only_write_comparison_always_ds_ = nullptr;
                std::map<RHI_DEPTH_STENCIL_STATE, ID3D11DepthStencilState*> depth_states_;

                RHI_Viewport main_viewport_;

                bool is_context_reading_buffer_ = false;
            };
        } // namespace RHI
    } // namespace Graphics
} // namespace CE_Kernel
