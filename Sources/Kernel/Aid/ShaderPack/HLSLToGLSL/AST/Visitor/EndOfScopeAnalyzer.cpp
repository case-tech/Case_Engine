#include "EndOfScopeAnalyzer.hpp"

#include "../AST.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void EndOfScopeAnalyzer::MarkEndOfScopesFromFunction(
                    FunctionDecl& func_decl_a)
            {
                Visit(func_decl_a.code_block_);
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void EndOfScopeAnalyzer::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(CodeBlock)
            {
                (void)args_a;
                if (!ast_a->stmnts_.empty())
                    Visit(ast_a->stmnts_.back());
            }

            IMPLEMENT_VISIT_PROC(ForLoopStmnt)
            {
                (void)args_a;
                (void)ast_a;
            }

            IMPLEMENT_VISIT_PROC(WhileLoopStmnt)
            {
                (void)args_a;
                (void)ast_a;
            }

            IMPLEMENT_VISIT_PROC(DoWhileLoopStmnt)
            {
                (void)args_a;
                (void)ast_a;
            }

            IMPLEMENT_VISIT_PROC(IfStmnt)
            {
                (void)args_a;
                Visit(ast_a->body_stmnt_);
                Visit(ast_a->else_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(SwitchStmnt)
            {
                (void)args_a;
                (void)ast_a;
            }

            IMPLEMENT_VISIT_PROC(ExprStmnt)
            {
                (void)args_a;
                (void)ast_a;
            }

            IMPLEMENT_VISIT_PROC(ReturnStmnt)
            {
                (void)args_a;
                ast_a->flags_ << ReturnStmnt::IsEndOfFunction;
            }

            IMPLEMENT_VISIT_PROC(CtrlTransferStmnt)
            {
                (void)args_a;
                (void)ast_a;
            }

#undef IMPLEMENT_VISIT_PROC
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
