#include "RHI_DX11_GPUTexture.hpp"

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace RHI
        {
            RHI_DX11_GPUTexture::RHI_DX11_GPUTexture(
                    const std::wstring& debug_name_a)
            {
                debug_name_ = debug_name_a;
            }

            RHI_DX11_GPUTexture::~RHI_DX11_GPUTexture()
            {
                ReleaseObject(srv_);
                ReleaseObject(texture_2d_);
                ReleaseObject(texture_3d_);

                if (is_loaded_from_file_)
                    return;

                if (is_depth_stencil_)
                {
                    ReleaseObject(dsv_);
                    ReleaseObject(dsv_readonly_);
                }

                if (bind_flags_ & D3D11_BIND_RENDTARGET)
                {
                    for (unsigned int i = 0; i < array_size_ * mip_levels_; i++)
                    {
                        ReleaseObject(rtvs_[i]);
                    }
                }
                
                if (bind_flags_ & D3D11_BIND_UNORDERED_ACCESS)
                {
                    for (unsigned int i = 0; i < mip_levels_; i++)
                    {
                        ReleaseObject(uavs_[i]);
                    }
                }

                mip_levels_ = 0;
                bind_flags_ = 0;
                array_size_ = 0;
            }

            void RHI_DX11_GPUTexture::CreateGPUTextureResource(
                    RHI* rhi_a,
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
                assert(rhi_a);
                RHI_DX11* rhi_dx11_ = static_cast<RHI_DX11*>(rhi_a);
                ID3D11Device* device_ = rhi_dx11_->GetDevice();
                assert(device_);

                ID3D11Texture2D* tex_2d_ = nullptr;
                ID3D11Texture3D* tex_3d_ = nullptr;
                ID3D11ShaderResourceView* srv_ = nullptr;
                ID3D11RenderTargetView** rtv_ = nullptr;
                ID3D11UnorderedAccessView** uav_ = nullptr;
                is_loaded_from_file_ = false;

                array_size_ = array_size_a;
                mip_levels_ = mip_a;
                bind_flags_ = bind_flags_a;
                depth_ = depth_a;
                width_ = width_a;
                height_ = height_a;
                is_cubemap_ = is_cubemap_a;
                is_depth_stencil_ = bind_flags_a & BIND_DEPTH_STENCIL;
                format_a = rhi_dx11_->GetFormat(format_a);

                DXGI_FORMAT depth_stencil_tex_format_;
                DXGI_FORMAT depth_stencil_srv_format_;
                DXGI_FORMAT depth_stencil_dsv_format_;
                if (is_depth_stencil_)
                {
                    switch (format_)
                    {
                    case DXGI_FORMAT_D32_FLOAT:
                        depth_stencil_tex_format_ = DXGI_FORMAT_R32_TYPELESS;
                        depth_stencil_srv_format_ = DXGI_FORMAT_R32_FLOAT;
                        depth_stencil_dsv_format_ = DXGI_FORMAT_D32_FLOAT;
                        break;
                    case DXGI_FORMAT_D16_UNORM:
                        depth_stencil_tex_format_ = DXGI_FORMAT_R16_TYPELESS;
                        depth_stencil_srv_format_ = DXGI_FORMAT_R16_UNORM;
                        depth_stencil_dsv_format_ = DXGI_FORMAT_D16_UNORM;
                        break;
                    case DXGI_FORMAT_D24_UNORM_S8_UINT:
                    default:
                        depth_stencil_tex_format_ = DXGI_FORMAT_R24G8_TYPELESS;
                        depth_stencil_srv_format_ = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
                        depth_stencil_dsv_format_ = DXGI_FORMAT_D24_UNORM_S8_UINT;
                        break;
                    }
                }

                if (depth_ > 0)
                {
                    CD3D11_TEXTURE3D_DESC tex_desc_;
                    tex_desc_.BindFlags = bind_flags_;
                    tex_desc_.CPUAccessFlags = 0;
                    tex_desc_.Format = mIsDepthStencil
                                             ? depth_stencil_tex_format_
                                             :  rhi_dx11_->GetFormat(format_a);
                    tex_desc_.Width = width_a;
                    tex_desc_.Height = height_a;
                    tex_desc_.Depth = depth_a;
                    tex_desc_.MipLevels = mip_levels_;
                    if (mip_levels_ > 1
                        && (BIND_RENDTARGET & bind_flags_a) != 0
                        && !is_depth_stencil_)
                        tex_desc_.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
                    else
                        tex_desc_.MiscFlags = 0;
                    tex_desc_.Usage = D3D11_USAGE_DEFAULT;
                    device_->CreateTexture3D(&tex_desc_, NULL, &tex_3d_);
                } 
                else
                {
                    CD3D11_TEXTURE2D_DESC tex_desc_;
                    tex_desc_.ArraySize = (is_cubemap_a && cubemap_array_size_ > 0)
                                                ? 6 * cubemap_array_size_
                                                : array_size_;
                    tex_desc_.BindFlags = bind_flags_a;
                    tex_desc_.MiscFlags = 0;
                    if (is_cubemap_a)
                        tex_desc_.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
                    if (mip_levels_ > 1
                        && (BIND_RENDTARGET & bind_flags_a) != 0
                        && !is_depth_stencil_)
                        tex_desc_.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
                    tex_desc_.CPUAccessFlags = 0;
                    tex_desc_.Format = is_depth_stencil_ ? depth_stencil_tex_format_
                                                     : format_;
                    tex_desc_.Width = width_a;
                    tex_desc_.Height = height_a;
                    tex_desc_.MipLevels = mip_levels_a;

                    if (samples_a > 1)
                    {
                        tex_desc_.SampleDesc.Count = samples_a;
                        unsigned int quality_levels_ = 0;
                        device_->CheckMultisampleQualityLevels(
                                rhi_dx11_->GetFormat(format_a),
                                samples_a,
                                &quality_levels_);
                        tex_desc_.SampleDesc.Quality = quality_levels_ - 1;
                    } 

                    else
                    {
                        tex_desc_.SampleDesc.Count = 1;
                        tex_desc_.SampleDesc.Quality = 0;
                    }
                    tex_desc_.Usage = D3D11_USAGE_DEFAULT;

                    device_->CreateTexture2D(&tex_desc_, NULL, &tex_2d_);
                }

                if (bind_flags_ & BIND_RENDTARGET)
                {
                    assert(!is_depth_stencil_);

                    D3D11_RENDTARGET_VIEW_DESC r_desc_;
                    r_desc_.Format = rhi_dx11_->GetFormat(format_a);
                    if (array_size_ == 1)
                    {
                        r_desc_.ViewDimension =
                                (depth_a > 0)
                                        ? D3D11_RTV_DIMENSION_TEXTURE3D
                                        : ((samples_a > 1)
                                                   ? D3D11_RTV_DIMENSION_TEXTURE2DMS
                                                   : D3D11_RTV_DIMENSION_TEXTURE2D);
                        
                        rtv_ = (ID3D11RenderTargetView**)malloc(
                                sizeof(ID3D11RenderTargetView*) * mip_a);
                        int current_depth_ = depth_a;
                        for (int i = 0; i < mip_a; i++)
                        {
                            if (depth_ > 0)
                            {
                                r_desc_.Texture3D.MipSlice = i;
                                r_desc_.Texture3D.FirstWSlice = 0;
                                r_desc_.Texture3D.WSize = currentDepth;
                                device_->CreateRenderTargetView(tex_3d_,
                                                               &r_desc_,
                                                               &rtv_[i]);
                                current_depth_ >>= 1;
                            } 
                            else
                            {
                                r_desc_.Texture2D.MipSlice = i;
                                device_->CreateRenderTargetView(tex_2d_,
                                                                &r_desc_,
                                                                &rtv_[i]);
                            }
                        }
                    } 

                    else
                    {
                        r_desc_.ViewDimension =
                                (samples_a > 1)
                                        ? D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY
                                        : D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
                        rtv_ = (ID3D11RenderTargetView**)malloc(
                                sizeof(ID3D11RenderTargetView*) * array_size_
                                * mip_a);
                        for (int i = 0; i < array_size_; i++)
                        {
                            for (int j = 0; j < mip_a; j++)
                            {
                                if (samples_ > 1)
                                {
                                    r_desc_.Texture2DMSArray.FirstArraySlice = i;
                                    r_desc_.Texture2DMSArray.ArraySize = 1;
                                } 

                                else
                                {
                                    r_desc_.Texture2DArray.MipSlice = j;
                                    r_desc_.Texture2DArray.FirstArraySlice = i;
                                    r_desc_.Texture2DArray.ArraySize = 1;
                                }
                                device_->CreateRenderTargetView(
                                        tex_2d_,
                                        &r_desc_,
                                        &rtv_[i * mip_a + j]);
                            }
                        }
                    }
                }

                if (bind_flags_a & BIND_DEPTH_STENCIL)
                {
                    D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc_;
                    dsv_desc_.Flags = 0;
                    dsv_desc_.Format = depth_stencil_dsv_format_;
                    if (depth_ == 1)
                    {
                        if (samples_ > 1)
                        {
                            dsv_desc_.ViewDimension =
                                    D3D11_DSV_DIMENSION_TEXTURE2DMS;
                        } 

                        else
                        {
                            dsv_desc_.ViewDimension =
                                    D3D11_DSV_DIMENSION_TEXTURE2D;
                            dsv_desc_.Texture2D.MipSlice = 0;
                        }
                    } 
                    else
                    {
                        if (samples_ > 1)
                        {
                            dsv_desc_.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
                            dsv_desc_.Texture2DMSArray.FirstArraySlice = 0;
                            dsv_desc_.Texture2DMSArray.ArraySize = depth_;
                        } 

                        else
                        {
                            dsv_desc_.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
                            dsv_desc_.Texture2DArray.MipSlice = 0;
                            dsv_desc_.Texture2DArray.FirstArraySlice = 0;
                            dsv_desc_.Texture2DArray.ArraySize = depth_;
                        }
                    }

                    device_->CreateDepthStencilView(tex_2d_, &dsv_desc_, &dsv_);
                    dsv_desc_.Flags |= D3D11_DSV_READ_ONLY_DEPTH;
                    if (depth_stencil_dsv_format_ == DXGI_FORMAT_D24_UNORM_S8_UINT)
                        dsv_desc_.Flags |= D3D11_DSV_READ_ONLY_STENCIL;
                    device_->CreateDepthStencilView(tex_2d_,
                                                    &dsv_desc_,
                                                    &dsv_read_only_);
                }

                if (bind_flags_ & BIND_UNORDERED_ACCESS)
                {
                    assert(!is_depth_stencil_);

                    D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc_;
                    uav_desc_.Buffer.FirstElement = 0;
                    uav_desc_.Buffer.Flags = 0;
                    int depth_elements_ = (depth_ > 0) ? depth_ : 1;
                    uav_desc_.Buffer.NumElements = width_ * height_ * depth_elements_;
                    uav_desc_.ViewDimension =
                            depth_ > 0 ? D3D11_UAV_DIMENSION_TEXTURE3D
                                      : D3D11_UAV_DIMENSION_TEXTURE2D;
                    uav_desc_.Format = format_;
                    uav_ = (ID3D11UnorderedAccessView**)malloc(
                            sizeof(ID3D11UnorderedAccessView*) * mip_);

                    int current_depth_ = depth_;
                    for (int i = 0; i < mip_a; i++)
                    {
                        if (current_depth_ > 0)
                        {
                            uav_desc_.Texture3D.MipSlice = i;
                            uav_desc_.Texture3D.FirstWSlice = 0;
                            uav_desc_.Texture3D.WSize = current_depth_;
                            device_->CreateUnorderedAccessView(tex_3d_,
                                                               &uav_desc_,
                                                               &(uav_[i]));
                            current_depth_ >>= 1;
                        } 
                        else
                        {
                            uav_desc_.Texture2D.MipSlice = i;
                            device_->CreateUnorderedAccessView(tex_2d_,
                                                              &uav_desc_,
                                                              &(uav_[i]));
                        }
                    }
                }
                
                if (bind_flags_ & BIND_SHADRESOURCE)
                {
                    D3D11_SHADRESOURCE_VIEW_DESC s_desc_ = CD3D11_SHADRESOURCE_VIEW_DESC();
                    s_desc_.Format = is_depth_stencil_
                                           ? depth_stencil_srv_format_
                                           : rhi_dx11_->GetFormat(format_);
                    if (array_size_ > 1)
                    {
                        if (is_cubemap_)
                        {
                            if (cubemap_array_size_ > 0)
                            {
                                s_desc_.TextureCubeArray.MipLevels = mip_a;
                                s_desc_.TextureCubeArray.MostDetailedMip = 0;
                                s_desc_.TextureCubeArray.NumCubes = cubemap_array_size_;
                                s_desc_.TextureCubeArray.First2DArrayFace = 0;
                                s_desc_.ViewDimension =  D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
                            } 

                            else
                            {
                                s_desc_.TextureCube.MipLevels = mip_a;
                                s_desc_.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
                            }
                            device_->CreateShaderResourceView(tex_2d_,
                                                              &s_desc_,
                                                              &srv_);
                        } 

                        else
                        {
                            if (samples_a > 1)
                            {
                                s_desc_.Texture2DMSArray.FirstArraySlice = 0;
                                s_desc_.Texture2DMSArray.ArraySize = array_size_;
                                s_desc_.ViewDimension =  D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
                            } 
                            
                            else
                            {
                                s_desc_.Texture2DArray.MipLevels = mip_a;
                                s_desc_.Texture2DArray.ArraySize = array_size_a;
                                s_desc_.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                            }
                            device_->CreateShaderResourceView(tex_2d_,
                                                              &s_desc_,
                                                              &srv_);
                        }
                    } 
                    else
                    {
                        if (depth_ > 0)
                        {
                            s_desc_.Texture3D.MipLevels = mip_a;
                            s_desc_.Texture3D.MostDetailedMip = 0;
                            s_desc_.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
                            device_->CreateShaderResourceView(tex_3d_,
                                                              &s_desc_,
                                                              &srv_);
                        } 
                        else
                        {
                            s_desc_.Texture2D.MipLevels = mip;
                            s_desc_.Texture2D.MostDetailedMip = 0;
                            s_desc_.ViewDimension =
                                    (samples_ > 1)
                                            ? D3D11_SRV_DIMENSION_TEXTURE2DMS
                                            : D3D11_SRV_DIMENSION_TEXTURE2D;
                            device_->CreateShaderResourceView(tex_2d_,
                                                              &s_desc_,
                                                              &srv_);
                        }
                    }
                }

                rtvs_ = rtv_;
                srv_ = srv_;
                uavs_ = uav_;
                texture_2d_ = tex_2d_;
                texture_3d_ = tex_3d_;
            }

            void RHI_DX11_GPUTexture::CreateGPUTextureResource(
                    RHI* rhi_a,
                    const std::string& path_a,
                    bool is_full_path_a,
                    bool is_3d_a,
                    bool skip_fallback_a,
                    bool* status_flag_a,
                    bool is_silent_a)
            {
                CreateGPUTextureResource(
                        rhi_a,
                        EveryRay_Core::Utility::ToWideString(path_a),
                        is_full_path_a,
                        is_3d_a,
                        skip_fallback_a,
                        status_flag_a,
                        is_silent_a);
            }

            void RHI_DX11_GPUTexture::CreateGPUTextureResource(
                    RHI* rhi_a,
                    const std::wstring& path_a,
                    bool is_full_path_a,
                    bool is_3d_a,
                    bool skip_fallback_a,
                    bool* status_flag_a,
                    bool is_silent_a)
            {
                assert(rhi_a);
                RHI_DX11* rhi_dx11_ = static_cast<RHI_DX11*>(rhi_a);
                ID3D11Device* device_ = rhi_dx11_->GetDevice();
                assert(device_);
                ID3D11DeviceContext1* context_ = rhi_dx11_->GetContext();
                assert(context_);

                is_loaded_from_file_ = true;

                std::wstring original_path_ = path_a;
                const wchar_t* postfix_dds_ = L".dds";
                const wchar_t* postfix_dds_capital_ = L".DDS";
                bool is_dds_ = (original_path_.substr(original_path_.length() - 4)
                              == std::wstring(postfix_dds_))
                             || (original_path_.substr(original_path_.length() - 4)
                                 == std::wstring(postfix_dds_capital_));

                ID3D11Resource* resource_tex_ = NULL;

                auto output_log_ = [is_silent_](const std::wstring& path_t_) 
                {
                    std::wstring msg_ = L"[Logger][RHI_DX11_GPUTexture] Failed to load texture from disk: " + path_t_ + L". Loading fallback texture instead unless forced not to. \n";
                    if (!is_silent_)
                        OUTPUT_LOG(msg_.c_str());
                };

                if (is_dds_)
                {
                    if (FAILED(DirectX::CreateDDSTextureFromFile(
                                device_,
                                context_,
                                is_full_path_ ? path_a.c_str()
                                           : EveryRay_Core::Utility::
                                                     GetFilePath(path_a)
                                                             .c_str(),
                                &resource_tex_,
                                &srv_)))
                    {
                        output_log_(is_full_path_ ? path_a.c_str()
                                             : EveryRay_Core::Utility::
                                                       GetFilePath(path_a)
                                                               .c_str());
                        if (!skip_fallback_)
                            LoadFallbackTexture(rhi_a, &resource_tex_, &srv_);
                        if (status_flag_)
                            *status_flag_ = false;
                    }
                } 
                else
                {
                    if (FAILED(DirectX::CreateWICTextureFromFile(
                                device_,
                                context_,
                                is_full_path_ ? path_.c_str()
                                           : EveryRay_Core::Utility::
                                                     GetFilePath(path_)
                                                             .c_str(),
                                &resource_tex_,
                                &srv_)))
                    {
                        output_log_(is_full_path_ ? path_.c_str()
                                             : EveryRay_Core::Utility::
                                                       GetFilePath(path_)
                                                               .c_str());
                        if (!skip_fallback_)
                            LoadFallbackTexture(rhi_a, &resource_tex_, &srv_);
                        if (status_flag_)
                            *status_flag_ = false;
                    }
                }

                if (!resource_tex_)
                    return;

                if (!is_3d_)
                {
                    if (FAILED(resource_tex_->QueryInterface(
                                IID_ID3D11Texture2D,
                                (void**)&texture_2d_)))
                    {
                        throw std::exception(
                                "RHI_DX11: Could not cast loaded texture "
                                "resource to Texture2D. Maybe wrong "
                                "dimension?");
                        if (status_flag_)
                            *status_flag_ = false;
                    } 
                    else
                    {
                        if (status_flag_)
                            *status_flag_ = true;
                    }
                } 
                else
                {
                    if (FAILED(resource_tex_->QueryInterface(
                                IID_ID3D11Texture3D,
                                (void**)&texture_3d_)))
                    {
                        throw std::exception(
                                "RHI_DX11: Could not cast loaded texture "
                                "resource to Texture3D. Maybe wrong "
                                "dimension?");
                        if (status_flag_)
                            *status_flag_ = false;
                    } 
                    else
                    {
                        if (status_flag_)
                            *status_flag_ = true;
                    }
                }

                resource_tex_->Release();
            }

            void RHI_DX11_GPUTexture::LoadFallbackTexture(
                    RHI* rhi_a,
                    ID3D11Resource** texture_a,
                    ID3D11ShaderResourceView** texture_view_a)
            {
                assert(rhi_a);
                RHI_DX11* rhi_dx11_ = static_cast<RHI_DX11*>(rhi_a);
                ID3D11Device* device_ = rhi_dx11_->GetDevice();
                assert(device_);
                ID3D11DeviceContext1* context_ = rhi_dx11_->GetContext();
                assert(context_);

                DirectX::CreateWICTextureFromFile(
                        device_,
                        context_,
                        EveryRay_Core::Utility::GetFilePath(
                                L"content\\textures\\uvChecker.jpg")
                                .c_str(),
                        texture_a,
                        texture_view_a);
            }
        } // namespace RHI
    } // namespace Graphics
} // namespace CE_Kernel
