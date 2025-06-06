// Copyright (c) 2025 Case Technologies

#pragma once
#include "Visitor.hpp"

#include <stack>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class VisitorTracker : public Visitor
            {
            protected:
                bool InsideGlobalScope() const;

                void PushFunctionDecl(FunctionDecl* func_decl_a);
                void PopFunctionDecl();

                bool InsideFunctionDecl() const;
                bool InsideEntryPoint() const;
                bool InsideSecondaryEntryPoint() const;

                FunctionDecl* ActiveFunctionDecl() const;
                StructDecl* ActiveFunctionStructDecl() const;

                void PushCallExpr(CallExpr* call_expr_a);
                void PopCallExpr();

                CallExpr* ActiveCallExpr() const;

                void PushLValueExpr(Expr* expr_a);
                void PopLValueExpr();

                Expr* ActiveLValueExpr() const;

                void PushStructDecl(StructDecl* struct_decl_a);
                void PopStructDecl();

                bool InsideStructDecl() const;
                StructDecl* ActiveStructDecl() const;

                inline const std::vector<StructDecl*>& GetStructDeclStack()
                        const
                {
                    return struct_decl_stack_;
                }

                void PushUniformBufferDecl(
                        UniformBufferDecl* uniform_buffer_decl_a);
                void PopUniformBufferDecl();

                bool InsideUniformBufferDecl() const;

                inline const std::vector<UniformBufferDecl*>&
                GetUniformBufferDeclStack() const
                {
                    return uniform_buffer_decl_stack_;
                }

                void PushVarDeclStmnt(VarDeclStmnt* var_decl_stmnt_a);
                void PopVarDeclStmnt();
                bool InsideVarDeclStmnt() const;

                VarDeclStmnt* ActiveVarDeclStmnt() const;

            private:
                std::stack<FunctionDecl*> func_decl_stack_;
                std::stack<CallExpr*> call_expr_stack_;
                std::stack<Expr*> lvalue_expr_stack_;
                std::vector<StructDecl*> struct_decl_stack_;
                std::vector<UniformBufferDecl*> uniform_buffer_decl_stack_;
                std::stack<VarDeclStmnt*> var_decl_stmnt_stack_;
                std::size_t stack_level_of_entry_point_ = (std::size_t)~0;
                std::size_t stack_level_of_2nd_entry_point_ = (std::size_t)~0;
            };

#undef VISITOR_VISIT_PROC
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel