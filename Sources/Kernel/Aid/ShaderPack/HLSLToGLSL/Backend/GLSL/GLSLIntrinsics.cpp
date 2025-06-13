#include "GLSLIntrinsics.hpp"

#include <map>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            static std::map<Intrinsic, std::string> GenerateIntrinsicGLSLMap()
            {
                using T = Intrinsic;

                return {
                        {T::Abs, "abs"},
                        {T::ACos, "acos"},
                        {T::All, "all"},
                        {T::AllMemoryBarrier, "memoryBarrier"},
                        {T::Any, "any"},
                        {T::AsDouble, "uint64BitsToDouble"},
                        {T::AsFloat, "uintBitsToFloat"},
                        {T::ASin, "asin"},
                        {T::AsInt, "floatBitsToInt"},
                        {T::AsUInt_1, "floatBitsToUint"},
                        {T::ATan, "atan"},
                        {T::ATan2, "atan"},
                        {T::Ceil, "ceil"},
                        {T::Clamp, "clamp"},
                        {T::Cos, "cos"},
                        {T::CosH, "cosh"},
                        {T::CountBits, "bitCount"},
                        {T::Cross, "cross"},
                        {T::DDX, "dFdx"},
                        {T::DDXCoarse, "dFdxCoarse"},
                        {T::DDXFine, "dFdxFine"},
                        {T::DDY, "dFdy"},
                        {T::DDYCoarse, "dFdyCoarse"},
                        {T::DDYFine, "dFdyFine"},
                        {T::Degrees, "degrees"},
                        {T::Determinant, "determinant"},
                        {T::Distance, "distance"},
                        {T::Dot, "dot"},
                        {T::Equal, "equal"}, // GLSL only
                        {T::EvaluateAttributeAtCentroid,
                         "interpolateAtCentroid"},
                        {T::EvaluateAttributeAtSample, "interpolateAtSample"},
                        {T::EvaluateAttributeSnapped, "interpolateAtOffset"},
                        {T::Exp, "exp"},
                        {T::Exp2, "exp2"},
                        {T::FaceForward, "faceforward"},
                        {T::FirstBitHigh, "findMSB"},
                        {T::FirstBitLow, "findLSB"},
                        {T::Floor, "floor"},
                        {T::FMA, "fma"},
                        {T::FMod, "mod"},
                        {T::Frac, "fract"},
                        {T::FrExp, "frexp"},
                        {T::FWidth, "fwidth"},
                        {T::GreaterThan, "greaterThan"},           // GLSL only
                        {T::GreaterThanEqual, "greaterThanEqual"}, // GLSL only
                        {T::GroupMemoryBarrier, "groupMemoryBarrier"},
                        {T::InterlockedAdd, "atomicAdd"},
                        {T::InterlockedAnd, "atomicAnd"},
                        {T::InterlockedCompareExchange, "atomicCompSwap"},
                        {T::InterlockedExchange, "atomicExchange"},
                        {T::InterlockedMax, "atomicMax"},
                        {T::InterlockedMin, "atomicMin"},
                        {T::InterlockedOr, "atomicOr"},
                        {T::InterlockedXor, "atomicXor"},
                        {T::IsInf, "isinf"},
                        {T::IsNaN, "isnan"},
                        {T::LdExp, "ldexp"},
                        {T::Length, "length"},
                        {T::Lerp, "mix"},
                        {T::LessThan, "lessThan"},           // GLSL only
                        {T::LessThanEqual, "lessThanEqual"}, // GLSL only
                        {T::Log, "log"},
                        {T::Log2, "log2"},
                        {T::MAD, "fma"},
                        {T::Max, "max"},
                        {T::Min, "min"},
                        {T::ModF, "modf"},
                        {T::Normalize, "normalize"},
                        {T::NotEqual, "notEqual"}, // GLSL only
                        {T::Not, "not"},           // GLSL only
                        {T::Pow, "pow"},
                        {T::Radians, "radians"},
                        {T::Reflect, "reflect"},
                        {T::Refract, "refract"},
                        {T::Round, "round"},
                        {T::RSqrt, "inversesqrt"},
                        {T::Sign, "sign"},
                        {T::Sin, "sin"},
                        {T::SinH, "sinh"},
                        {T::SmoothStep, "smoothstep"},
                        {T::Sqrt, "sqrt"},
                        {T::Step, "step"},
                        {T::Tan, "tan"},
                        {T::TanH, "tanh"},
                        {T::Transpose, "transpose"},
                        {T::Trunc, "trunc"},

                        {T::Tex1D_2, "texture"},
                        {T::Tex1D_4, "texture"},
                        {T::Tex1DBias, "texture"},
                        {T::Tex1DGrad, "textureGrad"},
                        {T::Tex1DLod, "textureLod"},
                        {T::Tex1DProj, "textureProj"},
                        {T::Tex2D_2, "texture"},
                        {T::Tex2D_4, "texture"},
                        {T::Tex2DBias, "texture"},
                        {T::Tex2DGrad, "textureGrad"},
                        {T::Tex2DLod, "textureLod"},
                        {T::Tex2DProj, "textureProj"},
                        {T::Tex3D_2, "texture"},
                        {T::Tex3D_4, "texture"},
                        {T::Tex3DBias, "texture"},
                        {T::Tex3DGrad, "textureGrad"},
                        {T::Tex3DLod, "textureLod"},
                        {T::Tex3DProj, "textureProj"},
                        {T::TexCube_2, "texture"},
                        {T::TexCube_4, "texture"},
                        {T::TexCubeBias, "texture"},
                        {T::TexCubeGrad, "textureGrad"},
                        {T::TexCubeLod, "textureLod"},

                        {T::Texture_GetDAzensions, "textureSize"},
                        {T::Texture_Load_1, "texelFetch"},
                        {T::Texture_Load_2, "texelFetch"},
                        {T::Texture_Load_3, "texelFetchOffset"},
                        {T::Texture_Gather_2, "textureGather"},
                        {T::Texture_Gather_3, "textureGatherOffset"},
                        {T::Texture_GatherRed_2, "textureGather"},
                        {T::Texture_GatherRed_3, "textureGatherOffset"},
                        {T::Texture_GatherRed_6, "textureGatherOffsets"},
                        {T::Texture_GatherGreen_2, "textureGather"},
                        {T::Texture_GatherGreen_3, "textureGatherOffset"},
                        {T::Texture_GatherGreen_6, "textureGatherOffsets"},
                        {T::Texture_GatherBlue_2, "textureGather"},
                        {T::Texture_GatherBlue_3, "textureGatherOffset"},
                        {T::Texture_GatherBlue_6, "textureGatherOffsets"},
                        {T::Texture_GatherAlpha_2, "textureGather"},
                        {T::Texture_GatherAlpha_3, "textureGatherOffset"},
                        {T::Texture_GatherAlpha_6, "textureGatherOffsets"},
                        {T::Texture_GatherCmp_3, "textureGather"},
                        {T::Texture_GatherCmp_4, "textureGatherOffset"},
                        {T::Texture_GatherCmpRed_3, "textureGather"},
                        {T::Texture_GatherCmpRed_4, "textureGatherOffset"},
                        {T::Texture_GatherCmpRed_7, "textureGatherOffsets"},
                        {T::Texture_Sample_2, "texture"},
                        {T::Texture_Sample_3, "textureOffset"},
                        {T::Texture_SampleBias_3, "texture"},
                        {T::Texture_SampleBias_4, "textureOffset"},
                        {T::Texture_SampleCmp_3, "texture"},
                        {T::Texture_SampleCmp_4, "textureOffset"},
                        {T::Texture_SampleCmpLevelZero_3, "textureLod"},
                        {T::Texture_SampleCmpLevelZero_4, "textureLodOffset"},
                        {T::Texture_SampleGrad_4, "textureGrad"},
                        {T::Texture_SampleGrad_5, "textureGradOffset"},
                        {T::Texture_SampleLevel_3, "textureLod"},
                        {T::Texture_SampleLevel_4, "textureLodOffset"},
                        {T::Texture_QueryLod, "textureQueryLod"},
                        {T::Texture_QueryLodUnclamped, "textureQueryLod"},

                        {T::StreamOutput_Append, "EmitVertex"},
                        {T::StreamOutput_RestartStrip, "EndPrAzitive"},

                        {T::Azage_Load, "AzageLoad"},           // GLSL only
                        {T::Azage_Store, "AzageStore"},         // GLSL only
                        {T::Azage_AtomicAdd, "AzageAtomicAdd"}, // GLSL only
                        {T::Azage_AtomicAnd, "AzageAtomicAnd"}, // GLSL only
                        {T::Azage_AtomicCompSwap,
                         "AzageAtomicCompSwap"}, // GLSL only
                        {T::Azage_AtomicExchange,
                         "AzageAtomicExchange"},                // GLSL only
                        {T::Azage_AtomicMax, "AzageAtomicMax"}, // GLSL only
                        {T::Azage_AtomicMin, "AzageAtomicMin"}, // GLSL only
                        {T::Azage_AtomicOr, "AzageAtomicOr"},   // GLSL only
                        {T::Azage_AtomicXor, "AzageAtomicXor"}, // GLSL only

                        {T::PackHalf2x16, "packHalf2x16"}, // GLSL only
                };
            }

            static std::map<Intrinsic, std::string>
            GenerateIntrinsicGLSL120Map()
            {
                using T = Intrinsic;

                return {
                        {T::Tex1D_2, "texture1D"},
                        {T::Tex1DBias, "texture1D"},
                        {T::Tex1DLod, "texture1DLod"},
                        {T::Tex1DProj, "texture1DProj"},
                        {T::Tex2D_2, "texture2D"},
                        {T::Tex2DBias, "texture2D"},
                        {T::Tex2DLod, "texture2DLod"},
                        {T::Tex2DProj, "texture2DProj"},
                        {T::Tex3D_2, "texture3D"},
                        {T::Tex3DBias, "texture3D"},
                        {T::Tex3DLod, "texture3DLod"},
                        {T::Tex3DProj, "textureProj"},
                        {T::TexCube_2, "textureCube"},
                        {T::TexCubeBias, "textureCube"},
                        {T::TexCubeLod, "textureCubeLod"},

                        {T::Texture_Sample_2, "texture2D"},
                };
            }

            const std::string* IntrinsicToGLSLKeyword(const Intrinsic intr_a,
                                                      bool use_glsl_120_a)
            {
                static const auto intrinsic_glsl_map_ = GenerateIntrinsicGLSLMap();
                static const auto intrinsic_glsl_120_map_ =
                        GenerateIntrinsicGLSL120Map();

                if (use_glsl_120_a)
                {
                    auto it_ = intrinsic_glsl_120_map_.find(intr_a);
                    if (it_ != intrinsic_glsl_120_map_.end())
                        return &(it_->second);
                }

                auto it_ = intrinsic_glsl_map_.find(intr_a);
                return (it_ != intrinsic_glsl_map_.end() ? &(it_->second) : nullptr);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
