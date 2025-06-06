#include "HLSLAnalyzer.hpp"

#include "../../Exception.hpp"
#include "../../Helper.hpp"
#include "../../Report/ReportIdents.hpp"
#include "HLSLIntrinsics.hpp"
#include "HLSLKeywords.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            static ShaderVersion GetShaderModel(const InputShaderVersion v_a)
            {
                switch (v_a)
                {
                case InputShaderVersion::Cg:
                    return {3, 1};
                case InputShaderVersion::HLSL3:
                    return {3, 1};
                case InputShaderVersion::HLSL4:
                    return {4, 1};
                case InputShaderVersion::HLSL5:
                    return {5, 1};
                case InputShaderVersion::HLSL6:
                    return {6, 0};
                default:
                    return {0, 0};
                }
            }

            HLSLAnalyzer::HLSLAnalyzer(Log* log_a) : Analyzer {log_a}
            {}

            void HLSLAnalyzer::DecorateASTPrimary(
                    Program& program_a,
                    const ShaderInput& input_desc_a,
                    const ShaderOutput& output_desc_a)
            {
                entry_point_ = input_desc_a.entry_point_;
                secondary_entry_point_ =
                        (input_desc_a.shader_target_
                                         == ShaderTarget::
                                                 TessellationEvaluationShader
                                 ? input_desc_a.secondary_entry_point_
                                 : "");
                
                shader_target_ = input_desc_a.shader_target_;
                version_in_ = input_desc_a.shader_version_;
                shader_model_ = GetShaderModel(input_desc_a.shader_version_);
                prefer_wrappers_ = output_desc_a.options_.prefer_wrappers_;
                program_ = &program_a;

                Visit(&program_a);

                if (!secondary_entry_point_.empty() && !secondary_entry_point_found_
                    && WarnEnabled(Warnings::UnlocatedObjects))
                    Warning(R_SecondEntryPointNotFound(secondary_entry_point_));

                AnalyzeSemanticSM3Remaining();
            }

            void HLSLAnalyzer::ErrorIfAttributeNotFound(
                    bool found_a,
                    const std::string& attrib_desc_a)
            {
                if (!found_a)
                    Error(R_MissingAttributeForEntryPoint(attrib_desc_a), nullptr);
            }

            bool HLSLAnalyzer::IsD3D9ShaderModel() const
            {
                return (version_in_ <= InputShaderVersion::HLSL3);
            }

            #define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
                    void HLSLAnalyzer::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(Program)
            {
                (void)args_a;
                for (auto it_ = ast_a->global_stmnts_.begin();
                     it_ != ast_a->global_stmnts_.end();
                     ++it_)
                {
                    auto stmnt_ = it_->get();

                    Visit(stmnt_);

                    if (auto uniform_buffer_decl_ = stmnt_->As<UniformBufferDecl>())
                    {
                        for (auto it_sub_ =
                                     uniform_buffer_decl_->local_stmnts_.begin();
                             it_sub_ != uniform_buffer_decl_->local_stmnts_.end();)
                        {
                            auto sub_stmnt_ = it_sub_->get();
                            if (sub_stmnt_->Type() != AST::Types::VarDeclStmnt)
                            {
                                ++it_;
                                it_ = ast_a->global_stmnts_.insert(it_, *it_sub_);
                                it_sub_ = uniform_buffer_decl_->local_stmnts_.erase(
                                        it_sub_);
                            } 
                            else
                                ++it_sub_;
                        }
                    }
                }

                if (shader_target_ == ShaderTarget::FragmentShader
                    && IsD3D9ShaderModel())
                    program_->layout_fragment_.pixel_center_integer_ = true;
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

            IMPLEMENT_VISIT_PROC(ArrayDimension)
            {
                (void)args_a;
                if (ast_a->expr_ && ast_a->expr_->Type() != AST::Types::NullExpr)
                {
                    Visit(ast_a->expr_);
                    auto value_ = EvaluateConstExprInt(*ast_a->expr_);
                    ast_a->size_ = value_;
                }
            }

            IMPLEMENT_VISIT_PROC(TypeSpecifier)
            {
                (void)args_a;
                AnalyzeTypeSpecifier(ast_a);
            }

            IMPLEMENT_VISIT_PROC(VarDecl)
            {
                (void)args_a;
                AnalyzeVarDecl(ast_a);
            }

            IMPLEMENT_VISIT_PROC(BufferDecl)
            {
                (void)args_a;
                Register(ast_a->ident_, ast_a);
                Visit(ast_a->array_dims_);
            }

            IMPLEMENT_VISIT_PROC(SamplerDecl)
            {
                (void)args_a;
                Register(ast_a->ident_, ast_a);
                Visit(ast_a->array_dims_);
            }

            IMPLEMENT_VISIT_PROC(StructDecl)
            {
                (void)args_a;
                if (!ast_a->base_struct_name_.empty())
                    ast_a->base_struct_ref_ = FetchStructDeclFromIdent(
                            ast_a->base_struct_name_);

                if (InsideStructDecl())
                    ast_a->flags_ << StructDecl::isNestedStruct;

                if (WarnEnabled(Warnings::DeclarationShadowing)
                    && ast_a->base_struct_ref_)
                {
                    for (const auto& var_decl_stmnt_ : ast_a->var_members_)
                    {
                        for (const auto& var_decl_ : var_decl_stmnt_->var_decls_)
                        {
                            const StructDecl* var_decl_owner_ = nullptr;
                            if (ast_a->base_struct_ref_->FetchVarDecl(var_decl_->ident_,
                                                                 &var_decl_owner_))
                                
                                Warning(R_DeclShadowsMemberOfBase(
                                                var_decl_->ident_,
                                                var_decl_owner_->ToString()),
                                        var_decl_.get());
                        }
                    }
                }

                if (ast_a->IsAnonymous())
                    ast_a->compatible_struct_ref_ = FindCompatibleStructDecl(*ast_a);

                if (!ast_a->compatible_struct_ref_)
                    Register(ast_a->ident_, ast_a);

                PushStructDecl(ast_a);
                {
                    if (ast_a->flags_(StructDecl::isNestedStruct)
                        && !ast_a->IsAnonymous())
                        Error(R_NestedStructsMustBeAnonymous, ast_a);

                    OpenScope();
                    {
                        Visit(ast_a->local_stmnts_);
                    }
                    CloseScope();
                }
                PopStructDecl();

                for (const auto& member_ : ast_a->var_members_)
                {
                    auto type_specifier_ = member_->type_specifier_.get();
                    if (type_specifier_->IsConstOrUniform()
                        || type_specifier_->is_input_ || type_specifier_->is_output_)
                        Error(R_InvalidTypeModifierForMemberField,
                              type_specifier_);
                }

                if (WarnEnabled(Warnings::EmptyStatementBody)
                    && ast_a->NumMemberVariables() == 0)
                    Warning(R_TypeHasNoMemberVariables(ast_a->ToString()), ast_a);
            }

            IMPLEMENT_VISIT_PROC(AliasDecl)
            {
                (void)args_a;
                AnalyzeTypeDenoter(ast_a->type_denoter_, ast_a);
                Register(ast_a->ident_, ast_a);
            }

            IMPLEMENT_VISIT_PROC(FunctionDecl)
            {
                (void)args_a;
                GetReportHandler().PushContextDesc(ast_a->ToString());

                const auto IsEntryPoint = (ast_a->ident_ == entry_point_);
                const auto IsSecondaryEntryPoint = (ast_a->ident_
                                                    == secondary_entry_point_);

                if (IsSecondaryEntryPoint && !ast_a->IsForwardDecl())
                    secondary_entry_point_found_ = true;

                AnalyzeSemanticFunctionReturn(ast_a->semantic_);

                Visit(ast_a->decl_stmnt_ref_->attribs_);
                Visit(ast_a->return_type_);

                for (auto& param_ : ast_a->parameters_)
                {
                    AnalyzeArrayDimensionList(
                            param_->var_decls_.front()->array_dims_);
                    AnalyzeTypeDenoter(param_->type_specifier_->type_denoter_,
                                       param_->type_specifier_.get());
                }

                if (!ast_a->IsMemberFunction())
                {
                    Register(ast_a->ident_, ast_a);
                }

                OpenScope();
                {
                    for (auto& param_ : ast_a->parameters_)
                        AnalyzeParameter(param_.get());

                    if (IsEntryPoint)
                        AnalyzeEntryPoint(ast_a);
                    else if (IsSecondaryEntryPoint)
                        AnalyzeSecondaryEntryPoint(ast_a);

                    PushFunctionDecl(ast_a);
                    {
                        Visit(ast_a->code_block_);
                    }
                    PopFunctionDecl();

                    AnalyzeFunctionEndOfScopes(*ast_a);
                    AnalyzeFunctionControlPath(*ast_a);
                }
                CloseScope();

                GetReportHandler().PopContextDesc();
            }

            IMPLEMENT_VISIT_PROC(BufferDeclStmnt)
            {
                (void)args_a;
                AnalyzeTypeDenoter(ast_a->type_denoter_->generic_type_denoter_, ast_a);

                Visit(ast_a->buffer_decls_);
            }

            IMPLEMENT_VISIT_PROC(UniformBufferDecl)
            {
                (void)args_a;
                if (ast_a->slot_registers_.size() > 1)
                    Error(R_BufferCanOnlyHaveOneSlot,
                          ast_a->slot_registers_[1].get());

                for (const auto& slot_register_ : ast_a->slot_registers_)
                {
                    if (slot_register_->shader_target_ != ShaderTarget::Undefined)
                        Error(R_UserCBuffersCantBeTargetSpecific,
                              slot_register_.get());
                }

                PushUniformBufferDecl(ast_a);
                {
                    Visit(ast_a->local_stmnts_);
                }
                PopUniformBufferDecl();
            }

            IMPLEMENT_VISIT_PROC(VarDeclStmnt)
            {
                (void)args_a;
                if (InsideGlobalScope() && !InsideUniformBufferDecl())
                    ast_a->MakeImplicitConst();

                Visit(ast_a->type_specifier_);
                Visit(ast_a->var_decls_);

                if (ast_a->HasAnyTypeModifierOf(
                            {TypeModifier::SNorm, TypeModifier::UNorm}))
                {
                    auto base_type_den_ = ast_a->type_specifier_->type_denoter_
                                               ->As<BaseTypeDenoter>();
                    if (!base_type_den_ || !IsRealType(base_type_den_->data_type_))
                        Error(R_IllegalUseOfNormModifiers,
                              ast_a->type_specifier_.get());
                }

                if (auto parent_struct_decl_ = ActiveStructDecl())
                {
                    const auto& type_den_ =
                            ast_a->type_specifier_->GetTypeDenoter()->GetAliased();
                    if (auto struct_type_den_ = type_den_.As<StructTypeDenoter>())
                    {
                        if (auto struct_decl_ = struct_type_den_->struct_decl_ref_)
                        {
                            struct_decl_->parent_struct_decl_refs_.insert(
                                    parent_struct_decl_);
                        }
                    }
                }
            }

            IMPLEMENT_VISIT_PROC(BasicDeclStmnt)
            {
                (void)args_a;
                Visit(ast_a->decl_object_);
            }

            IMPLEMENT_VISIT_PROC(CodeBlockStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);
                OpenScope();
                {
                    Visit(ast_a->code_block_);
                }
                CloseScope();
            }

            IMPLEMENT_VISIT_PROC(ForLoopStmnt)
            {
                (void)args_a;
                WarningOnNullStmnt(ast_a->body_stmnt_, "for loop");

                Visit(ast_a->attribs_);

                if (WarnEnabled(Warnings::DeclarationShadowing))
                {
                    std::map<const AST*, std::string> ast_ident_pairs_;
                    ast_a->init_stmnt_->CollectDeclIdents(ast_ident_pairs_);

                    for (const auto& it_ : ast_ident_pairs_)
                    {
                        if (auto symbol_ = FetchFromCurrentScopeOrNull(
                                    it_.second))
                        {
                            if (symbol_->Type() == AST::Types::VarDecl
                                || symbol_->Type() == AST::Types::BufferDecl
                                || symbol_->Type() == AST::Types::SamplerDecl)
                            {
                                Warning(R_DeclShadowsPreviousLocal(
                                                it_.second,
                                                symbol_->area_.Pos().ToString()),
                                        it_.first);
                            }
                        }
                    }
                }

                OpenScope();
                {
                    Visit(ast_a->init_stmnt_);
                    AnalyzeConditionalExpression(ast_a->condition_.get());
                    Visit(ast_a->iteration_);

                    OpenScope();
                    {
                        Visit(ast_a->body_stmnt_);
                    }
                    CloseScope();
                }
                CloseScope();
            }

            IMPLEMENT_VISIT_PROC(WhileLoopStmnt)
            {
                (void)args_a;
                WarningOnNullStmnt(ast_a->body_stmnt_, "while loop");

                Visit(ast_a->attribs_);

                OpenScope();
                {
                    AnalyzeConditionalExpression(ast_a->condition_.get());
                    Visit(ast_a->body_stmnt_);
                }
                CloseScope();
            }

            IMPLEMENT_VISIT_PROC(DoWhileLoopStmnt)
            {
                (void)args_a;
                WarningOnNullStmnt(ast_a->body_stmnt_, "do-while loop");

                Visit(ast_a->attribs_);

                OpenScope();
                {
                    Visit(ast_a->body_stmnt_);
                    AnalyzeConditionalExpression(ast_a->condition_.get());
                }
                CloseScope();
            }

            IMPLEMENT_VISIT_PROC(IfStmnt)
            {
                (void)args_a;
                WarningOnNullStmnt(ast_a->body_stmnt_, "if");

                Visit(ast_a->attribs_);

                OpenScope();
                {
                    AnalyzeConditionalExpression(ast_a->condition_.get());
                    Visit(ast_a->body_stmnt_);
                }
                CloseScope();

                Visit(ast_a->else_stmnt_);
            }

            IMPLEMENT_VISIT_PROC(ElseStmnt)
            {
                (void)args_a;
                WarningOnNullStmnt(ast_a->body_stmnt_, "else");

                OpenScope();
                {
                    Visit(ast_a->body_stmnt_);
                }
                CloseScope();
            }

            IMPLEMENT_VISIT_PROC(SwitchStmnt)
            {
                (void)args_a;
                Visit(ast_a->attribs_);

                OpenScope();
                {
                    Visit(ast_a->selector_);
                    Visit(ast_a->cases_);
                }
                CloseScope();
            }

            IMPLEMENT_VISIT_PROC(ExprStmnt)
            {
                (void)args_a;
                Visit(ast_a->expr_);

                GetTypeDenoterFrom(ast_a->expr_.get());

                if (!prefer_wrappers_)
                {
                    if (auto call_expr_ = ast_a->expr_->As<CallExpr>())
                        AnalyzeIntrinsicWrapperInlining(call_expr_);
                }
            }

            IMPLEMENT_VISIT_PROC(ReturnStmnt)
            {
                (void)args_a;
                TypeDenoterPtr return_type_den_;

                if (auto func_decl_ = ActiveFunctionDecl())
                {
                    if ((return_type_den_ = GetTypeDenoterFrom(
                                 func_decl_->return_type_.get()))
                        != nullptr)
                    {
                        if (return_type_den_->IsVoid())
                        {
                            if (ast_a->expr_)
                                Error(R_IllegalExprInReturnForVoidFunc,
                                      ast_a->expr_.get());
                        } 
                        else
                        {
                            if (!ast_a->expr_)
                                Error(R_MissingExprInReturnForFunc(
                                              return_type_den_->ToString()),
                                      ast_a);
                        }
                    }
                } 
                else
                    Error(R_ReturnOutsideFuncDecl, ast_a);

                if (ast_a->expr_)
                {
                    Visit(ast_a->expr_);

                    if (auto expr_type_den_ = GetTypeDenoterFrom(
                                ast_a->expr_.get()))
                    {
                        if (return_type_den_)
                            ValidateTypeCast(*expr_type_den_,
                                             *return_type_den_,
                                             R_ReturnExpression,
                                             ast_a->expr_.get());
                    }

                    if (InsideEntryPoint())
                        AnalyzeEntryPointOutput(ast_a->expr_.get());
                }
            }

            IMPLEMENT_VISIT_PROC(UnaryExpr)
            {
                (void)args_a;
                if (IsLValueOp(ast_a->op_))
                {
                    PushLValueExpr(ast_a);
                    {
                        Visit(ast_a->expr_);
                    }
                    PopLValueExpr();

                    AnalyzeLValueExpr(ast_a->expr_.get(), ast_a);
                } 
                else
                    Visit(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(PostUnaryExpr)
            {
                (void)args_a;
                if (IsLValueOp(ast_a->op_))
                {
                    PushLValueExpr(ast_a);
                    {
                        Visit(ast_a->expr_);
                    }
                    PopLValueExpr();

                    AnalyzeLValueExpr(ast_a->expr_.get(), ast_a);
                } 
                else
                    Visit(ast_a->expr_);
            }

            IMPLEMENT_VISIT_PROC(CallExpr)
            {
                (void)args_a;
                AnalyzeCallExpr(ast_a);
            }

            IMPLEMENT_VISIT_PROC(AssignExpr)
            {
                (void)args_a;
                PushLValueExpr(ast_a);
                {
                    Visit(ast_a->lvalue_expr_);
                }
                PopLValueExpr();

                Visit(ast_a->rvalue_expr_);

                ValidateTypeCastFrom(ast_a->rvalue_expr_.get(),
                                     ast_a->lvalue_expr_.get(),
                                     R_VarAssignment);
                AnalyzeLValueExpr(ast_a->lvalue_expr_.get(), ast_a);
            }

            IMPLEMENT_VISIT_PROC(ObjectExpr)
            {
                AnalyzeObjectExpr(ast_a, reinterpret_cast<PrefixArgs*>(args_a));
            }

            IMPLEMENT_VISIT_PROC(ArrayExpr)
            {
                (void)args_a;
                AnalyzeArrayExpr(ast_a);
            }

#undef IMPLEMENT_VISIT_PROC

            void HLSLAnalyzer::AnalyzeVarDecl(VarDecl* var_decl_a)
            {
                if (var_decl_a->namespace_expr_)
                    AnalyzeVarDeclStaticMember(var_decl_a);
                else
                    AnalyzeVarDeclLocal(var_decl_a);
            }

            void HLSLAnalyzer::AnalyzeVarDeclLocal(VarDecl* var_decl_a,
                                                   bool register_var_ident_a)
            {
                if (register_var_ident_a)
                    Register(var_decl_a->ident_, var_decl_a);

                AnalyzeArrayDimensionList(var_decl_a->array_dims_);
                AnalyzeSemanticVarDecl(var_decl_a->semantic_, var_decl_a);

                if (var_decl_a->semantic_.IsSystemValue())
                {
                    for (auto struct_decl_ : GetStructDeclStack())
                        struct_decl_->system_values_ref_[var_decl_a->ident_] = var_decl_a;
                }

                if (var_decl_a->initializer_)
                {
                    Visit(var_decl_a->initializer_);

                    ValidateTypeCastFrom(var_decl_a->initializer_.get(),
                                         var_decl_a,
                                         R_VarInitialization);

                    if (var_decl_a->struct_decl_ref_)
                        Error(R_MemberVarsCantHaveDefaultValues(
                                      var_decl_a->ToString()),
                              var_decl_a->initializer_.get());

                    var_decl_a->initializer_value_ = EvaluateOrDefault(
                            *(var_decl_a->initializer_));

                    if (!var_decl_a->initializer_value_.IsValid()
                        && var_decl_a->IsParameter())
                        Error(R_ExpectedConstExpr(
                                      R_DefaultArgOfFuncParam(var_decl_a->ident_)),
                              var_decl_a->initializer_.get());
                } 
                else if (auto var_decl_stmnt_ = var_decl_a->decl_stmnt_ref_)
                {
                    if (register_var_ident_a && !InsideGlobalScope()
                        && var_decl_stmnt_->type_specifier_->IsConst()
                        && !var_decl_stmnt_->flags_(VarDeclStmnt::IsParameter))
                    {
                        Error(R_MissingInitializerForConstant(var_decl_a->ident_),
                              var_decl_a);
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeVarDeclStaticMember(VarDecl* var_decl_a)
            {
                if (InsideGlobalScope())
                    Visit(var_decl_a->namespace_expr_);
                else
                    Error(R_StaticMembersCantBeDefinedInGlob(
                                  var_decl_a->ToString()),
                          var_decl_a->namespace_expr_.get());

                AnalyzeVarDeclLocal(var_decl_a, false);
                if (auto type_den_ = GetTypeDenoterFrom(
                            var_decl_a->namespace_expr_.get()))
                {
                    if (auto struct_type_den_ =
                                type_den_->GetAliased().As<StructTypeDenoter>())
                    {
                        if (auto member_var_decl_ =
                                    FetchVarDeclFromStruct(*struct_type_den_,
                                                           var_decl_a->ident_,
                                                           var_decl_a))
                        {
                            if (auto prev_var_def_ =
                                        member_var_decl_->static_member_var_ref_)
                            {
                                ReportHandler::HintForNextReport(
                                        R_PrevDefinitionAt(
                                                prev_var_def_->area_.Pos()
                                                        .ToString()));
                               
                                Error(R_StaticMemberVarRedef(
                                              var_decl_a->ToString()),
                                      var_decl_a);
                            } 
                            else if (!member_var_decl_->IsStatic())
                            {
                                ReportHandler::HintForNextReport(R_DeclaredAt(
                                        member_var_decl_->area_.Pos().ToString()));
                            
                                Error(R_IllegalDefOfNonStaticMemberVar(
                                              var_decl_a->ToString()),
                                      var_decl_a);
                            } 
                            else
                            {
                                if (auto decl_var_type_ = GetTypeDenoterFrom(
                                            member_var_decl_->decl_stmnt_ref_
                                                    ->type_specifier_.get()))
                                {
                                    if (auto def_var_type_ = GetTypeDenoterFrom(
                                                var_decl_a->decl_stmnt_ref_
                                                        ->type_specifier_.get()))
                                    {
                                        if (!decl_var_type_->Equals(*def_var_type_))
                                        {
                                            ReportHandler::HintForNextReport(
                                                    R_DeclaredAt(
                                                            member_var_decl_->area_
                                                                    .Pos()
                                                                    .ToString()));
                                            
                                            Error(R_DeclTypeDiffersFromDefType(
                                                          decl_var_type_
                                                                  ->ToString(),
                                                          def_var_type_
                                                                  ->ToString()),
                                                  var_decl_a);
                                        } 
                                        else if (!member_var_decl_->array_dims_
                                                            .empty())
                                        {
                                            Error(R_ArrayTypeCanOnlyAppearInDef(
                                                          member_var_decl_
                                                                  ->ToString()),
                                                  member_var_decl_);
                                        } 
                                        else
                                        {
                                            member_var_decl_->static_member_var_ref_ =
                                                    var_decl_a;
                                            var_decl_a->static_member_var_ref_ =
                                                    member_var_decl_;

                                            member_var_decl_->array_dims_ =
                                                    var_decl_a->array_dims_;
                                            member_var_decl_->ResetTypeDenoter();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeCallExpr(CallExpr* expr_a)
            {
                try
                {
                    if (expr_a->prefix_expr_)
                    {
                        Visit(expr_a->prefix_expr_);

                        const auto& prefix_type_den_ = expr_a->prefix_expr_
                                                            ->GetTypeDenoter()
                                                            ->GetAliased();
                        AnalyzeCallExprPrimary(expr_a, &prefix_type_den_);
                    } 
                    else
                    {
                        AnalyzeCallExprPrimary(expr_a);
                    }
                } 
                catch (const ASTRuntimeError& e_)
                {
                    Error(e_.what(), e_.GetAST(), e_.GetASTAppendices());
                } 
                catch (const std::exception& e_)
                {
                    Error(e_.what(), expr_a);
                }
            }

            void HLSLAnalyzer::AnalyzeCallExprPrimary(
                    CallExpr* call_expr_a,
                    const TypeDenoter* prefix_type_denoter_a)
            {
                PushCallExpr(call_expr_a);
                {
                    Visit(call_expr_a->arguments_);

                    if (!call_expr_a->ident_.empty())
                    {
                        auto intr_it_ = HLSLIntrinsicAdept::GetIntrinsicMap()
                                              .find(call_expr_a->ident_);
                        if (intr_it_
                            != HLSLIntrinsicAdept::GetIntrinsicMap().end())
                        {
                            AnalyzeCallExprIntrinsic(call_expr_a,
                                                     intr_it_->second,
                                                     call_expr_a->is_static_,
                                                     prefix_type_denoter_a);
                        }
                        else
                        {
                            AnalyzeCallExprFunction(call_expr_a,
                                                    call_expr_a->is_static_,
                                                    call_expr_a->prefix_expr_.get(),
                                                    prefix_type_denoter_a);
                        }
                    }

                    call_expr_a->ForEachOutputArgument([this](ExprPtr& arg_expr_,
                                                           VarDecl* param_) 
                    {
                        AnalyzeLValueExpr(arg_expr_.get(), nullptr, param_);
                    });
                }
                PopCallExpr();
            }

            void HLSLAnalyzer::AnalyzeCallExprFunction(
                    CallExpr* call_expr_a,
                    bool is_static_a,
                    const Expr* prefix_expr_a,
                    const TypeDenoter* prefix_type_denoter_a)
            {
                if (prefix_type_denoter_a)
                {
                    if (auto struct_type_den_ =
                                prefix_type_denoter_a->As<StructTypeDenoter>())
                    {
                        call_expr_a->func_decl_ref_ =
                                FetchFunctionDeclFromStruct(*struct_type_den_,
                                                            call_expr_a->ident_,
                                                            call_expr_a->arguments_,
                                                            call_expr_a);
                    } 
                    else
                    {
                        Error(R_InvalidMemberFuncForType(
                                      call_expr_a->ident_,
                                      prefix_type_denoter_a->ToString()),
                              call_expr_a);
                    }
                } 
                else
                {
                    if (auto symbol_ = FetchFunctionDecl(call_expr_a->ident_,
                                                        call_expr_a->arguments_,
                                                        call_expr_a))
                    {
                        call_expr_a->func_decl_ref_ = symbol_;
                    }
                }

                if (auto func_decl_ = call_expr_a->GetFunctionDecl())
                {
                    if (prefix_expr_a
                        && AnalyzeStaticAccessExpr(prefix_expr_a,
                                                   is_static_a,
                                                   call_expr_a))
                    {
                        if (is_static_a)
                        {
                            if (!func_decl_->IsStatic())
                                Error(R_IllegalStaticFuncCall(
                                              func_decl_->ToString()),
                                      call_expr_a);
                        } 
                        else
                        {
                            if (func_decl_->IsStatic())
                                Error(R_IllegalNonStaticFuncCall(
                                              func_decl_->ToString()),
                                      call_expr_a);
                        }
                    }

                    for (std::size_t i_ = call_expr_a->arguments_.size(),
                                     n_ = func_decl_->parameters_.size();
                         i_ < n_;
                         ++i_)
                    {
                        auto param_ = func_decl_->parameters_[i_].get();
                        if (!param_->var_decls_.empty())
                        {
                            auto param_var_ = param_->var_decls_.front().get();
                            if (param_var_->initializer_)
                                call_expr_a->default_param_refs_.push_back(param_var_);
                            else
                                Error(R_MissingInitializerForDefaultParam(
                                              param_var_->ident_),
                                      param_var_);
                        }
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeCallExprIntrinsic(
                    CallExpr* call_expr_a,
                    const HLSLIntrinsicEntry& intr_a,
                    bool is_static_a,
                    const TypeDenoter* prefix_type_denoter_a)
            {
                const auto intrinsic_ = intr_a.intrinsic_;

                if (IsGlobalIntrinsic(intrinsic_) || prefix_type_denoter_a)
                {
                    AnalyzeCallExprIntrinsicPrimary(call_expr_a, intr_a);

                    if (is_static_a)
                        Error(R_IllegalStaticIntrinsicCall(call_expr_a->ident_),
                              call_expr_a);
                }

                if (IsGlobalIntrinsic(intrinsic_))
                {
                    if (prefix_type_denoter_a)
                    {
                        Error(R_InvalidGlobalIntrinsicForType(
                                      call_expr_a->ident_,
                                      prefix_type_denoter_a->ToString()),
                              call_expr_a);
                    }
                }
                else
                {
                    if (prefix_type_denoter_a)
                    {
                        if (auto buffer_type_den_ =
                                    prefix_type_denoter_a->As<BufferTypeDenoter>())
                        {
                            AnalyzeCallExprIntrinsicFromBufferType(
                                    call_expr_a,
                                    buffer_type_den_->buffer_type_);

                            if (IsTextureCompareIntrinsic(intrinsic_))
                            {
                                if (auto buffer_decl_ =
                                            buffer_type_den_->buffer_decl_ref_)
                                    buffer_decl_->flags_
                                            << BufferDecl::IsUsedForCompare;
                            }
                        } 
                        else
                        {
                            Error(R_InvalidClassIntrinsicForType(
                                          call_expr_a->ident_,
                                          prefix_type_denoter_a->ToString()),
                                  call_expr_a);
                        }
                    } 
                    else
                    {
                        if (WarnEnabled(Warnings::DeclarationShadowing))
                            Warning(R_FuncCallShadowsClassIntrinsic(
                                            call_expr_a->ident_),
                                    call_expr_a);

                        AnalyzeCallExprFunction(call_expr_a,
                                                call_expr_a->is_static_,
                                                call_expr_a->prefix_expr_.get(),
                                                prefix_type_denoter_a);
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeCallExprIntrinsicPrimary(
                    CallExpr* call_expr_a,
                    const HLSLIntrinsicEntry& intr_a)
            {
                if (shader_model_ < intr_a.min_shader_model_)
                {
                    Error(R_InvalidShaderModelForIntrinsic(
                                  call_expr_a->ident_,
                                  intr_a.min_shader_model_.ToString(),
                                  shader_model_.ToString()),
                          call_expr_a);
                }

                call_expr_a->intrinsic_ = intr_a.intrinsic_;

                using T = Intrinsic;

                struct IntrinsicConversion
                {
                    T standard_intrinsic_;
                    std::size_t num_args_;
                    T overloaded_intrinsic_;
                };

                static const std::vector<IntrinsicConversion> intrinsic_conversions_ 
                {
                                {T::AsUInt_1, 3, T::AsUInt_3},
                                {T::Tex1D_2, 4, T::Tex1D_4},
                                {T::Tex2D_2, 4, T::Tex2D_4},
                                {T::Tex3D_2, 4, T::Tex3D_4},
                                {T::TexCube_2, 4, T::TexCube_4},
                                {T::Texture_Load_1, 2, T::Texture_Load_2},
                                {T::Texture_Load_1, 3, T::Texture_Load_3},

                                {T::Texture_Gather_2, 3, T::Texture_Gather_3},
                                {T::Texture_Gather_2, 4, T::Texture_Gather_4},
                                {T::Texture_GatherRed_2,
                                 3,
                                 T::Texture_GatherRed_3},
                                {T::Texture_GatherRed_2,
                                 4,
                                 T::Texture_GatherRed_4},
                                {T::Texture_GatherRed_2,
                                 6,
                                 T::Texture_GatherRed_6},
                                {T::Texture_GatherRed_2,
                                 7,
                                 T::Texture_GatherRed_7},
                                {T::Texture_GatherGreen_2,
                                 3,
                                 T::Texture_GatherGreen_3},
                                {T::Texture_GatherGreen_2,
                                 4,
                                 T::Texture_GatherGreen_4},
                                {T::Texture_GatherGreen_2,
                                 6,
                                 T::Texture_GatherGreen_6},
                                {T::Texture_GatherGreen_2,
                                 7,
                                 T::Texture_GatherGreen_7},
                                {T::Texture_GatherBlue_2,
                                 3,
                                 T::Texture_GatherBlue_3},
                                {T::Texture_GatherBlue_2,
                                 4,
                                 T::Texture_GatherBlue_4},
                                {T::Texture_GatherBlue_2,
                                 6,
                                 T::Texture_GatherBlue_6},
                                {T::Texture_GatherBlue_2,
                                 7,
                                 T::Texture_GatherBlue_7},
                                {T::Texture_GatherAlpha_2,
                                 3,
                                 T::Texture_GatherAlpha_3},
                                {T::Texture_GatherAlpha_2,
                                 4,
                                 T::Texture_GatherAlpha_4},
                                {T::Texture_GatherAlpha_2,
                                 6,
                                 T::Texture_GatherAlpha_6},
                                {T::Texture_GatherAlpha_2,
                                 7,
                                 T::Texture_GatherAlpha_7},

                                {T::Texture_GatherCmp_3,
                                 4,
                                 T::Texture_GatherCmp_4},
                                {T::Texture_GatherCmp_3,
                                 5,
                                 T::Texture_GatherCmp_5},
                                {T::Texture_GatherCmpRed_3,
                                 4,
                                 T::Texture_GatherCmpRed_4},
                                {T::Texture_GatherCmpRed_3,
                                 5,
                                 T::Texture_GatherCmpRed_5},
                                {T::Texture_GatherCmpRed_3,
                                 7,
                                 T::Texture_GatherCmpRed_7},
                                {T::Texture_GatherCmpRed_3,
                                 8,
                                 T::Texture_GatherCmpRed_8},
                                {T::Texture_GatherCmpGreen_3,
                                 4,
                                 T::Texture_GatherCmpGreen_4},
                                {T::Texture_GatherCmpGreen_3,
                                 5,
                                 T::Texture_GatherCmpGreen_5},
                                {T::Texture_GatherCmpGreen_3,
                                 7,
                                 T::Texture_GatherCmpGreen_7},
                                {T::Texture_GatherCmpGreen_3,
                                 8,
                                 T::Texture_GatherCmpGreen_8},
                                {T::Texture_GatherCmpBlue_3,
                                 4,
                                 T::Texture_GatherCmpBlue_4},
                                {T::Texture_GatherCmpBlue_3,
                                 5,
                                 T::Texture_GatherCmpBlue_5},
                                {T::Texture_GatherCmpBlue_3,
                                 7,
                                 T::Texture_GatherCmpBlue_7},
                                {T::Texture_GatherCmpBlue_3,
                                 8,
                                 T::Texture_GatherCmpBlue_8},
                                {T::Texture_GatherCmpAlpha_3,
                                 4,
                                 T::Texture_GatherCmpAlpha_4},
                                {T::Texture_GatherCmpAlpha_3,
                                 5,
                                 T::Texture_GatherCmpAlpha_5},
                                {T::Texture_GatherCmpAlpha_3,
                                 7,
                                 T::Texture_GatherCmpAlpha_7},
                                {T::Texture_GatherCmpAlpha_3,
                                 8,
                                 T::Texture_GatherCmpAlpha_8},

                                {T::Texture_Sample_2, 3, T::Texture_Sample_3},
                                {T::Texture_Sample_2, 4, T::Texture_Sample_4},
                                {T::Texture_Sample_2, 5, T::Texture_Sample_5},
                                {T::Texture_SampleBias_3,
                                 4,
                                 T::Texture_SampleBias_4},
                                {T::Texture_SampleBias_3,
                                 5,
                                 T::Texture_SampleBias_5},
                                {T::Texture_SampleBias_3,
                                 6,
                                 T::Texture_SampleBias_6},
                                {T::Texture_SampleCmp_3,
                                 4,
                                 T::Texture_SampleCmp_4},
                                {T::Texture_SampleCmp_3,
                                 5,
                                 T::Texture_SampleCmp_5},
                                {T::Texture_SampleCmp_3,
                                 6,
                                 T::Texture_SampleCmp_6},
                                {T::Texture_SampleCmpLevelZero_3,
                                 4,
                                 T::Texture_SampleCmpLevelZero_4},
                                {T::Texture_SampleCmpLevelZero_3,
                                 5,
                                 T::Texture_SampleCmpLevelZero_5},
                                {T::Texture_SampleGrad_4,
                                 5,
                                 T::Texture_SampleGrad_5},
                                {T::Texture_SampleGrad_4,
                                 6,
                                 T::Texture_SampleGrad_6},
                                {T::Texture_SampleGrad_4,
                                 7,
                                 T::Texture_SampleGrad_7},
                                {T::Texture_SampleLevel_3,
                                 4,
                                 T::Texture_SampleLevel_4},
                                {T::Texture_SampleLevel_3,
                                 5,
                                 T::Texture_SampleLevel_5},
                        };

                for (const auto& conversion_ : intrinsic_conversions_)
                {
                    if (call_expr_a->intrinsic_ == conversion_.standard_intrinsic_
                        && call_expr_a->arguments_.size() == conversion_.num_args_)
                    {
                        call_expr_a->intrinsic_ = conversion_.overloaded_intrinsic_;
                        break;
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeCallExprIntrinsicFromBufferType(
                    const CallExpr* call_expr_a,
                    const BufferType buffer_type_a)
            {
                const auto intrinsic_ = call_expr_a->intrinsic_;
                const auto& ident_ = call_expr_a->ident_;

                if (IsImageBufferType(buffer_type_a))
                {
                    if (!IsTextureIntrinsic(intrinsic_))
                        Error(R_InvalidIntrinsicForTexture(ident_), call_expr_a);
                } 
                else if (IsRWImageBufferType(buffer_type_a))
                {
                    if (!IsImageIntrinsic(intrinsic_))
                        Error(R_InvalidIntrinsicForRWTexture(ident_), call_expr_a);
                } 
                else if (IsStreamBufferType(buffer_type_a))
                {
                    if (IsStreamOutputIntrinsic(intrinsic_))
                    {
                        if (InsideEntryPoint()
                            && intrinsic_ == Intrinsic::StreamOutput_Append)
                        {
                            for (const auto& arg_ : call_expr_a->arguments_)
                                AnalyzeEntryPointOutput(arg_.get());
                        }
                    } 
                    else
                        Error(R_InvalidIntrinsicForStreamOutput(ident_),
                              call_expr_a);
                }

                if (IsTextureGatherIntrisic(intrinsic_))
                {
                    switch (buffer_type_a)
                    {
                    case BufferType::Texture2D:
                    case BufferType::Texture2DArray:
                        break;
                    case BufferType::TextureCube:
                    case BufferType::TextureCubeArray:
                        if (GetGatherIntrinsicOffsetParamCount(intrinsic_) != 0)
                            Error(R_InvalidClassIntrinsicForType(
                                          ident_,
                                          BufferTypeToString(buffer_type_a)),
                                  call_expr_a);
                        break;
                    default:
                        Error(R_InvalidClassIntrinsicForType(
                                      ident_,
                                      BufferTypeToString(buffer_type_a)),
                              call_expr_a);
                        break;
                    }
                }

                switch (intrinsic_)
                {
                case Intrinsic::Texture_Load_1:
                    if (buffer_type_a == BufferType::Texture2DMS
                        || buffer_type_a == BufferType::Texture2DMSArray)
                        Error(R_InvalidClassIntrinsicForType(
                                      ident_,
                                      BufferTypeToString(buffer_type_a)),
                              call_expr_a);
                    break;

                case Intrinsic::Texture_Load_2:
                    if (buffer_type_a == BufferType::Buffer)
                        Error(R_InvalidClassIntrinsicForType(
                                      ident_,
                                      BufferTypeToString(buffer_type_a)),
                              call_expr_a);
                    break;

                case Intrinsic::Texture_Load_3:
                    if (buffer_type_a != BufferType::Texture2DMS
                        && buffer_type_a != BufferType::Texture2DMSArray)
                        Error(R_InvalidClassIntrinsicForType(
                                      ident_,
                                      BufferTypeToString(buffer_type_a)),
                              call_expr_a);
                    break;

                default:
                    break;
                }
            }

            void HLSLAnalyzer::AnalyzeIntrinsicWrapperInlining(
                    CallExpr* call_expr_a)
            {
                if (call_expr_a->intrinsic_ == Intrinsic::Clip)
                {
                    call_expr_a->flags_ << CallExpr::CanInlineIntrinsicWrapper;
                }
            }

            void HLSLAnalyzer::AnalyzeObjectExpr(ObjectExpr* expr_a,
                                                 PrefixArgs* args_a)
            {
                if (expr_a->prefix_expr_)
                {
                    PrefixArgs prefix_args_;
                    {
                        prefix_args_.in_is_postfix_static_ = expr_a->is_static_;
                        prefix_args_.out_prefix_base_struct_ = nullptr;
                    }
                    Visit(expr_a->prefix_expr_, &prefix_args_);

                    const auto& prefix_type_den_ =
                            expr_a->prefix_expr_->GetTypeDenoter()->GetAliased();
                    if (auto struct_type_den_ =
                                prefix_type_den_.As<StructTypeDenoter>())
                    {
                        if (args_a != nullptr && args_a->in_is_postfix_static_
                            && !expr_a->is_static_)
                        {
                            AnalyzeObjectExprBaseStructDeclFromStruct(
                                    expr_a,
                                    *args_a,
                                    *struct_type_den_);
                        } 
                        else
                        {
                            AnalyzeObjectExprVarDeclFromStruct(
                                    expr_a,
                                    prefix_args_.out_prefix_base_struct_,
                                    *struct_type_den_);
                        }
                    } 
                    else if (prefix_type_den_.IsBase())
                    {
                        GetTypeDenoterFrom(expr_a);
                    }
                } 
                else
                {
                    if (auto symbol_ = FetchDecl(expr_a->ident_, expr_a))
                    {
                        expr_a->symbol_ref_ = symbol_;
                        if (args_a)
                        {
                            if (auto struct_decl_ = symbol_->As<StructDecl>())
                                args_a->out_prefix_base_struct_ = struct_decl_;
                        }

                        if (ActiveLValueExpr() == nullptr)
                            symbol_->flags_ << Decl::IsReadFrom;
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeObjectExprVarDeclFromStruct(
                    ObjectExpr* expr_a,
                    StructDecl* base_struct_decl_a,
                    const StructTypeDenoter& struct_type_den_a)
            {
                if (base_struct_decl_a)
                {
                    expr_a->symbol_ref_ = FetchVarDeclFromStruct(base_struct_decl_a,
                                                             expr_a->ident_,
                                                             expr_a);
                    if (expr_a->symbol_ref_)
                    {
                        if (auto var_decl_ = expr_a->symbol_ref_->As<VarDecl>())
                        {
                            if (!var_decl_->IsStatic())
                            {
                                if (auto active_struct_decl_ = ActiveStructDecl())
                                {
                                    if (base_struct_decl_a->IsBaseOf(
                                                active_struct_decl_,
                                                true))
                                        return;
                                }

                                if (expr_a->prefix_expr_
                                    && expr_a->prefix_expr_->flags_(
                                            ObjectExpr::IsBaseStructNamespace))
                                    return;
                            }
                        }
                    }
                } 
                else
                {
                    expr_a->symbol_ref_ = FetchVarDeclFromStruct(struct_type_den_a,
                                                             expr_a->ident_,
                                                             expr_a);
                }

                if (expr_a->symbol_ref_ && expr_a->prefix_expr_)
                {
                    if (AnalyzeStaticAccessExpr(expr_a->prefix_expr_.get(),
                                                expr_a->is_static_,
                                                expr_a))
                    {
                        AnalyzeStaticTypeSpecifier(
                                expr_a->symbol_ref_->FetchTypeSpecifier(),
                                expr_a->ident_,
                                expr_a,
                                expr_a->is_static_);
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeObjectExprBaseStructDeclFromStruct(
                    ObjectExpr* expr_a,
                    PrefixArgs& output_args_a,
                    const StructTypeDenoter& struct_type_den_a)
            {
                if (auto struct_decl_ = struct_type_den_a.struct_decl_ref_)
                {
                    if (auto symbol_ = struct_decl_->FetchBaseStructDecl(
                                expr_a->ident_))
                    {
                        expr_a->symbol_ref_ = symbol_;
                        expr_a->flags_ << ObjectExpr::IsBaseStructNamespace;
                        output_args_a.out_prefix_base_struct_ = symbol_;
                    } 
                    else
                        Error(R_IdentIsNotBaseOf(expr_a->ident_,
                                                 struct_decl_->ToString()),
                              expr_a);
                }
            }

            bool HLSLAnalyzer::AnalyzeStaticAccessExpr(const Expr* prefix_expr_a,
                                                       bool is_static_a,
                                                       const AST* ast_a)
            {
                if (prefix_expr_a)
                {
                    auto IsObjectExprWithTypename =
                            [](const Expr& expr_) -> bool {
                        if (auto object_expr_ = expr_.As<ObjectExpr>())
                        {
                            if (auto symbol_ = object_expr_->symbol_ref_)
                            {
                                return (symbol_->Type() == AST::Types::StructDecl
                                        || symbol_->Type()
                                                   == AST::Types::AliasDecl);
                            }
                        }
                        return false;
                    };

                    if (auto static_type_expr_ = AST::GetAs<ObjectExpr>(
                                prefix_expr_a->Find(IsObjectExprWithTypename,
                                                 SearchLValue)))
                    {
                        if (!is_static_a)
                        {
                            Error(R_IllegalNonStaticAccessToType(
                                          static_type_expr_->ident_),
                                  ast_a);
                            return false;
                        }
                    } 
                    else
                    {
                        if (is_static_a)
                        {
                            Error(R_IllegalStaticAccessToNonType, ast_a);
                            return false;
                        }
                    }
                }
                return true;
            }

            bool HLSLAnalyzer::AnalyzeStaticTypeSpecifier(
                    const TypeSpecifier* type_specifier_a,
                    const std::string& ident_a,
                    const Expr* expr_a,
                    bool is_static_a)
            {
                if (type_specifier_a)
                {
                    if (type_specifier_a->HasAnyStorageClassOf(
                                {StorageClass::Static}))
                    {
                        if (!is_static_a)
                        {
                            Error(R_IllegalNonStaticAccessToMember(ident_a),
                                  expr_a);
                            return false;
                        }
                    } 
                    else
                    {
                        if (is_static_a)
                        {
                            Error(R_IllegalStaticAccessToMember(ident_a), expr_a);
                            return false;
                        }
                    }
                }
                return true;
            }

            void HLSLAnalyzer::AnalyzeLValueExpr(Expr* expr_a,
                                                 const AST* ast_a,
                                                 VarDecl* param_a)
            {
                if (expr_a)
                {
                    if (auto lvalue_expr_ = expr_a->FetchLValueExpr())
                        AnalyzeLValueExprObject(lvalue_expr_, ast_a, param_a);
                    else
                    {
                        ast_a = (ast_a != nullptr ? ast_a : expr_a);
                        if (param_a && param_a->decl_stmnt_ref_)
                            Error(R_IllegalRValueAssignment(
                                          param_a->decl_stmnt_ref_->ToString()),
                                  ast_a);
                        else
                            Error(R_IllegalRValueAssignment, ast_a);
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeLValueExprObject(ObjectExpr* object_expr_a,
                                                       const AST* ast_a,
                                                       VarDecl* param_a)
            {
                AnalyzeLValueExpr(object_expr_a->prefix_expr_.get(), ast_a);

                if (auto symbol_ = object_expr_a->symbol_ref_)
                {
                    if (auto var_decl_ = symbol_->As<VarDecl>())
                    {
                        if (param_a != nullptr)
                        {
                            const auto& lhs_type_den_ = var_decl_->GetTypeDenoter();
                            const auto& rhs_type_den_ = param_a->GetTypeDenoter();

                            if (!lhs_type_den_->Equals(*rhs_type_den_))
                            {
                                Error(R_IllegalLValueAssignmentToTypeCast(
                                              object_expr_a->ident_,
                                              param_a->decl_stmnt_ref_->ToString()),
                                      (ast_a != nullptr ? ast_a : object_expr_a));
                            }
                        }

                        if (var_decl_->decl_stmnt_ref_->IsConstOrUniform())
                        {
                            Error(R_IllegalLValueAssignmentToConst(
                                          object_expr_a->ident_,
                                          (var_decl_->decl_stmnt_ref_->flags_(
                                                   VarDeclStmnt::
                                                           isImplicitConst)
                                                   ? R_Implicitly
                                                   : "")),
                                  (ast_a != nullptr ? ast_a : object_expr_a));
                        }
                    }
                } 
                else
                {
                    Error(R_MissingObjectExprSymbolRef(object_expr_a->ident_),
                          (ast_a != nullptr ? ast_a : object_expr_a));
                }
            }

            void HLSLAnalyzer::AnalyzeArrayExpr(ArrayExpr* expr_a)
            {
                Visit(expr_a->prefix_expr_.get());
                Visit(expr_a->array_indices_);

                GetTypeDenoterFrom(expr_a);

                for (auto& array_index_ : expr_a->array_indices_)
                {
                    if (auto type_den_ = GetTypeDenoterFrom(array_index_.get()))
                    {
                        const auto& type_den_aliased_ = type_den_->GetAliased();
                        if (auto base_type_den_ =
                                    type_den_aliased_.As<BaseTypeDenoter>())
                        {
                            const auto vector_dim_ = VectorTypeDim(
                                    base_type_den_->data_type_);
                            
                            const auto int_vec_type_ =
                                    VectorDataType(DataType::Int, vector_dim_);
                            ValidateTypeCast(*type_den_,
                                             BaseTypeDenoter(int_vec_type_),
                                             R_ArrayIndex,
                                             array_index_.get());
                        } 
                        else
                            Error(R_ArrayIndexMustHaveBaseType(
                                          type_den_aliased_.ToString()),
                                  array_index_.get());
                    }
                }

                if (WarnEnabled(Warnings::IndexBoundary))
                {
                    if (auto prefix_type_den_ = GetTypeDenoterFrom(
                                expr_a->prefix_expr_.get()))
                    {
                        if (auto prefix_array_type_den_ =
                                    prefix_type_den_->GetAliased()
                                            .As<ArrayTypeDenoter>())
                        {
                            for (std::size_t
                                         i_ = 0,
                                         n_ = std::min(
                                                 expr_a->array_indices_.size(),
                                                 prefix_array_type_den_->array_dims_
                                                         .size());
                                 i_ < n_;
                                 ++i_)
                            {
                                if (auto value_ = EvaluateOrDefault(
                                            *(expr_a->array_indices_[i_])))
                                {
                                    try
                                    {
                                        auto array_idx_ = static_cast<int>(
                                                value_.ToInt());
                                        
                                        prefix_array_type_den_->array_dims_[i_]
                                                ->ValidateIndexBoundary(
                                                        array_idx_);
                                    } 
                                    catch (const std::exception& e_)
                                    {
                                        Warning(e_.what(),
                                                expr_a->array_indices_[i_].get());
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeEntryPoint(FunctionDecl* func_decl_a)
            {
                if (func_decl_a->flags_.SetOnce(FunctionDecl::IsEntryPoint))
                {
                    program_->entry_point_ref_ = func_decl_a;

                    for (auto& param_ : func_decl_a->parameters_)
                    {
                        const auto& type_denoter_ = param_->type_specifier_
                                                          ->GetTypeDenoter()
                                                          ->GetAliased();
                        if (auto struct_type_den_ =
                                    type_denoter_.As<StructTypeDenoter>())
                        {
                            if (auto struct_decl_ = struct_type_den_->struct_decl_ref_)
                                func_decl_a->param_structs_.push_back(
                                        {nullptr,
                                         param_->var_decls_.front().get(),
                                         struct_decl_});
                        }
                    }

                    AnalyzeEntryPointInputOutput(func_decl_a);
                    AnalyzeEntryPointAttributes(
                            func_decl_a->decl_stmnt_ref_->attribs_);
                }
            }

            void HLSLAnalyzer::AnalyzeEntryPointInputOutput(
                    FunctionDecl* func_decl_a)
            {
                for (auto& param_ : func_decl_a->parameters_)
                {
                    if (param_->var_decls_.size() == 1)
                        AnalyzeEntryPointParameter(func_decl_a, param_.get());
                    else
                        Error(R_InvalidVarDeclCountInParam, param_.get());
                }

                const auto& return_type_den_ =
                        func_decl_a->return_type_->GetTypeDenoter()->GetAliased();
                if (auto struct_type_den_ = return_type_den_.As<StructTypeDenoter>())
                {
                    AnalyzeEntryPointParameterInOutStruct(
                            func_decl_a,
                            struct_type_den_->struct_decl_ref_,
                            false);
                }

                if (func_decl_a->flags_(FunctionDecl::IsEntryPoint))
                {
                    std::vector<Semantic> in_semantics_, out_semantics_;

                    for (const auto& param_ :
                         func_decl_a->input_semantics_.var_decl_refs_sv_)
                        in_semantics_.push_back(param_->semantic_);

                    for (const auto& param_ :
                         func_decl_a->output_semantics_.var_decl_refs_sv_)
                        out_semantics_.push_back(param_->semantic_);

                    if (IsSystemSemantic(func_decl_a->semantic_))
                        out_semantics_.push_back(func_decl_a->semantic_);

                    AnalyzeEntryPointSemantics(func_decl_a,
                                               in_semantics_,
                                               out_semantics_);
                }

                if (func_decl_a->semantic_.IsValid()
                    && !func_decl_a->output_semantics_.Empty())
                {
                    auto semantic_index_ = func_decl_a->semantic_.Index();

                    func_decl_a->output_semantics_.ForEach([&](VarDecl* var_decl_) {
                        if (!var_decl_->decl_stmnt_ref_->flags_(
                                    VarDeclStmnt::IsParameter))
                        {
                            var_decl_->semantic_ =
                                    IndexedSemantic(func_decl_a->semantic_,
                                                    semantic_index_);
                            ++semantic_index_;
                        }
                    });

                    func_decl_a->output_semantics_.UpdateDistribution();

                    if (!func_decl_a->semantic_.IsSystemValue())
                        func_decl_a->semantic_.Reset();
                }

                std::map<IndexedSemantic, int> output_semantic_counter_;

                func_decl_a->output_semantics_.ForEach([&output_semantic_counter_](
                                                          VarDecl* var_decl_) {
                    auto it_ = output_semantic_counter_.find(var_decl_->semantic_);
                    if (it_ != output_semantic_counter_.end())
                        ++it_->second;
                    else
                        output_semantic_counter_.insert({var_decl_->semantic_, 1});
                });

                for (const auto& it_ : output_semantic_counter_)
                {
                    if (it_.second > 1)
                        Error(R_DuplicateUseOfOutputSemantic(
                                it_.first.ToString()));
                }
            }

            void HLSLAnalyzer::AnalyzeEntryPointParameter(
                    FunctionDecl* func_decl_a,
                    VarDeclStmnt* param_a)
            {
                auto var_decl_ = param_a->var_decls_.front().get();

                if (param_a->IsUniform())
                {
                    if (param_a->IsOutput())
                        Error(R_UniformCantBeOutput, var_decl_);
                } 
                else
                {
                    if (param_a->IsOutput())
                        AnalyzeEntryPointParameterInOut(func_decl_a,
                                                        var_decl_,
                                                        false);
                
                    else if (param_a->IsInput())
                        AnalyzeEntryPointParameterInOut(func_decl_a,
                                                        var_decl_,
                                                        true);
                }
            }

            void HLSLAnalyzer::AnalyzeEntryPointParameterInOut(
                    FunctionDecl* func_decl_a,
                    VarDecl* var_decl_a,
                    bool input_a,
                    TypeDenoterPtr var_type_den_a)
            {
                if (!var_type_den_a)
                    var_type_den_a = var_decl_a->GetTypeDenoter()->GetSub();

                if (auto struct_type_den_ = var_type_den_a->As<StructTypeDenoter>())
                {
                    AnalyzeEntryPointParameterInOutStruct(
                            func_decl_a,
                            struct_type_den_->struct_decl_ref_,
                            input_a);
                } 
                else if (auto buffer_type_den_ =
                                   var_type_den_a->As<BufferTypeDenoter>())
                {
                    AnalyzeEntryPointParameterInOutBuffer(func_decl_a,
                                                          var_decl_a,
                                                          buffer_type_den_,
                                                          input_a);
                } 
                else if (auto array_type_den_ =
                                   var_type_den_a->As<ArrayTypeDenoter>())
                {
                    AnalyzeEntryPointParameterInOut(
                            func_decl_a,
                            var_decl_a,
                            input_a,
                            array_type_den_->sub_type_denoter_);
                } 
                else
                {
                    AnalyzeEntryPointParameterInOutVariable(func_decl_a,
                                                            var_decl_a,
                                                            input_a);
                }

                if (shader_target_ == ShaderTarget::GeometryShader)
                {
                    if (input_a)
                    {
                        const auto primitive_type_ =
                                var_decl_a->decl_stmnt_ref_->type_specifier_
                                        ->primitive_type_;
                        
                        if (primitive_type_ != PrimitiveType::Undefined)
                            program_->layout_geometry_.input_primitive_ =
                                    primitive_type_;
                    } 
                    else
                    {
                        if (auto buffer_type_den_ =
                                    var_type_den_a->As<BufferTypeDenoter>())
                        {
                            if (IsStreamBufferType(buffer_type_den_->buffer_type_))
                                program_->layout_geometry_.output_primitive_ =
                                        buffer_type_den_->buffer_type_;
                        }
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeEntryPointParameterInOutVariable(
                    FunctionDecl* func_decl_a,
                    VarDecl* var_decl_a,
                    bool input_a)
            {
                if (var_decl_a->semantic_.IsValid())
                {
                    if (var_decl_a->semantic_.IsSystemValue())
                        var_decl_a->flags_ << VarDecl::IsSystemValue;
                } 
                else
                    Error(R_MissingSemanticInEntryPointParam(var_decl_a->ident_),
                          var_decl_a);

                if (input_a)
                {
                    func_decl_a->input_semantics_.Add(var_decl_a);
                    var_decl_a->flags_ << VarDecl::IsShaderInput;
                } 
                else
                {
                    func_decl_a->output_semantics_.Add(var_decl_a);
                    var_decl_a->flags_ << VarDecl::IsShaderOutput;
                }
            }

            void HLSLAnalyzer::AnalyzeEntryPointParameterInOutStruct(
                    FunctionDecl* func_decl_a,
                    StructDecl* struct_decl_a,
                    bool input_a)
            {
                if (struct_decl_a)
                {
                    for (auto& member_ : struct_decl_a->var_members_)
                    {
                        for (auto& member_var_ : member_->var_decls_)
                            AnalyzeEntryPointParameterInOut(func_decl_a,
                                                            member_var_.get(),
                                                            input_a);
                    }

                    if (input_a)
                        struct_decl_a->AddFlagsRecursive(
                                StructDecl::IsShaderInput);
                    else
                        struct_decl_a->AddFlagsRecursive(
                                StructDecl::IsShaderOutput);
                }
            }

            void HLSLAnalyzer::AnalyzeEntryPointParameterInOutBuffer(
                    FunctionDecl* func_decl_a,
                    VarDecl* var_decl_a,
                    BufferTypeDenoter* buffer_type_den_a,
                    bool input_a)
            {
                if (IsStreamBufferType(buffer_type_den_a->buffer_type_)
                    || IsPatchBufferType(buffer_type_den_a->buffer_type_))
                {
                    if (buffer_type_den_a->generic_type_denoter_)
                        AnalyzeEntryPointParameterInOut(
                                func_decl_a,
                                var_decl_a,
                                input_a,
                                buffer_type_den_a->generic_type_denoter_);
                    else
                        Error(R_MissingGenericTypeDen(BufferTypeToString(
                                      buffer_type_den_a->buffer_type_)),
                              var_decl_a);
                } 
                else
                    Error(R_IllegalBufferTypeForEntryPoint(input_a ? R_Input
                                                                 : R_Output),
                          var_decl_a);
            }

            void HLSLAnalyzer::AnalyzeEntryPointAttributes(
                    const std::vector<AttributePtr>& attribs_a)
            {
                switch (shader_target_)
                {
                case ShaderTarget::TessellationControlShader:
                    AnalyzeEntryPointAttributesTessControlShader(attribs_a);
                    break;
                case ShaderTarget::TessellationEvaluationShader:
                    AnalyzeEntryPointAttributesTessEvaluationShader(attribs_a);
                    break;
                case ShaderTarget::GeometryShader:
                    AnalyzeEntryPointAttributesGeometryShader(attribs_a);
                    break;
                case ShaderTarget::FragmentShader:
                    AnalyzeEntryPointAttributesFragmentShader(attribs_a);
                    break;
                case ShaderTarget::ComputeShader:
                    AnalyzeEntryPointAttributesComputeShader(attribs_a);
                    break;
                default:
                    break;
                }
            }

            void HLSLAnalyzer::AnalyzeEntryPointAttributesTessControlShader(
                    const std::vector<AttributePtr>& attribs_a)
            {
                bool found_domain_ = false;
                bool found_output_control_points_ = false;
                bool found_output_topology_ = false;
                bool found_partitioning_ = false;
                bool found_patch_constant_func_ = false;

                for (const auto& attr_ : attribs_a)
                {
                    switch (attr_->attribute_type_)
                    {
                    case AttributeType::Domain:
                        AnalyzeAttributeDomain(attr_.get());
                        found_domain_ = true;
                        break;

                    case AttributeType::OutputControlPoints:
                        AnalyzeAttributeOutputControlPoints(attr_.get());
                        found_output_control_points_ = true;
                        break;

                    case AttributeType::OutputTopology:
                        AnalyzeAttributeOutputTopology(attr_.get());
                        found_output_topology_ = true;
                        break;

                    case AttributeType::Partitioning:
                        AnalyzeAttributePartitioning(attr_.get());
                        found_partitioning_ = true;
                        break;

                    case AttributeType::PatchConstantFunc:
                        AnalyzeAttributePatchConstantFunc(attr_.get());
                        found_patch_constant_func_ = true;
                        break;

                    default:
                        break;
                    }
                }

                ErrorIfAttributeNotFound(found_domain_, "domain(type)");
                ErrorIfAttributeNotFound(found_output_control_points_,
                                         "outputcontrolpoints(count)");
                ErrorIfAttributeNotFound(found_output_topology_,
                                         "outputtopology(topology)");
                ErrorIfAttributeNotFound(found_partitioning_,
                                         "partitioning(mode)");
                ErrorIfAttributeNotFound(found_patch_constant_func_,
                                         "patchconstantfunc(function)");
            }

            void HLSLAnalyzer::AnalyzeEntryPointAttributesTessEvaluationShader(
                    const std::vector<AttributePtr>& attribs_a)
            {
                bool found_domain_ = false;

                for (const auto& attr_ : attribs_a)
                {
                    switch (attr_->attribute_type_)
                    {
                    case AttributeType::Domain:
                        AnalyzeAttributeDomain(attr_.get());
                        found_domain_ = true;
                        break;

                    default:
                        break;
                    }
                }

                ErrorIfAttributeNotFound(found_domain_, "domain(type)");
            }

            void HLSLAnalyzer::AnalyzeEntryPointAttributesGeometryShader(
                    const std::vector<AttributePtr>& attribs_a)
            {
                for (const auto& attr_ : attribs_a)
                {
                    switch (attr_->attribute_type_)
                    {
                    case AttributeType::MaxVertexCount:
                        AnalyzeAttributeMaxVertexCount(attr_.get());
                        break;
                    default:
                        break;
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeEntryPointAttributesFragmentShader(
                    const std::vector<AttributePtr>& attribs_a)
            {
                for (const auto& attr_ : attribs_a)
                {
                    switch (attr_->attribute_type_)
                    {
                    case AttributeType::EarlyDepthStencil:
                        program_->layout_fragment_.early_depth_stencil_ = true;
                        break;
                    default:
                        break;
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeEntryPointAttributesComputeShader(
                    const std::vector<AttributePtr>& attribs_a)
            {
                bool found_num_threads_ = false;
                for (const auto& attr_ : attribs_a)
                {
                    switch (attr_->attribute_type_)
                    {
                    case AttributeType::NumThreads:
                        AnalyzeAttributeNumThreads(attr_.get());
                        found_num_threads_ = true;
                        break;

                    default:
                        break;
                    }
                }

                ErrorIfAttributeNotFound(found_num_threads_,
                                         "numthreads(x, y, z)");
            }

            void HLSLAnalyzer::AnalyzeEntryPointSemantics(
                    FunctionDecl* func_decl_a,
                    const std::vector<Semantic>& in_semantics_a,
                    const std::vector<Semantic>& out_semantics_a)
            {
                auto FindSemantics =
                        [&](const std::vector<Semantic>& present_semantics_,
                            const std::vector<Semantic>& search_semantics_,
                            const JoinableString& report_ident_) {
                            for (auto sem_ : present_semantics_)
                            {
                                if (IsSystemSemantic(sem_)
                                    && std::find(search_semantics_.begin(),
                                                 search_semantics_.end(),
                                                 sem_)
                                               == search_semantics_.end())
                                    
                                    Error(report_ident_(SemanticToString(sem_),
                                                      func_decl_a->ident_),
                                          func_decl_a);
                            }
                        };

                auto ValidateInSemantics =
                        [&](const std::vector<Semantic>& semantics) {
                            FindSemantics(in_semantics_a,
                                          semantics,
                                          R_InvalidInputSemanticInEntryPoint);
                        };

                auto ValidateOutSemantics =
                        [&](const std::vector<Semantic>& semantics) {
                            FindSemantics(out_semantics_a,
                                          semantics,
                                          R_InvalidOutputSemanticInEntryPoint);
                        };

                auto RequiredOutSemantics =
                        [&](const std::vector<Semantic>& semantics) {
                            FindSemantics(semantics,
                                          out_semantics_a,
                                          R_MissingOutputSemanticInEntryPoint);
                        };

                using T = Semantic;

#define COMMON_SEMANTICS                                                       \
    T::InstanceID, T::DepthGreaterEqual, T::DepthLessEqual, T::VertexID,       \
            T::PrimitiveID

#define COMMON_SEMANTICS_EX COMMON_SEMANTICS, T::ClipDistance, T::CullDistance

                switch (shader_target_)
                {
                case ShaderTarget::VertexShader:
                    if (IsD3D9ShaderModel())
                    {
                        ValidateInSemantics({COMMON_SEMANTICS,
                                             T::VertexPosition,
                                             T::PointSize,
                                             T::Target});
                    
                        ValidateOutSemantics({COMMON_SEMANTICS_EX,
                                              T::VertexPosition,
                                              T::PointSize,
                                              T::Target});
                    } 
                    else
                    {
                        ValidateInSemantics({COMMON_SEMANTICS});
                        ValidateOutSemantics({COMMON_SEMANTICS_EX,
                                              T::VertexPosition,
                                              T::PointSize});
                    }
                    break;

                case ShaderTarget::TessellationControlShader:
                    ValidateInSemantics({COMMON_SEMANTICS_EX,
                                         T::VertexPosition,
                                         T::PointSize,
                                         T::OutputControlPointID});
                    ValidateOutSemantics({COMMON_SEMANTICS_EX,
                                          T::VertexPosition,
                                          T::PointSize,
                                          T::InsideTessFactor,
                                          T::TessFactor});
                    break;

                case ShaderTarget::TessellationEvaluationShader:
                    ValidateInSemantics({COMMON_SEMANTICS_EX,
                                         T::VertexPosition,
                                         T::PointSize,
                                         T::InsideTessFactor,
                                         T::TessFactor,
                                         T::DomainLocation});
                    ValidateOutSemantics({COMMON_SEMANTICS_EX,
                                          T::VertexPosition,
                                          T::PointSize});
                    break;

                case ShaderTarget::GeometryShader:
                    ValidateInSemantics({COMMON_SEMANTICS_EX,
                                         T::VertexPosition,
                                         T::PointSize,
                                         T::GSInstanceID});
                    ValidateOutSemantics({COMMON_SEMANTICS_EX,
                                          T::VertexPosition,
                                          T::PointSize,
                                          T::IsFrontFace,
                                          T::ViewportArrayIndex,
                                          T::RenderTargetArrayIndex});
                    break;

                case ShaderTarget::FragmentShader:
                    if (!IsD3D9ShaderModel())
                    {
                        ValidateInSemantics({COMMON_SEMANTICS_EX,
                                             T::Coverage,
                                             T::InnerCoverage,
                                             T::Depth,
                                             T::SampleIndex,
                                             T::RenderTargetArrayIndex,
                                             T::FragCoord,
                                             T::IsFrontFace});
                        
                        ValidateOutSemantics({COMMON_SEMANTICS_EX,
                                              T::Coverage,
                                              T::InnerCoverage,
                                              T::Depth,
                                              T::SampleIndex,
                                              T::RenderTargetArrayIndex,
                                              T::Target,
                                              T::StencilRef});
                    }
                    RequiredOutSemantics({T::Target});
                    break;

                case ShaderTarget::ComputeShader:
                    ValidateInSemantics({COMMON_SEMANTICS_EX,
                                         T::GroupID,
                                         T::GroupIndex,
                                         T::GroupThreadID,
                                         T::DispatchThreadID});
                    
                    ValidateOutSemantics({COMMON_SEMANTICS_EX});
                    break;

                default:
                    break;
                }

#undef COMMON_SEMANTICS
#undef COMMON_SEMANTICS_EX
            }

            void HLSLAnalyzer::AnalyzeEntryPointOutput(Expr* expr_a)
            {
                if (auto object_expr_ = expr_a->FetchLValueExpr())
                {
                    if (auto var_decl_ = object_expr_->FetchVarDecl())
                    {
                        var_decl_->AddFlagsRecursive(VarDecl::IsEntryPointOutput);

                        if (auto struct_symbol_ref_ = var_decl_->GetTypeDenoter()
                                                           ->GetAliased()
                                                           .SymbolRef())
                        {
                            if (auto struct_decl_ =
                                        struct_symbol_ref_->As<StructDecl>())
                            {
                                struct_decl_->AddShaderOutputInstance(var_decl_);

                                if (program_->entry_point_ref_)
                                    program_->entry_point_ref_->param_structs_
                                            .push_back({expr_a,
                                                        var_decl_,
                                                        struct_decl_});

                                var_decl_->flags_ << VarDecl::IsEntryPointLocal;
                            }
                        }
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeSecondaryEntryPoint(
                    FunctionDecl* func_decl_a,
                    bool is_patch_constant_func_a)
            {
                if (func_decl_a->flags_.SetOnce(
                            FunctionDecl::IsSecondaryEntryPoint))
                {
                    if (is_patch_constant_func_a)
                        program_->layout_tess_control_.patch_const_function_ref_ =
                                func_decl_a;

                    AnalyzeEntryPointInputOutput(func_decl_a);
                    AnalyzeSecondaryEntryPointAttributes(
                            func_decl_a->decl_stmnt_ref_->attribs_);
                }
            }

            void HLSLAnalyzer::AnalyzeSecondaryEntryPointAttributes(
                    const std::vector<AttributePtr>& attribs_a)
            {
                switch (shader_target_)
                {
                case ShaderTarget::TessellationEvaluationShader:
                    AnalyzeSecondaryEntryPointAttributesTessEvaluationShader(
                            attribs_a);
                    break;
                default:
                    break;
                }
            }

            void HLSLAnalyzer::
                    AnalyzeSecondaryEntryPointAttributesTessEvaluationShader(
                            const std::vector<AttributePtr>& attribs_a)
            {
                for (const auto& attr_ : attribs_a)
                {
                    switch (attr_->attribute_type_)
                    {
                    case AttributeType::OutputTopology:
                        AnalyzeAttributeOutputTopology(attr_.get(), false);
                        break;
                    case AttributeType::Partitioning:
                        AnalyzeAttributePartitioning(attr_.get(), false);
                        break;
                    default:
                        break;
                    }
                }
            }

            bool HLSLAnalyzer::AnalyzeNumArgsAttribute(Attribute* attrib_a,
                                                       std::size_t min_num_args_a,
                                                       std::size_t max_num_args_a,
                                                       bool required_a)
            {
                auto num_args_ = attrib_a->arguments_.size();

                const std::string max_num_args_str_ =
                        (min_num_args_a == max_num_args_a ? ""
                                                  : std::to_string(max_num_args_a));

                if (num_args_ < min_num_args_a)
                {
                    if (required_a)
                    {
                        Error(R_TooFewArgsForAttribute("",
                                                       num_args_,
                                                       min_num_args_a,
                                                       max_num_args_str_),
                              attrib_a);
                    }
                } 
                else if (num_args_ > max_num_args_a)
                {
                    if (required_a)
                    {
                        Error(R_TooManyArgsForAttribute("",
                                                        num_args_,
                                                        min_num_args_a,
                                                        max_num_args_str_),
                              attrib_a);
                    }
                }
                else
                    return true;

                return false;
            }

            bool HLSLAnalyzer::AnalyzeNumArgsAttribute(
                    Attribute* attrib_a,
                    std::size_t expected_num_args_a,
                    bool required_a)
            {
                return AnalyzeNumArgsAttribute(attrib_a,
                                               expected_num_args_a,
                                               expected_num_args_a,
                                               required_a);
            }

            void HLSLAnalyzer::AnalyzeAttributeDomain(Attribute* attrib_a,
                                                      bool required_a)
            {
                if (AnalyzeNumArgsAttribute(attrib_a, 1, required_a))
                {
                    AnalyzeAttributeValue(
                            attrib_a->arguments_[0].get(),
                            program_->layoutTessEvaluation.domain_type_,
                            IsAttributeValueDomain,
                            R_ExpectedDomainTypeParamToBe,
                            required_a);
                }
            }

            void HLSLAnalyzer::AnalyzeAttributeOutputTopology(Attribute* attrib_a,
                                                              bool required_a)
            {
                if (AnalyzeNumArgsAttribute(attrib_a, 1, required_a))
                {
                    AnalyzeAttributeValue(
                            attrib_a->arguments_[0].get(),
                            program_->layoutTessEvaluation.output_topology_,
                            IsAttributeValueOutputTopology,
                            R_ExpectedOutputTopologyParamToBe,
                            required_a);
                }
            }

            void HLSLAnalyzer::AnalyzeAttributePartitioning(Attribute* attrib_a,
                                                            bool required_a)
            {
                if (AnalyzeNumArgsAttribute(attrib_a, 1, required_a))
                {
                    AnalyzeAttributeValue(
                            attrib_a->arguments_[0].get(),
                            program_->layoutTessEvaluation.partitioning_,
                            IsAttributeValuePartitioning,
                            R_ExpectedPartitioningModeParamToBe,
                            required_a);
                }
            }

            void HLSLAnalyzer::AnalyzeAttributeOutputControlPoints(
                    Attribute* attrib_a)
            {
                if (AnalyzeNumArgsAttribute(attrib_a, 1))
                {
                    auto count_param_variant_ = EvaluateConstExpr(
                            *attrib_a->arguments_[0]);

                    int count_param_ = -1;
                    if (count_param_variant_.Type() == Variant::Types::Int)
                        count_param_ = static_cast<int>(count_param_variant_.Int());

                    if (count_param_ >= 0)
                        program_->layout_tess_control_.outputControlPoints =
                                static_cast<unsigned int>(count_param_);
                    else
                        Error(R_ExpectedOutputCtrlPointParamToBe,
                              attrib_a->arguments_[0].get());
                }
            }

            void HLSLAnalyzer::AnalyzeAttributePatchConstantFunc(
                    Attribute* attrib_a)
            {
                if (AnalyzeNumArgsAttribute(attrib_a, 1))
                {
                    auto literal_expr_ = attrib_a->arguments_[0]->As<LiteralExpr>();
                    if (literal_expr_
                        && literal_expr_->data_type_ == DataType::String)
                    {
                        auto literal_value_ = literal_expr_->GetStringValue();
                        if (auto patch_const_func_ = FetchFunctionDecl(
                                    literal_value_))
                        {
                            AnalyzeSecondaryEntryPoint(patch_const_func_, true);
                        } 
                        else
                            Error(R_EntryPointForPatchFuncNotFound(
                                          literal_value_),
                                  attrib_a->arguments_[0].get());
                    } 
                    else
                        Error(R_ExpectedPatchFuncParamToBe,
                              attrib_a->arguments_[0].get());
                }
            }

            void HLSLAnalyzer::AnalyzeAttributeMaxVertexCount(Attribute* attrib_a)
            {
                if (AnalyzeNumArgsAttribute(attrib_a, 1))
                {
                    int expr_value_ = EvaluateConstExprInt(*attrib_a->arguments_[0]);
                    if (expr_value_ > 0)
                        program_->layout_geometry_.max_vertices_ =
                                static_cast<unsigned int>(expr_value_);
                    else
                        Error(R_MaxVertexCountMustBeGreaterZero, attrib_a);
                }
            }

            void HLSLAnalyzer::AnalyzeAttributeNumThreads(Attribute* attrib_a)
            {
                if (AnalyzeNumArgsAttribute(attrib_a, 3))
                {
                    for (int i_ = 0; i_ < 3; ++i_)
                    {
                        AnalyzeAttributeNumThreadsArgument(
                                attrib_a->arguments_[i_].get(),
                                program_->layoutCompute.num_threads_[i_]);
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeAttributeNumThreadsArgument(
                    Expr* expr_a,
                    unsigned int& value_a)
            {
                int expr_value_ = EvaluateConstExprInt(*expr_a);
                if (expr_value_ > 0)
                    value_a = static_cast<unsigned int>(expr_value_);
                else
                    Error(R_NumThreadsMustBeGreaterZero, expr_a);
            }

            void HLSLAnalyzer::AnalyzeAttributeValue(
                    Expr* arg_expr_a,
                    AttributeValue& value_a,
                    const OnValidAttributeValueProc& expected_value_func_a,
                    const std::string& expectation_desc_a,
                    bool required_a)
            {
                std::string literal_value_;
                if (!AnalyzeAttributeValuePrimary(arg_expr_a,
                                                  value_a,
                                                  expected_value_func_a,
                                                  literal_value_)
                    && required_a)
                    Error(expectation_desc_a + R_ButGot(literal_value_), arg_expr_a);
            }

            bool HLSLAnalyzer::AnalyzeAttributeValuePrimary(
                    Expr* arg_expr_a,
                    AttributeValue& value_a,
                    const OnValidAttributeValueProc& expected_value_func_a,
                    std::string& literal_value_a)
            {
                if (auto literal_expr_ = arg_expr_a->As<LiteralExpr>())
                {
                    literal_value_a = literal_expr_->GetStringValue();
                    value_a = HLSLKeywordToAttributeValue(literal_value_a);
                    return expected_value_func_a(value_a);
                }
                return false;
            }

            void HLSLAnalyzer::AnalyzeSemantic(IndexedSemantic& semantic_a)
            {
                if (semantic_a == Semantic::FragCoord
                    && shader_target_ != ShaderTarget::FragmentShader)
                {
                    semantic_a = IndexedSemantic(Semantic::VertexPosition,
                                               semantic_a.Index());
                }
            }

            void HLSLAnalyzer::AnalyzeSemanticSM3(IndexedSemantic& semantic_a,
                                                  bool input_a)
            {
                if ((shader_target_ == ShaderTarget::VertexShader
                     && semantic_a == Semantic::VertexPosition && input_a)
                    || (shader_target_ == ShaderTarget::VertexShader
                        && semantic_a == Semantic::Target)
                    || (shader_target_ == ShaderTarget::FragmentShader
                        && semantic_a == Semantic::Target && input_a)
                    || (semantic_a == Semantic::PointSize && input_a))
                {
                    switch (semantic_a)
                    {
                    case Semantic::PointSize:
                        semantic_a.MakeUserDefined("PSIZE");
                        break;
                    case Semantic::VertexPosition:
                        semantic_a.MakeUserDefined("POSITION");
                        break;
                    case Semantic::Target:
                        semantic_a.MakeUserDefined("COLOR");
                        break;
                    default:
                        break;
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeSemanticSM3Remaining()
            {
                if (!var_decl_sm3_semantics_.empty())
                {
                    for (auto var_decl_ : var_decl_sm3_semantics_)
                        AnalyzeSemanticSM3(var_decl_->semantic_,
                                           var_decl_->flags_(
                                                   VarDecl::IsShaderInput));

                    if (auto entry_point_l_ = program_->entry_point_ref_)
                    {
                        entry_point_l_->input_semantics_.UpdateDistribution();
                        entry_point_l_->output_semantics_.UpdateDistribution();
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeSemanticVarDecl(IndexedSemantic& semantic_a,
                                                      VarDecl* var_decl_a)
            {
                AnalyzeSemantic(semantic_a);

                if (IsD3D9ShaderModel())
                {
                    if (semantic_a == Semantic::VertexPosition
                        || semantic_a == Semantic::Target
                        || semantic_a == Semantic::PointSize)
                    {
                        var_decl_sm3_semantics_.insert(var_decl_a);
                    }
                }
            }

            void HLSLAnalyzer::AnalyzeSemanticFunctionReturn(
                    IndexedSemantic& semantic_a)
            {
                AnalyzeSemantic(semantic_a);
                if (IsD3D9ShaderModel())
                    AnalyzeSemanticSM3(semantic_a, false);
            }

            void HLSLAnalyzer::AnalyzeArrayDimensionList(
                    const std::vector<ArrayDimensionPtr>& array_dims_a)
            {
                Visit(array_dims_a);

                for (std::size_t i_ = 1; i_ < array_dims_a.size(); ++i_)
                {
                    auto dim_ = array_dims_a[i_].get();
                    if (dim_->HasDynamicSize())
                        Error(R_SecondaryArrayDimMustBeExplicit, dim_);
                }
            }

            void HLSLAnalyzer::AnalyzeParameter(VarDeclStmnt* param_a)
            {
                Visit(param_a);

                AnalyzeTypeSpecifierForParameter(param_a->type_specifier_.get());

                if (auto struct_decl_ = param_a->type_specifier_->struct_decl_.get())
                    Error(R_StructsCantBeDefinedInParam(struct_decl_->ToString()),
                          param_a);
            }
        } // namespace ShaderPack
    } // namespace AId
} // namespace CE_Kernel
