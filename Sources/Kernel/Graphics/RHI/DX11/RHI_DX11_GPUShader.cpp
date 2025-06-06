#include "RHI_DX11_GPUShader.hpp"

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace RHI
        {
            static const std::string vertex_shader_model_ = "vs_5_0";
            static const std::string pixel_shader_model_ = "ps_5_0";
            static const std::string hull_shader_model_ = "hs_5_0";
            static const std::string domain_shader_model_ = "ds_5_0";
            static const std::string geometry_shader_model_ = "gs_5_0";
            static const std::string compute_shader_model_ = "cs_5_0";

            RHI_DX11_GPUShader::RHI_DX11_GPUShader()
            {}

            RHI_DX11_GPUShader::~RHI_DX11_GPUShader()
            {
                ReleaseObject(vs_);
                ReleaseObject(ps_);
                ReleaseObject(gs_);
                ReleaseObject(hs_);
                ReleaseObject(ds_);
                ReleaseObject(cs_);
            }

            void RHI_DX11_GPUShader::CompileShader(
                    RHI* rhi_a,
                    const std::string& path_a,
                    const std::string& shader_entry_a,
                    RHI_SHADTYPE type_a,
                    RHI_InputLayout* il_a)
            {
                shader_type_ = type_;

                assert(rhi_a);
                RHI_DX11* dx11_rhi_ = static_cast<RHI_DX11*>(rhi_a);
                assert(dx11_rhi_);

                assert(!shader_entry_a.empty());

                std::string compiler_error_message_ =
                        "RHI_DX11: Failed to compile blob from shader: "
                        + path_a + " with shader entry: " + shader_entry_a;
                std::string create_error_message_ =
                        "RHI_DX11: Failed to create shader from blob: "
                        + path_a;

                ID3DBlob* blob_ = nullptr;
                switch (shader_type_)
                {
                case VERTEX:
                    if (FAILED(CompileBlob(Utility::GetFilePath(
                                                   Utility::ToWideString(
                                                           path_a))
                                                   .c_str(),
                                           shader_entry_a.c_str(),
                                           vertex_shader_model_.c_str(),
                                           &blob_)))
                        throw std::exception(compiler_error_message_.c_str());
                    if (FAILED(dx11_rhi_->GetDevice()->CreateVertexShader(
                                blob_->GetBufferPointer(),
                                blob_->GetBufferSize(),
                                NULL,
                                &vs_)))
                        throw std::exception(create_error_message_.c_str());
                    break;
                case PIXEL:
                    if (FAILED(CompileBlob(Utility::GetFilePath(
                                                   Utility::ToWideString(
                                                           path_a))
                                                   .c_str(),
                                           shader_entry_a.c_str(),
                                           pixel_shader_model_.c_str(),
                                           &blob_)))
                        throw std::exception(compiler_error_message_.c_str());
                    if (FAILED(dx11_rhi_->GetDevice()->CreatePixelShader(
                                blob_->GetBufferPointer(),
                                blob_->GetBufferSize(),
                                NULL,
                                &ps_)))
                        throw std::exception(create_error_message_.c_str());
                    break;
                case COMPUTE:
                    if (FAILED(CompileBlob(Utility::GetFilePath(
                                                   Utility::ToWideString(
                                                           path_a))
                                                   .c_str(),
                                           shader_entry_.c_str(),
                                           compute_shader_model_.c_str(),
                                           &blob_)))
                        throw std::exception(compiler_error_message_.c_str());
                    if (FAILED(dx11_rhi_->GetDevice()->CreateComputeShader(
                                blob_->GetBufferPointer(),
                                blob_->GetBufferSize(),
                                NULL,
                                &cs_)))
                        throw std::exception(create_error_message_.c_str());
                    break;
                case GEOMETRY:
                    if (FAILED(CompileBlob(Utility::GetFilePath(
                                                   Utility::ToWideString(
                                                           path_a))
                                                   .c_str(),
                                           shader_entry_a.c_str(),
                                           geometry_shader_model_.c_str(),
                                           &blob_)))
                        throw std::exception(compiler_error_message_.c_str());
                    if (FAILED(dx11_rhi_->GetDevice()->CreateGeometryShader(
                                blob_->GetBufferPointer(),
                                blob_->GetBufferSize(),
                                NULL,
                                &gs_)))
                        throw std::exception(create_error_message_.c_str());
                    break;
                case TESSELLATION_HULL:
                    if (FAILED(CompileBlob(Utility::GetFilePath(
                                                   Utility::ToWideString(
                                                           path_a))
                                                   .c_str(),
                                           shader_entry_a.c_str(),
                                           hull_shader_model_.c_str(),
                                           &blob_)))
                        throw std::exception(compiler_error_message_.c_str());
                    if (FAILED(dx11_rhi_->GetDevice()->CreateHullShader(
                                blob_->GetBufferPointer(),
                                blob_->GetBufferSize(),
                                NULL,
                                &hs_)))
                        throw std::exception(create_error_message_.c_str());
                    break;
                case TESSELLATION_DOMAIN:
                    if (FAILED(CompileBlob(Utility::GetFilePath(
                                                   Utility::ToWideString(
                                                           path_a))
                                                   .c_str(),
                                           shader_entry_a.c_str(),
                                           domain_shader_model_.c_str(),
                                           &blob_)))
                        throw std::exception(compiler_error_message_.c_str());
                    if (FAILED(dx11_rhi_->GetDevice()->CreateDomainShader(
                                blob_->GetBufferPointer(),
                                blob_->GetBufferSize(),
                                NULL,
                                &ds_)))
                        throw std::exception(create_error_message_.c_str());
                    break;
                }

                if (il_a && shader_type_ == VERTEX)
                    dx11_rhi_->CreateInputLayout(
                            il_,
                            il_->input_element_descriptions_,
                            il_->input_element_description_count_,
                            blob_->GetBufferPointer(),
                            static_cast<unsigned int>(blob_->GetBufferSize()));

                blob_->Release();
            }

            void* RHI_DX11_GPUShader::GetShaderObject()
            {
                switch (shader_type_)
                {
                case VERTEX:
                    return vs_;
                case PIXEL:
                    return ps_;
                case COMPUTE:
                    return cs_;
                case GEOMETRY:
                    return gs_;
                case TESSELLATION_HULL:
                    return hs_;
                case TESSELLATION_DOMAIN:
                    return ds_;
                }

                return nullptr;
            }

            HRESULT RHI_DX11_GPUShader::CompileBlob(_In_ LPCWSTR src_file_a,
                                                       _In_ LPCSTR entry_point_a,
                                                       _In_ LPCSTR profile_a,
                                                       _Outptr_ ID3DBlob** blob_a)
            {
                if (!src_file_a || !entry_point_a || !profile_a || !blob_a)
                    return E_INVALIDARG;

                *blob_a = nullptr;

                unsigned int flags_ = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
                flags_ |= D3DCOMPILE_DEBUG;
#endif

                const D3D_SHADMACRO defines_[] = {"EXAMPLE_DEFINE",
                                                    "1",
                                                    NULL,
                                                    NULL};

                ID3DBlob* shader_blob_ = nullptr;
                ID3DBlob* error_blob_ = nullptr;
                HRESULT hr_ =
                        D3DCompileFromFile(src_file_a,
                                           defines_,
                                           D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                           entry_point_a,
                                           profile_a,
                                           flags_,
                                           0,
                                           &shader_blob_,
                                           &error_blob_);
                if (FAILED(hr_))
                {
                    if (error_blob_)
                    {
                        OutputDebugStringA(
                                (char*)error_blob_->GetBufferPointer());
                        error_blob_->Release();
                    }

                    if (shader_blob_)
                        shader_blob_->Release();

                    return hr_;
                }

                *blob_ = shader_blob_;

                return hr_;
            }
        } // namespace RHI
    } // namespace Graphics
} // namespace CE_Kernel
