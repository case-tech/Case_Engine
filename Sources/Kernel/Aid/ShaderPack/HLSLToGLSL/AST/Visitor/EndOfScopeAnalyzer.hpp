// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../SymbolTable.hpp"
#include "../Token.hpp"
#include "Visitor.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class EndOfScopeAnalyzer : private Visitor
            {
            public:
                void MarkEndOfScopesFromFunction(FunctionDecl& func_decl_a);

            private:
                DECL_VISIT_PROC(CodeBlock);

                DECL_VISIT_PROC(ForLoopStmnt);
                DECL_VISIT_PROC(WhileLoopStmnt);
                DECL_VISIT_PROC(DoWhileLoopStmnt);
                DECL_VISIT_PROC(IfStmnt);
                DECL_VISIT_PROC(SwitchStmnt);
                DECL_VISIT_PROC(ExprStmnt);
                DECL_VISIT_PROC(ReturnStmnt);
                DECL_VISIT_PROC(CtrlTransferStmnt);
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
