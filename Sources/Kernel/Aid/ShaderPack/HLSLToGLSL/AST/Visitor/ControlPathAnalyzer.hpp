// Copyright (c) 2025 Case Technologies

#pragma once
#include "Visitor.hpp"

#include <stack>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class ControlPathAnalyzer : private Visitor
            {
            public:
                void MarkControlPathsFromFunction(FunctionDecl& func_decl_a);

            private:
                void PushReturnPath(bool return_path_a);
                bool PopReturnPath();

                void VisitStmntList(const std::vector<StmntPtr>& stmnts_a);

                DECL_VISIT_PROC(CodeBlock);

                DECL_VISIT_PROC(FunctionDecl);
                DECL_VISIT_PROC(UniformBufferDecl);

                DECL_VISIT_PROC(BufferDeclStmnt);
                DECL_VISIT_PROC(SamplerDeclStmnt);
                DECL_VISIT_PROC(VarDeclStmnt);
                DECL_VISIT_PROC(AliasDeclStmnt);
                DECL_VISIT_PROC(BasicDeclStmnt);

                DECL_VISIT_PROC(NullStmnt);
                DECL_VISIT_PROC(CodeBlockStmnt);
                DECL_VISIT_PROC(ForLoopStmnt);
                DECL_VISIT_PROC(WhileLoopStmnt);
                DECL_VISIT_PROC(DoWhileLoopStmnt);
                DECL_VISIT_PROC(IfStmnt);
                DECL_VISIT_PROC(ElseStmnt);
                DECL_VISIT_PROC(SwitchStmnt);
                DECL_VISIT_PROC(ExprStmnt);
                DECL_VISIT_PROC(ReturnStmnt);
                DECL_VISIT_PROC(CtrlTransferStmnt);

            private:
                std::stack<bool> return_path_stack_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
