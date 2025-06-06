#include "HLSLIntrinsics.hpp"
#include "../../AST/AST.hpp"
#include "../../Exception.hpp"
#include "../../Helper.hpp"
#include "../../Report/ReportIdents.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            static HLSLIntrinsicsMap GenerateIntrinsicMap()
            {
                using T = Intrinsic;

                return {
                        {"abort", {T::Abort, 4, 0}},
                        {"abs", {T::Abs, 1, 1}},
                        {"acos", {T::ACos, 1, 1}},
                        {"all", {T::All, 1, 1}},
                        {"AllMemoryBarrier", {T::AllMemoryBarrier, 5, 0}},
                        {"AllMemoryBarrierWithGroupSync",
                         {T::AllMemoryBarrierWithGroupSync, 5, 0}},
                        {"any", {T::Any, 1, 1}},
                        {"asdouble", {T::AsDouble, 5, 0}},
                        {"asfloat", {T::AsFloat, 4, 0}},
                        {"asin", {T::ASin, 1, 1}},
                        {"asint", {T::AsInt, 4, 0}},
                        {"asuint", {T::AsUInt_1, 4, 0}}, // AsUInt_3: 5.0
                        {"atan", {T::ATan, 1, 1}},
                        {"atan2", {T::ATan2, 1, 1}},
                        {"ceil", {T::Ceil, 1, 1}},
                        {"CheckAccessFullyMapped",
                         {T::CheckAccessFullyMapped, 5, 0}},
                        {"clamp", {T::Clamp, 1, 1}},
                        {"clip", {T::Clip, 1, 1}},
                        {"cos", {T::Cos, 1, 1}},
                        {"cosh", {T::CosH, 1, 1}},
                        {"countbits", {T::CountBits, 5, 0}},
                        {"cross", {T::Cross, 1, 1}},
                        {"D3DCOLORtoUBYTE4", {T::D3DCOLORtoUBYTE4, 1, 1}},
                        {"ddx", {T::DDX, 2, 1}},
                        {"ddx_coarse", {T::DDXCoarse, 5, 0}},
                        {"ddx_fine", {T::DDXFine, 5, 0}},
                        {"ddy", {T::DDY, 2, 1}},
                        {"ddy_coarse", {T::DDYCoarse, 5, 0}},
                        {"ddy_fine", {T::DDYFine, 5, 0}},
                        {"degrees", {T::Degrees, 1, 1}},
                        {"determinant", {T::Determinant, 1, 1}},
                        {"DeviceMemoryBarrier", {T::DeviceMemoryBarrier, 5, 0}},
                        {"DeviceMemoryBarrierWithGroupSync",
                         {T::DeviceMemoryBarrierWithGroupSync, 5, 0}},
                        {"distance", {T::Distance, 1, 1}},
                        {"dot", {T::Dot, 1, 0}},
                        {"dst", {T::Dst, 5, 0}},
                        {"errorf", {T::ErrorF, 4, 0}},
                        {"EvaluateAttributeAtCentroid",
                         {T::EvaluateAttributeAtCentroid, 5, 0}},
                        {"EvaluateAttributeAtSample",
                         {T::EvaluateAttributeAtSample, 5, 0}},
                        {"EvaluateAttributeSnapped",
                         {T::EvaluateAttributeSnapped, 5, 0}},
                        {"exp", {T::Exp, 1, 1}},
                        {"exp2", {T::Exp2, 1, 1}},
                        {"f16tof32", {T::F16toF32, 5, 0}},
                        {"f32tof16", {T::F32toF16, 5, 0}},
                        {"faceforward", {T::FaceForward, 1, 1}},
                        {"firstbithigh", {T::FirstBitHigh, 5, 0}},
                        {"firstbitlow", {T::FirstBitLow, 5, 0}},
                        {"floor", {T::Floor, 1, 1}},
                        {"fma", {T::FMA, 5, 0}},
                        {"fmod", {T::FMod, 1, 1}},
                        {"frac", {T::Frac, 1, 1}},
                        {"frexp", {T::FrExp, 2, 1}},
                        {"fwidth", {T::FWidth, 2, 1}},
                        {"GetRenderTargetSampleCount",
                         {T::GetRenderTargetSampleCount, 4, 0}},
                        {"GetRenderTargetSamplePosition",
                         {T::GetRenderTargetSamplePosition, 4, 0}},
                        {"GroupMemoryBarrier", {T::GroupMemoryBarrier, 5, 0}},
                        {"GroupMemoryBarrierWithGroupSync",
                         {T::GroupMemoryBarrierWithGroupSync, 5, 0}},
                        {"InterlockedAdd", {T::InterlockedAdd, 5, 0}},
                        {"InterlockedAnd", {T::InterlockedAnd, 5, 0}},
                        {"InterlockedCompareExchange",
                         {T::InterlockedCompareExchange, 5, 0}},
                        {"InterlockedCompareStore",
                         {T::InterlockedCompareStore, 5, 0}},
                        {"InterlockedExchange", {T::InterlockedExchange, 5, 0}},
                        {"InterlockedMax", {T::InterlockedMax, 5, 0}},
                        {"InterlockedMin", {T::InterlockedMin, 5, 0}},
                        {"InterlockedOr", {T::InterlockedOr, 5, 0}},
                        {"InterlockedXor", {T::InterlockedXor, 5, 0}},
                        {"isfinite", {T::IsFinite, 1, 1}},
                        {"isinf", {T::IsInf, 1, 1}},
                        {"isnan", {T::IsNaN, 1, 1}},
                        {"ldexp", {T::LdExp, 1, 1}},
                        {"length", {T::Length, 1, 1}},
                        {"lerp", {T::Lerp, 1, 1}},
                        {"lit", {T::Lit, 1, 1}},
                        {"log", {T::Log, 1, 1}},
                        {"log10", {T::Log10, 1, 1}},
                        {"log2", {T::Log2, 1, 1}},
                        {"mad", {T::MAD, 5, 0}},
                        {"max", {T::Max, 1, 1}},
                        {"min", {T::Min, 1, 1}},
                        {"modf", {T::ModF, 1, 1}},
                        {"msad4", {T::MSAD4, 5, 0}},
                        {"mul", {T::Mul, 1, 0}},
                        {"normalize", {T::Normalize, 1, 1}},
                        {"pow", {T::Pow, 1, 1}},
                        {"printf", {T::PrintF, 4, 0}},
                        {"Process2DQuadTessFactorsAvg",
                         {T::Process2DQuadTessFactorsAvg, 5, 0}},
                        {"Process2DQuadTessFactorsMax",
                         {T::Process2DQuadTessFactorsMax, 5, 0}},
                        {"Process2DQuadTessFactorsMin",
                         {T::Process2DQuadTessFactorsMin, 5, 0}},
                        {"ProcessIsolineTessFactors",
                         {T::ProcessIsolineTessFactors, 5, 0}},
                        {"ProcessQuadTessFactorsAvg",
                         {T::ProcessQuadTessFactorsAvg, 5, 0}},
                        {"ProcessQuadTessFactorsMax",
                         {T::ProcessQuadTessFactorsMax, 5, 0}},
                        {"ProcessQuadTessFactorsMin",
                         {T::ProcessQuadTessFactorsMin, 5, 0}},
                        {"ProcessTriTessFactorsAvg",
                         {T::ProcessTriTessFactorsAvg, 5, 0}},
                        {"ProcessTriTessFactorsMax",
                         {T::ProcessTriTessFactorsMax, 5, 0}},
                        {"ProcessTriTessFactorsMin",
                         {T::ProcessTriTessFactorsMin, 5, 0}},
                        {"radians", {T::Radians, 1, 0}},
                        {"rcp", {T::Rcp, 5, 0}},
                        {"reflect", {T::Reflect, 1, 0}},
                        {"refract", {T::Refract, 1, 1}},
                        {"reversebits", {T::ReverseBits, 5, 0}},
                        {"round", {T::Round, 1, 1}},
                        {"rsqrt", {T::RSqrt, 1, 1}},
                        {"saturate", {T::Saturate, 1, 0}},
                        {"sign", {T::Sign, 1, 1}},
                        {"sin", {T::Sin, 1, 1}},
                        {"sincos", {T::SinCos, 1, 1}},
                        {"sinh", {T::SinH, 1, 1}},
                        {"smoothstep", {T::SmoothStep, 1, 1}},
                        {"sqrt", {T::Sqrt, 1, 1}},
                        {"step", {T::Step, 1, 1}},
                        {"tan", {T::Tan, 1, 1}},
                        {"tanh", {T::TanH, 1, 1}},
                        {"transpose", {T::Transpose, 1, 0}},
                        {"trunc", {T::Trunc, 1, 0}},

                        {"tex1D", {T::Tex1D_2, 1, 0}}, // Tex1D_4: 2.1
                        {"tex1Dbias", {T::Tex1DBias, 2, 1}},
                        {"tex1Dgrad", {T::Tex1DGrad, 2, 1}},
                        {"tex1Dlod", {T::Tex1DLod, 3, 1}},
                        {"tex1Dproj", {T::Tex1DProj, 2, 1}},
                        {"tex2D", {T::Tex2D_2, 1, 1}}, // Tex2D_4: 2.1
                        {"tex2Dbias", {T::Tex2DBias, 2, 1}},
                        {"tex2Dgrad", {T::Tex2DGrad, 2, 1}},
                        {"tex2Dlod", {T::Tex2DLod, 3, 0}},
                        {"tex2Dproj", {T::Tex2DProj, 2, 1}},
                        {"tex3D", {T::Tex3D_2, 1, 1}}, // Tex3D_4: 2.1
                        {"tex3Dbias", {T::Tex3DBias, 2, 1}},
                        {"tex3Dgrad", {T::Tex3DGrad, 2, 1}},
                        {"tex3Dlod", {T::Tex3DLod, 3, 1}},
                        {"tex3Dproj", {T::Tex3DProj, 2, 1}},
                        {"texCUBE", {T::TexCube_2, 1, 1}}, // TexCube_4: 2.1
                        {"texCUBEbias", {T::TexCubeBias, 2, 1}},
                        {"texCUBEgrad", {T::TexCubeGrad, 2, 1}},
                        {"texCUBElod", {T::TexCubeLod, 3, 1}},
                        {"texCUBEproj", {T::TexCubeProj, 2, 1}},

                        {"GetDimensions", {T::Texture_GetDimensions, 5, 0}},
                        {"Load", {T::Texture_Load_1, 4, 0}},
                        {"Sample", {T::Texture_Sample_2, 4, 0}},
                        {"SampleBias", {T::Texture_SampleBias_3, 4, 0}},
                        {"SampleCmp", {T::Texture_SampleCmp_3, 4, 0}},
                        {"SampleCmpLevelZero",
                         {T::Texture_SampleCmpLevelZero_3,
                          4,
                          0}}, // Identical to SampleCmp (but only for Level 0)
                        {"SampleGrad", {T::Texture_SampleGrad_4, 4, 0}},
                        {"SampleLevel", {T::Texture_SampleLevel_3, 4, 0}},
                        {"CalculateLevelOfDetail",
                         {T::Texture_QueryLod, 4, 1}}, // Fragment shader only
                        {"CalculateLevelOfDetailUnclamped",
                         {T::Texture_QueryLodUnclamped, 4, 1}}, // Fragment
                                                                // shader only

                        {"Append", {T::StreamOutput_Append, 4, 0}},
                        {"RestartStrip", {T::StreamOutput_RestartStrip, 4, 0}},

                        {"Gather", {T::Texture_Gather_2, 4, 1}},
                        {"GatherRed", {T::Texture_GatherRed_2, 5, 0}},
                        {"GatherGreen", {T::Texture_GatherGreen_2, 5, 0}},
                        {"GatherBlue", {T::Texture_GatherBlue_2, 5, 0}},
                        {"GatherAlpha", {T::Texture_GatherAlpha_2, 5, 0}},
                        {"GatherCmp", {T::Texture_GatherCmp_3, 5, 0}},
                        {"GatherCmpRed", {T::Texture_GatherCmpRed_3, 5, 0}},
                        {"GatherCmpGreen", {T::Texture_GatherCmpGreen_3, 5, 0}},
                        {"GatherCmpBlue", {T::Texture_GatherCmpBlue_3, 5, 0}},
                        {"GatherCmpAlpha", {T::Texture_GatherCmpAlpha_3, 5, 0}},
                };
            }

            enum class IntrinsicReturnType
            {
                Void,

                Bool,
                Int,
                Int2,
                Int3,
                Int4,
                UInt,
                UInt2,
                UInt3,
                UInt4,
                Float,
                Float2,
                Float3,
                Float4,
                Double,

                GenericArg0,
                GenericArg1,
                GenericArg2,

                FloatGenericArg0,
                BoolGenericArg0,
            };

            static DataType IntrinsicReturnTypeToDataType(
                    const IntrinsicReturnType t_a)
            {
                switch (t_a)
                {
                case IntrinsicReturnType::Bool:
                    return DataType::Bool;
                case IntrinsicReturnType::Int:
                    return DataType::Int;
                case IntrinsicReturnType::Int2:
                    return DataType::Int2;
                case IntrinsicReturnType::Int3:
                    return DataType::Int3;
                case IntrinsicReturnType::Int4:
                    return DataType::Int4;
                case IntrinsicReturnType::UInt:
                    return DataType::UInt;
                case IntrinsicReturnType::UInt2:
                    return DataType::UInt2;
                case IntrinsicReturnType::UInt3:
                    return DataType::UInt3;
                case IntrinsicReturnType::UInt4:
                    return DataType::UInt4;
                case IntrinsicReturnType::Float:
                    return DataType::Float;
                case IntrinsicReturnType::Float2:
                    return DataType::Float2;
                case IntrinsicReturnType::Float3:
                    return DataType::Float3;
                case IntrinsicReturnType::Float4:
                    return DataType::Float4;
                case IntrinsicReturnType::Double:
                    return DataType::Double;
                default:
                    return DataType::Undefined;
                }
            }

            static std::size_t IntrinsicReturnTypeToArgIndex(
                    const IntrinsicReturnType t_a)
            {
                switch (t_a)
                {
                case IntrinsicReturnType::GenericArg0:
                    return 0;
                case IntrinsicReturnType::GenericArg1:
                    return 1;
                case IntrinsicReturnType::GenericArg2:
                    return 2;
                default:
                    return (std::size_t)~0;
                }
            }

            static TypeDenoterPtr DeriveCommonTypeDenoter(
                    std::size_t major_arg_index_a,
                    const std::vector<ExprPtr>& args_a,
                    bool use_min_dimension_a = false)
            {
                if (major_arg_index_a < args_a.size())
                {
                    auto common_type_denoter_ =
                            args_a[major_arg_index_a]->GetTypeDenoter()->GetSub();

                    for (std::size_t i_ = 0, n_ = args_a.size(); i_ < n_; ++i_)
                    {
                        if (i_ != major_arg_index_a)
                        {
                            common_type_denoter_ =
                                    TypeDenoter::FindCommonTypeDenoter(
                                            common_type_denoter_,
                                            args_a[i_]->GetTypeDenoter()->GetSub(),
                                            use_min_dimension_a);
                        }
                    }

                    return common_type_denoter_;
                }
                return nullptr;
            }

            struct IntrinsicSignature
            {
                IntrinsicSignature(int num_args_a = 0);
                IntrinsicSignature(int num_args_min_a, int num_args_max_a);
                IntrinsicSignature(IntrinsicReturnType return_type_a,
                                   int num_args_a = 0);

                TypeDenoterPtr GetTypeDenoterWithArgs(
                        const std::vector<ExprPtr>& args_a) const;

                IntrinsicReturnType return_type_ = IntrinsicReturnType::Void;
                int num_args_min_ = 0;
                int num_args_max_ = 0;
            };

            IntrinsicSignature::IntrinsicSignature(int num_args_a)
                : num_args_min_ {num_args_a}, num_args_max_ {num_args_a}
            {}

            IntrinsicSignature::IntrinsicSignature(int num_args_min_a,
                                                   int num_args_max_a)
                : num_args_min_ {num_args_min_a}, num_args_max_ {num_args_max_a}
            {}

            IntrinsicSignature::IntrinsicSignature(
                    IntrinsicReturnType return_type_a,
                    int num_args_a)
                : return_type_ {return_type_a}, num_args_min_ {num_args_a},
                  num_args_max_ {num_args_a}
            {}

            TypeDenoterPtr IntrinsicSignature::GetTypeDenoterWithArgs(
                    const std::vector<ExprPtr>& args_a) const
            {
                if (num_args_min_ >= 0)
                {
                    auto num_min_ = static_cast<std::size_t>(num_args_min_);
                    auto num_max_ = static_cast<std::size_t>(num_args_max_);

                    if (args_a.size() < num_min_ || args_a.size() > num_max_)
                    {
                        RuntimeErr(R_InvalidIntrinsicArgCount(
                                "",
                                (num_min_ < num_max_
                                         ? std::to_string(num_min_) + "-"
                                                   + std::to_string(num_max_)
                                         : std::to_string(num_min_)),
                                args_a.size()));
                    }
                }

                if (return_type_ != IntrinsicReturnType::Void)
                {
                    const auto return_type_fixed_ = IntrinsicReturnTypeToDataType(
                            return_type_);
                    if (return_type_fixed_ != DataType::Undefined)
                        return std::make_shared<BaseTypeDenoter>(
                                return_type_fixed_);

                    const auto return_type_by_arg_index_ =
                            IntrinsicReturnTypeToArgIndex(return_type_);
                    if (return_type_by_arg_index_ < args_a.size())
                        return DeriveCommonTypeDenoter(return_type_by_arg_index_,
                                                       args_a);
                }

                return std::make_shared<VoidTypeDenoter>();
            }

            static std::map<Intrinsic, IntrinsicSignature>
            GenerateIntrinsicSignatureMap()
            {
                using T = Intrinsic;
                using Ret = IntrinsicReturnType;

                return {
                        {T::Abort, {}},
                        {T::Abs, {Ret::GenericArg0, 1}},
                        {T::ACos, {Ret::GenericArg0, 1}},
                        {T::All, {Ret::Bool, 1}},
                        {T::AllMemoryBarrier, {}},
                        {T::AllMemoryBarrierWithGroupSync, {}},
                        {T::Any, {Ret::Bool, 1}},
                        {T::AsDouble, {Ret::Double, 2}},
                        {T::AsFloat, {Ret::GenericArg0, 1}},
                        {T::ASin, {Ret::GenericArg0, 1}},
                        {T::AsInt, {Ret::GenericArg0, 1}},
                        {T::AsUInt_1, {Ret::GenericArg0, 1}},
                        {T::AsUInt_3, {3}},
                        {T::ATan, {Ret::GenericArg0, 1}},
                        {T::ATan2, {Ret::GenericArg1, 2}},
                        {T::Ceil, {Ret::GenericArg0, 1}},
                        {T::CheckAccessFullyMapped, {Ret::Bool, 1}},
                        {T::Clamp, {Ret::GenericArg0, 3}},
                        {T::Clip, {1}},
                        {T::Cos, {Ret::GenericArg0, 1}},
                        {T::CosH, {Ret::GenericArg0, 1}},
                        {T::CountBits, {Ret::UInt, 1}},
                        {T::Cross, {Ret::Float3, 2}},
                        {T::D3DCOLORtoUBYTE4, {Ret::Int4, 1}},
                        {T::DDX, {Ret::GenericArg0, 1}},
                        {T::DDXCoarse, {Ret::GenericArg0, 1}},
                        {T::DDXFine, {Ret::GenericArg0, 1}},
                        {T::DDY, {Ret::GenericArg0, 1}},
                        {T::DDYCoarse, {Ret::GenericArg0, 1}},
                        {T::DDYFine, {Ret::GenericArg0, 1}},
                        {T::Degrees, {Ret::GenericArg0, 1}},
                        {T::Determinant, {Ret::Float, 1}},
                        {T::DeviceMemoryBarrier, {}},
                        {T::DeviceMemoryBarrierWithGroupSync, {}},
                        {T::Distance, {Ret::Float, 2}},
                        {T::Dot, {Ret::Float, 2}}, // float or int
                        {T::Dst, {Ret::GenericArg0, 2}},
                        {T::ErrorF, {-1}},
                        {T::Equal, {Ret::Bool, 2}}, // GLSL only
                        {T::EvaluateAttributeAtCentroid, {Ret::GenericArg0, 1}},
                        {T::EvaluateAttributeAtSample, {Ret::GenericArg0, 2}},
                        {T::EvaluateAttributeSnapped, {Ret::GenericArg0, 2}},
                        {T::Exp, {Ret::GenericArg0, 1}},
                        {T::Exp2, {Ret::GenericArg0, 1}},
                        {T::F16toF32, {Ret::Float, 1}},
                        {T::F32toF16, {Ret::UInt, 1}},
                        {T::FaceForward, {Ret::GenericArg0, 3}},
                        {T::FirstBitHigh, {Ret::Int, 1}},
                        {T::FirstBitLow, {Ret::Int, 1}},
                        {T::Floor, {Ret::GenericArg0, 1}},
                        {T::FMA, {Ret::GenericArg0, 3}},
                        {T::FMod, {Ret::GenericArg0, 2}},
                        {T::Frac, {Ret::GenericArg0, 1}},
                        {T::FrExp, {Ret::GenericArg0, 2}},
                        {T::FWidth, {Ret::GenericArg0, 1}},
                        {T::GetRenderTargetSampleCount, {Ret::UInt}},
                        {T::GetRenderTargetSamplePosition, {Ret::Float2, 1}},
                        {T::GreaterThan, {Ret::Bool, 2}},      // GLSL only
                        {T::GreaterThanEqual, {Ret::Bool, 2}}, // GLSL only
                        {T::GroupMemoryBarrier, {}},
                        {T::GroupMemoryBarrierWithGroupSync, {}},
                        {T::InterlockedAdd, {2, 3}},
                        {T::InterlockedAnd, {2, 3}},
                        {T::InterlockedCompareExchange, {4}},
                        {T::InterlockedCompareStore, {3}},
                        {T::InterlockedExchange, {3}},
                        {T::InterlockedMax, {2, 3}},
                        {T::InterlockedMin, {2, 3}},
                        {T::InterlockedOr, {2, 3}},
                        {T::InterlockedXor, {2, 3}},
                        {T::IsFinite,
                         {Ret::GenericArg0, 1}}, // bool with size as input
                        {T::IsInf,
                         {Ret::GenericArg0, 1}}, // bool with size as input
                        {T::IsNaN,
                         {Ret::GenericArg0, 1}}, // bool with size as input
                        {T::LdExp,
                         {Ret::GenericArg0, 2}}, // float with size as input
                        {T::Length, {Ret::Float, 1}},
                        {T::Lerp, {Ret::GenericArg0, 3}},
                        {T::LessThan, {Ret::Bool, 2}},      // GLSL only
                        {T::LessThanEqual, {Ret::Bool, 2}}, // GLSL only
                        {T::Lit, {Ret::Float4, 3}},
                        {T::Log, {Ret::GenericArg0, 1}},
                        {T::Log10, {Ret::GenericArg0, 1}},
                        {T::Log2, {Ret::GenericArg0, 1}},
                        {T::MAD, {Ret::GenericArg0, 3}},
                        {T::Max, {Ret::GenericArg0, 2}},
                        {T::Min, {Ret::GenericArg0, 2}},
                        {T::ModF, {Ret::GenericArg0, 2}},
                        {T::MSAD4, {Ret::UInt4, 3}},
                        {T::Normalize, {Ret::GenericArg0, 1}},
                        {T::NotEqual, {Ret::Bool, 2}}, // GLSL only
                        {T::Not, {Ret::Bool, 1}},      // GLSL only
                        {T::Pow, {Ret::GenericArg0, 2}},
                        {T::PrintF, {-1}},
                        {T::Process2DQuadTessFactorsAvg, {5}},
                        {T::Process2DQuadTessFactorsMax, {5}},
                        {T::Process2DQuadTessFactorsMin, {5}},
                        {T::ProcessIsolineTessFactors, {4}},
                        {T::ProcessQuadTessFactorsAvg, {5}},
                        {T::ProcessQuadTessFactorsMax, {5}},
                        {T::ProcessQuadTessFactorsMin, {5}},
                        {T::ProcessTriTessFactorsAvg, {5}},
                        {T::ProcessTriTessFactorsMax, {5}},
                        {T::ProcessTriTessFactorsMin, {5}},
                        {T::Radians, {Ret::GenericArg0, 1}},
                        {T::Rcp, {Ret::GenericArg0, 1}},
                        {T::Reflect, {Ret::GenericArg0, 2}},
                        {T::Refract, {Ret::GenericArg0, 3}},
                        {T::ReverseBits, {Ret::UInt, 1}},
                        {T::Round, {Ret::GenericArg0, 1}},
                        {T::RSqrt, {Ret::GenericArg0, 1}},
                        {T::Saturate, {Ret::GenericArg0, 1}},
                        {T::Sign, {Ret::GenericArg0, 1}},
                        {T::Sin, {Ret::GenericArg0, 1}},
                        {T::SinCos, {3}},
                        {T::SinH, {Ret::GenericArg0, 1}},
                        {T::SmoothStep, {Ret::GenericArg2, 3}},
                        {T::Sqrt, {Ret::GenericArg0, 1}},
                        {T::Step, {Ret::GenericArg0, 2}},
                        {T::Tan, {Ret::GenericArg0, 1}},
                        {T::TanH, {Ret::GenericArg0, 1}},
                        {T::Tex1D_2, {Ret::Float4, 2}},
                        {T::Tex1D_4, {Ret::Float4, 4}},
                        {T::Tex1DBias, {Ret::Float4, 2}},
                        {T::Tex1DGrad, {Ret::Float4, 4}},
                        {T::Tex1DLod, {Ret::Float4, 2}},
                        {T::Tex1DProj, {Ret::Float4, 2}},
                        {T::Tex2D_2, {Ret::Float4, 2}},
                        {T::Tex2D_4, {Ret::Float4, 4}},
                        {T::Tex2DBias, {Ret::Float4, 2}},
                        {T::Tex2DGrad, {Ret::Float4, 4}},
                        {T::Tex2DLod, {Ret::Float4, 2}},
                        {T::Tex2DProj, {Ret::Float4, 2}},
                        {T::Tex3D_2, {Ret::Float4, 2}},
                        {T::Tex3D_4, {Ret::Float4, 4}},
                        {T::Tex3DBias, {Ret::Float4, 2}},
                        {T::Tex3DGrad, {Ret::Float4, 4}},
                        {T::Tex3DLod, {Ret::Float4, 2}},
                        {T::Tex3DProj, {Ret::Float4, 2}},
                        {T::TexCube_2, {Ret::Float4, 2}},
                        {T::TexCube_4, {Ret::Float4, 4}},
                        {T::TexCubeBias, {Ret::Float4, 2}},
                        {T::TexCubeGrad, {Ret::Float4, 4}},
                        {T::TexCubeLod, {Ret::Float4, 2}},
                        {T::TexCubeProj, {Ret::Float4, 2}},
                        {T::Trunc, {Ret::GenericArg0, 1}},

                        {T::Texture_GetDimensions, {3}},
                        {T::Texture_QueryLod, {Ret::Float, 2}},
                        {T::Texture_QueryLodUnclamped, {Ret::Float, 2}},

                        {T::Texture_Load_1, {Ret::Float4, 1}},
                        {T::Texture_Load_2, {Ret::Float4, 2}},
                        {T::Texture_Load_3, {Ret::Float4, 3}},

                        {T::Texture_Sample_2, {Ret::Float4, 2}},
                        {T::Texture_Sample_3, {Ret::Float4, 3}},
                        {T::Texture_Sample_4, {Ret::Float4, 4}},
                        {T::Texture_Sample_5, {Ret::Float4, 5}},
                        {T::Texture_SampleBias_3, {Ret::Float4, 3}},
                        {T::Texture_SampleBias_4, {Ret::Float4, 4}},
                        {T::Texture_SampleBias_5, {Ret::Float4, 5}},
                        {T::Texture_SampleBias_6, {Ret::Float4, 6}},
                        {T::Texture_SampleCmp_3, {Ret::Float, 3}},
                        {T::Texture_SampleCmp_4, {Ret::Float, 4}},
                        {T::Texture_SampleCmp_5, {Ret::Float, 5}},
                        {T::Texture_SampleCmp_6, {Ret::Float, 6}},
                        {T::Texture_SampleCmpLevelZero_3, {Ret::Float, 3}},
                        {T::Texture_SampleCmpLevelZero_4, {Ret::Float, 4}},
                        {T::Texture_SampleCmpLevelZero_5, {Ret::Float, 5}},
                        {T::Texture_SampleGrad_4, {Ret::Float4, 4}},
                        {T::Texture_SampleGrad_5, {Ret::Float4, 5}},
                        {T::Texture_SampleGrad_6, {Ret::Float4, 6}},
                        {T::Texture_SampleGrad_7, {Ret::Float4, 7}},
                        {T::Texture_SampleLevel_3, {Ret::Float4, 3}},
                        {T::Texture_SampleLevel_4, {Ret::Float4, 4}},
                        {T::Texture_SampleLevel_5, {Ret::Float4, 5}},

                        {T::Texture_Gather_2, {Ret::Float4, 2}},
                        {T::Texture_Gather_3, {Ret::Float4, 3}},
                        {T::Texture_Gather_4, {Ret::Float4, 4}},
                        {T::Texture_GatherRed_2, {Ret::Float4, 2}},
                        {T::Texture_GatherRed_3, {Ret::Float4, 3}},
                        {T::Texture_GatherRed_4, {Ret::Float4, 4}},
                        {T::Texture_GatherRed_6, {Ret::Float4, 6}},
                        {T::Texture_GatherRed_7, {Ret::Float4, 7}},
                        {T::Texture_GatherGreen_2, {Ret::Float4, 2}},
                        {T::Texture_GatherGreen_3, {Ret::Float4, 3}},
                        {T::Texture_GatherGreen_4, {Ret::Float4, 4}},
                        {T::Texture_GatherGreen_6, {Ret::Float4, 6}},
                        {T::Texture_GatherGreen_7, {Ret::Float4, 7}},
                        {T::Texture_GatherBlue_2, {Ret::Float4, 2}},
                        {T::Texture_GatherBlue_3, {Ret::Float4, 3}},
                        {T::Texture_GatherBlue_4, {Ret::Float4, 4}},
                        {T::Texture_GatherBlue_6, {Ret::Float4, 6}},
                        {T::Texture_GatherBlue_7, {Ret::Float4, 7}},
                        {T::Texture_GatherAlpha_2, {Ret::Float4, 2}},
                        {T::Texture_GatherAlpha_3, {Ret::Float4, 3}},
                        {T::Texture_GatherAlpha_4, {Ret::Float4, 4}},
                        {T::Texture_GatherAlpha_6, {Ret::Float4, 6}},
                        {T::Texture_GatherAlpha_7, {Ret::Float4, 7}},
                        {T::Texture_GatherCmp_3, {Ret::Float4, 3}},
                        {T::Texture_GatherCmp_4, {Ret::Float4, 4}},
                        {T::Texture_GatherCmp_5, {Ret::Float4, 5}},
                        {T::Texture_GatherCmpRed_3, {Ret::Float4, 3}},
                        {T::Texture_GatherCmpRed_4, {Ret::Float4, 4}},
                        {T::Texture_GatherCmpRed_5, {Ret::Float4, 5}},
                        {T::Texture_GatherCmpRed_7, {Ret::Float4, 7}},
                        {T::Texture_GatherCmpRed_8, {Ret::Float4, 8}},
                        {T::Texture_GatherCmpGreen_3, {Ret::Float4, 3}},
                        {T::Texture_GatherCmpGreen_4, {Ret::Float4, 4}},
                        {T::Texture_GatherCmpGreen_5, {Ret::Float4, 5}},
                        {T::Texture_GatherCmpGreen_7, {Ret::Float4, 7}},
                        {T::Texture_GatherCmpGreen_8, {Ret::Float4, 8}},
                        {T::Texture_GatherCmpBlue_3, {Ret::Float4, 3}},
                        {T::Texture_GatherCmpBlue_4, {Ret::Float4, 4}},
                        {T::Texture_GatherCmpBlue_5, {Ret::Float4, 5}},
                        {T::Texture_GatherCmpBlue_7, {Ret::Float4, 7}},
                        {T::Texture_GatherCmpBlue_8, {Ret::Float4, 8}},
                        {T::Texture_GatherCmpAlpha_3, {Ret::Float4, 3}},
                        {T::Texture_GatherCmpAlpha_4, {Ret::Float4, 4}},
                        {T::Texture_GatherCmpAlpha_5, {Ret::Float4, 5}},
                        {T::Texture_GatherCmpAlpha_7, {Ret::Float4, 7}},
                        {T::Texture_GatherCmpAlpha_8, {Ret::Float4, 8}},

                        {T::StreamOutput_Append, {1}},
                        {T::StreamOutput_RestartStrip, {}},

                        {T::Image_Load, {Ret::Float4, 2}},
                        {T::Image_Store, {3}},
                        {T::Image_AtomicAdd, {2, 3}},
                        {T::Image_AtomicAnd, {2, 3}},
                        {T::Image_AtomicCompSwap, {4}},
                        {T::Image_AtomicExchange, {3}},
                        {T::Image_AtomicMax, {2, 3}},
                        {T::Image_AtomicMin, {2, 3}},
                        {T::Image_AtomicOr, {2, 3}},
                        {T::Image_AtomicXor, {2, 3}},

                        {T::PackHalf2x16, {Ret::UInt, 1}},
                };
            }

            static const auto g_intrinsic_signature_map_ =
                    GenerateIntrinsicSignatureMap();

            HLSLIntrinsicAdept::HLSLIntrinsicAdept()
            {
                for (const auto& it_ : HLSLIntrinsicAdept::GetIntrinsicMap())
                    SetIntrinsicIdent(it_.second.intrinsic_, it_.first);

                FillOverloadedIntrinsicIdents();
            }

            TypeDenoterPtr HLSLIntrinsicAdept::GetIntrinsicReturnType(
                    const Intrinsic intrinsic_a,
                    const std::vector<ExprPtr>& args_a,
                    const TypeDenoterPtr& prefix_type_denoter_a) const
            {
                switch (intrinsic_a)
                {
                case Intrinsic::Mul:
                    return DeriveReturnTypeMul(args_a);

                case Intrinsic::Transpose:
                    return DeriveReturnTypeTranspose(args_a);

                case Intrinsic::Not:
                case Intrinsic::Equal:
                case Intrinsic::NotEqual:
                case Intrinsic::LessThan:
                case Intrinsic::LessThanEqual:
                case Intrinsic::GreaterThan:
                case Intrinsic::GreaterThanEqual:
                    return DeriveReturnTypeVectorCompare(args_a);

                default:
                    if (IsTextureLoadIntrinsic(intrinsic_a)
                        || IsTextureSampleIntrinsic(intrinsic_a))
                    {
                        if (IsTextureCompareIntrinsic(intrinsic_a))
                            return DeriveReturnTypeTextureSampleCmp(
                                    GetGenericTextureTypeFromPrefix(
                                            intrinsic_a,
                                            prefix_type_denoter_a));
                        else
                            return DeriveReturnTypeTextureSample(
                                    GetGenericTextureTypeFromPrefix(
                                            intrinsic_a,
                                            prefix_type_denoter_a));
                    } else if (IsTextureGatherIntrisic(intrinsic_a))
                    {
                        if (IsTextureCompareIntrinsic(intrinsic_a))
                            return DeriveReturnTypeTextureGatherCmp(
                                    GetGenericTextureTypeFromPrefix(
                                            intrinsic_a,
                                            prefix_type_denoter_a));
                        else
                            return DeriveReturnTypeTextureGather(
                                    GetGenericTextureTypeFromPrefix(
                                            intrinsic_a,
                                            prefix_type_denoter_a));
                    }

                    return DeriveReturnType(intrinsic_a, args_a);
                }
            }

            std::vector<TypeDenoterPtr>
            HLSLIntrinsicAdept::GetIntrinsicParameterTypes(
                    const Intrinsic intrinsic_a,
                    const std::vector<ExprPtr>& args_a) const
            {
                std::vector<TypeDenoterPtr> param_type_denoters_;

                switch (intrinsic_a)
                {
                case Intrinsic::Dot:
                    DeriveParameterTypes(param_type_denoters_,
                                         intrinsic_a,
                                         args_a,
                                         true);
                    break;
                case Intrinsic::Mul:
                    DeriveParameterTypesMul(param_type_denoters_, args_a);
                    break;
                case Intrinsic::Transpose:
                    DeriveParameterTypesTranspose(param_type_denoters_, args_a);
                    break;
                case Intrinsic::FirstBitHigh:
                case Intrinsic::FirstBitLow:
                    DeriveParameterTypesFirstBit(param_type_denoters_,
                                                 args_a,
                                                 intrinsic_a);
                    break;
                default:
                    DeriveParameterTypes(param_type_denoters_, intrinsic_a, args_a);
                    break;
                }

                return param_type_denoters_;
            }

            std::vector<std::size_t>
            HLSLIntrinsicAdept::GetIntrinsicOutputParameterIndices(
                    const Intrinsic intrinsic_a) const
            {
                switch (intrinsic_a)
                {
                case Intrinsic::AsUInt_3:
                    return {1, 2};

                case Intrinsic::InterlockedAdd:
                case Intrinsic::InterlockedAnd:
                case Intrinsic::InterlockedExchange:
                case Intrinsic::InterlockedMax:
                case Intrinsic::InterlockedMin:
                case Intrinsic::InterlockedOr:
                case Intrinsic::InterlockedXor:
                    return {2};

                case Intrinsic::InterlockedCompareExchange:
                    return {3};

                case Intrinsic::SinCos:
                    return {1, 2};

                default:
                    break;
                }
                return {};
            }

            const HLSLIntrinsicsMap& HLSLIntrinsicAdept::GetIntrinsicMap()
            {
                static const HLSLIntrinsicsMap intrinsic_map_ =
                        GenerateIntrinsicMap();
                return intrinsic_map_;
            }

            TypeDenoterPtr HLSLIntrinsicAdept::DeriveReturnType(
                    const Intrinsic intrinsic_a,
                    const std::vector<ExprPtr>& args_a) const
            {
                auto it_ = g_intrinsic_signature_map_.find(intrinsic_a);
                if (it_ != g_intrinsic_signature_map_.end())
                    return it_->second.GetTypeDenoterWithArgs(args_a);
                else
                    RuntimeErr(R_FailedToDeriveIntrinsicType(
                            GetIntrinsicIdent(intrinsic_a)));
            }

            TypeDenoterPtr HLSLIntrinsicAdept::DeriveReturnTypeMul(
                    const std::vector<ExprPtr>& args_a) const
            {
                if (args_a.size() != 2)
                    RuntimeErr(R_InvalidIntrinsicArgCount("mul"));

                auto type0_ = args_a[0]->GetTypeDenoter()->GetSub();
                auto type1_ = args_a[1]->GetTypeDenoter()->GetSub();
                auto type_den_ = DeriveReturnTypeMulPrimary(args_a, type0_, type1_);

                return type_den_;
            }

            TypeDenoterPtr HLSLIntrinsicAdept::DeriveReturnTypeMulPrimary(
                    const std::vector<ExprPtr>& args_a,
                    const TypeDenoterPtr& type0_a,
                    const TypeDenoterPtr& type1_a) const
            {
                if (args_a.size() != 2)
                    RuntimeErr(R_InvalidIntrinsicArgCount("mul"));

                if (type0_a->IsScalar())
                    return type1_a;

                if (type0_a->IsVector())
                {
                    if (type1_a->IsScalar())
                        return type0_a;

                    if (type1_a->IsVector())
                    {
                        auto base_data_type0_ = BaseDataType(
                                static_cast<BaseTypeDenoter&>(*type0_a).data_type_);
                        return std::make_shared<BaseTypeDenoter>(base_data_type0_);
                    }

                    if (type1_a->IsMatrix())
                    {
                        auto data_type1_ =
                                static_cast<BaseTypeDenoter&>(*type1_a).data_type_;
                        auto base_data_type1_ = BaseDataType(data_type1_);
                        auto matrix_type_dim1_ = MatrixTypeDim(data_type1_);
                        return std::make_shared<BaseTypeDenoter>(
                                VectorDataType(base_data_type1_,
                                               matrix_type_dim1_.second));
                    }
                }

                if (type0_a->IsMatrix())
                {
                    if (type1_a->IsScalar())
                        return type0_a;

                    if (type1_a->IsVector())
                    {
                        auto data_type0_ =
                                static_cast<BaseTypeDenoter&>(*type0_a).data_type_;
                        auto base_data_type0_ = BaseDataType(data_type0_);
                        auto matrix_type_dim0_ = MatrixTypeDim(data_type0_);
                        return std::make_shared<BaseTypeDenoter>(
                                VectorDataType(base_data_type0_,
                                               matrix_type_dim0_.first));
                    }

                    if (type1_a->IsMatrix())
                    {
                        auto data_type0_ =
                                static_cast<BaseTypeDenoter&>(*type0_a).data_type_;
                        auto base_data_type0_ = BaseDataType(data_type0_);
                        auto matrix_type_dim0_ = MatrixTypeDim(data_type0_);

                        auto data_type1_ =
                                static_cast<BaseTypeDenoter&>(*type1_a).data_type_;
                        auto matrix_type_dim1_ = MatrixTypeDim(data_type1_);

                        return std::make_shared<BaseTypeDenoter>(
                                MatrixDataType(base_data_type0_,
                                               matrix_type_dim0_.first,
                                               matrix_type_dim1_.second));
                    }
                }

                RuntimeErr(R_InvalidIntrinsicArgs("mul"));
            }

            TypeDenoterPtr HLSLIntrinsicAdept::DeriveReturnTypeTranspose(
                    const std::vector<ExprPtr>& args_a) const
            {
                if (args_a.size() != 1)
                    RuntimeErr(R_InvalidIntrinsicArgCount("transpose"));

                const auto& arg0_type_den_ =
                        args_a[0]->GetTypeDenoter()->GetAliased();

                if (arg0_type_den_.IsMatrix())
                {
                    auto arg0_data_type_ = static_cast<const BaseTypeDenoter&>(
                                                arg0_type_den_)
                                                .data_type_;
                    auto arg0_base_data_type_ = BaseDataType(arg0_data_type_);
                    auto arg0_matrix_type_dim_ = MatrixTypeDim(arg0_data_type_);
                    return std::make_shared<BaseTypeDenoter>(
                            MatrixDataType(arg0_base_data_type_,
                                           arg0_matrix_type_dim_.second,
                                           arg0_matrix_type_dim_.first));
                }

                RuntimeErr(R_InvalidIntrinsicArgs("transpose"));
            }

            TypeDenoterPtr HLSLIntrinsicAdept::DeriveReturnTypeVectorCompare(
                    const std::vector<ExprPtr>& args_a) const
            {
                if (args_a.size() < 1 || args_a.size() > 2)
                    RuntimeErr(R_InvalidIntrinsicArgCount("vector-compare"));

                auto arg0_type_den_ = args_a[0]->GetTypeDenoter()->GetSub();

                if (auto arg0_base_type_den_ = arg0_type_den_->As<BaseTypeDenoter>())
                {
                    const auto vec_type_size_ = VectorTypeDim(
                            arg0_base_type_den_->data_type_);
                    return std::make_shared<BaseTypeDenoter>(
                            VectorDataType(DataType::Bool, vec_type_size_));
                }

                return arg0_type_den_;
            }

            TypeDenoterPtr HLSLIntrinsicAdept::DeriveReturnTypeTextureSample(
                    const BaseTypeDenoterPtr& generic_type_denoter_a) const
            {
                return generic_type_denoter_a;
            }

            TypeDenoterPtr HLSLIntrinsicAdept::DeriveReturnTypeTextureSampleCmp(
                    const BaseTypeDenoterPtr&) const
            {
                return std::make_shared<BaseTypeDenoter>(DataType::Float);
            }

            TypeDenoterPtr HLSLIntrinsicAdept::DeriveReturnTypeTextureGather(
                    const BaseTypeDenoterPtr& generic_type_denoter_a) const
            {
                return std::make_shared<BaseTypeDenoter>(VectorDataType(
                        BaseDataType(generic_type_denoter_a->data_type_),
                        4));
            }

            TypeDenoterPtr HLSLIntrinsicAdept::DeriveReturnTypeTextureGatherCmp(
                    const BaseTypeDenoterPtr& generic_type_denoter_a) const
            {
                (void)generic_type_denoter_a;
                return std::make_shared<BaseTypeDenoter>(DataType::Float4);
            }

            void HLSLIntrinsicAdept::DeriveParameterTypes(
                    std::vector<TypeDenoterPtr>& param_type_denoters_a,
                    const Intrinsic intrinsic_a,
                    const std::vector<ExprPtr>& args_a,
                    bool use_min_dimension_a) const
            {
                if (!args_a.empty() && IsGlobalIntrinsic(intrinsic_a))
                {
                    auto common_type_denoter_ =
                            DeriveCommonTypeDenoter(0, args_a, use_min_dimension_a);

                    param_type_denoters_a.resize(args_a.size());
                    for (std::size_t i_ = 0, n_ = args_a.size(); i_ < n_; ++i_)
                    {
                        param_type_denoters_a[i_] = common_type_denoter_;
                    }
                }
            }

            void HLSLIntrinsicAdept::DeriveParameterTypesMul(
                    std::vector<TypeDenoterPtr>& param_type_denoters_a,
                    const std::vector<ExprPtr>& args_a) const
            {
                if (args_a.size() != 2)
                    RuntimeErr(R_InvalidIntrinsicArgCount("mul"));

                auto type0_ = args_a[0]->GetTypeDenoter()->GetSub();
                auto type1_ = args_a[1]->GetTypeDenoter()->GetSub();

                if (type0_->IsVector())
                {
                    if (type1_->IsVector())
                    {
                        DeriveParameterTypes(param_type_denoters_a,
                                             Intrinsic::Mul,
                                             args_a);
                    } 
                    else if (type1_->IsMatrix())
                    {
                        param_type_denoters_a.push_back(
                                TypeDenoter::FindCommonTypeDenoter(type0_,
                                                                   type1_));
                        param_type_denoters_a.push_back(type1_);
                    }
                } 
                else if (type0_->IsMatrix())
                {
                    if (type1_->IsVector())
                    {
                        param_type_denoters_a.push_back(type0_);
                        param_type_denoters_a.push_back(
                                TypeDenoter::FindCommonTypeDenoter(type0_,
                                                                   type1_));
                    } 
                    else if (type1_->IsMatrix())
                    {
                        DeriveParameterTypes(param_type_denoters_a,
                                             Intrinsic::Mul,
                                             args_a);
                    }
                }
            }

            void HLSLIntrinsicAdept::DeriveParameterTypesTranspose(
                    std::vector<TypeDenoterPtr>& param_type_denoters_a,
                    const std::vector<ExprPtr>& args_a) const
            {
                (void)args_a;
                (void)param_type_denoters_a;
            }

            void HLSLIntrinsicAdept::DeriveParameterTypesFirstBit(
                    std::vector<TypeDenoterPtr>& param_type_denoters_a,
                    const std::vector<ExprPtr>& args_a,
                    const Intrinsic intrinsic_a) const
            {
                if (args_a.size() != 1)
                    RuntimeErr(R_InvalidIntrinsicArgCount(
                            intrinsic_a == Intrinsic::FirstBitLow
                                    ? "firstbitlow"
                                    : "firstbithigh"));

                auto type0_ = args_a[0]->GetTypeDenoter()->GetSub();

                if (auto base_type0_ = type0_->As<BaseTypeDenoter>())
                {
                    const auto base_data_type_ = BaseDataType(base_type0_->data_type_);

                    if (IsScalarType(base_data_type_)
                        || IsVectorType(base_data_type_))
                    {
                        if (base_data_type_ != DataType::Int
                            && base_data_type_ != DataType::UInt)
                        {
                            const auto int_vector_type_ =
                                    VectorDataType(DataType::Int,
                                                   VectorTypeDim(base_data_type_));
                            type0_ = std::make_shared<BaseTypeDenoter>(
                                    int_vector_type_);
                        }
                        param_type_denoters_a.push_back(type0_);
                    }
                }
            }

            BaseTypeDenoterPtr
            HLSLIntrinsicAdept::GetGenericTextureTypeFromPrefix(
                    const Intrinsic intrinsic_a,
                    const TypeDenoterPtr& prefix_type_denoter_a) const
            {
                const auto& type_den_ = prefix_type_denoter_a->GetAliased();
                if (auto buffer_type_den_ = type_den_.As<BufferTypeDenoter>())
                {
                    auto generic_type_den_ =
                            buffer_type_den_->GetGenericTypeDenoter();
                    if (generic_type_den_->Type() == TypeDenoter::Types::Base)
                    {
                        return std::static_pointer_cast<BaseTypeDenoter>(
                                generic_type_den_);
                    }
                    RuntimeErr(
                            R_ExpectedBaseTypeDen(generic_type_den_->ToString()));
                }
                RuntimeErr(R_MissingTypeInTextureIntrinsic(
                        GetIntrinsicIdent(intrinsic_a)));
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel