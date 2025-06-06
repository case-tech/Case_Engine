#include "TypeConverter.hpp"

#include "../AST.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void TypeConverter::Convert(Program& program_a,
                                        const OnVisitVarDecl& on_visit_var_decl_a)
            {
                if (on_visit_var_decl_a)
                {
                    on_visit_var_decl_ = on_visit_var_decl_a;
                    Visit(&program_a);
                }
            }

            void TypeConverter::ConvertExprType(Expr* expr_a)
            {
                if (expr_a && reset_expr_types_)
                    expr_a->ResetTypeDenoter();
            }

            void TypeConverter::ConvertExpr(const ExprPtr& expr_a)
            {
                if (expr_a)
                {
                    Visit(expr_a);

                    if (reset_expr_types_)
                    {
                        expr_a->ResetTypeDenoter();
                        reset_expr_types_ = false;
                    }
                }
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void TypeConverter::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(VarDecl)
            {
                VISIT_DEFAULT(VarDecl);
                if (on_visit_var_decl_(*ast_a))
                    converted_symbols_.insert(ast_a);
            }

            IMPLEMENT_VISIT_PROC(ForLoopStmnt)
            {
                VISIT_DEFAULT(ForLoopStmnt);
                ConvertExpr(ast_a->condition_);
                ConvertExpr(ast_a->iteration_);
            }

            IMPLEMENT_VISIT_PROC(WhileLoopStmnt)
            {
                VISIT_DEFAULT(WhileLoopStmnt);
                ConvertExpr(ast_a->condition_);
            }

            IMPLEMENT_VISIT_PROC(DoWhileLoopStmnt)
            {
                VISIT_DEFAULT(DoWhileLoopStmnt);
                ConvertExpr(ast_a->condition_);
            }

            IMPLEMENT_VISIT_PROC(IfStmnt)
            {
                VISIT_DEFAULT(IfStmnt);
                ConvertExpr(ast_a->condition_);
            }

            IMPLEMENT_VISIT_PROC(SwitchStmnt)
            {
                VISIT_DEFAULT(SwitchStmnt);
                ConvertExpr(ast_a->selector_);
            }

            IMPLEMENT_VISIT_PROC(ExprStmnt)
            {
                VISIT_DEFAULT(ExprStmnt);
                ConvertExpr(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(ReturnStmnt)
            {
                if (ast_a->expr_)
                {
                    VISIT_DEFAULT(ReturnStmnt);
                    ConvertExpr(ast_a->expr_);
                }
            }

            IMPLEMENT_VISIT_PROC(SequenceExpr)
            {
                VISIT_DEFAULT(SequenceExpr);
                ConvertExprType(ast_a);
            }

            IMPLEMENT_VISIT_PROC(TernaryExpr)
            {
                VISIT_DEFAULT(TernaryExpr);
                ConvertExprType(ast_a);
            }

            IMPLEMENT_VISIT_PROC(BinaryExpr)
            {
                VISIT_DEFAULT(BinaryExpr);
                ConvertExprType(ast_a);
            }

            IMPLEMENT_VISIT_PROC(UnaryExpr)
            {
                VISIT_DEFAULT(UnaryExpr);
                ConvertExprType(ast_a);
            }

            IMPLEMENT_VISIT_PROC(PostUnaryExpr)
            {
                VISIT_DEFAULT(PostUnaryExpr);
                ConvertExprType(ast_a);
            }

            IMPLEMENT_VISIT_PROC(CallExpr)
            {
                VISIT_DEFAULT(CallExpr);
                ConvertExprType(ast_a);
            }

            IMPLEMENT_VISIT_PROC(BracketExpr)
            {
                VISIT_DEFAULT(BracketExpr);
                ConvertExprType(ast_a);
            }

            IMPLEMENT_VISIT_PROC(CastExpr)
            {
                VISIT_DEFAULT(CastExpr);
                ConvertExprType(ast_a);
            }

            IMPLEMENT_VISIT_PROC(ObjectExpr)
            {
                VISIT_DEFAULT(ObjectExpr);

                if (auto symbol_ = ast_a->symbol_ref_)
                {
                    if (converted_symbols_.find(symbol_)
                        != converted_symbols_.end())
                        reset_expr_types_ = true;
                }

                ConvertExprType(ast_a);
            }

            IMPLEMENT_VISIT_PROC(AssignExpr)
            {
                VISIT_DEFAULT(AssignExpr);
                ConvertExprType(ast_a);
            }

            IMPLEMENT_VISIT_PROC(ArrayExpr)
            {
                VISIT_DEFAULT(ArrayExpr);
                ConvertExprType(ast_a);
            }

            IMPLEMENT_VISIT_PROC(InitializerExpr)
            {
                VISIT_DEFAULT(InitializerExpr);
                ConvertExprType(ast_a);
            }

#undef IMPLEMENT_VISIT_PROC
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel