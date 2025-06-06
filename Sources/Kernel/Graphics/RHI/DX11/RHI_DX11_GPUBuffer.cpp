#include "RHI_DX11_GPUBuffer.h"

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace RHI
        {

            RHI_DX11_GPUBuffer::RHI_DX11_GPUBuffer(
                    const std::string& debug_name_a)
            {}

            RHI_DX11_GPUBuffer::~RHI_DX11_GPUBuffer()
            {
                ReleaseObject(buffer_);
                ReleaseObject(buffer_srv_);
                ReleaseObject(buffer_uav_);
            }

            void RHI_DX11_GPUBuffer::CreateGPUBufferResource(
                    RHI* rhi_a,
                    void* data_a,
                    unsigned int objects_count_a,
                    unsigned int byte_stride_a,
                    bool is_dynamic_a,
                    RHI_BIND_FLAG bind_flags_a,
                    unsigned int cpu_access_flags_a,
                    RHI_RESOURCE_MISC_FLAG misc_flags_a,
                    RHI_FORMAT format_a)
            {
                assert(rhi_a);
                RHI_DX11* rhi_dx11_ = static_cast<RHI_DX11*>(rhi_a);
                ID3D11Device* device_ = rhi_dx11_->GetDevice();
                assert(device_);

                D3D11_SUBRESOURCE_DATA init_data_ = {data_a, 0, 0};

                rhi_format_ = format_a;
                format_ = rhi_dx11_a->GetFormat(format_a);
                stride_ = byte_stride_a;
                byte_size_ = objects_count_a * byte_stride_a;

                D3D11_BUFFDESC buf_desc_;
                buf_desc_.ByteWidth = objects_count_a * byte_stride_a;
                if (is_dynamic_a)
                    buf_desc_.Usage = D3D11_USAGE_DYNAMIC;
                else
                {
                    if (cpu_access_flags_a & D3D11_CPU_ACCESS_WRITE
                        && cpu_access_flags_a & D3D11_CPU_ACCESS_READ)
                        buf_desc_.Usage = D3D11_USAGE_STAGING;
                    else
                        buf_desc_.Usage = D3D11_USAGE_DEFAULT;
                }
                buf_desc_.BindFlags = bindFlags;
                buf_desc_.CPUAccessFlags = is_dynamic_a ? D3D11_CPU_ACCESS_WRITE
                                                    : cpu_access_flags_a;
                buf_desc_.MiscFlags = misc_flags_a;
                buf_desc_.StructureByteStride = byte_stride_a;
                if (buf_desc_.BindFlags & D3D11_BIND_CONSTANT_BUFFER)
                    buf_desc_.StructureByteStride = 0;
                if (FAILED(device_->CreateBuffer(&buf_desc_,
                                                  data_a != NULL ? &init_data_
                                                              : NULL,
                                                   &buffer_)))
                    throw std::exception(
                            "RHI_DX11: Failed to create GPU buffer.");

                if (buf_desc_.BindFlags & D3D11_BIND_SHADRESOURCE)
                {
                    D3D11_SHADRESOURCE_VIEW_DESC srv_desc_;
                    srv_desc_.Format = format_a;
                    srv_desc_.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
                    srv_desc_.Buffer.FirstElement = 0;
                    srv_desc_.Buffer.NumElements = objects_count_a;
                    if (FAILED(device_->CreateShaderResourceView(buffer_,
                                                                &srv_desc_,
                                                                &buffer_srv_)))
                        throw std::exception(
                                "RHI_DX11: Failed to create SRV of GPU "
                                "buffer.");
                }

                if (buf_desc_.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
                {
                    D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc_;
                    uav_desc_.Format = format_a;
                    uav_desc_.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
                    uav_desc_.Buffer.FirstElement = 0;
                    uav_desc_.Buffer.NumElements = objects_count_a;
                    uav_desc_.Buffer.Flags = 0;
                    
                        if (FAILED(device_->CreateUnorderedAccessView(buffer_,
                                                                      &uav_desc_,
                                                                      &buffer_uav_)))
                        throw std::exception(
                                "RHI_DX11: Failed to create UAV of GPU "
                                "buffer.");
                }
            }

            void RHI_DX11_GPUBuffer::Map(
                    RHI* rhi_a,
                    D3D11_MAP map_flags_a,
                    D3D11_MAPPED_SUBRESOURCE* out_mapped_resource_a)
            {
                assert(rhi_a);
                RHI_DX11* rhi_dx11_ = static_cast<RHI_DX11*>(rhi_a);
                ID3D11DeviceContext1* context_ = rhi_dx11_->GetContext();
                assert(context_);

                if (FAILED(context_->Map(buffer_,
                                         0,
                                         map_flags_a,
                                         0,
                                         out_mapped_resource_a)))
                    throw std::exception(
                            "RHI_DX11: Failed to map GPU buffer.");
            }

            void RHI_DX11_GPUBuffer::Unmap(RHI* rhi_a)
            {
                assert(rhi_a);
                RHI_DX11* rhi_dx11_ = static_cast<RHI_DX11*>(rhi_a);
                ID3D11DeviceContext1* context_ = rhi_dx11_->GetContext();
                assert(context_);

                context_->Unmap(buffer_, 0);
            }

            void RHI_DX11_GPUBuffer::Update(RHI* rhi_a,
                                               void* data_a,
                                               int data_size_a)
            {
                assert(rhi_a);
                rhi_a->UpdateBuffer(this, data_a, data_size_a);
            }

        } // namespace RHI
    } // namespace Graphics
} // namespace CE_Kernel
