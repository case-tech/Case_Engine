// Copyright (c) 2025 Case Technologies

#pragma once
#include "Export.hpp"

#include <map>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            enum class ShaderTarget
            {
                Undefined,

                VertexShader,
                TessellationControlShader,
                TessellationEvaluationShader,
                GeometryShader,
                FragmentShader,
                ComputeShader,
            };

            enum class InputShaderVersion
            {
                Cg = 2,

                HLSL3 = 3,
                HLSL4 = 4,
                HLSL5 = 5,
                HLSL6 = 6,

                GLSL = 0x0000ffff,
                ESSL = 0x0001ffff,
                VKSL = 0x0002ffff,
            };

            enum class OutputShaderVersion
            {
                GLSL110 = 110,
                GLSL120 = 120,
                GLSL130 = 130,
                GLSL140 = 140,
                GLSL150 = 150,
                GLSL330 = 330,
                GLSL400 = 400,
                GLSL410 = 410,
                GLSL420 = 420,
                GLSL430 = 430,
                GLSL440 = 440,
                GLSL450 = 450,
                GLSL460 = 460,
                GLSL = 0x0000ffff,

                ESSL100 = (0x00010000 + 100),
                ESSL300 = (0x00010000 + 300),
                ESSL310 = (0x00010000 + 310),
                ESSL320 = (0x00010000 + 320),
                ESSL = 0x0001ffff,

                VKSL450 = (0x00020000 + 450),
                VKSL = 0x0002ffff,
            };

            enum class IntermediateLanguage
            {
                SPIRV,
            };

            HTG_EXPORT std::string ToString(const ShaderTarget target_a);
            HTG_EXPORT std::string ToString(
                    const InputShaderVersion shader_version_a);
           
            HTG_EXPORT std::string ToString(
                    const OutputShaderVersion shader_version_a);
            
            HTG_EXPORT std::string ToString(
                    const IntermediateLanguage language_a);
            
            HTG_EXPORT bool IsLanguageHLSL(
                    const InputShaderVersion shader_version_a);
            
            HTG_EXPORT bool IsLanguageGLSL(
                    const InputShaderVersion shader_version_a);
            
            HTG_EXPORT bool IsLanguageGLSL(
                    const OutputShaderVersion shader_version_a);
            
            HTG_EXPORT bool IsLanguageESSL(
                    const OutputShaderVersion shader_version_a);
            
            HTG_EXPORT bool IsLanguageVKSL(
                    const OutputShaderVersion shader_version_a);
            
            HTG_EXPORT const std::map<std::string, int>&
            GetGLSLExtensionEnumeration();
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel