#include "Visitor.hpp"

#include "../../Report/ReportIdents.hpp"
#include "../AST.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void Visitor::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(Program)
            {
                (void)args_a;
                Visit(ast_a->global_stmnts_);
            }

            IMPLEMENT_VISIT_PROC(CodeBlock)
            {
                (void)args_a;
                Visit(ast_a->stmnts_);
            }

            IMPLEMENT_VISIT_PROC(Attribute)
            {
                (void)args_a;
                Visit(ast_a->arguments_);
            }

            IMPLEMENT_VISIT_PROC(SwitchCase)
            {
                (void)args_a;
                Visit(ast_a->expr_);
                Visit(ast_a->stmnts_);
            }

            IMPLEMENT_VISIT_PROC(SamplerValue)
            {
                (void)args_a;
                Visit(ast_a->value_);
            }

            IMPLEMENT_VISIT_PROC(Register)
            {
                (void)args_a;
                (void)ast_a;
            }

            IMPLEMENT_VISIT_PROC(PackOffset)
            {
                (void)args_a;
                (void)ast_a;
            }

            IMPLEMENT_VISIT_PROC(ArrayDimension)
            {
                (void)args_a;
                Visit(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(TypeSpecifier)
            {
                (void)args_a;
                Visit(ast_a->struct_decl_);
            }

            IMPLEMENT_VISIT_PROC(VarDecl)
            {
                (void)args_a;
                Visit(ast_a->namespace_expr_);
                Visit(ast_a->array_dims_);
                Visit(ast_a->slot_registers_);
                Visit(ast_a->pack_offset_);
                Visit(ast_a->annotations_);
                Visit(ast_a->initializer_);
            }

            IMPLEMENT_VISIT_PROC(BufferDecl)
            {
                (void)args_a;
                Visit(ast_a->array_dims_);
                Visit(ast_a->slot_registers_);
                Visit(ast_a->annotations_);
            }

            IMPLEMENT_VISIT_PROC(SamplerDecl)
            {
                (void)args_a;
                Visit(ast_a->array_dims_);
                Visit(ast_a->slot_registers_);
                Visit(ast_a->sampler_values_);
            }

            IMPLEMENT_VISIT_PROC(StructDecl)
            {
                (void)args_a;
                Visit(ast_a->local_stmnts_);
            }

            IMPLEMENT_VISIT_PROC(AliasDecl)
            {
                (void)args_a;
                (void)ast_a;
            }

            IMPLEMENT_VISIT_PROC(FunctionDecl)
            {
                (void)args_a;
                Visit(ast_a->return_type_);
                Visit(ast_a->parameters_);
                Visit(ast_a->annotations_);
                Visit(ast_a->code_block_);
            }

            IMPLEMENT_VISIT_PROC(UniformBufferDecl)
            {
                (void)args_a;
                Visit(ast_a->slot_registers_);
                Visit(ast_a->local_stmnts_);
            }

            IMPLEMENT_VISIT_PROC(BufferDeclStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->buffer_decls_);
            }

            IMPLEMENT_VISIT_PROC(SamplerDeclStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->sampler_decls_);
            }

            IMPLEMENT_VISIT_PROC(VarDeclStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->type_specifier_);
                Visit(ast_a->var_decls_);
            }

            IMPLEMENT_VISIT_PROC(AliasDeclStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->struct_decl_);
                Visit(ast_a->alias_decls_);
            }

            IMPLEMENT_VISIT_PROC(BasicDeclStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->decl_object_);
            }

            IMPLEMENT_VISIT_PROC(NullStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
            }

            IMPLEMENT_VISIT_PROC(CodeBlockStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->code_block_);
            }

            IMPLEMENT_VISIT_PROC(ForLoopStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->init_stmnt_);
                Visit(ast_a->condition_);
                Visit(ast_a->iteration_);
                Visit(ast_a->body_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(WhileLoopStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->condition_);
                Visit(ast_a->body_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(DoWhileLoopStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->body_stmnt_);
                Visit(ast_a->condition_);
            }

            IMPLEMENT_VISIT_PROC(IfStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->condition_);
                Visit(ast_a->body_stmnt_);
                Visit(ast_a->else_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(ElseStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->body_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(SwitchStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->selector_);
                Visit(ast_a->cases_);
            }

            IMPLEMENT_VISIT_PROC(ExprStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(ReturnStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                Visit(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(CtrlTransferStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
            }

            IMPLEMENT_VISIT_PROC(LayoutStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
            }

            IMPLEMENT_VISIT_PROC(NullExpr)
            {
                (void)args_a;
                (void)ast_a;
            }

            IMPLEMENT_VISIT_PROC(SequenceExpr)
            {
                (void)args_a;
                Visit(ast_a->exprs_);
            }

            IMPLEMENT_VISIT_PROC(LiteralExpr)
            {
                (void)args_a;
                (void)ast_a;
            }

            IMPLEMENT_VISIT_PROC(TypeSpecifierExpr)
            {
                (void)args_a;
                Visit(ast_a->type_specifier_);
            }

            IMPLEMENT_VISIT_PROC(TernaryExpr)
            {
                (void)args_a;
                Visit(ast_a->cond_expr_);
                Visit(ast_a->then_expr_);
                Visit(ast_a->else_expr_);
            }

            IMPLEMENT_VISIT_PROC(BinaryExpr)
            {
                (void)args_a;
                Visit(ast_a->lhs_expr_);
                Visit(ast_a->rhs_expr_);
            }

            IMPLEMENT_VISIT_PROC(UnaryExpr)
            {
                (void)args_a;
                Visit(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(PostUnaryExpr)
            {
                (void)args_a;
                Visit(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(CallExpr)
            {
                (void)args_a;
                Visit(ast_a->prefix_expr_);
                Visit(ast_a->arguments_);
            }

            IMPLEMENT_VISIT_PROC(BracketExpr)
            {
                (void)args_a;
                Visit(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(AssignExpr)
            {
                (void)args_a;
                Visit(ast_a->lvalue_expr_);
                Visit(ast_a->rvalue_expr_);
            }

            IMPLEMENT_VISIT_PROC(ObjectExpr)
            {
                (void)args_a;
                Visit(ast_a->prefix_expr_);
            }

            IMPLEMENT_VISIT_PROC(ArrayExpr)
            {
                (void)args_a;
                Visit(ast_a->prefix_expr_);
                Visit(ast_a->array_indices_);
            }

            IMPLEMENT_VISIT_PROC(CastExpr)
            {
                (void)args_a;
                Visit(ast_a->type_specifier_);
                Visit(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(InitializerExpr)
            {
                (void)args_a;
                Visit(ast_a->exprs_);
            }

#undef IMPLEMENT_VISIT_PROC
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel