#include "Optimizer.hpp"

#include "../AST.hpp"
#include "../ASTFactory.hpp"
#include "ExprEvaluator.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void Optimizer::Optimize(Program& program_a)
            {
                Visit(&program_a);
            }

            void Optimizer::OptimizeStmntList(std::vector<StmntPtr>& stmnts_a)
            {
                for (auto it_ = stmnts_a.begin(); it_ != stmnts_a.end();)
                {
                    if (CanRemoveStmnt(**it_))
                        it_ = stmnts_a.erase(it_);
                    else
                        ++it_;
                }
            }

            void Optimizer::OptimizeExpr(ExprPtr& expr_a)
            {
                if (expr_a)
                {
                    ExprEvaluator expr_evaluator_;
                    if (auto value_ = expr_evaluator_.EvaluateOrDefault(*expr_a))
                    {
                        if (auto literal_expr_ =
                                    ASTFactory::MakeLiteralExprOrNull(value_))
                            expr_a = literal_expr_;
                    }
                }
            }

            bool Optimizer::CanRemoveStmnt(const Stmnt& ast_a) const
            {
                if (ast_a.Type() == AST::Types::NullStmnt)
                    return true;

                if (ast_a.Type() == AST::Types::CodeBlockStmnt)
                {
                    auto& code_block_stmnt_ = static_cast<const CodeBlockStmnt&>(
                            ast_a);
                    if (code_block_stmnt_.code_block_->stmnts_.empty())
                        return true;
                }

                return false;
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void Optimizer::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(CodeBlock)
            {
                (void)args_a;
                OptimizeStmntList(ast_a->stmnts_);
                VISIT_DEFAULT(CodeBlock);
            }

            IMPLEMENT_VISIT_PROC(SwitchCase)
            {
                (void)args_a;
                OptimizeStmntList(ast_a->stmnts_);
                VISIT_DEFAULT(SwitchCase);
            }

            IMPLEMENT_VISIT_PROC(ArrayDAzension)
            {
                (void)args_a;
                OptimizeExpr(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(VarDecl)
            {
                (void)args_a;
                OptimizeExpr(ast_a->initializer_);
            }

            IMPLEMENT_VISIT_PROC(ForLoopStmnt)
            {
                (void)args_a;
                Visit(ast_a->init_stmnt_);
                OptimizeExpr(ast_a->condition_);
                OptimizeExpr(ast_a->iteration_);
                Visit(ast_a->body_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(WhileLoopStmnt)
            {
                (void)args_a;
                OptimizeExpr(ast_a->condition_);
                Visit(ast_a->body_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(DoWhileLoopStmnt)
            {
                (void)args_a;
                OptimizeExpr(ast_a->condition_);
                Visit(ast_a->body_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(IfStmnt)
            {
                (void)args_a;
                OptimizeExpr(ast_a->condition_);
                Visit(ast_a->body_stmnt_);
                Visit(ast_a->else_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(ElseStmnt)
            {
                (void)args_a;
                Visit(ast_a->body_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(SwitchStmnt)
            {
                (void)args_a;
                OptimizeExpr(ast_a->selector_);
                Visit(ast_a->cases_);
            }

            IMPLEMENT_VISIT_PROC(ExprStmnt)
            {
                (void)args_a;
                OptimizeExpr(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(ReturnStmnt)
            {
                (void)args_a;
                OptimizeExpr(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(SequenceExpr)
            {
                (void)args_a;
                VISIT_DEFAULT(SequenceExpr);
                for (auto& sub_expr_ : ast_a->exprs_)
                    OptimizeExpr(sub_expr_);
            }

            IMPLEMENT_VISIT_PROC(TernaryExpr)
            {
                (void)args_a;
                VISIT_DEFAULT(TernaryExpr);
                OptimizeExpr(ast_a->cond_expr_);
                OptimizeExpr(ast_a->then_expr_);
                OptimizeExpr(ast_a->else_expr_);
            }

            IMPLEMENT_VISIT_PROC(BinaryExpr)
            {
                (void)args_a;
                VISIT_DEFAULT(BinaryExpr);
                OptimizeExpr(ast_a->lhs_expr_);
                OptimizeExpr(ast_a->rhs_expr_);
            }

            IMPLEMENT_VISIT_PROC(UnaryExpr)
            {
                (void)args_a;
                VISIT_DEFAULT(UnaryExpr);
                OptimizeExpr(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(PostUnaryExpr)
            {
                (void)args_a;
                VISIT_DEFAULT(PostUnaryExpr);
                OptimizeExpr(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(BracketExpr)
            {
                (void)args_a;
                VISIT_DEFAULT(BracketExpr);

                if (auto sub_bracket_expr_ = ast_a->expr_->As<BracketExpr>())
                    ast_a->expr_ = sub_bracket_expr_->expr_;

                OptimizeExpr(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(ObjectExpr)
            {
                (void)args_a;
                VISIT_DEFAULT(ObjectExpr);
                OptimizeExpr(ast_a->prefix_expr_);
            }

            IMPLEMENT_VISIT_PROC(AssignExpr)
            {
                (void)args_a;
                VISIT_DEFAULT(AssignExpr);
                OptimizeExpr(ast_a->lvalue_expr_);
                OptimizeExpr(ast_a->rvalue_expr_);
            }

            IMPLEMENT_VISIT_PROC(ArrayExpr)
            {
                (void)args_a;
                VISIT_DEFAULT(ArrayExpr);
                for (auto& sub_expr_ : ast_a->array_indices_)
                    OptimizeExpr(sub_expr_);
            }

            IMPLEMENT_VISIT_PROC(CastExpr)
            {
                (void)args_a;
                VISIT_DEFAULT(CastExpr);
                OptimizeExpr(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(InitializerExpr)
            {
                (void)args_a;
                VISIT_DEFAULT(InitializerExpr);
                for (auto& sub_expr_ : ast_a->exprs_)
                    OptimizeExpr(sub_expr_);
            }

#undef AzPLEMENT_VISIT_PROC
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel