#include "StructParameterAnalyzer.hpp"

#include "../AST.hpp"

#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void StructParameterAnalyzer::MarkStructsFromEntryPoint(
                    Program& program_a,
                    const ShaderTarget shader_target_a)
            {
                program_ = (&program_a);
                shader_target_ = shader_target_a;

                Visit(program_a.entry_point_ref_);
                Visit(program_a.layout_tess_control_.patch_const_function_ref_);
            }

            bool StructParameterAnalyzer::NotVisited(const AST* ast_a)
            {
                if (visit_set_.find(ast_a) == visit_set_.end())
                {
                    visit_set_.insert(ast_a);
                    return true;
                }
                return false;
            }

            void StructParameterAnalyzer::VisitStmntList(
                    const std::vector<StmntPtr>& stmnts_a)
            {
                for (auto& stmnt_ : stmnts_a)
                {
                    if (!stmnt_->flags_(AST::IsDeadCode))
                        Visit(stmnt_);
                }
            }

            bool StructParameterAnalyzer::IsVariableAnEntryPointParameter(
                    VarDeclStmnt* var_a) const
            {
                const auto& entry_point_params_ =
                        program_->entry_point_ref_->parameters_;

                auto entry_point_it_ =
                        std::find_if(entry_point_params_.begin(),
                                     entry_point_params_.end(),
                                     [var_a](const VarDeclStmntPtr& param_) {
                                         return (param_.get() == var_a);
                                     });

                return (entry_point_it_ != entry_point_params_.end());
            }

            bool StructParameterAnalyzer::IsActiveFunctionDeclEntryPoint() const
            {
                if (auto func_decl_ = ActiveFunctionDecl())
                    return func_decl_->flags_(FunctionDecl::IsEntryPoint);
                else
                    return false;
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void StructParameterAnalyzer::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

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
                Visit(ast_a->type_denoter_->SymbolRef());
                VISIT_DEFAULT(TypeSpecifier);
            }

            IMPLEMENT_VISIT_PROC(VarDecl)
            {
                if (NotVisited(ast_a))
                {
                    auto IsVarEntryPointIO = [](const VarDecl* var_decl_) {
                        return var_decl_->flags_(VarDecl::IsEntryPointOutput);
                    };

                    auto InsideShaderIOStruct = [this]() -> bool {
                        if (auto struct_decl_ = ActiveStructDecl())
                            return struct_decl_->flags_(
                                    StructDecl::IsShaderInput
                                    | StructDecl::IsShaderOutput);
                        else
                            return false;
                    };

                    if (!InsideShaderIOStruct())
                    {
                        if (!IsActiveFunctionDeclEntryPoint()
                            || !IsVarEntryPointIO(ast_a)
                            || shader_target_ == ShaderTarget::GeometryShader)
                        {
                            auto decl_stmnt_ = ast_a->decl_stmnt_ref_;

                            if (auto struct_decl_ = decl_stmnt_->type_specifier_
                                                          ->GetStructDeclRef())
                            {
                                if (!IsActiveFunctionDeclEntryPoint()
                                    || !IsVarEntryPointIO(ast_a)
                                    || struct_decl_
                                               ->HasMultipleShaderOutputInstances())
                                {
                                    if (!IsVariableAnEntryPointParameter(
                                                decl_stmnt_))
                                    {
                                        struct_decl_->AddFlagsRecursiveParents(
                                                StructDecl::
                                                        isNonEntryPointParam);
                                    }
                                }
                            }
                        }
                    }

                    Visit(ast_a->decl_stmnt_ref_);
                    Visit(ast_a->buffer_decl_ref_);

                    VISIT_DEFAULT(VarDecl);
                }
            }

            IMPLEMENT_VISIT_PROC(StructDecl)
            {
                if (NotVisited(ast_a))
                {
                    if (ast_a->NumMemberFunctions() > 0)
                        ast_a->AddFlagsRecursiveParents(
                                StructDecl::isNonEntryPointParam);

                    PushStructDecl(ast_a);
                    {
                        VISIT_DEFAULT(StructDecl);
                    }
                    PopStructDecl();
                }
            }

            IMPLEMENT_VISIT_PROC(BufferDecl)
            {
                (void)args_a;
                if (NotVisited(ast_a))
                    Visit(ast_a->decl_stmnt_ref_);
            }

            IMPLEMENT_VISIT_PROC(FunctionDecl)
            {
                if (NotVisited(ast_a))
                {
                    if (!ast_a->flags_(FunctionDecl::IsEntryPoint)
                        || shader_target_ == ShaderTarget::GeometryShader)
                    {
                        if (auto struct_decl_ =
                                    ast_a->return_type_->GetStructDeclRef())
                        {
                            if (!ast_a->flags_(FunctionDecl::IsEntryPoint)
                                || struct_decl_
                                           ->HasMultipleShaderOutputInstances())
                            {
                                struct_decl_->AddFlagsRecursiveParents(
                                        StructDecl::isNonEntryPointParam);
                            }
                        }
                    }

                    PushFunctionDecl(ast_a);
                    {
                        VISIT_DEFAULT(FunctionDecl);
                    }
                    PopFunctionDecl();
                }
            }

            IMPLEMENT_VISIT_PROC(UniformBufferDecl)
            {
                if (NotVisited(ast_a))
                    VISIT_DEFAULT(UniformBufferDecl);
            }

            IMPLEMENT_VISIT_PROC(BufferDeclStmnt)
            {
                if (NotVisited(ast_a))
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

            IMPLEMENT_VISIT_PROC(CallExpr)
            {
                Visit(ast_a->GetFunctionDecl());
                VISIT_DEFAULT(CallExpr);
            }

            IMPLEMENT_VISIT_PROC(ObjectExpr)
            {
                if (NotVisited(ast_a))
                {
                    Visit(ast_a->symbol_ref_);
                    VISIT_DEFAULT(ObjectExpr);
                }
            }

#undef AzPLEMENT_VISIT_PROC
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel