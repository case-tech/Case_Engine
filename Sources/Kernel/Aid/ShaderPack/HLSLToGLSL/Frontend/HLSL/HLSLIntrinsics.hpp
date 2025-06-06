// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../AST/ASTEnums.hpp"
#include "../../AST/TypeDenoter.hpp"
#include "../../ShaderVersion.hpp"
#include "../IntrinsicAdept.hpp"

#include <map>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            struct HLSLIntrinsicEntry
            {
                inline HLSLIntrinsicEntry(Intrinsic intrinsic_a,
                                          int major_a,
                                          int minor_a)
                    : intrinsic_ {intrinsic_a}, min_shader_model_ {major_a, minor_a}
                {}

                Intrinsic intrinsic_;
                ShaderVersion min_shader_model_;
            };

            using HLSLIntrinsicsMap = std::map<std::string, HLSLIntrinsicEntry>;

            class HLSLIntrinsicAdept : public IntrinsicAdept
            {
            public:
                HLSLIntrinsicAdept();

                TypeDenoterPtr GetIntrinsicReturnType(
                        const Intrinsic intrinsic_a,
                        const std::vector<ExprPtr>& args_a,
                        const TypeDenoterPtr& prefix_type_denoter_a =
                                nullptr) const override;

                std::vector<TypeDenoterPtr> GetIntrinsicParameterTypes(
                        const Intrinsic intrinsic_a,
                        const std::vector<ExprPtr>& args_a) const override;

                std::vector<std::size_t> GetIntrinsicOutputParameterIndices(
                        const Intrinsic intrinsic_a) const override;

                static const HLSLIntrinsicsMap& GetIntrinsicMap();

            private:
                TypeDenoterPtr DeriveReturnType(
                        const Intrinsic intrinsic_a,
                        const std::vector<ExprPtr>& args_a) const;
                
                TypeDenoterPtr DeriveReturnTypeMul(
                        const std::vector<ExprPtr>& args_a) const;
                
                TypeDenoterPtr DeriveReturnTypeMulPrimary(
                        const std::vector<ExprPtr>& args_a,
                        const TypeDenoterPtr& type0_a,
                        const TypeDenoterPtr& type1_a) const;
                
                TypeDenoterPtr DeriveReturnTypeTranspose(
                        const std::vector<ExprPtr>& args_a) const;
                
                TypeDenoterPtr DeriveReturnTypeVectorCompare(
                        const std::vector<ExprPtr>& args_a) const;
                
                TypeDenoterPtr DeriveReturnTypeTextureSample(
                        const BaseTypeDenoterPtr& generic_type_denoter_a) const;
                
                TypeDenoterPtr DeriveReturnTypeTextureSampleCmp(
                        const BaseTypeDenoterPtr& generic_type_denoter_a) const;
                
                TypeDenoterPtr DeriveReturnTypeTextureGather(
                        const BaseTypeDenoterPtr& generic_type_denoter_a) const;
                
                TypeDenoterPtr DeriveReturnTypeTextureGatherCmp(
                        const BaseTypeDenoterPtr& generic_type_denoter_a) const;

                void DeriveParameterTypes(
                        std::vector<TypeDenoterPtr>& param_type_denoters_a,
                        const Intrinsic intrinsic_a,
                        const std::vector<ExprPtr>& args_a,
                        bool use_min_dimension_a = false) const;

                void DeriveParameterTypesMul(
                        std::vector<TypeDenoterPtr>& param_type_denoters_a,
                        const std::vector<ExprPtr>& args_a) const;
                
                void DeriveParameterTypesTranspose(
                        std::vector<TypeDenoterPtr>& param_type_denoters_a,
                        const std::vector<ExprPtr>& args_a) const;
                
                void DeriveParameterTypesFirstBit(
                        std::vector<TypeDenoterPtr>& param_type_denoters_a,
                        const std::vector<ExprPtr>& args_a,
                        const Intrinsic intrinsic_a) const;

                BaseTypeDenoterPtr GetGenericTextureTypeFromPrefix(
                        const Intrinsic intrinsic_a,
                        const TypeDenoterPtr& prefix_type_denoter_a) const;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
