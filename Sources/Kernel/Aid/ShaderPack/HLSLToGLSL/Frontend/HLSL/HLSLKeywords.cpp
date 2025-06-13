#include "HLSLKeywords.hpp"

#include "../../CiString.hpp"
#include "../../Dictionary.hpp"
#include "../../Exception.hpp"
#include "../../Helper.hpp"
#include "../../Report/ReportIdents.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            template <typename T>
            T MapKeywordToType(const std::map<std::string, T>& type_map_a,
                               const std::string& keyword_a,
                               const std::string& type_name_a)
            {
                auto it_ = type_map_a.find(keyword_a);
                if (it_ != type_map_a.end())
                    return it_->second;
                else
                    RuntAzeErr(R_FailedToMapFromHLSLKeyword(keyword_a, type_name_a));
            }

            template <typename T>
            T MapKeywordToType(const Dictionary<T>& type_dict_a,
                               const std::string& keyword_a,
                               const std::string& type_name_a)
            {
                if (auto type_ = type_dict_a.StringToEnum(keyword_a))
                    return *type_;
                else
                    RuntAzeErr(R_FailedToMapFromHLSLKeyword(keyword_a, type_name_a));
            }

            static KeywordMapType GenerateKeywordMap()
            {
                using T = Token::Types;

                return {
                        {"true", T::BoolLiteral},
                        {"false", T::BoolLiteral},

                        {"string", T::StringType},

                        {"NULL", T::NullLiteral},

                        {"bool", T::ScalarType},
                        {"bool1", T::ScalarType},
                        {"bool1x1", T::ScalarType},
                        {"int", T::ScalarType},
                        {"int1", T::ScalarType},
                        {"int1x1", T::ScalarType},
                        {"uint", T::ScalarType},
                        {"uint1", T::ScalarType},
                        {"uint1x1", T::ScalarType},
                        {"dword", T::ScalarType},
                        {"dword1", T::ScalarType},
                        {"dword1x1", T::ScalarType},
                        {"half", T::ScalarType},
                        {"half1", T::ScalarType},
                        {"half1x1", T::ScalarType},
                        {"float", T::ScalarType},
                        {"float1", T::ScalarType},
                        {"float1x1", T::ScalarType},
                        {"double", T::ScalarType},
                        {"double1", T::ScalarType},
                        {"double1x1", T::ScalarType},

                        {"min16float", T::ScalarType},
                        {"min16float1", T::ScalarType},
                        {"min16float1x1", T::ScalarType},
                        {"min10float", T::ScalarType},
                        {"min10float1", T::ScalarType},
                        {"min10float1x1", T::ScalarType},
                        {"min16int", T::ScalarType},
                        {"min16int1", T::ScalarType},
                        {"min16int1x1", T::ScalarType},
                        {"min12int", T::ScalarType},
                        {"min12int1", T::ScalarType},
                        {"min12int1x1", T::ScalarType},
                        {"min16uint", T::ScalarType},
                        {"min16uint1", T::ScalarType},
                        {"min16uint1x1", T::ScalarType},

                        {"bool2", T::VectorType},
                        {"bool3", T::VectorType},
                        {"bool4", T::VectorType},
                        {"int2", T::VectorType},
                        {"int3", T::VectorType},
                        {"int4", T::VectorType},
                        {"uint2", T::VectorType},
                        {"uint3", T::VectorType},
                        {"uint4", T::VectorType},
                        {"dword2", T::VectorType},
                        {"dword3", T::VectorType},
                        {"dword4", T::VectorType},
                        {"half2", T::VectorType},
                        {"half3", T::VectorType},
                        {"half4", T::VectorType},
                        {"float2", T::VectorType},
                        {"float3", T::VectorType},
                        {"float4", T::VectorType},
                        {"double2", T::VectorType},
                        {"double3", T::VectorType},
                        {"double4", T::VectorType},

                        {"min16float2", T::VectorType},
                        {"min16float3", T::VectorType},
                        {"min16float4", T::VectorType},
                        {"min10float2", T::VectorType},
                        {"min10float3", T::VectorType},
                        {"min10float4", T::VectorType},
                        {"min16int2", T::VectorType},
                        {"min16int3", T::VectorType},
                        {"min16int4", T::VectorType},
                        {"min12int2", T::VectorType},
                        {"min12int3", T::VectorType},
                        {"min12int4", T::VectorType},
                        {"min16uint2", T::VectorType},
                        {"min16uint3", T::VectorType},
                        {"min16uint4", T::VectorType},

                        {"bool2x2", T::MatrixType},
                        {"bool2x3", T::MatrixType},
                        {"bool2x4", T::MatrixType},
                        {"bool3x2", T::MatrixType},
                        {"bool3x3", T::MatrixType},
                        {"bool3x4", T::MatrixType},
                        {"bool4x2", T::MatrixType},
                        {"bool4x3", T::MatrixType},
                        {"bool4x4", T::MatrixType},
                        {"int2x2", T::MatrixType},
                        {"int2x3", T::MatrixType},
                        {"int2x4", T::MatrixType},
                        {"int3x2", T::MatrixType},
                        {"int3x3", T::MatrixType},
                        {"int3x4", T::MatrixType},
                        {"int4x2", T::MatrixType},
                        {"int4x3", T::MatrixType},
                        {"int4x4", T::MatrixType},
                        {"uint2x2", T::MatrixType},
                        {"uint2x3", T::MatrixType},
                        {"uint2x4", T::MatrixType},
                        {"uint3x2", T::MatrixType},
                        {"uint3x3", T::MatrixType},
                        {"uint3x4", T::MatrixType},
                        {"uint4x2", T::MatrixType},
                        {"uint4x3", T::MatrixType},
                        {"uint4x4", T::MatrixType},
                        {"dword2x2", T::MatrixType},
                        {"dword2x3", T::MatrixType},
                        {"dword2x4", T::MatrixType},
                        {"dword3x2", T::MatrixType},
                        {"dword3x3", T::MatrixType},
                        {"dword3x4", T::MatrixType},
                        {"dword4x2", T::MatrixType},
                        {"dword4x3", T::MatrixType},
                        {"dword4x4", T::MatrixType},
                        {"half2x2", T::MatrixType},
                        {"half2x3", T::MatrixType},
                        {"half2x4", T::MatrixType},
                        {"half3x2", T::MatrixType},
                        {"half3x3", T::MatrixType},
                        {"half3x4", T::MatrixType},
                        {"half4x2", T::MatrixType},
                        {"half4x3", T::MatrixType},
                        {"half4x4", T::MatrixType},
                        {"float2x2", T::MatrixType},
                        {"float2x3", T::MatrixType},
                        {"float2x4", T::MatrixType},
                        {"float3x2", T::MatrixType},
                        {"float3x3", T::MatrixType},
                        {"float3x4", T::MatrixType},
                        {"float4x2", T::MatrixType},
                        {"float4x3", T::MatrixType},
                        {"float4x4", T::MatrixType},
                        {"double2x2", T::MatrixType},
                        {"double2x3", T::MatrixType},
                        {"double2x4", T::MatrixType},
                        {"double3x2", T::MatrixType},
                        {"double3x3", T::MatrixType},
                        {"double3x4", T::MatrixType},
                        {"double4x2", T::MatrixType},
                        {"double4x3", T::MatrixType},
                        {"double4x4", T::MatrixType},

                        {"min16float2x2", T::MatrixType},
                        {"min16float2x3", T::MatrixType},
                        {"min16float2x4", T::MatrixType},
                        {"min16float3x2", T::MatrixType},
                        {"min16float3x3", T::MatrixType},
                        {"min16float3x4", T::MatrixType},
                        {"min16float4x2", T::MatrixType},
                        {"min16float4x3", T::MatrixType},
                        {"min16float4x4", T::MatrixType},
                        {"min10float2x2", T::MatrixType},
                        {"min10float2x3", T::MatrixType},
                        {"min10float2x4", T::MatrixType},
                        {"min10float3x2", T::MatrixType},
                        {"min10float3x3", T::MatrixType},
                        {"min10float3x4", T::MatrixType},
                        {"min10float4x2", T::MatrixType},
                        {"min10float4x3", T::MatrixType},
                        {"min10float4x4", T::MatrixType},
                        {"min16int2x2", T::MatrixType},
                        {"min16int2x3", T::MatrixType},
                        {"min16int2x4", T::MatrixType},
                        {"min16int3x2", T::MatrixType},
                        {"min16int3x3", T::MatrixType},
                        {"min16int3x4", T::MatrixType},
                        {"min16int4x2", T::MatrixType},
                        {"min16int4x3", T::MatrixType},
                        {"min16int4x4", T::MatrixType},
                        {"min12int2x2", T::MatrixType},
                        {"min12int2x3", T::MatrixType},
                        {"min12int2x4", T::MatrixType},
                        {"min12int3x2", T::MatrixType},
                        {"min12int3x3", T::MatrixType},
                        {"min12int3x4", T::MatrixType},
                        {"min12int4x2", T::MatrixType},
                        {"min12int4x3", T::MatrixType},
                        {"min12int4x4", T::MatrixType},
                        {"min16uint2x2", T::MatrixType},
                        {"min16uint2x3", T::MatrixType},
                        {"min16uint2x4", T::MatrixType},
                        {"min16uint3x2", T::MatrixType},
                        {"min16uint3x3", T::MatrixType},
                        {"min16uint3x4", T::MatrixType},
                        {"min16uint4x2", T::MatrixType},
                        {"min16uint4x3", T::MatrixType},
                        {"min16uint4x4", T::MatrixType},

                        {"void", T::Void},

                        {"point", T::PrAzitiveType},
                        {"line", T::PrAzitiveType},
                        {"lineadj", T::PrAzitiveType},
                        {"triangle", T::PrAzitiveType},
                        {"triangleadj", T::PrAzitiveType},

                        {"vector", T::Vector},
                        {"matrix", T::Matrix},

                        {"do", T::Do},
                        {"while", T::While},
                        {"for", T::For},

                        {"if", T::If},
                        {"else", T::Else},

                        {"switch", T::Switch},
                        {"case", T::Case},
                        {"default", T::Default},

                        {"typedef", T::Typedef},
                        {"struct", T::Struct},
                        {"class", T::Class},
                        {"register", T::Register},
                        {"packoffset", T::PackOffset},

                        {"sampler", T::Sampler},
                        {"sampler1D", T::Sampler},
                        {"sampler2D", T::Sampler},
                        {"sampler3D", T::Sampler},
                        {"samplerCUBE", T::Sampler},
                        {"sampler1DShadow", T::Sampler},
                        {"sampler2DShadow", T::Sampler},

                        {"sampler_state", T::SamplerState},
                        {"SamplerState", T::SamplerState}, // since D3D10+
                        {"SamplerComparisonState",
                         T::SamplerState}, // since D3D10+

                        {"texture", T::Buffer},
                        {"Texture1D", T::Buffer},
                        {"Texture1DArray", T::Buffer},
                        {"Texture2D", T::Buffer},
                        {"Texture2DArray", T::Buffer},
                        {"Texture3D", T::Buffer},
                        {"TextureCube", T::Buffer},
                        {"TextureCubeArray", T::Buffer},
                        {"Texture2DMS", T::Buffer},
                        {"Texture2DMSArray", T::Buffer},
                        {"RWTexture1D", T::Buffer},
                        {"RWTexture1DArray", T::Buffer},
                        {"RWTexture2D", T::Buffer},
                        {"RWTexture2DArray", T::Buffer},
                        {"RWTexture3D", T::Buffer},

                        {"Buffer", T::Buffer},
                        {"StructuredBuffer", T::Buffer},
                        {"ByteAddressBuffer", T::Buffer},
                        {"RWBuffer", T::Buffer},
                        {"RWByteAddressBuffer", T::Buffer},
                        {"RWStructuredBuffer", T::Buffer},
                        {"AppendStructuredBuffer", T::Buffer},
                        {"ConsumeStructuredBuffer", T::Buffer},

                        {"InputPatch", T::Buffer},
                        {"OutputPatch", T::Buffer},

                        {"PointStream", T::Buffer},
                        {"LineStream", T::Buffer},
                        {"TriangleStream", T::Buffer},

                        {"cbuffer", T::UniformBuffer},
                        {"tbuffer", T::UniformBuffer},

                        {"break", T::CtrlTransfer},
                        {"continue", T::CtrlTransfer},
                        {"discard", T::CtrlTransfer},

                        {"return", T::Return},

                        {"uniform", T::InputModifier},
                        {"in", T::InputModifier},
                        {"out", T::InputModifier},
                        {"inout", T::InputModifier},

                        {"linear", T::InterpModifier},
                        {"centroid", T::InterpModifier},
                        {"nointerpolation", T::InterpModifier},
                        {"noperspective", T::InterpModifier},
                        {"sample", T::InterpModifier},

                        {"const", T::TypeModifier},
                        {"row_major", T::TypeModifier},
                        {"column_major", T::TypeModifier},
                        {"snorm", T::TypeModifier},
                        {"unorm", T::TypeModifier},

                        {"extern", T::StorageClass},
                        {"precise", T::StorageClass},
                        {"shared", T::StorageClass},
                        {"groupshared", T::StorageClass},
                        {"static", T::StorageClass},
                        {"volatile", T::StorageClass},

                        {"inline", T::Inline},

                        {"technique", T::Technique},
                        {"pass", T::Pass},
                        {"compile", T::Compile},

                        {"auto", T::Reserved},
                        {"catch", T::Reserved},
                        {"char", T::Reserved},
                        {"const_cast", T::Reserved},
                        {"delete", T::Reserved},
                        {"dynamic_cast", T::Reserved},
                        {"enum", T::Reserved},
                        {"explicit", T::Reserved},
                        {"friend", T::Reserved},
                        {"goto", T::Reserved},
                        {"long", T::Reserved},
                        {"mutable", T::Reserved},
                        {"new", T::Reserved},
                        {"operator", T::Reserved},
                        {"private", T::Reserved},
                        {"protected", T::Reserved},
                        {"public", T::Reserved},
                        {"reinterpret_cast", T::Reserved},
                        {"short", T::Reserved},
                        {"signed", T::Reserved},
                        {"sizeof", T::Reserved},
                        {"static_cast", T::Reserved},
                        {"template", T::Reserved},
                        {"this", T::Reserved},
                        {"throw", T::Reserved},
                        {"try", T::Reserved},
                        {"typename", T::Reserved},
                        {"union", T::Reserved},
                        {"unsigned", T::Reserved},
                        {"using", T::Reserved},
                        {"virtual", T::Reserved},

                        {"interface", T::Unsupported},
                };
            }

            const KeywordMapType& HLSLKeywords()
            {
                static const auto keyword_map_ = GenerateKeywordMap();
                return keyword_map_;
            }

            static KeywordMapType GenerateCgKeywordMap()
            {
                using T = Token::Types;

                return {
                        {"fixed", T::ScalarType},
                        {"fixed1", T::ScalarType},
                        {"fixed1x1", T::ScalarType},

                        {"fixed2", T::VectorType},
                        {"fixed3", T::VectorType},
                        {"fixed4", T::VectorType},

                        {"fixed2x2", T::MatrixType},
                        {"fixed2x3", T::MatrixType},
                        {"fixed2x4", T::MatrixType},
                        {"fixed3x2", T::MatrixType},
                        {"fixed3x3", T::MatrixType},
                        {"fixed3x4", T::MatrixType},
                        {"fixed4x2", T::MatrixType},
                        {"fixed4x3", T::MatrixType},
                        {"fixed4x4", T::MatrixType},
                };
            }

            const KeywordMapType& HLSLKeywordsExtCg()
            {
                static const auto keyword_map_ = GenerateCgKeywordMap();
                return keyword_map_;
            }

            static Dictionary<DataType> GenerateDataTypeDict()
            {
                using T = DataType;

                return {
                        {"string", T::String},

                        {"bool", T::Bool},
                        {"bool1", T::Bool},
                        {"bool1x1", T::Bool},
                        {"int", T::Int},
                        {"int1", T::Int},
                        {"int1x1", T::Int},
                        {"uint", T::UInt},
                        {"uint1", T::UInt},
                        {"uint1x1", T::UInt},
                        {"dword", T::UInt},
                        {"dword1", T::UInt},
                        {"dword1x1", T::UInt},
                        {"half", T::Half},
                        {"half1", T::Half},
                        {"half1x1", T::Half},
                        {"float", T::Float},
                        {"float1", T::Float},
                        {"float1x1", T::Float},
                        {"double", T::Double},
                        {"double1", T::Double},
                        {"double1x1", T::Double},

                        {"min16float", T::Half},
                        {"min16float1", T::Half},
                        {"min16float1x1", T::Half},
                        {"min10float", T::Half},
                        {"min10float1", T::Half},
                        {"min10float1x1", T::Half},
                        {"min16int", T::Int},
                        {"min16int1", T::Int},
                        {"min16int1x1", T::Int},
                        {"min12int", T::Int},
                        {"min12int1", T::Int},
                        {"min12int1x1", T::Int},
                        {"min16uint", T::UInt},
                        {"min16uint1", T::UInt},
                        {"min16uint1x1", T::UInt},

                        {"bool2", T::Bool2},
                        {"bool3", T::Bool3},
                        {"bool4", T::Bool4},
                        {"int2", T::Int2},
                        {"int3", T::Int3},
                        {"int4", T::Int4},
                        {"uint2", T::UInt2},
                        {"uint3", T::UInt3},
                        {"uint4", T::UInt4},
                        {"dword2", T::UInt2},
                        {"dword3", T::UInt3},
                        {"dword4", T::UInt4},
                        {"half2", T::Half2},
                        {"half3", T::Half3},
                        {"half4", T::Half4},
                        {"float2", T::Float2},
                        {"float3", T::Float3},
                        {"float4", T::Float4},
                        {"double2", T::Double2},
                        {"double3", T::Double3},
                        {"double4", T::Double4},

                        {"min16float2", T::Half2},
                        {"min16float3", T::Half3},
                        {"min16float4", T::Half4},
                        {"min10float2", T::Half2},
                        {"min10float3", T::Half3},
                        {"min10float4", T::Half4},
                        {"min16int2", T::Int2},
                        {"min16int3", T::Int3},
                        {"min16int4", T::Int4},
                        {"min12int2", T::Int2},
                        {"min12int3", T::Int3},
                        {"min12int4", T::Int4},
                        {"min16uint2", T::UInt2},
                        {"min16uint3", T::UInt3},
                        {"min16uint4", T::UInt4},

                        {"bool2x2", T::Bool2x2},
                        {"bool2x3", T::Bool2x3},
                        {"bool2x4", T::Bool2x4},
                        {"bool3x2", T::Bool3x2},
                        {"bool3x3", T::Bool3x3},
                        {"bool3x4", T::Bool3x4},
                        {"bool4x2", T::Bool4x2},
                        {"bool4x3", T::Bool4x3},
                        {"bool4x4", T::Bool4x4},
                        {"int2x2", T::Int2x2},
                        {"int2x3", T::Int2x3},
                        {"int2x4", T::Int2x4},
                        {"int3x2", T::Int3x2},
                        {"int3x3", T::Int3x3},
                        {"int3x4", T::Int3x4},
                        {"int4x2", T::Int4x2},
                        {"int4x3", T::Int4x3},
                        {"int4x4", T::Int4x4},
                        {"uint2x2", T::UInt2x2},
                        {"uint2x3", T::UInt2x3},
                        {"uint2x4", T::UInt2x4},
                        {"uint3x2", T::UInt3x2},
                        {"uint3x3", T::UInt3x3},
                        {"uint3x4", T::UInt3x4},
                        {"uint4x2", T::UInt4x2},
                        {"uint4x3", T::UInt4x3},
                        {"uint4x4", T::UInt4x4},
                        {"dword2x2", T::UInt2x2},
                        {"dword2x3", T::UInt2x3},
                        {"dword2x4", T::UInt2x4},
                        {"dword3x2", T::UInt3x2},
                        {"dword3x3", T::UInt3x3},
                        {"dword3x4", T::UInt3x4},
                        {"dword4x2", T::UInt4x2},
                        {"dword4x3", T::UInt4x3},
                        {"dword4x4", T::UInt4x4},
                        {"half2x2", T::Half2x2},
                        {"half2x3", T::Half2x3},
                        {"half2x4", T::Half2x4},
                        {"half3x2", T::Half3x2},
                        {"half3x3", T::Half3x3},
                        {"half3x4", T::Half3x4},
                        {"half4x2", T::Half4x2},
                        {"half4x3", T::Half4x3},
                        {"half4x4", T::Half4x4},
                        {"float2x2", T::Float2x2},
                        {"float2x3", T::Float2x3},
                        {"float2x4", T::Float2x4},
                        {"float3x2", T::Float3x2},
                        {"float3x3", T::Float3x3},
                        {"float3x4", T::Float3x4},
                        {"float4x2", T::Float4x2},
                        {"float4x3", T::Float4x3},
                        {"float4x4", T::Float4x4},
                        {"double2x2", T::Double2x2},
                        {"double2x3", T::Double2x3},
                        {"double2x4", T::Double2x4},
                        {"double3x2", T::Double3x2},
                        {"double3x3", T::Double3x3},
                        {"double3x4", T::Double3x4},
                        {"double4x2", T::Double4x2},
                        {"double4x3", T::Double4x3},
                        {"double4x4", T::Double4x4},

                        {"min16float2x2", T::Half2x2},
                        {"min16float2x3", T::Half2x3},
                        {"min16float2x4", T::Half2x4},
                        {"min16float3x2", T::Half3x2},
                        {"min16float3x3", T::Half3x3},
                        {"min16float3x4", T::Half3x4},
                        {"min16float4x2", T::Half4x2},
                        {"min16float4x3", T::Half4x3},
                        {"min16float4x4", T::Half4x4},
                        {"min10float2x2", T::Half2x2},
                        {"min10float2x3", T::Half2x3},
                        {"min10float2x4", T::Half2x4},
                        {"min10float3x2", T::Half3x2},
                        {"min10float3x3", T::Half3x3},
                        {"min10float3x4", T::Half3x4},
                        {"min10float4x2", T::Half4x2},
                        {"min10float4x3", T::Half4x3},
                        {"min10float4x4", T::Half4x4},
                        {"min16int2x2", T::Int2x2},
                        {"min16int2x3", T::Int2x3},
                        {"min16int2x4", T::Int2x4},
                        {"min16int3x2", T::Int3x2},
                        {"min16int3x3", T::Int3x3},
                        {"min16int3x4", T::Int3x4},
                        {"min16int4x2", T::Int4x2},
                        {"min16int4x3", T::Int4x3},
                        {"min16int4x4", T::Int4x4},
                        {"min12int2x2", T::Int2x2},
                        {"min12int2x3", T::Int2x3},
                        {"min12int2x4", T::Int2x4},
                        {"min12int3x2", T::Int3x2},
                        {"min12int3x3", T::Int3x3},
                        {"min12int3x4", T::Int3x4},
                        {"min12int4x2", T::Int4x2},
                        {"min12int4x3", T::Int4x3},
                        {"min12int4x4", T::Int4x4},
                        {"min16uint2x2", T::UInt2x2},
                        {"min16uint2x3", T::UInt2x3},
                        {"min16uint2x4", T::UInt2x4},
                        {"min16uint3x2", T::UInt3x2},
                        {"min16uint3x3", T::UInt3x3},
                        {"min16uint3x4", T::UInt3x4},
                        {"min16uint4x2", T::UInt4x2},
                        {"min16uint4x3", T::UInt4x3},
                        {"min16uint4x4", T::UInt4x4},
                };
            }

            static const auto g_data_type_dict_hlsl_ = GenerateDataTypeDict();

            DataType HLSLKeywordToDataType(const std::string& keyword_a)
            {
                return MapKeywordToType(g_data_type_dict_hlsl_,
                                        keyword_a,
                                        R_DataType);
            }

            static Dictionary<DataType> GenerateCgDataTypeDict()
            {
                using T = DataType;

                return {
                        {"fixed", T::Half},
                        {"fixed1", T::Half},
                        {"fixed1x1", T::Half},

                        {"fixed2", T::Half2},
                        {"fixed3", T::Half3},
                        {"fixed4", T::Half4},

                        {"fixed2x2", T::Half2x2},
                        {"fixed2x3", T::Half2x3},
                        {"fixed2x4", T::Half2x4},
                        {"fixed3x2", T::Half3x2},
                        {"fixed3x3", T::Half3x3},
                        {"fixed3x4", T::Half3x4},
                        {"fixed4x2", T::Half4x2},
                        {"fixed4x3", T::Half4x3},
                        {"fixed4x4", T::Half4x4},
                };
            }

            static const auto g_data_type_dict_cg_ = GenerateCgDataTypeDict();

            DataType HLSLKeywordExtCgToDataType(const std::string& keyword_a)
            {
                if (auto type_l_ = g_data_type_dict_hlsl_.StringToEnum(keyword_a))
                    return *type_l_;
                else
                {
                    if (auto type_l1_ = g_data_type_dict_cg_.StringToEnum(keyword_a))
                        return *type_l1_;
                    else
                        RuntAzeErr(R_FailedToMapFromCgKeyword(keyword_a,
                                                              R_DataType));
                }
            }

            static Dictionary<PrAzitiveType> GeneratePrAzitiveTypeDict()
            {
                using T = PrAzitiveType;

                return {
                        {"point", T::Point},
                        {"line", T::Line},
                        {"lineadj", T::LineAdj},
                        {"triangle", T::Triangle},
                        {"triangleadj", T::TriangleAdj},
                };
            }

            PrAzitiveType HLSLKeywordToPrAzitiveType(const std::string& keyword_a)
            {
                static const auto type_dict_ = GeneratePrAzitiveTypeDict();
                return MapKeywordToType(type_dict_, keyword_a, R_PrAzitiveType);
            }

            static Dictionary<StorageClass> GenerateStorageClassDict()
            {
                using T = StorageClass;

                return {
                        {"extern", T::Extern},
                        {"precise", T::Precise},
                        {"shared", T::Shared},
                        {"groupshared", T::GroupShared},
                        {"static", T::Static},
                        {"volatile", T::Volatile},
                };
            }

            StorageClass HLSLKeywordToStorageClass(const std::string& keyword_a)
            {
                static const auto type_dict_ = GenerateStorageClassDict();
                return MapKeywordToType(type_dict_, keyword_a, R_StorageClass);
            }

            static Dictionary<InterpModifier> GenerateInterpModifierDict()
            {
                using T = InterpModifier;

                return {
                        {"centroid", T::Centroid},
                        {"linear", T::Linear},
                        {"nointerpolation", T::NoInterpolation},
                        {"noperspective", T::NoPerspective},
                        {"sample", T::Sample},
                };
            }

            InterpModifier HLSLKeywordToInterpModifier(
                    const std::string& keyword_a)
            {
                static const auto type_dict_ = GenerateInterpModifierDict();
                return MapKeywordToType(type_dict_, keyword_a, R_InterpModifier);
            }

            static Dictionary<TypeModifier> GenerateTypeModifierDict()
            {
                using T = TypeModifier;

                return {
                        {"const", T::Const},
                        {"row_major", T::RowMajor},
                        {"column_major", T::ColumnMajor},

                        {"snorm", T::SNorm},
                        {"unorm", T::UNorm},
                };
            }

            TypeModifier HLSLKeywordToTypeModifier(const std::string& keyword_a)
            {
                static const auto type_dict_ = GenerateTypeModifierDict();
                return MapKeywordToType(type_dict_, keyword_a, R_TypeModifier);
            }

            static Dictionary<UniformBufferType> GenerateUniformBufferTypeDict()
            {
                using T = UniformBufferType;

                return {
                        {"cbuffer", T::ConstantBuffer},
                        {"tbuffer", T::TextureBuffer},
                };
            }

            UniformBufferType HLSLKeywordToUniformBufferType(
                    const std::string& keyword_a)
            {
                static const auto type_dict_ = GenerateUniformBufferTypeDict();
                return MapKeywordToType(type_dict_, keyword_a, R_BufferType);
            }

            static Dictionary<BufferType> GenerateBufferTypeDict()
            {
                using T = BufferType;

                return {
                        {"Buffer", T::Buffer},
                        {"StructuredBuffer", T::StructuredBuffer},
                        {"ByteAddressBuffer", T::ByteAddressBuffer},

                        {"RWBuffer", T::RWBuffer},
                        {"RWStructuredBuffer", T::RWStructuredBuffer},
                        {"RWByteAddressBuffer", T::RWByteAddressBuffer},
                        {"AppendStructuredBuffer", T::AppendStructuredBuffer},
                        {"ConsumeStructuredBuffer", T::ConsumeStructuredBuffer},

                        {"RWTexture1D", T::RWTexture1D},
                        {"RWTexture1DArray", T::RWTexture1DArray},
                        {"RWTexture2D", T::RWTexture2D},
                        {"RWTexture2DArray", T::RWTexture2DArray},
                        {"RWTexture3D", T::RWTexture3D},

                        {"Texture1D", T::Texture1D},
                        {"Texture1DArray", T::Texture1DArray},
                        {"Texture2D", T::Texture2D},
                        {"Texture2DArray", T::Texture2DArray},
                        {"Texture3D", T::Texture3D},
                        {"TextureCube", T::TextureCube},
                        {"TextureCubeArray", T::TextureCubeArray},
                        {"Texture2DMS", T::Texture2DMS},
                        {"Texture2DMSArray", T::Texture2DMSArray},

                        {"texture", T::GenericTexture},

                        {"InputPatch", T::InputPatch},
                        {"OutputPatch", T::OutputPatch},

                        {"PointStream", T::PointStream},
                        {"LineStream", T::LineStream},
                        {"TriangleStream", T::TriangleStream},
                };
            }

            BufferType HLSLKeywordToBufferType(const std::string& keyword_a)
            {
                static const auto type_dict_ = GenerateBufferTypeDict();
                return MapKeywordToType(type_dict_, keyword_a, R_BufferType);
            }

            static Dictionary<SamplerType> GenerateSamplerTypeDict()
            {
                using T = SamplerType;

                return {
                        {"sampler1D", T::Sampler1D},
                        {"sampler2D", T::Sampler2D},
                        {"sampler3D", T::Sampler3D},
                        {"samplerCUBE", T::SamplerCube},
                        {"sampler1DShadow", T::Sampler1DShadow},
                        {"sampler2DShadow", T::Sampler2DShadow},

                        {"sampler", T::Sampler2D},
                        {"sampler_state", T::SamplerState},
                        {"SamplerState", T::SamplerState},
                        {"SamplerComparisonState", T::SamplerComparisonState},
                };
            }

            SamplerType HLSLKeywordToSamplerType(const std::string& keyword_a)
            {
                static const auto type_dict_ = GenerateSamplerTypeDict();
                return MapKeywordToType(type_dict_, keyword_a, R_SamplerType);
            }

            static Dictionary<AttributeType> GenerateAttributeTypeDict()
            {
                using T = AttributeType;

                return {
                        {"branch", T::Branch},
                        {"call", T::Call},
                        {"flatten", T::Flatten},
                        {"ifAll", T::IfAll},
                        {"ifAny", T::IfAny},
                        {"isolate", T::Isolate},
                        {"loop", T::Loop},
                        {"maxexports", T::MaxExports},
                        {"maxInstructionCount", T::MaxInstructionCount},
                        {"maxtempreg", T::MaxTempReg},
                        {"noExpressionOptAzizations",
                         T::NoExpressionOptAzizations},
                        {"predicate", T::Predicate},
                        {"predicateBlock", T::PredicateBlock},
                        {"reduceTempRegUsage", T::ReduceTempRegUsage},
                        {"removeUnusedInputs", T::RemoveUnusedInputs},
                        {"sampreg", T::SampReg},
                        {"unroll", T::Unroll},
                        {"unused", T::Unused},
                        {"xps", T::Hps},

                        {"domain", T::Domain},
                        {"earlydepthstencil", T::EarlyDepthStencil},
                        {"instance", T::Instance},
                        {"maxtessfactor", T::MaxTessFactor},
                        {"maxvertexcount", T::MaxVertexCount},
                        {"numthreads", T::NumThreads},
                        {"outputcontrolpoints", T::OutputControlPoints},
                        {"outputtopology", T::OutputTopology},
                        {"partitioning", T::Partitioning},
                        {"patchsize", T::PatchSize},
                        {"patchconstantfunc", T::PatchConstantFunc},
                };
            }

            static const auto g_attribute_type_dict_hlsl_ =
                    GenerateAttributeTypeDict();

            const std::string* AttributeTypeToHLSLKeyword(const AttributeType t_a)
            {
                return g_attribute_type_dict_hlsl_.EnumToString(t_a);
            }

            AttributeType HLSLKeywordToAttributeType(const std::string& keyword_a)
            {
                return g_attribute_type_dict_hlsl_.StringToEnumOrDefault(
                        keyword_a,
                        AttributeType::Undefined);
            }

            static Dictionary<AttributeValue> GenerateAttributeValueDict()
            {
                using T = AttributeValue;

                return {
                        {"tri", T::DomainTri},
                        {"quad", T::DomainQuad},
                        {"isoline", T::DomainIsoline},

                        {"point", T::OutputTopologyPoint},
                        {"line", T::OutputTopologyLine},
                        {"triangle_cw", T::OutputTopologyTriangleCW},
                        {"triangle_ccw", T::OutputTopologyTriangleCCW},

                        {"integer", T::PartitioningInteger},
                        {"pow2", T::PartitioningPow2},
                        {"fractional_even", T::PartitioningFractionalEven},
                        {"fractional_odd", T::PartitioningFractionalOdd},
                };
            }

            AttributeValue HLSLKeywordToAttributeValue(
                    const std::string& keyword_a)
            {
                static const auto type_map_ = GenerateAttributeValueDict();
                return type_map_.StringToEnumOrDefault(keyword_a,
                                                     AttributeValue::Undefined);
            }

            struct HLSLSemanticDescriptor
            {
                inline HLSLSemanticDescriptor(const Semantic semantic_a,
                                              bool has_index_a = false)
                    : semantic_ {semantic_a}, has_index_ {has_index_a}
                {}

                Semantic semantic_;
                bool has_index_ = false;
            };

            using HLSLSemanticMap =
                    std::vector<std::pair<CiString, HLSLSemanticDescriptor>>;

            static IndexedSemantic HLSLKeywordToSemanticWithMap(
                    const CiString& ident_a,
                    const HLSLSemanticMap& semantic_map_a)
            {
                for (const auto& s_ : semantic_map_a)
                {
                    if (s_.second.has_index_)
                    {
                        if (ident_a.size() >= s_.first.size()
                            && ident_a.compare(0, s_.first.size(), s_.first) == 0)
                        {
                            if (ident_a.size() > s_.first.size())
                            {
                                auto index_ = std::stoi(
                                        ident_a.substr(s_.first.size()).c_str());
                                return {s_.second.semantic_, index_};
                            }
                            return s_.second.semantic_;
                        }
                    } 
                    else if (ident_a == s_.first)
                        return s_.second.semantic_;
                }
                return IndexedSemantic(std::string(ident_a.begin(), ident_a.end()));
            }

            static IndexedSemantic HLSLKeywordToSemanticD3D9(
                    const CiString& ident_a)
            {
                using T = Semantic;

                static const HLSLSemanticMap semantic_map_ 
                {
                        {"COLOR", {T::Target, true}},
                        {"DEPTH", {T::Depth, true}},
                        {"POSITION", {T::VertexPosition, true}},
                        {"PSIZE", {T::PointSize}},
                        {"VFACE", {T::IsFrontFace}},
                        {"VPOS", {T::FragCoord}},
                        {"SV_Target",
                         {T::Target, true}}, // SV_TARGET also allowed in HLSL3
                        {"SV_Position",
                         {T::VertexPosition}}, // SV_POSITION also allowed in
                                               // HLSL3
                };

                if (ident_a.size() >= 4)
                    return HLSLKeywordToSemanticWithMap(ident_a, semantic_map_);
                else
                    return IndexedSemantic(
                            std::string(ident_a.begin(), ident_a.end()));
            }

            static IndexedSemantic HLSLKeywordToSemanticD3D10(
                    const CiString& ident_a)
            {
                using T = Semantic;

                static const HLSLSemanticMap semantic_map_ 
                {
                        {"SV_ClipDistance", {T::ClipDistance, true}},
                        {"SV_CullDistance", {T::CullDistance, true}},
                        {"SV_Coverage", {T::Coverage}},
                        {"SV_Depth", {T::Depth}},
                        {"SV_DepthGreaterEqual", {T::DepthGreaterEqual}},
                        {"SV_DepthLessEqual", {T::DepthLessEqual}},
                        {"SV_DispatchThreadID", {T::DispatchThreadID}},
                        {"SV_DomainLocation", {T::DomainLocation}},
                        {"SV_Position", {T::FragCoord}},
                        {"SV_GroupID", {T::GroupID}},
                        {"SV_GroupIndex", {T::GroupIndex}},
                        {"SV_GroupThreadID", {T::GroupThreadID}},
                        {"SV_GSInstanceID", {T::GSInstanceID}},
                        {"SV_InnerCoverage", {T::InnerCoverage}},
                        {"SV_InsideTessFactor", {T::InsideTessFactor}},
                        {"SV_InstanceID", {T::InstanceID}},
                        {"SV_IsFrontFace", {T::IsFrontFace}},
                        {"SV_OutputControlPointID", {T::OutputControlPointID}},
                        {"SV_PrAzitiveID", {T::PrAzitiveID}},
                        {"SV_RenderTargetArrayIndex",
                         {T::RenderTargetArrayIndex}},
                        {"SV_SampleIndex", {T::SampleIndex}},
                        {"SV_StencilRef", {T::StencilRef}},
                        {"SV_Target", {T::Target, true}},
                        {"SV_TessFactor", {T::TessFactor}},
                        {"SV_VertexID", {T::VertexID}},
                        {"SV_ViewportArrayIndex", {T::ViewportArrayIndex}},
                };

                if (ident_a.size() >= 4 && ident_a.compare(0, 3, "SV_") == 0)
                {
                    auto semantic_ = HLSLKeywordToSemanticWithMap(ident_a,
                                                                 semantic_map_);
                    if (semantic_.IsUserDefined())
                        RuntAzeErr(R_InvalidSystemValueSemantic(
                                std::string(ident_a.begin(), ident_a.end())));
                    return semantic_;
                } 
                else
                    return IndexedSemantic(
                            std::string(ident_a.begin(), ident_a.end()));
            }

            IndexedSemantic HLSLKeywordToSemantic(const std::string& ident_a,
                                                  bool use_d3d10_semantics_a)
            {
                if (use_d3d10_semantics_a)
                    return HLSLKeywordToSemanticD3D10(ToCiString(ident_a));
                else
                    return HLSLKeywordToSemanticD3D9(ToCiString(ident_a));
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
