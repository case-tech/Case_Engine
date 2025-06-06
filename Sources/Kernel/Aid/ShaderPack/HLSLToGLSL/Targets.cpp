#include "Targets.hpp"

#include "Frontend/GLSL/GLSLExtensions.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            HTG_EXPORT std::string ToString(const ShaderTarget target_a)
            {
                switch (target_a)
                {
                case ShaderTarget::Undefined:
                    return "Undefined";
                case ShaderTarget::VertexShader:
                    return "Vertex Shader";
                case ShaderTarget::FragmentShader:
                    return "Fragment Shader";
                case ShaderTarget::GeometryShader:
                    return "Geometry Shader";
                case ShaderTarget::TessellationControlShader:
                    return "Tessellation-Control Shader";
                case ShaderTarget::TessellationEvaluationShader:
                    return "Tessellation-Evaluation Shader";
                case ShaderTarget::ComputeShader:
                    return "Compute Shader";
                }
                return "";
            }

            HTG_EXPORT std::string ToString(
                    const InputShaderVersion shader_version_a)
            {
                switch (shader_version_a)
                {
                case InputShaderVersion::Cg:
                    return "Cg";

                case InputShaderVersion::HLSL3:
                    return "HLSL 3.0";
                case InputShaderVersion::HLSL4:
                    return "HLSL 4.0";
                case InputShaderVersion::HLSL5:
                    return "HLSL 5.0";
                case InputShaderVersion::HLSL6:
                    return "HLSL 6.0";

                case InputShaderVersion::GLSL:
                    return "GLSL";
                case InputShaderVersion::ESSL:
                    return "ESSL";
                case InputShaderVersion::VKSL:
                    return "VKSL";
                }
                return "";
            }

            HTG_EXPORT std::string ToString(
                    const OutputShaderVersion shader_version_a)
            {
                switch (shader_version_a)
                {
                case OutputShaderVersion::GLSL110:
                    return "GLSL 1.10";
                case OutputShaderVersion::GLSL120:
                    return "GLSL 1.20";
                case OutputShaderVersion::GLSL130:
                    return "GLSL 1.30";
                case OutputShaderVersion::GLSL140:
                    return "GLSL 1.40";
                case OutputShaderVersion::GLSL150:
                    return "GLSL 1.50";
                case OutputShaderVersion::GLSL330:
                    return "GLSL 3.30";
                case OutputShaderVersion::GLSL400:
                    return "GLSL 4.00";
                case OutputShaderVersion::GLSL410:
                    return "GLSL 4.10";
                case OutputShaderVersion::GLSL420:
                    return "GLSL 4.20";
                case OutputShaderVersion::GLSL430:
                    return "GLSL 4.30";
                case OutputShaderVersion::GLSL440:
                    return "GLSL 4.40";
                case OutputShaderVersion::GLSL450:
                    return "GLSL 4.50";
                case OutputShaderVersion::GLSL460:
                    return "GLSL 4.60";
                case OutputShaderVersion::GLSL:
                    return "GLSL";

                case OutputShaderVersion::ESSL100:
                    return "ESSL 1.00";
                case OutputShaderVersion::ESSL300:
                    return "ESSL 3.00";
                case OutputShaderVersion::ESSL310:
                    return "ESSL 3.10";
                case OutputShaderVersion::ESSL320:
                    return "ESSL 3.20";
                case OutputShaderVersion::ESSL:
                    return "ESSL";

                case OutputShaderVersion::VKSL450:
                    return "VKSL 4.50";
                case OutputShaderVersion::VKSL:
                    return "VKSL";
                }
                return "";
            }

            HTG_EXPORT std::string ToString(const IntermediateLanguage language_a)
            {
                switch (language_a)
                {
                case IntermediateLanguage::SPIRV:
                    return "SPIR-V";
                }
                return "";
            }

            HTG_EXPORT bool IsLanguageHLSL(
                    const InputShaderVersion shader_version_a)
            {
                return (shader_version_a >= InputShaderVersion::Cg
                        && shader_version_a <= InputShaderVersion::HLSL6);
            }

            HTG_EXPORT bool IsLanguageGLSL(
                    const InputShaderVersion shader_version_a)
            {
                return (shader_version_a >= InputShaderVersion::GLSL
                        && shader_version_a <= InputShaderVersion::VKSL);
            }

            HTG_EXPORT bool IsLanguageGLSL(
                    const OutputShaderVersion shader_version_a)
            {
                return ((shader_version_a >= OutputShaderVersion::GLSL110
                         && shader_version_a <= OutputShaderVersion::GLSL460)
                        || shader_version_a == OutputShaderVersion::GLSL);
            }

            HTG_EXPORT bool IsLanguageESSL(
                    const OutputShaderVersion shader_version_a)
            {
                return ((shader_version_a >= OutputShaderVersion::ESSL100
                         && shader_version_a <= OutputShaderVersion::ESSL320)
                        || shader_version_a == OutputShaderVersion::ESSL);
            }

            HTG_EXPORT bool IsLanguageVKSL(
                    const OutputShaderVersion shader_version_a)
            {
                return (shader_version_a == OutputShaderVersion::VKSL450
                        || shader_version_a == OutputShaderVersion::VKSL);
            }

            HTG_EXPORT const std::map<std::string, int>&
            GetGLSLExtensionEnumeration()
            {
                return GetGLSLExtensionVersionMap();
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel