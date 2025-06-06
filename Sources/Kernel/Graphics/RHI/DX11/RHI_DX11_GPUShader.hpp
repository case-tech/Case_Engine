// Copyright (c) 2025 Case Technologies

#pragma once
#include "RHI_DX11.hpp"

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace RHI
        {
            class RHI_DX11_GPUShader : public RHI_GPUShader
            {
            public:
                RHI_DX11_GPUShader();
                virtual ~RHI_DX11_GPUShader();

                virtual void CompileShader(
                        RHI* rhi_a,
                        const std::string& path_a,
                        const std::string& shader_entry_a,
                        RHI_SHADER_TYPE type_a,
                        RHI_InputLayout* il_a = nullptr) override;
                
                virtual void* GetShaderObject() override;

            private:
                HRESULT CompileBlob(_In_ LPCWSTR src_file_a,
                                    _In_ LPCSTR entry_point_a,
                                    _In_ LPCSTR profile_a,
                                    _Outptr_ ID3DBlob** blob_a);

                ID3D11VertexShader* vs_ = nullptr;
                ID3D11GeometryShader* gs_ = nullptr;
                ID3D11HullShader* hs_ = nullptr;
                ID3D11DomainShader* ds_ = nullptr;
                ID3D11PixelShader* ps_ = nullptr;
                ID3D11ComputeShader* cs_ = nullptr;
            };
        } // namespace RHI
    } // namespace Graphics
} // namespace CE_Kernel
