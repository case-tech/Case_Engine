// Copyright (c) 2025 Case Technologies

#pragma once
#include "../AST/ASTEnums.hpp"
#include "../AST/TypeDenoter.hpp"
#include "../AST/Visitor/Visitor.hpp"

#include <array>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
#define INTRINSIC_IDX(I)                                                       \
    (static_cast<std::size_t>(I) - static_cast<std::size_t>(Intrinsic::Abort))

            class IntrinsicAdept
            {
            public:
                IntrinsicAdept(const IntrinsicAdept&) = delete;
                IntrinsicAdept& operator=(const IntrinsicAdept&) = delete;

                virtual ~IntrinsicAdept();

                static const IntrinsicAdept& Get();
                const std::string& GetIntrinsicIdent(
                        const Intrinsic intrinsic_a) const;

                virtual TypeDenoterPtr GetIntrinsicReturnType(
                        const Intrinsic intrinsic_a,
                        const std::vector<ExprPtr>& args_a,
                        const TypeDenoterPtr& prefix_type_denoter_a =
                                nullptr) const = 0;

                virtual std::vector<TypeDenoterPtr> GetIntrinsicParameterTypes(
                        const Intrinsic intrinsic_a,
                        const std::vector<ExprPtr>& args_a) const = 0;
                
                virtual std::vector<std::size_t>
                GetIntrinsicOutputParameterIndices(
                        const Intrinsic intrinsic_a) const = 0;

            protected:
                IntrinsicAdept();

                void SetIntrinsicIdent(const Intrinsic intrinsic_a,
                                       const std::string& ident_a);
                
                void FillOverloadedIntrinsicIdents();

                [[noreturn]]
                void ThrowAmbiguousIntrinsicCall(
                        const Intrinsic intrinsic_a,
                        const std::vector<ExprPtr>& args_a);

            private:
                static const std::size_t num_intrinsics_ =
                        (INTRINSIC_IDX(Intrinsic::StreamOutput_RestartStrip)
                         + 1u);
                std::array<std::string, num_intrinsics_> intrinsic_idents_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel