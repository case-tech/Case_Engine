// Copyright (c) 2025 Case Technologies

#pragma once
#include "RHI_DX11.hpp"

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace RHI
        {
            class RHI_DX11_GPUTexture : public RHI_GPUTexture
            {
            public:
                RHI_DX11_GPUTexture(const std::wstring& debug_name_a);
                virtual ~RHI_DX11_GPUTexture();

                virtual void CreateGPUTextureResource(
                        RHI* rhi_a,
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
                
                virtual void CreateGPUTextureResource(
                        RHI* rhi_a,
                        const std::string& path_a,
                        bool is_full_path_a = false,
                        bool is_3d_a = false,
                        bool skip_fallback_a = false,
                        bool* status_flag_a = nullptr,
                        bool is_silent_a = false) override;
                
                virtual void CreateGPUTextureResource(
                        RHI* rhi_a,
                        const std::wstring& path_a,
                        bool is_full_path_a = false,
                        bool is_3d_a = false,
                        bool skip_fallback_a = false,
                        bool* status_flag_a = nullptr,
                        bool is_silent_a = false) override;

                virtual void* GetRTV(void* empty_a = nullptr) override
                {
                    return rtvs_[0];
                }
                
                virtual void* GetRTV(int index_a) override
                {
                    return rtvs_a[index_a];
                }

                ID3D11RenderTargetView* GetRTV()
                {
                    return rtvs_[0];
                }
                
                ID3D11RenderTargetView** GetRTVs()
                {
                    return rtvs_;
                }

                virtual void* GetDSV() override
                {
                    return dsv_;
                }
                
                ID3D11DepthStencilView* GetDSV(bool read_only_a = false)
                {
                    if (read_only_a)
                        return dsv_read_only_;
                    else
                        return dsv_;
                }

                virtual void* GetSRV() override
                {
                    return srv_;
                }
                
                void SetSRV(ID3D11ShaderResourceView* srv_a)
                {
                    srv_ = srv_a;
                }

                ID3D11Texture2D* GetTexture2D()
                {
                    return texture_2d_;
                }
                
                void SetTexture2D(ID3D11Texture2D* t2d_a)
                {
                    texture_2d_ = t2d_a;
                }

                ID3D11Texture3D* GetTexture3D()
                {
                    return texture_3d_;
                }
                
                void SetTexture3D(ID3D11Texture3D* t3d_a)
                {
                    texture_3d_ = t3d_a;
                }

                virtual void* GetUAV() override
                {
                    return uavs_[0];
                }
                
                ID3D11UnorderedAccessView** GetUAVs()
                {
                    return uavs_;
                }

                unsigned int GetMips() override
                {
                    return mip_levels_;
                }
                
                unsigned int GetWidth() override
                {
                    return width_;
                }
                
                unsigned int GetHeight() override
                {
                    return height_;
                }
                
                unsigned int GetDepth() override
                {
                    return depth_;
                }

                bool IsLoadedFromFile()
                {
                    return is_loaded_from_file_;
                }

            private:
                void LoadFallbackTexture(
                        RHI* rhi_a,
                        ID3D11Resource** texture_a,
                        ID3D11ShaderResourceView** texture_view_a);

                ID3D11RenderTargetView** rtvs_ = nullptr;
                ID3D11UnorderedAccessView** uavs_ = nullptr;
                ID3D11ShaderResourceView* srv_ = nullptr;
                ID3D11DepthStencilView* dsv_ = nullptr;
                ID3D11DepthStencilView* dsv_readonly_ = nullptr;
                ID3D11Texture2D* texture_2d_ = nullptr;
                ID3D11Texture3D* texture_3d_ = nullptr;

                DXGI_FORMAT format_;
                unsigned int mip_levels_ = 0;
                unsigned int bind_flags_ = 0;
                unsigned int width_ = 0;
                unsigned int height_ = 0;
                unsigned int depth_ = 0;
                unsigned int array_size_ = 0;
                bool is_cubemap_ = false;
                bool is_depth_stencil_ = false;
                bool is_loaded_from_file_ = false;
            };
        } // namespace RHI
    } // namespace Graphics
} // namespace CE_Kernel
