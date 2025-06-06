// Copyright (c) 2025 Case Technologies

#pragma once
#define DX11
#define VK
#define GL

#if defined(DX11)
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>
using namespace Microsoft::WRL;

#endif
#elif defined(VK)
#include <vulkan.h>
#elif defined(GL)
#include <glew.h>
#include <wglew.h>
#endif

#define RHI_MAX_GRAPHICS_COMMAND_LISTS 8
#define RHI_MAX_COMPUTE_COMMAND_LISTS 2
#define RHI_MAX_BOUND_VERTEX_BUFFERS 2

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace RHI
        {
            static const int DefaultFrameRate = 60;
            static inline void AbstractRHIMethodAssert()
            {
                assert(("You called an abstract method from RHI", 0));
            }

            enum GRAPHICS_API
            {
                DX11,
                VK,
                GL
            };

            enum RHI_SHADTYPE
            {
                VERTEX,
                GEOMETRY,
                TESSELLATION_HULL,
                TESSELLATION_DOMAIN,
                PIXEL,
                COMPUTE
            };

            enum RHI_DEPTH_STENCIL_STATE
            {
                DISABLED,
                DEPTH_ONLY_READ_COMPARISON_NEVER,
                DEPTH_ONLY_READ_COMPARISON_LESS,
                DEPTH_ONLY_READ_COMPARISON_EQUAL,
                DEPTH_ONLY_READ_COMPARISON_LESS_EQUAL,
                DEPTH_ONLY_READ_COMPARISON_GREATER,
                DEPTH_ONLY_READ_COMPARISON_NOT_EQUAL,
                DEPTH_ONLY_READ_COMPARISON_GREATEQUAL,
                DEPTH_ONLY_READ_COMPARISON_ALWAYS,
                DEPTH_ONLY_WRITE_COMPARISON_NEVER,
                DEPTH_ONLY_WRITE_COMPARISON_LESS,
                DEPTH_ONLY_WRITE_COMPARISON_EQUAL,
                DEPTH_ONLY_WRITE_COMPARISON_LESS_EQUAL,
                DEPTH_ONLY_WRITE_COMPARISON_GREATER,
                DEPTH_ONLY_WRITE_COMPARISON_NOT_EQUAL,
                DEPTH_ONLY_WRITE_COMPARISON_GREATEQUAL,
                DEPTH_ONLY_WRITE_COMPARISON_ALWAYS
            };

            enum RHI_BLEND_STATE
            {
                NO_BLEND,
                ALPHA_TO_COVERAGE,
                ALPHA_TO_COVERAGE_4_TARGETS,
                ALPHA_BLEND
            };

            enum RHI_SAMPLSTATE
            {
                BILINEAR_WRAP,
                BILINEAR_CLAMP,
                BILINEAR_BORDER,
                BILINEAR_MIRROR,
                TRILINEAR_WRAP,
                TRILINEAR_CLAMP,
                TRILINEAR_BORDER,
                TRILINEAR_MIRROR,
                ANISOTROPIC_WRAP,
                ANISOTROPIC_CLAMP,
                ANISOTROPIC_BORDER,
                ANISOTROPIC_MIRROR,
                SHADOW_SS
            };

            enum RHI_RASTERIZSTATE
            {
                NO_CULLING,
                BACK_CULLING,
                FRONT_CULLING,
                WIREFRAME,
                NO_CULLING_NO_DEPTH_SCISSOR_ENABLED,
                SHADOW_RS
            };

            enum RHI_RESOURCE_STATE
            {
                RESOURCE_STATE_COMMON,
                RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
                RESOURCE_STATE_INDEX_BUFFER,
                RESOURCE_STATE_RENDTARGET,
                RESOURCE_STATE_UNORDERED_ACCESS,
                RESOURCE_STATE_DEPTH_WRITE,
                RESOURCE_STATE_DEPTH_READ,
                RESOURCE_STATE_NON_PIXEL_SHADRESOURCE,
                RESOURCE_STATE_PIXEL_SHADRESOURCE,
                RESOURCE_STATE_INDIRECT_ARGUMENT,
                RESOURCE_STATE_COPY_DEST,
                RESOURCE_STATE_COPY_SOURCE,
                RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
                RESOURCE_STATE_GENERIC_READ,
                RESOURCE_STATE_PRESENT
            };

            enum RHI_FORMAT
            {
                FORMAT_UNKNOWN = 0,
                FORMAT_R32G32B32A32_TYPELESS,
                FORMAT_R32G32B32A32_FLOAT,
                FORMAT_R32G32B32A32_UINT,
                FORMAT_R32G32B32_TYPELESS,
                FORMAT_R32G32B32_FLOAT,
                FORMAT_R32G32B32_UINT,
                FORMAT_R16G16B16A16_TYPELESS,
                FORMAT_R16G16B16A16_FLOAT,
                FORMAT_R16G16B16A16_UNORM,
                FORMAT_R16G16B16A16_UINT,
                FORMAT_R32G32_TYPELESS,
                FORMAT_R32G32_FLOAT,
                FORMAT_R32G32_UINT,
                FORMAT_R10G10B10A2_TYPELESS,
                FORMAT_R10G10B10A2_UNORM,
                FORMAT_R10G10B10A2_UINT,
                FORMAT_R11G11B10_FLOAT,
                FORMAT_R8G8B8A8_TYPELESS,
                FORMAT_R8G8B8A8_UNORM,
                FORMAT_R8G8B8A8_UNORM_sRGB,
                FORMAT_R8G8B8A8_UINT,
                FORMAT_R16G16_TYPELESS,
                FORMAT_R16G16_FLOAT,
                FORMAT_R16G16_UNORM,
                FORMAT_R16G16_UINT,
                FORMAT_R32_TYPELESS,
                FORMAT_D32_FLOAT,
                FORMAT_R32_FLOAT,
                FORMAT_R32_UINT,
                FORMAT_D24_UNORM_S8_UINT,
                FORMAT_R8G8_TYPELESS,
                FORMAT_R8G8_UNORM,
                FORMAT_R8G8_UINT,
                FORMAT_D16_UNORM,
                FORMAT_R16_TYPELESS,
                FORMAT_R16_FLOAT,
                FORMAT_R16_UNORM,
                FORMAT_R16_UINT,
                FORMAT_R8_TYPELESS,
                FORMAT_R8_UNORM,
                FORMAT_R8_UINT
            };

            enum RHI_PRIMITIVE_TYPE
            {
                PRIMITIVE_TOPOLOGY_POINTLIST,
                PRIMITIVE_TOPOLOGY_LINELIST,
                PRIMITIVE_TOPOLOGY_TRIANGLELIST,
                PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
                PRIMITIVE_TOPOLOGY_CONTROL_POINT_PATCHLIST
            };

            typedef enum RHI_BIND_FLAG
            {
                BIND_NONE = 0x0L,
                BIND_VERTEX_BUFFER = 0x1L,
                BIND_INDEX_BUFFER = 0x2L,
                BIND_CONSTANT_BUFFER = 0x4L,
                BIND_SHADRESOURCE = 0x8L,
                BIND_STREAM_OUTPUT = 0x10L,
                BIND_RENDTARGET = 0x20L,
                BIND_DEPTH_STENCIL = 0x40L,
                BIND_UNORDERED_ACCESS = 0x80L
            } RHI_BIND_FLAG;

            constexpr inline RHI_BIND_FLAG operator~(RHI_BIND_FLAG a_a)
            {
                return static_cast<RHI_BIND_FLAG>(
                        ~static_cast<
                                std::underlying_type<RHI_BIND_FLAG>::type>(
                                a_a));
            }
            
            constexpr inline RHI_BIND_FLAG operator|(RHI_BIND_FLAG a_a,
                                                        RHI_BIND_FLAG b_a)
            {
                return static_cast<RHI_BIND_FLAG>(
                        static_cast<
                                std::underlying_type<RHI_BIND_FLAG>::type>(a_a)
                        | static_cast<
                                std::underlying_type<RHI_BIND_FLAG>::type>(
                                b_a));
            }

            constexpr inline RHI_BIND_FLAG operator&(RHI_BIND_FLAG a_a,
                                                        RHI_BIND_FLAG b_a)
            {
                return static_cast<RHI_BIND_FLAG>(
                        static_cast<
                                std::underlying_type<RHI_BIND_FLAG>::type>(a_a)
                        & static_cast<
                                std::underlying_type<RHI_BIND_FLAG>::type>(
                                b_a));
            }
            
            constexpr inline RHI_BIND_FLAG operator^(RHI_BIND_FLAG a_a,
                                                        RHI_BIND_FLAG b_a)
            {
                return static_cast<RHI_BIND_FLAG>(
                        static_cast<
                                std::underlying_type<RHI_BIND_FLAG>::type>(a_a)
                        ^ static_cast<
                                std::underlying_type<RHI_BIND_FLAG>::type>(
                                b_a));
            }

            enum RHI_RESOURCE_MISC_FLAG
            {
                RESOURCE_MISC_NONE = 0x0L,
                RESOURCE_MISC_TEXTURECUBE = 0x4L,
                RESOURCE_MISC_DRAWINDIRECT_ARGS = 0x10L,
                RESOURCE_MISC_BUFFSTRUCTURED = 0x40L,
            };

            enum RHI_DESCRIPTOR_HEAP_TYPE
            {
                RHI_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV = 0,
                RHI_DESCRIPTOR_HEAP_TYPE_SAMPLER =
                        (RHI_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV + 1),
                RHI_DESCRIPTOR_HEAP_TYPE_RTV =
                        (RHI_DESCRIPTOR_HEAP_TYPE_SAMPLER + 1),
                RHI_DESCRIPTOR_HEAP_TYPE_DSV =
                        (RHI_DESCRIPTOR_HEAP_TYPE_RTV + 1)
            };

            enum RHI_SHADVISIBILITY
            {
                RHI_SHADVISIBILITY_ALL = 0,
                RHI_SHADVISIBILITY_VERTEX = 1,
                RHI_SHADVISIBILITY_HULL = 2,
                RHI_SHADVISIBILITY_DOMAIN = 3,
                RHI_SHADVISIBILITY_GEOMETRY = 4,
                RHI_SHADVISIBILITY_PIXEL = 5,
                RHI_SHADVISIBILITY_AMPLIFICATION = 6,
                RHI_SHADVISIBILITY_MESH = 7
            };

            enum RHI_DESCRIPTOR_RANGE_TYPE
            {
                RHI_DESCRIPTOR_RANGE_TYPE_SRV = 0,
                RHI_DESCRIPTOR_RANGE_TYPE_UAV = 1,
                RHI_DESCRIPTOR_RANGE_TYPE_CBV = 2
            };

            struct RHI_INPUT_ELEMENT_DESC
            {
                const char* semantic_name_;
                unsigned int semantic_index_;
                RHI_FORMAT format_;
                unsigned int input_slot_;
                unsigned int aligned_byte_offset_;
                bool is_per_vertex_ = true;
                unsigned int instance_data_step_rate_ = 0;
            };

            struct RHI_Viewport
            {
                float top_left_x_;
                float top_left_y_;
                float width_;
                float height_;
                float min_depth_ = 0.0f;
                float max_depth_ = 1.0f;
            };

            struct RHI_Rect
            {
                long int left_;
                long int top_;
                long int right_;
                long int bottom_;
            };

            class RHI_InputLayout
            {
            public:
                RHI_InputLayout(
                        RHI_INPUT_ELEMENT_DESC* input_element_descriptions_a,
                        unsigned int input_element_description_count_a)
                {
                    input_element_descriptions_ = new RHI_INPUT_ELEMENT_DESC
                            [input_element_description_count_a];

                    memcpy(input_element_descriptions_,
                           input_element_descriptions_a,
                           input_element_description_count_a
                                   * sizeof(RHI_INPUT_ELEMENT_DESC));

                    input_element_description_count_ =
                            input_element_description_count_a;
                };

                virtual ~RHI_InputLayout()
                {
                    input_element_descriptions_ = NULL;
                }

                RHI_INPUT_ELEMENT_DESC* input_element_descriptions_;
                unsigned int input_element_description_count_;
            };

            class RHI_GPURootSignature;
            class RHI_GPUResource;
            class RHI_GPUTexture;
            class RHI_GPUBuffer;
            class RHI_GPUShader;

            class RHI
            {
            public:
                RHI() {}
                virtual ~RHI() {}

                virtual bool Initialize(void* window_handle_a,
                                        unsigned int width_a,
                                        unsigned int height_a,
                                        bool is_fullscreen_a,
                                        bool is_reset_a = false) = 0;

                virtual void BeginGraphicsCommandList(int index_a = 0) = 0;
                virtual void EndGraphicsCommandList(int index_a = 0) = 0;

                virtual void BeginComputeCommandList(int index_a = 0) = 0;
                virtual void EndComputeCommandList(int index_a = 0) = 0;

                virtual void BeginCopyCommandList(int index_a = 0) = 0;
                virtual void EndCopyCommandList(int index_a = 0) = 0;

                virtual void ClearMainRenderTarget(float colors_a[4]) = 0;
                virtual void ClearMainDepthStencilTarget(float depth_a,
                                                         unsigned int stencil_ = 0) = 0;
                
                virtual void ClearRenderTarget(RHI_GPUTexture* render_target_a,
                                               float colors_a[4],
                                               int rtv_array_index_a = -1) = 0;
                
                virtual void ClearDepthStencilTarget(
                        RHI_GPUTexture* depth_target_a,
                        float depth_a,
                        unsigned int stencil_a = -1) = 0;
                
                virtual void ClearUAV(RHI_GPUResource* render_target_a,
                                      float colors_a[4]) = 0;
                
                virtual void ClearUAV(RHI_GPUBuffer* buffer_a,
                                      unsigned int clear_a) = 0;

                virtual RHI_GPUShader* CreateGPUShader() = 0;
                virtual RHI_GPUBuffer* CreateGPUBuffer(
                        const std::string& debug_name_a) = 0;
                
                virtual RHI_GPUTexture* CreateGPUTexture(
                        const std::wstring& debug_name_a) = 0;
                
                virtual RHI_GPURootSignature* CreateRootSignature(
                        unsigned int num_root_params_a = 0,
                        unsigned int num_static_samplers_a = 0) = 0;
                
                virtual RHI_InputLayout* CreateInputLayout(
                        RHI_INPUT_ELEMENT_DESC* input_element_descriptions_a,
                        unsigned int input_element_description_count_a) = 0;

                virtual void CreateTexture(RHI_GPUTexture* out_texture_a,
                                           unsigned int width_a,
                                           unsigned int height_a,
                                           unsigned int samples,
                                           RHI_FORMAT format_a,
                                           RHI_BIND_FLAG bind_flags_a,
                                           int mip_a = 1,
                                           int depth_a = -1,
                                           int array_size_a = 1,
                                           bool is_cubemap_a = false,
                                           int cubemap_array_size_a = -1) = 0;
                
                virtual void CreateTexture(RHI_GPUTexture* out_texture_a,
                                           const std::string& path_a,
                                           bool is_full_path_a = false) = 0;
                
                virtual void CreateTexture(RHI_GPUTexture* out_texture_a,
                                           const std::wstring& path_a,
                                           bool is_full_path_a = false) = 0;

                virtual void CreateBuffer(
                        RHI_GPUBuffer* out_buffer_a,
                        void* data_a,
                        unsigned int objects_count_a,
                        unsigned int byte_stride_a,
                        bool is_dynamic_a = false,
                        RHI_BIND_FLAG bind_flags_a = BIND_NONE,
                        unsigned int cpu_access_flags_a = 0,
                        RHI_RESOURCE_MISC_FLAG misc_flags_a =
                                RESOURCE_MISC_NONE,
                        RHI_FORMAT format_a = FORMAT_UNKNOWN) = 0;
                
                virtual void CopyBuffer(RHI_GPUBuffer* dest_buffer_a,
                                        RHI_GPUBuffer* src_buffer_a,
                                        int cmd_list_index_a,
                                        bool is_in_copy_queue_a = false) = 0;
                
                virtual void BeginBufferRead(RHI_GPUBuffer* buffer_a,
                                             void** output_a) = 0;
                
                virtual void EndBufferRead(RHI_GPUBuffer* buffer_a) = 0;

                virtual void CopyGPUTextureSubresourceRegion(
                        RHI_GPUResource* dest_buffer_a,
                        UINT dst_subresource_a,
                        UINT dst_x_a,
                        UINT dst_y_a,
                        UINT dst_z_a,
                        RHI_GPUResource* src_buffer_a,
                        unsigned int src_subresource_a,
                        bool is_in_copy_queue_or_skip_transitions_a = false) = 0;

                virtual void Draw(unsigned int vertex_count_a) = 0;
                virtual void DrawIndexed(unsigned int index_count_a) = 0;
                virtual void DrawInstanced(unsigned int vertex_count_per_instance_a,
                                           unsigned int instance_count_a,
                                           unsigned int start_vertex_location_a,
                                           unsigned int start_instance_location_a) = 0;
                
                virtual void DrawIndexedInstanced(
                        unsigned int index_count_per_instance_a,
                        unsigned int instance_count_a,
                        unsigned int start_index_location_a,
                        int base_vertex_location_a,
                        unsigned int start_instance_location_a) = 0;
                
                virtual void DrawIndexedInstancedIndirect(
                        RHI_GPUBuffer* an_args_buffer_a,
                        unsigned int aligned_byte_offset_a) = 0;

                virtual void Dispatch(unsigned int thread_group_count_x_a,
                                      unsigned int thread_group_count_y_a,
                                      unsigned int thread_group_count_z_a) = 0;

                virtual void GenerateMips(
                        RHI_GPUTexture* texture_a,
                        RHI_GPUTexture* srgb_texture_a = nullptr) = 0;
               
                virtual void GenerateMipsWithTextureReplacement(
                        RHI_GPUTexture** texture_a,
                        std::function<void(RHI_GPUTexture**)>
                                replacement_callback_a) = 0;
                
                virtual void ReplaceOriginalTexturesWithMipped() = 0;
                virtual void ExecuteCommandLists(int command_list_index_a = 0,
                                                 bool is_compute_a = false) = 0;
                
                virtual void ExecuteCopyCommandList() = 0;

                virtual void PresentGraphics() = 0;
                virtual void PresentCompute() = 0;

                virtual bool ProjectCubemapToSH(
                        RHI_GPUTexture* texture_a,
                        UINT order_a,
                        float* result_r_a,
                        float* result_g_a,
                        float* result_b_a) = 0;

                virtual void SaveGPUTextureToFile(
                        RHI_GPUTexture* texture_a,
                        const std::wstring& path_name_a) = 0;

                virtual void SetMainRenderTargets(int cmdListIndex = 0) = 0;
                virtual void SetRenderTargets(
                        const std::vector<RHI_GPUTexture*>& render_targets_a,
                        RHI_GPUTexture* depth_target_a = nullptr,
                        RHI_GPUTexture* uav_a = nullptr,
                        int rtv_array_index_a = -1) = 0;
                
                virtual void SetDepthTarget(
                        RHI_GPUTexture* depth_target_a) = 0;
                
                virtual void SetRenderTargetFormats(
                        const std::vector<RHI_GPUTexture*>& render_targets_a,
                        RHI_GPUTexture* depth_target_a = nullptr) = 0;
                
                virtual void SetMainRenderTargetFormats() = 0;

                virtual void SetDepthStencilState(
                        RHI_DEPTH_STENCIL_STATE ds_a,
                        UINT stencil_ref_a = 0xffffffff) = 0;
                
                virtual RHI_DEPTH_STENCIL_STATE GetCurrentDepthStencilState()
                {
                    return current_ds_;
                }

                virtual void SetBlendState(RHI_BLEND_STATE bs_a,
                                           const float blend_factor_a[4] = nullptr,
                                           unsigned int sample_mask_a = 0xffffffff) = 0;
                
                virtual RHI_BLEND_STATE GetCurrentBlendState()
                {
                    return current_bs_;
                }

                virtual void SetRasterizerState(
                        RHI_RASTERIZSTATE rs_a) = 0;
                
                virtual RHI_RASTERIZSTATE GetCurrentRasterizerState()
                {
                    return current_rs_;
                }

                virtual void SetViewport(const RHI_Viewport& viewport_a) = 0;
                virtual const RHI_Viewport& GetCurrentViewport()
                {
                    return current_viewport_;
                }

                virtual void SetRect(const RHI_Rect& rect_a) = 0;
                virtual const RHI_Rect& GetCurrentRect()
                {
                    return current_rect_;
                }

                virtual void SetShader(RHI_GPUShader* shader_a) = 0;
                virtual void SetShaderResources(
                        RHI_SHADTYPE shader_type_a,
                        const std::vector<RHI_GPUResource*>& srvs_a,
                        UINT start_slot_a = 0,
                        RHI_GPURootSignature* rs_a = nullptr,
                        int root_param_index_a = -1,
                        bool is_compute_rs_a = false,
                        bool skip_automatic_transition_a = false) = 0;
                
                virtual void SetUnorderedAccessResources(
                        RHI_SHADTYPE shader_type_a,
                        const std::vector<RHI_GPUResource*>& uavs_a,
                        UINT start_slot_a = 0,
                        RHI_GPURootSignature* rs_a = nullptr,
                        int root_param_index_a = -1,
                        bool is_compute_rs_a = false,
                        bool skip_automatic_transition_a = false) = 0;
                
                virtual void SetConstantBuffers(
                        RHI_SHADTYPE shader_type_a,
                        const std::vector<RHI_GPUBuffer*>& cbs_a,
                        unsigned int start_slot_a = 0,
                        RHI_GPURootSignature* rs_a = nullptr,
                        int root_param_index_a = -1,
                        bool is_compute_rs_a = false) = 0;
                
                virtual void SetSamplers(
                        RHI_SHADTYPE shader_type_a,
                        const std::vector<RHI_SAMPLSTATE>& samplers_a,
                        unsigned int start_slot_a = 0,
                        RHI_GPURootSignature* rs_a = nullptr) = 0;

                virtual void SetRootSignature(RHI_GPURootSignature* rs_a,
                                              bool is_compute_a = false) = 0;
                
                virtual void SetRootConstant(unsigned int constant_a,
                                             unsigned int root_index_a,
                                             unsigned int an_offset_a = 0,
                                             bool is_compute_a = false) = 0;

                virtual void SetIndexBuffer(RHI_GPUBuffer* buffer_a,
                                            unsigned int offset_a = 0) = 0;
                
                virtual void SetVertexBuffers(
                        const std::vector<RHI_GPUBuffer*>&
                                vertex_buffers_a) = 0;
                
                virtual void SetInputLayout(RHI_InputLayout* il_a) = 0;
                virtual void SetEmptyInputLayout() = 0;

                virtual void SetTopologyType(RHI_PRIMITIVE_TYPE type_a) = 0;
                virtual RHI_PRIMITIVE_TYPE GetCurrentTopologyType() = 0;

                virtual void SetGPUDescriptorHeap(
                        RHI_DESCRIPTOR_HEAP_TYPE type_a,
                        bool reset_a) = 0;
                
                virtual void SetGPUDescriptorHeapImGui(int cmd_list_index_a) = 0;

                virtual void TransitionResources(
                        const std::vector<RHI_GPUResource*>& resources_a,
                        const std::vector<RHI_RESOURCE_STATE>& states_a,
                        int cmd_list_index_a = 0,
                        bool is_copy_queue_a = false,
                        int subresource_index_a = -1) = 0;
                
                virtual void TransitionResources(
                        const std::vector<RHI_GPUResource*>& resources_a,
                        RHI_RESOURCE_STATE state_a,
                        int cmd_list_index_a = 0,
                        bool is_copy_queue_a = false,
                        int subresource_index_a = -1) = 0;
                
                virtual void TransitionMainRenderTargetToPresent(
                        int cmd_list_index_a = 0) = 0;

                virtual bool IsPSOReady(const std::string& name_a,
                                        bool is_compute_a = false) = 0;
                
                virtual void InitializePSO(const std::string& name_a,
                                           bool is_compute_a = false) = 0;
                
                virtual void SetRootSignatureToPSO(const std::string& name_a,
                                                   RHI_GPURootSignature* rs_a,
                                                   bool is_compute_a = false) = 0;
                virtual void SetTopologyTypeToPSO(
                        const std::string& name_a,
                        RHI_PRIMITIVE_TYPE type_a) = 0;

                virtual void FinalizePSO(const std::string& name_a,
                                         bool is_compute_a = false) = 0;
                
                virtual void SetPSO(const std::string& name_a,
                                    bool is_compute_a = false) = 0;
                
                virtual void UnsetPSO() = 0;
                virtual void UnbindRenderTargets() = 0;
                virtual void UnbindResourcesFromShader(
                        RHI_SHADTYPE shader_type_a,
                        bool unbind_shader_a = true) = 0;

                virtual void UpdateBuffer(
                        RHI_GPUBuffer* buffer_a,
                        void* data_a,
                        int data_size_a,
                        bool update_for_all_back_buffers_a = false) = 0;

                virtual bool IsHardwareRaytracingSupported() = 0;
                virtual bool IsRootConstantSupported() = 0;

                virtual void OnWindowSizeChanged(int width_a, int height_a) = 0;

                virtual void WaitForGpuOnGraphicsFence() = 0;
                virtual void WaitForGpuOnComputeFence() = 0;
                virtual void WaitForGpuOnCopyFence() = 0;

                virtual void ResetReplacementMippedTexturesPool() = 0;
                virtual void ResetDescriptorManager() = 0;
                virtual void ResetRHI(int width_a,
                                      int height_a,
                                      bool is_fullscreen_a) = 0;

                virtual void BeginEventTag(const std::string& name_a,
                                           bool is_compute_queue_a = false) = 0;
          
                virtual void EndEventTag(bool is_compute_queue_a = false) = 0;

                inline const int GetPrepareGraphicsCommandListIndex()
                {
                    return prepare_graphics_command_list_index_;
                }

                inline const int GetCurrentGraphicsCommandListIndex()
                {
                    return current_graphics_command_list_index_;
                }
                
                inline const int GetCurrentComputeCommandListIndex()
                {
                    return current_compute_command_list_index_;
                }

                GRAPHICS_API GetAPI()
                {
                    return api_;
                }

            protected:
                void* window_handle_;

                GRAPHICS_API api_;
                bool is_fullscreen_ = false;

                RHI_RASTERIZSTATE current_rs_;
                RHI_BLEND_STATE current_bs_;
                RHI_DEPTH_STENCIL_STATE current_ds_;

                RHI_Viewport current_viewport_;
                RHI_Rect current_rect_;

                const int prepare_graphics_command_list_index_ =
                        RHI_MAX_GRAPHICS_COMMAND_LISTS - 1;
                
                int current_graphics_command_list_index_ = -1;
                int current_compute_command_list_index_ = -1;
            };

            class RHI_GPURootSignature
            {
            public:
                RHI_GPURootSignature(unsigned int num_root_params_a = 0,
                                     unsigned int num_static_samplers_a = 0) {}
                
                virtual ~RHI_GPURootSignature() {}

                virtual void InitConstant(RHI* rhi_a,
                                          unsigned int index_a,
                                          unsigned int reg_index_a,
                                          unsigned int num_dwords_a,
                                          RHI_SHADVISIBILITY visibility_a =
                                                  RHI_SHADVISIBILITY_ALL)
                {
                    AbstractRHIMethodAssert();
                };

                virtual void InitStaticSampler(
                        RHI* rhi_a,
                        UINT reg_index_a,
                        const RHI_SAMPLSTATE& sampler_a,
                        RHI_SHADVISIBILITY visibility_a =
                                RHI_SHADVISIBILITY_ALL)
                {
                    AbstractRHIMethodAssert();
                }

                virtual void InitDescriptorTable(
                        RHI* rhi_a,
                        int root_param_index_a,
                        const std::vector<RHI_DESCRIPTOR_RANGE_TYPE>& ranges_a,
                        const std::vector<UINT>& register_indices_a,
                        const std::vector<UINT>& descriptor_counters_a,
                        RHI_SHADVISIBILITY visibility_a =
                                RHI_SHADVISIBILITY_ALL)
                {
                    AbstractRHIMethodAssert();
                }

                virtual void Finalize(RHI* rhi_a,
                                      const std::string& name_a,
                                      bool needs_input_assembler_a = false)
                {
                    AbstractRHIMethodAssert();
                }

                virtual int GetStaticSamplersCount()
                {
                    AbstractRHIMethodAssert();
                    return 0;
                }

                virtual int GetRootParameterCount()
                {
                    AbstractRHIMethodAssert();
                    return 0;
                }

                virtual int GetRootParameterCBVCount(int param_index_a)
                {
                    AbstractRHIMethodAssert();
                    return 0;
                }
                
                virtual int GetRootParameterSRVCount(int param_index_a)
                {
                    AbstractRHIMethodAssert();
                    return 0;
                }
                
                virtual int GetRootParameterUAVCount(int param_index_a)
                {
                    AbstractRHIMethodAssert();
                    return 0;
                }
            };

            class RHI_GPUResource
            {
            public:
                RHI_GPUResource()
                {}
                virtual ~RHI_GPUResource()
                {}

                virtual void* GetSRV() = 0;
                virtual void* GetUAV() = 0;
                virtual void* GetResource() = 0;

                virtual RHI_RESOURCE_STATE GetCurrentState() = 0;
                virtual void SetCurrentState(RHI_RESOURCE_STATE aState) = 0;

                inline virtual bool IsBuffer() = 0;
            };

            class RHI_GPUTexture : public RHI_GPUResource
            {
            public:
                RHI_GPUTexture()
                {}
                virtual ~RHI_GPUTexture()
                {}

                virtual void CreateGPUTextureResource(
                        RHI* rhi_a,
                        UINT width_a,
                        UINT height_a,
                        UINT samples_a,
                        RHI_FORMAT format_a,
                        RHI_BIND_FLAG bind_flags_a = BIND_NONE,
                        int mip_a = 1,
                        int depth_a = -1,
                        int array_size_a = 1,
                        bool is_cubemap_a = false,
                        int cubemap_arrayS_size_a = -1)
                {
                    AbstractRHIMethodAssert();
                }

                virtual void CreateGPUTextureResource(
                        RHI* rhi_a,
                        const std::string& path_a,
                        bool is_full_path_a = false,
                        bool is_3d_a = false,
                        bool skip_fallback_a = false,
                        bool* status_flag_a = nullptr,
                        bool is_silent_a = false)
                {
                    AbstractRHIMethodAssert();
                }
                
                virtual void CreateGPUTextureResource(
                        RHI* rhi_a,
                        const std::wstring& path_a,
                        bool is_full_path_a = false,
                        bool is_3d_a = false,
                        bool skip_fallback_a = false,
                        bool* status_flag_a = nullptr,
                        bool is_silent_a = false)
                {
                    AbstractRHIMethodAssert();
                }

                virtual void* GetRTV(void* empty_a = nullptr)
                {
                    AbstractRHIMethodAssert();
                    return nullptr;
                }
                
                virtual void* GetRTV(int index_a)
                {
                    AbstractRHIMethodAssert();
                    return nullptr;
                }
                
                virtual void* GetDSV()
                {
                    AbstractRHIMethodAssert();
                    return nullptr;
                }
                
                virtual void* GetSRV()
                {
                    AbstractRHIMethodAssert();
                    return nullptr;
                }
                
                virtual void* GetUAV()
                {
                    AbstractRHIMethodAssert();
                    return nullptr;
                }
                
                virtual void* GetResource()
                {
                    AbstractRHIMethodAssert();
                    return nullptr;
                }

                virtual unsigned int GetMips()
                {
                    AbstractRHIMethodAssert();
                    return 0;
                }
                
                virtual unsigned int GetCalculatedMipCount()
                {
                    AbstractRHIMethodAssert();
                    return 0;
                }
                
                virtual unsigned int GetWidth()
                {
                    AbstractRHIMethodAssert();
                    return 0;
                }
                
                virtual unsigned int GetHeight()
                {
                    AbstractRHIMethodAssert();
                    return 0;
                }
                
                virtual unsigned int GetDepth()
                {
                    AbstractRHIMethodAssert();
                    return 0;
                }

                virtual RHI_RESOURCE_STATE GetCurrentState()
                {
                    AbstractRHIMethodAssert();
                    return RHI_RESOURCE_STATE::RESOURCE_STATE_COMMON;
                }
                
                virtual void SetCurrentState(RHI_RESOURCE_STATE state_a)
                {
                    AbstractRHIMethodAssert();
                }

                inline virtual bool IsBuffer()
                {
                    AbstractRHIMethodAssert();
                    return false;
                }

                std::wstring debug_name_;
            };

            class RHI_GPUBuffer : public RHI_GPUResource
            {
            public:
                RHI_GPUBuffer() {}
                virtual ~RHI_GPUBuffer() {}

                virtual void CreateGPUBufferResource(
                        RHI* rhi_a,
                        void* data_a,
                        UINT objects_count_a,
                        UINT byte_stride_a,
                        bool is_dynamic_a = false,
                        RHI_BIND_FLAG bind_flags_a = BIND_NONE,
                        UINT cpu_access_flags_a = 0,
                        RHI_RESOURCE_MISC_FLAG misc_flags_a =
                                RESOURCE_MISC_NONE,
                        RHI_FORMAT format_a = FORMAT_UNKNOWN)
                {
                    AbstractRHIMethodAssert();
                }
               
                virtual void* GetBuffer()
                {
                    AbstractRHIMethodAssert();
                    return nullptr;
                }
                
                virtual int GetSize()
                {
                    AbstractRHIMethodAssert();
                    return 0;
                }
                
                virtual UINT GetStride()
                {
                    AbstractRHIMethodAssert();
                    return 0;
                }
                
                virtual RHI_FORMAT GetFormatRhi()
                {
                    AbstractRHIMethodAssert();
                    return RHI_FORMAT::FORMAT_UNKNOWN;
                }

                virtual void* GetSRV()
                {
                    AbstractRHIMethodAssert();
                    return nullptr;
                }
                
                virtual void* GetUAV()
                {
                    AbstractRHIMethodAssert();
                    return nullptr;
                }
            };

            class RHI_GPUShader
            {
            public:
                RHI_GPUShader() {}
                virtual ~RHI_GPUShader() {}

                virtual void CompileShader(RHI* rhi_a,
                                           const std::string& path_a,
                                           const std::string& shader_entry_a,
                                           RHI_SHADTYPE type_a,
                                           RHI_InputLayout* il_a = nullptr)
                {
                    AbstractRHIMethodAssert();
                };
                
                virtual void* GetShaderObject()
                {
                    AbstractRHIMethodAssert();
                    return nullptr;
                }

                RHI_SHADTYPE shader_type_;
            };

            template <typename T>
            class RHI_GPUConstantBuffer
            {
            public:
                T Data;

            protected:
                RHI_GPUBuffer* buffer_ = nullptr;
                bool initialized_;

            public:
                RHI_GPUConstantBuffer() : initialized_(false)
                {
                    memset(&Data, sizeof(T));
                }

                bool IsInitialized()
                {
                    return initialized_;
                }
                
                void Release()
                {
                    DeleteObject(buffer_);
                }

                RHI_GPUBuffer* Buffer() const
                {
                    return buffer_;
                }

                void Initialize(RHI* rhi_a, const std::string& name_a)
                {
                    buffer_ = rhi_a->CreateGPUBuffer(name_a);
                    buffer_->CreateGPUBufferResource(
                            rhi_a,
                            &Data,
                            1,
                            BitmaskAlign(static_cast<unsigned int>(sizeof(T)),
                                            GPU_BUFFALIGNMENT),
                            true,
                            BIND_CONSTANT_BUFFER);
                    
                    initialized_ = true;
                }

                void ApplyChanges(RHI* rhi_a)
                {
                    assert(initialized_);
                    assert(rhi_a);
                    assert(buffer_);

                    rhi_a->UpdateBuffer(buffer_, &Data, sizeof(T));
                }
            };
        } // namespace RHI
    } // namespace Graphics
} // namespace CE_Kernel
