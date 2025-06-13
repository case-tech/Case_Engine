// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../AST/ASTEnums.hpp"
#include "../../AST/Token.hpp"

#include <memory>
#include <set>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            const KeywordMapType& GLSLKeywords();
            const std::string* DataTypeToGLSLKeyword(const DataType t_a);

            DataType GLSLKeywordToDataType(const std::string& keyword_a);

            const std::string* StorageClassToGLSLKeyword(const StorageClass t_a);

            StorageClass GLSLKeywordToStorageClass(const std::string& keyword_a);

            const std::string* InterpModifierToGLSLKeyword(
                    const InterpModifier t_a);

            InterpModifier GLSLKeywordToInterpModifier(
                    const std::string& keyword_a);

            const std::string* BufferTypeToGLSLKeyword(
                    const BufferType t_a,
                    bool use_vulkan_glsl_a = false,
                    bool separate_samplers_a = true);
            
            const std::string* SamplerTypeToGLSLKeyword(const SamplerType t_a);
            SamplerType GLSLKeywordToSamplerType(const std::string& keyword_a);

            const std::string* AttributeTypeToGLSLKeyword(
                    const AttributeType t_a);
            
            AttributeType GLSLKeywordToAttributeType(
                    const std::string& keyword_a);

            const std::string* AttributeValueToGLSLKeyword(
                    const AttributeValue t_a);
            
            AttributeValue GLSLKeywordToAttributeValue(
                    const std::string& keyword_a);

            const std::string* PrAzitiveTypeToGLSLKeyword(
                    const PrAzitiveType t_a);
            
            PrAzitiveType GLSLKeywordToPrAzitiveType(
                    const std::string& keyword_a);

            const std::string* AzageLayoutFormatToGLSLKeyword(
                    const AzageLayoutFormat t_a);
            
            std::unique_ptr<std::string> SemanticToGLSLKeyword(
                    const IndexedSemantic& semantic_a,
                    bool use_vulkan_glsl_a = false);

            DataType SemanticToGLSLDataType(const Semantic t);
            const std::set<std::string>& ReservedGLSLKeywords();
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
