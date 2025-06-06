#include "ReferenceAnalyzer.hpp"

#include "../../Exception.hpp"
#include "../../Report/ReportHandler.hpp"
#include "../../Report/ReportIdents.hpp"
#include "../AST.hpp"

#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void ReferenceAnalyzer::MarkReferencesFromEntryPoint(
                    Program& program_a,
                    const ShaderTarget shader_target_a)
            {
                program_ = (&program_a);
                shader_target_ = shader_target_a;

                Visit(program_a.entry_point_ref_);
                Visit(program_a.layout_tess_control_.patch_const_function_ref_);
            }

            bool ReferenceAnalyzer::Reachable(AST* ast_a)
            {
                return (ast_a ? ast_a->flags_.SetOnce(AST::IsReachable) : false);
            }

            void ReferenceAnalyzer::VisitStmntList(
                    const std::vector<StmntPtr>& stmnts_a)
            {
                for (auto& stmnt_ : stmnts_a)
                {
                    if (!stmnt_->flags_(AST::IsDeadCode))
                        Visit(stmnt_);
                }
            }

            void ReferenceAnalyzer::MarkLValueExpr(const Expr* expr_a)
            {
                if (expr_a)
                {
                    if (auto object_expr_ = expr_a->As<ObjectExpr>())
                        MarkLValueExprObject(object_expr_);
                    else if (auto bracket_expr_ = expr_a->As<BracketExpr>())
                        MarkLValueExpr(bracket_expr_->expr_.get());
                    else if (auto array_expr_ = expr_a->As<ArrayExpr>())
                        MarkLValueExpr(array_expr_->prefix_expr_.get());
                }
            }

            void ReferenceAnalyzer::MarkLValueExprObject(
                    const ObjectExpr* object_expr_a)
            {
                if (object_expr_a)
                {
                    MarkLValueExpr(object_expr_a->prefix_expr_.get());

                    if (auto symbol_ = object_expr_a->symbol_ref_)
                    {
                        symbol_->flags_ << Decl::IsWrittenTo;
                    }
                }
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void ReferenceAnalyzer::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(CodeBlock)
            {
                (void)args_a;
                VisitStmntList(ast_a->stmnts_);
            }

            IMPLEMENT_VISIT_PROC(SwitchCase)
            {
                (void)args_a;
                Visit(ast_a->expr_);
                VisitStmntList(ast_a->stmnts_);
            }

            IMPLEMENT_VISIT_PROC(TypeSpecifier)
            {
                if (Reachable(ast_a))
                {
                    Visit(ast_a->type_denoter_->SymbolRef());
                    VISIT_DEFAULT(TypeSpecifier);
                }
            }

            IMPLEMENT_VISIT_PROC(VarDecl)
            {
                (void)args_a;
                if (Reachable(ast_a))
                {
                    Visit(ast_a->decl_stmnt_ref_);
                    Visit(ast_a->buffer_decl_ref_);
                    Visit(ast_a->static_member_var_ref_);
                    VISIT_DEFAULT(VarDecl);
                }
            }

            IMPLEMENT_VISIT_PROC(StructDecl)
            {
                (void)args_a;
                if (Reachable(ast_a))
                {
                    Visit(ast_a->var_members_);
                    Reachable(ast_a->decl_stmnt_ref_);
                }
            }

            IMPLEMENT_VISIT_PROC(BufferDecl)
            {
                (void)args_a;
                if (Reachable(ast_a))
                    Visit(ast_a->decl_stmnt_ref_);
            }

            IMPLEMENT_VISIT_PROC(SamplerDecl)
            {
                (void)args_a;
                if (Reachable(ast_a))
                    Visit(ast_a->decl_stmnt_ref_);
            }

            IMPLEMENT_VISIT_PROC(FunctionDecl)
            {
                (void)args_a;
                if (Reachable(ast_a))
                {
                    if (ast_a->IsForwardDecl())
                    {
                        if (ast_a->func_impl_ref_)
                            Visit(ast_a->func_impl_ref_);
                        else
                            RuntimeErr(R_MissingFuncImpl(
                                               ast_a->ToString(false)),
                                       ast_a);
                    } 
                    else
                    {
                        for (auto func_forward_decl_ :
                             ast_a->func_forward_decl_refs_)
                            Visit(func_forward_decl_);
                    }

                    PushFunctionDecl(ast_a);
                    {
                        VISIT_DEFAULT(FunctionDecl);
                    }
                    PopFunctionDecl();

                    Reachable(ast_a->decl_stmnt_ref_);
                }
            }

            IMPLEMENT_VISIT_PROC(UniformBufferDecl)
            {
                if (Reachable(ast_a))
                {
                    VISIT_DEFAULT(UniformBufferDecl);
                    Reachable(ast_a->decl_stmnt_ref_);
                }
            }

            IMPLEMENT_VISIT_PROC(VarDeclStmnt)
            {
                if (Reachable(ast_a))
                    VISIT_DEFAULT(VarDeclStmnt);
            }

            IMPLEMENT_VISIT_PROC(BufferDeclStmnt)
            {
                if (Reachable(ast_a))
                {
                    if (auto generic_type_denoter_ =
                                ast_a->type_denoter_->generic_type_denoter_
                                        .get())
                    {
                        if (auto struct_type_den_ =
                                    generic_type_denoter_->As<StructTypeDenoter>())
                        {
                            Visit(struct_type_den_->struct_decl_ref_);
                        }
                    }

                    VISIT_DEFAULT(BufferDeclStmnt);
                }
            }

            IMPLEMENT_VISIT_PROC(SamplerDeclStmnt)
            {
                (void)args_a;
                Reachable(ast_a);
            }

            IMPLEMENT_VISIT_PROC(UnaryExpr)
            {
                if (IsLValueOp(ast_a->op_))
                    MarkLValueExpr(ast_a->expr_.get());
                VISIT_DEFAULT(UnaryExpr);
            }

            IMPLEMENT_VISIT_PROC(PostUnaryExpr)
            {
                if (IsLValueOp(ast_a->op_))
                    MarkLValueExpr(ast_a->expr_.get());
                VISIT_DEFAULT(PostUnaryExpr);
            }

            IMPLEMENT_VISIT_PROC(CallExpr)
            {
                if (auto func_decl_ = ast_a->GetFunctionImpl())
                {
                    auto func_call_it_ =
                            std::find_if(call_expr_stack_.begin(),
                                         call_expr_stack_.end(),
                                         [func_decl_](CallExpr* call_expr_) {
                                             return (call_expr_->GetFunctionImpl()
                                                     == func_decl_);
                                         });

                    if (func_call_it_ != call_expr_stack_.end())
                    {
                        ReportHandler::HintForNextReport(R_CallStack + ":");
                        for (auto func_call_ : call_expr_stack_)
                            ReportHandler::HintForNextReport(
                                    "  '"
                                    + func_call_->GetFunctionDecl()->ToString(
                                            false)
                                    + "' (" + func_call_->area_.Pos().ToString()
                                    + ")");

                        RuntimeErr(R_IllegalRecursiveCall(func_decl_->ToString()),
                                   ast_a);
                    }

                    call_expr_stack_.push_back(ast_a);
                    {
                        Visit(func_decl_);
                    }
                    call_expr_stack_.pop_back();

                    if (func_decl_)
                    {
                        if (auto struct_decl_ = func_decl_->struct_decl_ref_)
                            Visit(struct_decl_);
                    }
                }

                if (ast_a->intrinsic_ != Intrinsic::Undefined)
                {
                    if ((ast_a->intrinsic_ >= Intrinsic::Image_AtomicAdd
                         && ast_a->intrinsic_
                                    <= Intrinsic::Image_AtomicExchange)
                        || ast_a->intrinsic_ == Intrinsic::Image_Load)
                    {
                        if (!ast_a->arguments_.empty())
                        {
                            const auto& type_den_ = ast_a->arguments_[0]
                                                          ->GetTypeDenoter()
                                                          ->GetAliased();
                            if (auto buffer_type_den_ =
                                        type_den_.As<BufferTypeDenoter>())
                            {
                                if (IsRWImageBufferType(
                                            buffer_type_den_->buffer_type_))
                                {
                                    if (auto buffer_decl_ =
                                                buffer_type_den_->buffer_decl_ref_)
                                        buffer_decl_->flags_ << BufferDecl::
                                                        IsUsedForImageRead;
                                }
                            }
                        }
                    }

                    if (!ast_a->flags_(CallExpr::CanInlineIntrinsicWrapper))
                        program_->RegisterIntrinsicUsage(ast_a->intrinsic_,
                                                         ast_a->arguments_);
                }

                ast_a->ForEachOutputArgument(
                        [this](ExprPtr& arg_expr_, VarDecl*) {
                            MarkLValueExpr(arg_expr_.get());
                        });

                VISIT_DEFAULT(CallExpr);
            }

            IMPLEMENT_VISIT_PROC(ObjectExpr)
            {
                if (auto var_decl_ = ast_a->FetchVarDecl())
                {
                    if (var_decl_->semantic_ == Semantic::FragCoord
                        && shader_target_ == ShaderTarget::FragmentShader)
                    {
                        program_->layout_fragment_.frag_coord_used_ = true;
                    }
                }

                if (ast_a->prefix_expr_)
                {
                    const auto& prefix_type_den_ =
                            ast_a->prefix_expr_->GetTypeDenoter()->GetAliased();
                    if (prefix_type_den_.IsMatrix())
                    {
                        auto prefix_base_type_den_ =
                                prefix_type_den_.As<BaseTypeDenoter>();
                        program_->used_matrix_subscripts_.insert(
                                {prefix_base_type_den_->data_type_,
                                 ast_a->ident_});
                    }
                }

                Visit(ast_a->symbol_ref_);

                VISIT_DEFAULT(ObjectExpr);
            }

            IMPLEMENT_VISIT_PROC(AssignExpr)
            {
                MarkLValueExpr(ast_a->lvalue_expr_.get());

                VISIT_DEFAULT(AssignExpr);
            }
#undef IMPLEMENT_VISIT_PROC
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel