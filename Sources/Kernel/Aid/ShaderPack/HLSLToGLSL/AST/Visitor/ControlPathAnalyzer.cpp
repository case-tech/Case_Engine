#include "ControlPathAnalyzer.hpp"

#include "../AST.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void ControlPathAnalyzer::MarkControlPathsFromFunction(
                    FunctionDecl& func_decl_a)
            {
                Visit(&func_decl_a);
            }

            void ControlPathAnalyzer::PushReturnPath(bool return_path_a)
            {
                return_path_stack_.push(return_path_a);
            }

            bool ControlPathAnalyzer::PopReturnPath()
            {
                if (!return_path_stack_.empty())
                {
                    auto v_ = return_path_stack_.top();
                    return_path_stack_.pop();
                    return v_;
                }

                return false;
            }

            void ControlPathAnalyzer::VisitStmntList(
                    const std::vector<StmntPtr>& stmnts_a)
            {
                bool has_return_path_ = false;

                for (auto& ast_ : stmnts_a)
                {
                    if (has_return_path_)
                    {
                        ast_->flags_ << AST::IsDeadCode;
                    } 
                    else
                    {
                        Visit(ast_);
                        if (PopReturnPath())
                            has_return_path_ = true;
                    }
                }

                PushReturnPath(has_return_path_);
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void ControlPathAnalyzer::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(CodeBlock)
            {
                (void)args_a;
                VisitStmntList(ast_a->stmnts_);
            }

            IMPLEMENT_VISIT_PROC(FunctionDecl)
            {
                (void)args_a;
                Visit(ast_a->code_block_);

                if (!PopReturnPath())
                {
                    if (!ast_a->return_type_->type_denoter_->IsVoid()
                        && !ast_a->IsForwardDecl())
                    {
                        ast_a->flags_ << FunctionDecl::HasNonReturnControlPath;
                    }
                }
            }

            IMPLEMENT_VISIT_PROC(UniformBufferDecl)
            {
                (void)args_a;
                (void)ast_a;
                PushReturnPath(false);
            }

            IMPLEMENT_VISIT_PROC(BufferDeclStmnt)
            {
                (void)args_a;
                (void)ast_a;
                PushReturnPath(false);
            }

            IMPLEMENT_VISIT_PROC(SamplerDeclStmnt)
            {
                (void)args_a;
                (void)ast_a;
                PushReturnPath(false);
            }

            IMPLEMENT_VISIT_PROC(VarDeclStmnt)
            {
                (void)args_a;
                (void)ast_a;
                PushReturnPath(false);
            }

            IMPLEMENT_VISIT_PROC(AliasDeclStmnt)
            {
                (void)args_a;
                (void)ast_a;
                PushReturnPath(false);
            }

            IMPLEMENT_VISIT_PROC(BasicDeclStmnt)
            {
                (void)args_a;
                (void)ast_a;
                PushReturnPath(false);
            }

            IMPLEMENT_VISIT_PROC(NullStmnt)
            {
                (void)args_a;
                (void)ast_a;
                PushReturnPath(false);
            }

            IMPLEMENT_VISIT_PROC(CodeBlockStmnt)
            {
                (void)args_a;
                Visit(ast_a->code_block_);
            }

            IMPLEMENT_VISIT_PROC(ForLoopStmnt)
            {
                (void)args_a;
                Visit(ast_a->body_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(WhileLoopStmnt)
            {
                (void)args_a;
                Visit(ast_a->body_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(DoWhileLoopStmnt)
            {
                (void)args_a;
                Visit(ast_a->body_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(IfStmnt)
            {
                (void)args_a;
                Visit(ast_a->body_stmnt_);
                auto then_path_ = PopReturnPath();

                Visit(ast_a->else_stmnt_);
                auto else_path_ = PopReturnPath();

                PushReturnPath(then_path_ && else_path_);
            }

            IMPLEMENT_VISIT_PROC(ElseStmnt)
            {
                (void)args_a;
                Visit(ast_a->body_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(SwitchStmnt)
            {
                (void)args_a;

                bool has_default_case_ = false;

                for (auto& switch_case_ : ast_a->cases_)
                {
                    if (switch_case_->IsDefaultCase())
                        has_default_case_ = true;

                    VisitStmntList(switch_case_->stmnts_);
                    if (!PopReturnPath())
                    {
                        PushReturnPath(false);
                        return;
                    }
                }

                PushReturnPath(has_default_case_);
            }

            IMPLEMENT_VISIT_PROC(ExprStmnt)
            {
                (void)args_a;
                (void)ast_a;
                PushReturnPath(false);
            }

            IMPLEMENT_VISIT_PROC(ReturnStmnt)
            {
                (void)args_a;
                (void)ast_a;
                PushReturnPath(true);
            }

            IMPLEMENT_VISIT_PROC(CtrlTransferStmnt)
            {
                (void)args_a;
                (void)ast_a;
                PushReturnPath(false);
            }

#undef IMPLEMENT_VISIT_PROC
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
