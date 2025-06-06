// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../SymbolTable.hpp"
#include "../../Targets.hpp"
#include "../Token.hpp"
#include "VisitorTracker.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class ReferenceAnalyzer : private VisitorTracker
            {
            public:
                void MarkReferencesFromEntryPoint(
                        Program& program_a,
                        const ShaderTarget shader_target_a);

            private:
                bool Reachable(AST* ast_a);

                void VisitStmntList(const std::vector<StmntPtr>& stmnts_a);

                void MarkLValueExpr(const Expr* expr_a);
                void MarkLValueExprObject(const ObjectExpr* object_expr_a);

                DECL_VISIT_PROC(CodeBlock);
                DECL_VISIT_PROC(SwitchCase);
                DECL_VISIT_PROC(TypeSpecifier);

                DECL_VISIT_PROC(VarDecl);
                DECL_VISIT_PROC(StructDecl);
                DECL_VISIT_PROC(BufferDecl);
                DECL_VISIT_PROC(SamplerDecl);

                DECL_VISIT_PROC(FunctionDecl);
                DECL_VISIT_PROC(UniformBufferDecl);
                DECL_VISIT_PROC(BufferDeclStmnt);
                DECL_VISIT_PROC(SamplerDeclStmnt);
                DECL_VISIT_PROC(VarDeclStmnt);

                DECL_VISIT_PROC(UnaryExpr);
                DECL_VISIT_PROC(PostUnaryExpr);
                DECL_VISIT_PROC(CallExpr);
                DECL_VISIT_PROC(ObjectExpr);
                DECL_VISIT_PROC(AssignExpr);

            private:
                Program* program_ = nullptr;
                ShaderTarget shader_target_ = ShaderTarget::VertexShader;

                std::vector<CallExpr*> call_expr_stack_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel