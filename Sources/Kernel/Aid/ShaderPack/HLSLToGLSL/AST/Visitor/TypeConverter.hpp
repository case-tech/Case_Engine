// Copyright (c) 2025 Case Technologies

#pragma once
#include "Visitor.hpp"

#include <functional>
#include <set>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class TypeConverter : public Visitor
            {
            public:
                using OnVisitVarDecl = std::function<bool(VarDecl& var_decl_)>;
                void Convert(Program& program_a,
                             const OnVisitVarDecl& on_visit_var_decl_a);

            private:
                void ConvertExprType(Expr* expr_a);
                void ConvertExpr(const ExprPtr& expr_a);

                DECL_VISIT_PROC(VarDecl);

                DECL_VISIT_PROC(ForLoopStmnt);
                DECL_VISIT_PROC(WhileLoopStmnt);
                DECL_VISIT_PROC(DoWhileLoopStmnt);
                DECL_VISIT_PROC(IfStmnt);
                DECL_VISIT_PROC(SwitchStmnt);
                DECL_VISIT_PROC(ExprStmnt);
                DECL_VISIT_PROC(ReturnStmnt);

                DECL_VISIT_PROC(SequenceExpr);
                DECL_VISIT_PROC(TernaryExpr);
                DECL_VISIT_PROC(BinaryExpr);
                DECL_VISIT_PROC(UnaryExpr);
                DECL_VISIT_PROC(PostUnaryExpr);
                DECL_VISIT_PROC(CallExpr);
                DECL_VISIT_PROC(BracketExpr);
                DECL_VISIT_PROC(CastExpr);
                DECL_VISIT_PROC(ObjectExpr);
                DECL_VISIT_PROC(AssignExpr);
                DECL_VISIT_PROC(ArrayExpr);
                DECL_VISIT_PROC(InitializerExpr);

            private:
                OnVisitVarDecl on_visit_var_decl_;

                bool reset_expr_types_ = false;
                std::set<AST*> converted_symbols_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
