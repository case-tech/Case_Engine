// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../Flags.hpp"
#include "../../Variant.hpp"
#include "Visitor.hpp"

#include <functional>
#include <stack>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class ExprEvaluator : private Visitor
            {
            public:
                enum : unsigned int
                {
                    EvaluateReducedBinaryExpr = (1 << 0),
                };

                ExprEvaluator(Flags flags_a = 0);

                using OnObjectExprCallback =
                        std::function<Variant(ObjectExpr* expr_)>;
                
                Variant Evaluate(Expr& expr_a,
                                 const OnObjectExprCallback&
                                         on_object_expr_callback_a = nullptr);

                Variant EvaluateOrDefault(
                        Expr& expr_a,
                        const Variant& default_value_a = {},
                        const OnObjectExprCallback& on_object_expr_callback_a =
                                nullptr);

                void Abort();

            private:
                void Push(const Variant& v_a);
                Variant Pop();

                void SetObjectExprCallback(
                        const OnObjectExprCallback& callback_a);

                Variant EvaluateBinaryOp(const BinaryExpr* ast_a,
                                         Variant lhs_a,
                                         Variant rhs_a);
                
                Variant EvaluateUnaryOp(const UnaryExpr* ast_a, Variant rhs_a);

                DECL_VISIT_PROC(NullExpr);
                DECL_VISIT_PROC(SequenceExpr);
                DECL_VISIT_PROC(LiteralExpr);
                DECL_VISIT_PROC(TypeSpecifierExpr);
                DECL_VISIT_PROC(TernaryExpr);
                DECL_VISIT_PROC(BinaryExpr);
                DECL_VISIT_PROC(UnaryExpr);
                DECL_VISIT_PROC(PostUnaryExpr);
                DECL_VISIT_PROC(CallExpr);
                DECL_VISIT_PROC(BracketExpr);
                DECL_VISIT_PROC(AssignExpr);
                DECL_VISIT_PROC(ObjectExpr);
                DECL_VISIT_PROC(ArrayExpr);
                DECL_VISIT_PROC(CastExpr);
                DECL_VISIT_PROC(InitializerExpr);

            private:
                std::stack<Variant> variant_stack_;
                OnObjectExprCallback on_object_expr_callback_;

                bool throw_on_failure_ = true;
                bool abort_ = false;

                Flags flags_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
