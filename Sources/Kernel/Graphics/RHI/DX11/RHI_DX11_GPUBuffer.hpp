// Copyright (c) 2025 Case Technologies

#pragma once
#include "RHI_DX11.hpp"

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace RHI
        {
            class RHI_DX11_GPUBuffer : public RHI_GPUBuffer
            {
            public:
                RHI_DX11_GPUBuffer(const std::string& debug_name_a);
                virtual ~RHI_DX11_GPUBuffer();

                virtual void CreateGPUBufferResource(
                        RHI* rhi_a,
                        void* data_a,
                        unsigned int objects_count_a,
                        unsigned int byte_stride_a,
                        bool is_dynamic_a = false,
                        RHI_BIND_FLAG bind_flags_a = BIND_NONE,
                        unsigned int cpu_access_flags_a = 0,
                        RHI_RESOURCE_MISC_FLAG misc_flags_a =
                                RESOURCE_MISC_NONE,
                        RHI_FORMAT format_a = FORMAT_UNKNOWN) override;
                
                virtual void* GetBuffer() override
                {
                    return buffer_;
                }
                
                virtual void* GetSRV() override
                {
                    return buffer_srv_;
                }
                
                virtual void* GetUAV() override
                {
                    return buffer_uav_;
                }
                
                virtual int GetSize() override
                {
                    return byte_size_;
                }
                
                virtual UINT GetStride() override
                {
                    return stride_;
                }
                
                virtual ER_RHI_FORMAT GetFormatRhi() override
                {
                    return rhi_format_;
                }
                
                virtual void* GetResource()
                {
                    return buffer_;
                }

                virtual RHI_RESOURCE_STATE GetCurrentState()
                {
                    return RHI_RESOURCE_STATE::RESOURCE_STATE_COMMON;
                }
                
                virtual void SetCurrentState(RHI_RESOURCE_STATE state_a)
                {}

                inline virtual bool IsBuffer() override
                {
                    return true;
                }

                void Map(RHI* rhi_a,
                         D3D11_MAP map_flags_a,
                         D3D11_MAPPED_SUBRESOURCE* out_mapped_resource_a);
                
                void Unmap(RHI* rhi_a);
                void Update(RHI* rhi_a, void* data_a, int data_size_a);
                DXGI_FORMAT GetFormat()
                {
                    return format_;
                }

            private:
                ID3D11Buffer* buffer_ = nullptr;
                ID3D11UnorderedAccessView* buffer_uav_ = nullptr;
                ID3D11ShaderResourceView* buffer_srv_ = nullptr;

                DXGI_FORMAT format_;
                RHI_FORMAT rhi_format_;
                unsigne dint stride_;
                int byte_size_ = 0;
            };
        } // namespace RHI
    } // namespace Graphics
} // namespace CE_Kernel
