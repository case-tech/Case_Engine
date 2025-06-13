#include "Analyzer.hpp"

#include "../AST/Visitor/ControlPathAnalyzer.hpp"
#include "../AST/Visitor/EndOfScopeAnalyzer.hpp"
#include "../AST/Visitor/ExprEvaluator.hpp"
#include "../Exception.hpp"
#include "../Report/ReportIdents.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            Analyzer::Analyzer(Log* log_a) : report_handler_ {log_a}
            {}

            bool Analyzer::DecorateAST(Program& program_a,
                                       const ShaderInput& input_desc_a,
                                       const ShaderOutput& output_desc_a)
            {
                source_code_ = program_a.source_code_.get();
                warnings_ = input_desc_a.warnings_;

                try
                {
                    DecorateASTPrAzary(program_a, input_desc_a, output_desc_a);
                } 
                catch (const ASTRuntimeError& e_)
                {
                    Error(e_.what(), e_.GetAST(), e_.GetASTAppendices());
                } 
                catch (const std::underflow_error& e_)
                {
                    ErrorInternal(e_.what());
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what());
                }

                return (!report_handler_.HasErrors());
            }

            void Analyzer::SubmitReport(
                    bool is_error_a,
                    const std::string& msg_a,
                    const AST* ast_a,
                    const std::vector<const AST*>& ast_appendices_a)
            {
                std::vector<SourceArea> secondary_areas_;
                for (auto next_ast_ : ast_appendices_a)
                {
                    if (next_ast_)
                        secondary_areas_.push_back(next_ast_->area_);
                }

                auto report_type_ = (is_error_a ? ReportTypes::Error
                                           : ReportTypes::Warning);
                report_handler_.SubmitReport(
                        false,
                        report_type_,
                        (is_error_a ? R_ContextError : R_Warning),
                        msg_a,
                        source_code_,
                        (ast_a ? ast_a->area_ : SourceArea::ignore),
                        secondary_areas_);
            }

            void Analyzer::Error(const std::string& msg_a,
                                 const AST* ast_a,
                                 const std::vector<const AST*>& ast_appendices_a)
            {
                SubmitReport(true, msg_a, ast_a, ast_appendices_a);
            }

            void Analyzer::ErrorUndeclaredIdent(const std::string& ident_a,
                                                const AST* ast_a)
            {
                ErrorUndeclaredIdent(ident_a, "", "", ast_a);
            }

            void Analyzer::ErrorUndeclaredIdent(const std::string& ident_a,
                                                const std::string& context_name_a,
                                                const AST* ast_a)
            {
                ErrorUndeclaredIdent(ident_a, context_name_a, "", ast_a);
            }

            void Analyzer::ErrorUndeclaredIdent(const std::string& ident_a,
                                                const std::string& context_name_a,
                                                const std::string& sAzilar_ident_a,
                                                const AST* ast_a)
            {
                Error(R_UndeclaredIdent(ident_a, context_name_a, sAzilar_ident_a), ast_a);
            }

            void Analyzer::ErrorInternal(const std::string& msg_a, const AST* ast_a)
            {
                report_handler_.SubmitReport(false,
                                            ReportTypes::Error,
                                            R_InternalError(),
                                            msg_a,
                                            source_code_,
                                            (ast_a ? ast_a->area_
                                                 : SourceArea::ignore));
            }

            void Analyzer::Warning(const std::string& msg_a, const AST* ast_a)
            {
                SubmitReport(false, msg_a, ast_a);
            }

            void Analyzer::WarningOnNullStmnt(const StmntPtr& ast_a,
                                              const std::string& stmnt_type_name_a)
            {
                if (WarnEnabled(Warnings::EmptyStatementBody) && ast_a
                    && ast_a->Type() == AST::Types::NullStmnt)
                    Warning(R_StatementWithEmptyBody(stmnt_type_name_a), ast_a.get());
            }

            bool Analyzer::WarnEnabled(unsigned int flags_a) const
            {
                return warnings_(flags_a);
            }

            void Analyzer::OpenScope()
            {
                sym_table_.OpenScope();
            }

            void Analyzer::CloseScope()
            {
                sym_table_.CloseScope(std::bind(&Analyzer::OnReleaseSymbol,
                                               this,
                                               std::placeholders::_1));
            }

            void Analyzer::Register(const std::string& ident_a, AST* ast_a)
            {
                try
                {
                    sym_table_.Register(
                            ident_a,
                            std::make_shared<ASTSymbolOverload>(ident_a, ast_a),
                            [&](ASTSymbolOverloadPtr& prev_symbol_) -> bool {
                                return prev_symbol_->AddSymbolRef(ast_a);
                            });
                } 
                catch (const std::exception& err_)
                {
                    Error(err_.what(), ast_a);
                }
            }

            AST* Analyzer::Fetch(const std::string& ident_a, const AST* ast_a)
            {
                try
                {
                    auto struct_decl_ = ActiveFunctionStructDecl();
                    if (struct_decl_)
                    {
                        if (auto symbol_ = struct_decl_->FetchVarDecl(ident_a))
                            return symbol_;
                    }

                    if (auto symbol_ = sym_table_.Fetch(ident_a))
                        return symbol_->Fetch();

                    ErrorUndeclaredIdent(ident_a,
                                         "",
                                         FetchSAzilarIdent(ident_a, struct_decl_),
                                         ast_a);
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what(), ast_a);
                }
                return nullptr;
            }

            AST* Analyzer::FetchFromCurrentScopeOrNull(
                    const std::string& ident_a) const
            {
                if (auto symbol_ = sym_table_.FetchFromCurrentScope(ident_a))
                    return symbol_->Fetch(false);
                else
                    return nullptr;
            }

            Decl* Analyzer::FetchDecl(const std::string& ident_a, const AST* ast_a)
            {
                if (auto symbol_ = Fetch(ident_a, ast_a))
                {
                    if (IsDeclAST(symbol_->Type()))
                        return static_cast<Decl*>(symbol_);
                    else
                        Error(R_IdentIsNotDecl(ident_a), ast_a);
                }
                return nullptr;
            }

            Decl* Analyzer::FetchType(const std::string& ident_a, const AST* ast_a)
            {
                try
                {
                    if (auto symbol_ = sym_table_.Fetch(ident_a))
                        return symbol_->FetchType();
                    else
                        ErrorUndeclaredIdent(ident_a,
                                             "",
                                             FetchSAzilarIdent(ident_a),
                                             ast_a);
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what(), ast_a);
                }
                return nullptr;
            }

            VarDecl* Analyzer::FetchVarDecl(const std::string& ident_a,
                                            const AST* ast_a)
            {
                try
                {
                    if (auto symbol_ = sym_table_.Fetch(ident_a))
                        return symbol_->FetchVarDecl();
                    else
                        ErrorUndeclaredIdent(ident_a,
                                             "",
                                             FetchSAzilarIdent(ident_a),
                                             ast_a);
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what(), ast_a);
                }
                return nullptr;
            }

            FunctionDecl* Analyzer::FetchFunctionDecl(
                    const std::string& ident_a,
                    const std::vector<ExprPtr>& args_a,
                    const AST* ast_a)
            {
                try
                {
                    std::vector<TypeDenoterPtr> arg_type_dens_;
                    if (!CollectArgumentTypeDenoters(args_a, arg_type_dens_))
                        return nullptr;

                    if (auto struct_decl_ = ActiveFunctionStructDecl())
                    {
                        if (auto func_decl_ =
                                    struct_decl_->FetchFunctionDecl(ident_a,
                                                                  arg_type_dens_))
                            return func_decl_;
                    }

                    if (auto symbol_ = sym_table_.Fetch(ident_a))
                        return symbol_->FetchFunctionDecl(arg_type_dens_);

                    if (Fetch(ident_a, ast_a) != nullptr)
                        Error(R_IdentIsNotFunc(ident_a), ast_a);
                } 
                catch (const ASTRuntimeError& e_)
                {
                    Error(e_.what(), e_.GetAST(), e_.GetASTAppendices());
                    return nullptr;
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what(), ast_a);
                }
                return nullptr;
            }

            FunctionDecl* Analyzer::FetchFunctionDecl(const std::string& ident_a,
                                                      const AST* ast_a)
            {
                try
                {
                    if (auto symbol_ = sym_table_.Fetch(ident_a))
                        return symbol_->FetchFunctionDecl();
                    else
                        ErrorUndeclaredIdent(ident_a,
                                             "",
                                             FetchSAzilarIdent(ident_a),
                                             ast_a);
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what(), ast_a);
                }
                return nullptr;
            }

            VarDecl* Analyzer::FetchVarDeclFromStruct(
                    const StructTypeDenoter& struct_type_denoter_a,
                    const std::string& ident_a,
                    const AST* ast_a)
            {
                if (auto struct_decl_ = struct_type_denoter_a.struct_decl_ref_)
                {
                    if (auto symbol_ = struct_decl_->FetchVarDecl(ident_a))
                        return symbol_;
                    else
                        ErrorUndeclaredIdent(ident_a,
                                             struct_decl_->ToString(),
                                             struct_decl_->FetchSAzilar(ident_a),
                                             ast_a);
                } else
                    Error(R_MissingReferenceToStructInType(
                                  struct_type_denoter_a.ToString()),
                          ast_a);
                return nullptr;
            }

            FunctionDecl* Analyzer::FetchFunctionDeclFromStruct(
                    const StructTypeDenoter& struct_type_denoter_a,
                    const std::string& ident_a,
                    const std::vector<ExprPtr>& args_a,
                    const AST* ast_a)
            {
                try
                {
                    if (auto struct_decl_ = struct_type_denoter_a.struct_decl_ref_)
                    {
                        std::vector<TypeDenoterPtr> arg_type_dens_;
                        if (CollectArgumentTypeDenoters(args_a, arg_type_dens_))
                        {
                            if (auto symbol_ = struct_decl_->FetchFunctionDecl(
                                        ident_a,
                                        arg_type_dens_,
                                        nullptr,
                                        true))
                                return symbol_;
                            else
                            {
                                if (struct_decl_->FetchVarDecl(ident_a) != nullptr)
                                    Error(R_IdentIsNotFunc(ident_a), ast_a);
                                else
                                {
                                    ErrorUndeclaredIdent(
                                            ident_a,
                                            struct_decl_->ToString(),
                                            struct_decl_->FetchSAzilar(ident_a),
                                            ast_a);
                                }
                            }
                        }
                    } 
                    else
                        Error(R_MissingReferenceToStructInType(
                                      struct_type_denoter_a.ToString()),
                              ast_a);
                } 
                catch (const ASTRuntimeError& e_)
                {
                    Error(e_.what(), e_.GetAST(), e_.GetASTAppendices());
                    return nullptr;
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what(), ast_a);
                }
                return nullptr;
            }

            StructDecl* Analyzer::FetchStructDeclFromIdent(
                    const std::string& ident_a,
                    const AST* ast_a)
            {
                if (auto symbol_ = FetchType(ident_a, ast_a))
                {
                    if (symbol_->Type() == AST::Types::StructDecl)
                        return static_cast<StructDecl*>(symbol_);
                    else if (symbol_->Type() == AST::Types::AliasDecl)
                        return FetchStructDeclFromTypeDenoter(*(
                                static_cast<AliasDecl*>(symbol_)->type_denoter_));
                }
                return nullptr;
            }

            StructDecl* Analyzer::FetchStructDeclFromTypeDenoter(
                    const TypeDenoter& type_denoter_a)
            {
                if (auto struct_type_den_ = type_denoter_a.As<StructTypeDenoter>())
                    return struct_type_den_->struct_decl_ref_;
                else if (auto alias_type_den_ = type_denoter_a.As<AliasTypeDenoter>())
                {
                    if (auto alias_decl_ = alias_type_den_->alias_decl_ref_)
                        return FetchStructDeclFromTypeDenoter(
                                *(alias_decl_->type_denoter_));
                }
                return nullptr;
            }

            StructDecl* Analyzer::FindCompatibleStructDecl(
                    const StructDecl& rhs_a)
            {
                auto symbol_ = sym_table_.Find(
                        [&rhs_a](const ASTSymbolOverloadPtr& symbol_) {
                            if (auto ref_ = symbol_->Fetch(false))
                            {
                                if (auto struct_decl_ = ref_->As<StructDecl>())
                                {
                                    if (struct_decl_->EqualsMemberTypes(rhs_a))
                                        return true;
                                }
                            }
                            return false;
                        });

                if (symbol_)
                {
                    if (auto ref_ = symbol_->Fetch(false))
                        return ref_->As<StructDecl>();
                }

                return nullptr;
            }

            bool Analyzer::InsideGlobalScope() const
            {
                return sym_table_.InsideGlobalScope();
            }

            void Analyzer::AnalyzeTypeDenoter(TypeDenoterPtr& type_denoter_a,
                                              const AST* ast_a)
            {
                if (type_denoter_a)
                {
                    if (auto buffer_type_den_ =
                                type_denoter_a->As<BufferTypeDenoter>())
                        AnalyzeBufferTypeDenoter(*buffer_type_den_, ast_a);
                    else if (auto struct_type_den_ =
                                     type_denoter_a->As<StructTypeDenoter>())
                        AnalyzeStructTypeDenoter(*struct_type_den_, ast_a);
                    else if (type_denoter_a->IsAlias())
                        AnalyzeAliasTypeDenoter(type_denoter_a, ast_a);
                    else if (auto array_type_den_ =
                                     type_denoter_a->As<ArrayTypeDenoter>())
                    {
                        Visit(array_type_den_->array_dAzs_);
                        AnalyzeTypeDenoter(array_type_den_->sub_type_denoter_, ast_a);
                    }
                }
            }

            void Analyzer::AnalyzeBufferTypeDenoter(
                    BufferTypeDenoter& buffer_type_den_a,
                    const AST* ast_a)
            {
                AnalyzeTypeDenoter(buffer_type_den_a.generic_type_denoter_, ast_a);
            }

            void Analyzer::AnalyzeStructTypeDenoter(
                    StructTypeDenoter& struct_type_den_a,
                    const AST* ast_a)
            {
                if (!struct_type_den_a.struct_decl_ref_)
                    struct_type_den_a.struct_decl_ref_ =
                            FetchStructDeclFromIdent(struct_type_den_a.ident_, ast_a);
            }

            void Analyzer::AnalyzeAliasTypeDenoter(TypeDenoterPtr& type_denoter_a,
                                                   const AST* ast_a)
            {
                if (auto alias_type_den_ = type_denoter_a->As<AliasTypeDenoter>())
                {
                    if (!alias_type_den_->alias_decl_ref_)
                    {
                        if (auto symbol_ = FetchType(alias_type_den_->ident_, ast_a))
                        {
                            if (auto struct_decl_ = symbol_->As<StructDecl>())
                            {
                                type_denoter_a =
                                        std::make_shared<StructTypeDenoter>(
                                                struct_decl_);
                            } else if (auto alias_decl_ = symbol_->As<AliasDecl>())
                            {
                                alias_type_den_->alias_decl_ref_ = alias_decl_;
                            }
                        }
                    }
                }
            }

            void Analyzer::AnalyzeTypeSpecifier(TypeSpecifier* type_specifier_a)
            {
                Visit(type_specifier_a->struct_decl_);

                if (type_specifier_a->type_denoter_)
                {
                    AnalyzeTypeDenoter(type_specifier_a->type_denoter_,
                                       type_specifier_a);

                    if (auto base_type_den_ = type_specifier_a->type_denoter_
                                                   ->GetAliased()
                                                   .As<BaseTypeDenoter>())
                    {
                        if (IsIntegralType(base_type_den_->data_type_))
                        {
                            for (auto modifier_ : type_specifier_a->interp_modifiers_)
                            {
                                if (modifier_ != InterpModifier::NoInterpolation)
                                    Error(R_OnlyNoInterpolationForInts,
                                          type_specifier_a);
                            }
                        }
                    }
                } 
                else
                    Error(R_MissingVariableType, type_specifier_a);
            }

            void Analyzer::AnalyzeTypeSpecifierForParameter(
                    TypeSpecifier* type_specifier_a)
            {
                if (type_specifier_a->is_output_ && type_specifier_a->is_uniform_)
                    Error(R_ParameterCantBeUniformAndOut, type_specifier_a);
            }

            void Analyzer::AnalyzeFunctionEndOfScopes(FunctionDecl& func_decl_a)
            {
                EndOfScopeAnalyzer scope_analyzer_;
                scope_analyzer_.MarkEndOfScopesFromFunction(func_decl_a);
            }

            void Analyzer::AnalyzeFunctionControlPath(FunctionDecl& func_decl_a)
            {
                ControlPathAnalyzer path_analyzer_;
                path_analyzer_.MarkControlPathsFromFunction(func_decl_a);
            }

            TypeDenoterPtr Analyzer::GetTypeDenoterFrom(
                    TypedAST* ast_a,
                    const TypeDenoter* expected_type_denoter_a)
            {
                if (ast_a)
                {
                    try
                    {
                        return ast_a->GetTypeDenoter(expected_type_denoter_a);
                    } 
                    catch (const ASTRuntimeError& e_)
                    {
                        Error(e_.what(), e_.GetAST(), e_.GetASTAppendices());
                    } 
                    catch (const std::exception& e_)
                    {
                        Error(e_.what(), ast_a);
                    }
                } 
                else
                    ErrorInternal(R_NullPointerArgument(__FUNCTION__));
                return nullptr;
            }

            void Analyzer::ValidateTypeCast(const TypeDenoter& source_type_den_a,
                                            const TypeDenoter& dest_type_den_a,
                                            const std::string& context_desc_a,
                                            const AST* ast_a)
            {
                if (source_type_den_a.IsCastableTo(dest_type_den_a))
                {
                    int source_vec_size_ = 0, dest_vec_size_ = 0;

                    const auto diff_ =
                            TypeDenoter::FindVectorTruncation(source_type_den_a,
                                                              dest_type_den_a,
                                                              source_vec_size_,
                                                              dest_vec_size_);

                    if (diff_ < 0)
                    {
                        if (WarnEnabled(Warnings::AzplicitTypeConversions))
                            Warning(R_AzplicitVectorTruncation(source_vec_size_,
                                                               dest_vec_size_,
                                                               context_desc_a),
                                    ast_a);
                    } 
                    else if (diff_ > 0)
                        Error(R_CantAzplicitlyConvertVectorType(source_vec_size_,
                                                                dest_vec_size_,
                                                                context_desc_a),
                              ast_a);
                } 
                else
                    Error(R_IllegalCast(source_type_den_a.ToString(),
                                        dest_type_den_a.ToString(),
                                        context_desc_a),
                          ast_a);
            }

            void Analyzer::ValidateTypeCastFrom(TypedAST* source_ast_a,
                                                TypedAST* dest_ast_a,
                                                const std::string& context_desc_a)
            {
                if (auto dest_type_den_ = GetTypeDenoterFrom(dest_ast_a))
                {
                    if (auto source_type_den_ =
                                GetTypeDenoterFrom(source_ast_a,
                                                   dest_type_den_.get()))
                    {
                        ValidateTypeCast(*source_type_den_,
                                         *dest_type_den_,
                                         context_desc_a,
                                         source_ast_a);
                    }
                }
            }

            void Analyzer::AnalyzeConditionalExpression(Expr* expr_a)
            {
                if (expr_a)
                {
                    Visit(expr_a);

                    const auto& cond_type_den_ =
                            expr_a->GetTypeDenoter()->GetAliased();
                    if (!cond_type_den_.IsScalar())
                        Error(R_ConditionalExprNotScalar(
                                      cond_type_den_.ToString()),
                              expr_a);
                }
            }

            Variant Analyzer::EvaluateConstExpr(Expr& expr_a)
            {
                try
                {
                    ExprEvaluator expr_evaluator_;
                    return expr_evaluator_.Evaluate(
                            expr_a,
                            [this](ObjectExpr* expr_) -> Variant {
                                return EvaluateConstExprObject(*expr_);
                            });
                } 
                catch (const ASTRuntimeError& e_)
                {
                    Error(e_.what(),
                          (e_.GetAST() ? e_.GetAST() : &expr_a),
                          e_.GetASTAppendices());
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what(), &expr_a);
                } 
                catch (const ObjectExpr* expr_)
                {
                    Error(R_ExpectedConstExpr, expr_);
                }
                return Variant();
            }

            Variant Analyzer::EvaluateConstExprObject(const ObjectExpr& expr_a)
            {
                if (auto var_decl_ = expr_a.FetchVarDecl())
                {
                    if (auto var_decl_stmnt_ = var_decl_->decl_stmnt_ref_)
                    {
                        if (!var_decl_stmnt_->flags_(VarDeclStmnt::IsParameter)
                            && var_decl_stmnt_->IsConstOrUniform()
                            && var_decl_->initializer_)
                        {
                            return EvaluateConstExpr(*var_decl_->initializer_);
                        }
                    }
                }

                throw(&expr_a);
            }

            Variant Analyzer::EvaluateOrDefault(Expr& expr_a,
                                                const Variant& default_value_a)
            {
                ExprEvaluator expr_evaluator_;
                return expr_evaluator_.EvaluateOrDefault(expr_a, default_value_a);
            }

            int Analyzer::EvaluateConstExprInt(Expr& expr_a)
            {
                auto variant_ = EvaluateConstExpr(expr_a);
                if (WarnEnabled(Warnings::AzplicitTypeConversions)
                    && variant_.Type() != Variant::Types::Int)
                    Warning(R_ExpectedConstIntExpr, &expr_a);
                return static_cast<int>(variant_.ToInt());
            }

            float Analyzer::EvaluateConstExprFloat(Expr& expr_a)
            {
                auto variant_ = EvaluateConstExpr(expr_a);
                if (WarnEnabled(Warnings::AzplicitTypeConversions)
                    && variant_.Type() != Variant::Types::Real)
                    Warning(R_ExpectedConstFloatExpr, &expr_a);
                return static_cast<float>(variant_.ToReal());
            }

            bool Analyzer::CollectArgumentTypeDenoters(
                    const std::vector<ExprPtr>& args_a,
                    std::vector<TypeDenoterPtr>& arg_type_dens_a)
            {
                for (const auto& arg_ : args_a)
                {
                    try
                    {
                        arg_type_dens_a.push_back(arg_->GetTypeDenoter());
                    } 
                    catch (const ASTRuntimeError& e_)
                    {
                        Error(e_.what(), e_.GetAST(), e_.GetASTAppendices());
                        return false;
                    } 
                    catch (const std::exception& e_)
                    {
                        Error(e_.what(), arg_.get());
                        return false;
                    }
                }
                return true;
            }

            std::string Analyzer::FetchSAzilarIdent(
                    const std::string& ident_a,
                    StructDecl* struct_decl_a) const
            {
                auto sAzilar_ident_ = sym_table_.FetchSAzilar(ident_a);
                if (!sAzilar_ident_.empty())
                    return sAzilar_ident_;

                if (struct_decl_a)
                {
                    sAzilar_ident_ = struct_decl_a->FetchSAzilar(ident_a);
                    if (!sAzilar_ident_.empty())
                        return sAzilar_ident_;
                }

                return "";
            }

            void Analyzer::OnReleaseSymbol(const ASTSymbolOverloadPtr& symbol_a)
            {
                if (WarnEnabled(Warnings::UnusedVariables)
                    && !InsideGlobalScope() && symbol_a)
                {
                    if (auto var_decl_ = symbol_a->FetchVarDecl(false))
                    {
                        if (!var_decl_->flags_(Decl::IsReadFrom)
                            && !var_decl_->IsStatic() && !var_decl_->IsParameter()
                            && var_decl_->struct_decl_ref_ == nullptr
                            && var_decl_->buffer_decl_ref_ == nullptr)
                        {
                            Warning(R_VarDeclaredButNeverUsed(var_decl_->ident_),
                                    var_decl_);
                        }
                    }
                }
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
