// Copyright (c) 2025 Case Technologies

#pragma once
#include "Export.hpp"

#undef max

#include <limits>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            namespace Reflection
            {
                enum class Filter
                {
                    MinMagMipPoint = 0,
                    MinMagPointMipLinear = 0x1,
                    MinPointMagLinearMipPoint = 0x4,
                    MinPointMagMipLinear = 0x5,
                    MinLinearMagMipPoint = 0x10,
                    MinLinearMagPointMipLinear = 0x11,
                    MinMagLinearMipPoint = 0x14,
                    MinMagMipLinear = 0x15,
                    Anisotropic = 0x55,
                    ComparisonMinMagMipPoint = 0x80,
                    ComparisonMinMagPointMipLinear = 0x81,
                    ComparisonMinPointMagLinearMipPoint = 0x84,
                    ComparisonMinPointMagMipLinear = 0x85,
                    ComparisonMinLinearMagMipPoint = 0x90,
                    ComparisonMinLinearMagPointMipLinear = 0x91,
                    ComparisonMinMagLinearMipPoint = 0x94,
                    ComparisonMinMagMipLinear = 0x95,
                    ComparisonAnisotropic = 0xd5,
                    MinimumMinMagMipPoint = 0x100,
                    MinimumMinMagPointMipLinear = 0x101,
                    MinimumMinPointMagLinearMipPoint = 0x104,
                    MinimumMinPointMagMipLinear = 0x105,
                    MinimumMinLinearMagMipPoint = 0x110,
                    MinimumMinLinearMagPointMipLinear = 0x111,
                    MinimumMinMagLinearMipPoint = 0x114,
                    MinimumMinMagMipLinear = 0x115,
                    MinimumAnisotropic = 0x155,
                    MaximumMinMagMipPoint = 0x180,
                    MaximumMinMagPointMipLinear = 0x181,
                    MaximumMinPointMagLinearMipPoint = 0x184,
                    MaximumMinPointMagMipLinear = 0x185,
                    MaximumMinLinearMagMipPoint = 0x190,
                    MaximumMinLinearMagPointMipLinear = 0x191,
                    MaximumMinMagLinearMipPoint = 0x194,
                    MaximumMinMagMipLinear = 0x195,
                    MaximumAnisotropic = 0x1d5,
                };

                enum class TextureAddressMode
                {
                    Wrap = 1,
                    Mirror = 2,
                    Clamp = 3,
                    Border = 4,
                    MirrorOnce = 5,
                };

                enum class ComparisonFunc
                {
                    Never = 1,
                    Less = 2,
                    Equal = 3,
                    LessEqual = 4,
                    Greater = 5,
                    NotEqual = 6,
                    GreaterEqual = 7,
                    Always = 8,
                };

                enum class FieldType
                {
                    Undefined,
                    Bool,
                    Int,
                    UInt,
                    Half,
                    Float,
                    Double,
                    Record,
                };

                enum class ResourceType
                {
                    Undefined,

                    Texture1D,
                    Texture2D,
                    Texture3D,
                    TextureCube,
                    Texture1DArray,
                    Texture2DArray,
                    TextureCubeArray,
                    Texture2DMS,
                    Texture2DMSArray,

                    RWTexture1D,
                    RWTexture2D,
                    RWTexture3D,
                    RWTextureCube,
                    RWTexture1DArray,
                    RWTexture2DArray,
                    RWTextureCubeArray,
                    RWTexture2DMS,
                    RWTexture2DMSArray,

                    Sampler1D,
                    Sampler2D,
                    Sampler3D,
                    SamplerCube,
                    Sampler1DArray,
                    Sampler2DArray,
                    SamplerCubeArray,
                    Sampler2DMS,
                    Sampler2DMSArray,
                    Sampler2DRect,

                    Buffer,
                    ByteAddressBuffer,
                    StructuredBuffer,
                    AppendStructuredBuffer,
                    ConsumeStructuredBuffer,

                    RWBuffer,
                    RWByteAddressBuffer,
                    RWStructuredBuffer,

                    ConstantBuffer,
                    TextureBuffer,
                    SamplerState,
                    SamplerComparisonState,
                };

                struct SamplerStateDesc
                {
                    Filter filter_ = Filter::MinMagMipLinear;
                    TextureAddressMode address_u_ = TextureAddressMode::Clamp;
                    TextureAddressMode address_v_ = TextureAddressMode::Clamp;
                    TextureAddressMode address_w_ = TextureAddressMode::Clamp;
                    float mip_lod_bias_ = 0.0f;
                    unsigned int max_anisotropy_ = 1u;
                    ComparisonFunc comparison_func_ = ComparisonFunc::Never;
                    float border_color_[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                    float min_lod_ = -std::numeric_limits<float>::max();
                    float max_lod_ = std::numeric_limits<float>::max();
                };

                struct Attribute
                {
                    Attribute() = default;

                    inline Attribute(const std::string& name_a, int slot_a)
                        : name_ {name_a}, slot_ {slot_a}
                    {}

                    bool referenced_ = false;
                    std::string name_;
                    int slot_ = -1;
                };

                struct Resource
                {
                    bool referenced_ = false;
                    ResourceType type_ = ResourceType::Undefined;
                    std::string name_;
                    int slot_ = -1;
                };

                struct Field
                {
                    bool referenced_ = false;
                    std::string name_;
                    FieldType type_ = FieldType::Undefined;

                    unsigned int dimensions_[2] = {1, 1};
                    int type_record_index_ = -1;
                    unsigned int size_ = 0;
                    unsigned int offset_ = 0;
                    std::vector<unsigned int> array_elements_;
                };

                struct Record
                {
                    bool referenced_ = false;
                    std::string name_;
                    int base_record_index_ = -1;
                    std::vector<Field> fields_;
                    unsigned int size_ = 0;
                    unsigned int padding_ = 0;
                };

                struct ConstantBuffer
                {
                    bool referenced_ = false;
                    ResourceType type_ = ResourceType::Undefined;
                    std::string name_;
                    int slot_ = -1;
                    std::vector<Field> fields_;
                    unsigned int size_ = 0;
                    unsigned int padding_ = 0;
                };

                struct SamplerState
                {
                    ResourceType type_ = ResourceType::Undefined;
                    std::string name_;
                    int slot_ = -1;
                    bool referenced_ = false;
                };

                struct StaticSamplerState
                {
                    ResourceType type_ = ResourceType::Undefined;
                    std::string name_;
                    SamplerStateDesc desc_;
                };

                struct NumThreads
                {
                    int x_ = 0;
                    int y_ = 0;
                    int z_ = 0;
                };

                struct ReflectionData
                {
                    std::vector<std::string> macros_;
                    std::vector<Record> records_;
                    std::vector<Attribute> input_attributes_;
                    std::vector<Attribute> output_attributes_;
                    std::vector<Attribute> uniforms_;
                    std::vector<Resource> resources_;
                    std::vector<ConstantBuffer> constant_buffers_;
                    std::vector<SamplerState> sampler_states_;
                    std::vector<StaticSamplerState> static_sampler_states_;
                    NumThreads num_threads_;
                };
            } // namespace Reflection

            HTG_EXPORT std::string ToString(const Reflection::Filter t_a);
            HTG_EXPORT std::string ToString(
                    const Reflection::TextureAddressMode t_a);
            
            HTG_EXPORT std::string ToString(const Reflection::ComparisonFunc t_a);
            HTG_EXPORT std::string ToString(const Reflection::ResourceType t_a);
            HTG_EXPORT void PrintReflection(
                    std::ostream& stream_a,
                    const Reflection::ReflectionData& reflection_data_a,
                    bool referenced_only_a = false);
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel