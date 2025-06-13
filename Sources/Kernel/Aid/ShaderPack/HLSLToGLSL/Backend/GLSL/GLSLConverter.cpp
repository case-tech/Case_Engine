#include "GLSLConverter.hpp"

#include "../../AST/AST.hpp"
#include "../../AST/ASTFactory.hpp"
#include "../../AST/Visitor/ExprConverter.hpp"
#include "../../Exception.hpp"
#include "../../Frontend/GLSL/GLSLKeywords.hpp"
#include "../../Helper.hpp"
#include "../../Report/ReportIdents.hpp"

#include <utility>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            static const char* g_std_name_self_param_ = "self";
            static const char* g_std_name_base_member_ = "base";
            static const char* g_std_name_dummy_ = "dummy";

            struct CodeBlockStmntArgs
            {
                bool disable_new_scope_;
            };

            bool GLSLConverter::ConvertVarDeclType(VarDecl& var_decl_a)
            {
                if (var_decl_a.semantic_.IsSystemValue())
                {
                    const auto data_type_ = SemanticToGLSLDataType(
                            var_decl_a.semantic_);
                    
                    if (data_type_ != DataType::Undefined)
                        return ConvertVarDeclBaseTypeDenoter(var_decl_a, data_type_);
                }
                return false;
            }

            bool GLSLConverter::ConvertVarDeclBaseTypeDenoter(
                    VarDecl& var_decl_a,
                    const DataType data_type_a)
            {
                if (auto var_decl_stmnt_ = var_decl_a.decl_stmnt_ref_)
                {
                    if (auto var_type_den_ = var_decl_a.GetTypeDenoter()
                                                  ->GetAliased()
                                                  .As<BaseTypeDenoter>())
                    {
                        if (var_type_den_->data_type_ != data_type_a)
                        {
                            auto new_var_type_den_ =
                                    std::make_shared<BaseTypeDenoter>(data_type_a);

                            var_decl_stmnt_->type_specifier_->type_denoter_ =
                                    new_var_type_den_;
                            var_decl_stmnt_->type_specifier_->ResetTypeDenoter();

                            var_decl_a.SetCustomTypeDenoter(new_var_type_den_);

                            return true;
                        }
                    }
                }
                return false;
            }

            void GLSLConverter::ConvertASTPrAzary(
                    Program& program_a,
                    const ShaderInput& input_desc_a,
                    const ShaderOutput& output_desc_a)
            {
                shader_target_ = input_desc_a.shader_target_;
                version_out_ = output_desc_a.shader_version_;
                options_ = output_desc_a.options_;
                auto_binding_ = output_desc_a.options_.auto_binding_;
                auto_binding_slot_ = output_desc_a.options_.auto_binding_start_slot_;
                separate_samplers_ = output_desc_a.options_.separate_samplers_;

                Visit(&program_a);
            }

            bool GLSLConverter::IsVKSL() const
            {
                return IsLanguageVKSL(version_out_);
            }

            bool GLSLConverter::UseSeparateSamplers() const
            {
                return (IsVKSL() && separate_samplers_);
            }

#define IMPLEMENT_VISIT_PROC(AST_NAME)                                         \
    void GLSLConverter::Visit##AST_NAME(AST_NAME* ast_a, void* args_a)

            IMPLEMENT_VISIT_PROC(Program)
            {
                (void)args_a;
                auto entry_point_ = ast_a->entry_point_ref_;

                switch (shader_target_)
                {
                case ShaderTarget::VertexShader:
                    if (GetNameMangling().use_always_semantics_)
                        RenameIdentOfInOutVarDecls(
                                entry_point_->input_semantics_.var_decl_refs_,
                                true,
                                true);
                    
                    RenameIdentOfInOutVarDecls(
                            entry_point_->output_semantics_.var_decl_refs_,
                            false);
                    break;

                case ShaderTarget::FragmentShader:
                    RenameIdentOfInOutVarDecls(
                            entry_point_->input_semantics_.var_decl_refs_,
                            true);
                    
                    if (GetNameMangling().use_always_semantics_)
                        RenameIdentOfInOutVarDecls(
                                entry_point_->output_semantics_.var_decl_refs_,
                                false,
                                true);
                    break;

                default:
                    RenameIdentOfInOutVarDecls(
                            entry_point_->input_semantics_.var_decl_refs_,
                            true);
                    
                    RenameIdentOfInOutVarDecls(
                            entry_point_->output_semantics_.var_decl_refs_,
                            false);
                    break;
                }

                RegisterGlobalDeclIdents(
                        entry_point_->input_semantics_.var_decl_refs_);
                
                RegisterGlobalDeclIdents(
                        entry_point_->output_semantics_.var_decl_refs_);

                RegisterGlobalDeclIdents(
                        entry_point_->input_semantics_.var_decl_refs_sv_);
                
                RegisterGlobalDeclIdents(
                        entry_point_->output_semantics_.var_decl_refs_sv_);

                if (shader_target_ != ShaderTarget::VertexShader
                    && shader_target_ != ShaderTarget::ComputeShader)
                    AddMissingInterpModifiers(
                            entry_point_->input_semantics_.var_decl_refs_);
                
                if (shader_target_ != ShaderTarget::FragmentShader
                    && shader_target_ != ShaderTarget::ComputeShader)
                    AddMissingInterpModifiers(
                            entry_point_->output_semantics_.var_decl_refs_);

                VisitScopedStmntList(ast_a->global_stmnts_);
            }

            IMPLEMENT_VISIT_PROC(CodeBlock)
            {
                (void)args_a;
                RemoveDeadCode(ast_a->stmnts_);
                UnrollStmnts(ast_a->stmnts_);

                VisitScopedStmntList(ast_a->stmnts_);
            }

            IMPLEMENT_VISIT_PROC(CallExpr)
            {
                VISIT_DEFAULT(CallExpr);

                if (ast_a->intrinsic_ != Intrinsic::Undefined)
                {
                    if (IsTextureIntrinsic(ast_a->intrinsic_)
                        && ast_a->prefix_expr_)
                    {
                        ast_a->PushPrefixToArguments();

                        if (UseSeparateSamplers()
                            && !IsTextureLoadIntrinsic(ast_a->intrinsic_)
                            && ast_a->arguments_.size() >= 2)
                        {
                            auto arg1_expr_ = ast_a->arguments_[1].get();
                            if (IsSamplerStateTypeDenoter(
                                        arg1_expr_->GetTypeDenoter()))
                            {
                                ast_a->MergeArguments(
                                        0,
                                        ASTFactory::
                                                MakeTextureSamplerBindingCallExpr);
                            }
                        }
                    }
                }

                if (!UseSeparateSamplers())
                {
                    MoveAllIf(ast_a->arguments_,
                              GetProgram()->disabled_ast_,
                              [&](const ExprPtr& expr_) {
                                  return IsSamplerStateTypeDenoter(
                                          expr_->GetTypeDenoter());
                              });
                }

                if (ast_a->intrinsic_ != Intrinsic::Undefined)
                    ConvertIntrinsicCall(ast_a);
                else
                    ConvertFunctionCall(ast_a);
            }

            IMPLEMENT_VISIT_PROC(SwitchCase)
            {
                (void)args_a;
                RemoveDeadCode(ast_a->stmnts_);

                Visit(ast_a->expr_);
                VisitScopedStmntList(ast_a->stmnts_);
            }

            IMPLEMENT_VISIT_PROC(TypeSpecifier)
            {
                auto type_den_ = ast_a->type_denoter_->GetSub();
                if (auto struct_type_den_ = type_den_->As<StructTypeDenoter>())
                {
                    if (auto struct_decl_ = struct_type_den_->struct_decl_ref_)
                    {
                        if (struct_decl_->compatible_struct_ref_)
                        {
                            struct_type_den_->struct_decl_ref_ =
                                    struct_decl_->compatible_struct_ref_;
                            
                            GetProgram()->disabled_ast_.emplace_back(
                                    std::move(ast_a->struct_decl_));
                        }
                    }
                }

                VISIT_DEFAULT(TypeSpecifier);
            }

            IMPLEMENT_VISIT_PROC(VarDecl)
            {
                if (ast_a->IsStatic())
                {
                    if (auto struct_decl_ = ast_a->struct_decl_ref_)
                    {
                        ast_a->ident_ = struct_decl_->ident_ + "_"
                                        + ast_a->ident_;
                        
                        ast_a->ident_.AppendPrefix(
                                GetNameMangling().namespace_prefix_);
                    }
                }

                RegisterDeclIdent(ast_a);
                VISIT_DEFAULT(VarDecl);
            }

            IMPLEMENT_VISIT_PROC(BufferDecl)
            {
                RegisterDeclIdent(ast_a);
                ConvertSlotRegisters(ast_a->slot_registers_);
                VISIT_DEFAULT(BufferDecl);
            }

            IMPLEMENT_VISIT_PROC(SamplerDecl)
            {
                RegisterDeclIdent(ast_a);
                ConvertSlotRegisters(ast_a->slot_registers_);
                VISIT_DEFAULT(SamplerDecl);
            }

            IMPLEMENT_VISIT_PROC(StructDecl)
            {
                (void)args_a;
                if (!ast_a->IsAnonymous())
                {
                    while (Fetch(ast_a->ident_))
                        RenameIdentOf(ast_a);

                    RegisterDeclIdent(ast_a);
                }

                LabelAnonymousDecl(ast_a);
                RenameReservedKeyword(ast_a->ident_);

                if (auto base_struct_ = ast_a->base_struct_ref_)
                {
                    auto base_member_type_den_ =
                            std::make_shared<StructTypeDenoter>(base_struct_);
                    
                    auto base_member_type_ = ASTFactory::MakeTypeSpecifier(
                            base_member_type_den_);
                    
                    auto base_member_ = ASTFactory::MakeVarDeclStmnt(
                            base_member_type_,
                            GetNameMangling().namespace_prefix_
                                    + g_std_name_base_member_);

                    base_member_->flags_ << VarDeclStmnt::IsBaseMember;
                    base_member_->var_decls_.front()->struct_decl_ref_ = ast_a;

                    ast_a->local_stmnts_.insert(ast_a->local_stmnts_.begin(),
                                            base_member_);
                    
                    ast_a->var_members_.insert(ast_a->var_members_.begin(),
                                               base_member_);
                }

                PushStructDecl(ast_a);
                OpenScope();
                {
                    VisitScopedStmntList(ast_a->local_stmnts_);
                }
                CloseScope();
                PopStructDecl();

                if (!UseSeparateSamplers())
                    RemoveSamplerStateVarDeclStmnts(ast_a->var_members_);

                MoveNestedStructDecls(ast_a->local_stmnts_, false);

                if (ast_a->NumMemberVariables(true) == 0)
                {
                    auto dummy_member_ = ASTFactory::MakeVarDeclStmnt(
                            DataType::Int,
                            GetNameMangling().temporary_prefix_ + g_std_name_dummy_);
                    
                    ast_a->var_members_.push_back(dummy_member_);
                }
            }

            IMPLEMENT_VISIT_PROC(FunctionDecl)
            {
                (void)args_a;
                PushFunctionDecl(ast_a);
                OpenScope();
                {
                    ConvertFunctionDecl(ast_a);
                }
                CloseScope();
                PopFunctionDecl();
            }

            IMPLEMENT_VISIT_PROC(UniformBufferDecl)
            {
                (void)args_a;
                PushUniformBufferDecl(ast_a);
                {
                    ConvertSlotRegisters(ast_a->slot_registers_);
                    VisitScopedStmntList(ast_a->local_stmnts_);

                    MoveNestedStructDecls(ast_a->local_stmnts_, true);
                }
                PopUniformBufferDecl();
            }

            IMPLEMENT_VISIT_PROC(VarDeclStmnt)
            {
                auto sub_type_den_ = ast_a->type_specifier_->type_denoter_;

                while (sub_type_den_)
                {
                    const auto& type_den_ = sub_type_den_->GetAliased();
                    if (auto array_type_den_ = type_den_.As<ArrayTypeDenoter>())
                    {
                        for (auto& var_decl_ : ast_a->var_decls_)
                        {
                            var_decl_->array_dAzs_.insert(
                                    var_decl_->array_dAzs_.begin(),
                                    array_type_den_->array_dAzs_.begin(),
                                    array_type_den_->array_dAzs_.end());
                        }

                        sub_type_den_ = array_type_den_->sub_type_denoter_;
                    } 
                    else
                        break;
                }

                ast_a->type_specifier_->type_denoter_ = sub_type_den_;

                if (InsideUniformBufferDecl())
                {
                    const auto& type_den_ =
                            ast_a->type_specifier_->type_denoter_->GetAliased();
                    
                    if (type_den_.IsMatrix())
                        ast_a->type_specifier_->SwapMatrixStorageLayout(
                                TypeModifier::RowMajor);
                }

                if (!InsideFunctionDecl())
                {
                    ast_a->type_specifier_->type_modifiers_.erase(
                            TypeModifier::Const);
                }

                VISIT_DEFAULT(VarDeclStmnt);
            }

            IMPLEMENT_VISIT_PROC(AliasDeclStmnt)
            {
                if (ast_a->struct_decl_ && ast_a->struct_decl_->ident_.Empty()
                    && !ast_a->alias_decls_.empty())
                {
                    ast_a->struct_decl_->ident_ = ast_a->alias_decls_.front()->ident_;

                    for (auto& alias_decl_ : ast_a->alias_decls_)
                        alias_decl_->type_denoter_->SetIdentIfAnonymous(
                                ast_a->struct_decl_->ident_);
                }

                VISIT_DEFAULT(AliasDeclStmnt);
            }

            IMPLEMENT_VISIT_PROC(CodeBlockStmnt)
            {
                bool disable_new_scope_ =
                        (args_a != nullptr
                                 ? reinterpret_cast<CodeBlockStmntArgs*>(args_a)
                                           ->disable_new_scope_
                                 : false);

                if (!disable_new_scope_)
                {
                    OpenScope();
                    {
                        Visit(ast_a->code_block_);
                    }
                    CloseScope();
                } 
                else
                    Visit(ast_a->code_block_);
            }

            IMPLEMENT_VISIT_PROC(ForLoopStmnt)
            {
                (void)args_a;
                ConvertEntryPointReturnStmntToCodeBlock(ast_a->body_stmnt_);

                OpenScope();
                {
                    Visit(ast_a->init_stmnt_);
                    Visit(ast_a->condition_);
                    Visit(ast_a->iteration_);

                    if (ast_a->body_stmnt_->Type() == AST::Types::CodeBlockStmnt)
                    {
                        CodeBlockStmntArgs body_stmnt_args_;
                        body_stmnt_args_.disable_new_scope_ = true;
                        VisitScopedStmnt(ast_a->body_stmnt_, &body_stmnt_args_);
                    } 
                    else
                        VisitScopedStmnt(ast_a->body_stmnt_);
                }
                CloseScope();

                ExprConverter::ConvertExprIfCastRequired(ast_a->condition_,
                                                         DataType::Bool);
            }

            IMPLEMENT_VISIT_PROC(WhileLoopStmnt)
            {
                (void)args_a;
                ConvertEntryPointReturnStmntToCodeBlock(ast_a->body_stmnt_);

                OpenScope();
                {
                    Visit(ast_a->condition_);
                    VisitScopedStmnt(ast_a->body_stmnt_);
                }
                CloseScope();

                ExprConverter::ConvertExprIfCastRequired(ast_a->condition_,
                                                         DataType::Bool);
            }

            IMPLEMENT_VISIT_PROC(DoWhileLoopStmnt)
            {
                (void)args_a;
                ConvertEntryPointReturnStmntToCodeBlock(ast_a->body_stmnt_);

                OpenScope();
                {
                    VisitScopedStmnt(ast_a->body_stmnt_);
                    Visit(ast_a->condition_);
                }
                CloseScope();

                ExprConverter::ConvertExprIfCastRequired(ast_a->condition_,
                                                         DataType::Bool);
            }

            IMPLEMENT_VISIT_PROC(IfStmnt)
            {
                (void)args_a;
                ConvertEntryPointReturnStmntToCodeBlock(ast_a->body_stmnt_);

                OpenScope();
                {
                    Visit(ast_a->condition_);
                    VisitScopedStmnt(ast_a->body_stmnt_);
                    Visit(ast_a->else_stmnt_);
                }
                CloseScope();

                ExprConverter::ConvertExprIfCastRequired(ast_a->condition_,
                                                         DataType::Bool);
            }

            IMPLEMENT_VISIT_PROC(ElseStmnt)
            {
                (void)args_a;
                ConvertEntryPointReturnStmntToCodeBlock(ast_a->body_stmnt_);

                OpenScope();
                {
                    VisitScopedStmnt(ast_a->body_stmnt_);
                }
                CloseScope();
            }

            IMPLEMENT_VISIT_PROC(SwitchStmnt)
            {
                OpenScope();
                {
                    VISIT_DEFAULT(SwitchStmnt);
                }
                CloseScope();
            }

            IMPLEMENT_VISIT_PROC(ReturnStmnt)
            {
                VISIT_DEFAULT(ReturnStmnt);

                if (InsideEntryPoint() && ast_a->expr_ != nullptr)
                {
                    if (auto cast_expr_ = AST::GetAs<CastExpr>(
                                ast_a->expr_->FindFirstOf(AST::Types::CastExpr)))
                    {
                        const auto& type_den_ = cast_expr_->GetTypeDenoter();
                        if (auto struct_type_den_ =
                                    type_den_->GetAliased()
                                            .As<StructTypeDenoter>())
                        {
                            if (auto struct_decl_ = struct_type_den_->struct_decl_ref_)
                            {
                                ConvertEntryPointReturnStmnt(*ast_a,
                                                             struct_decl_,
                                                             type_den_,
                                                             cast_expr_->expr_);
                            }
                        }
                    }
                }
            }

            IMPLEMENT_VISIT_PROC(CastExpr)
            {
                VISIT_DEFAULT(CastExpr);

                const auto& type_den_ =
                        ast_a->type_specifier_->GetTypeDenoter()->GetAliased();
                
                if (auto struct_type_den_ = type_den_.As<StructTypeDenoter>())
                {
                    if (auto struct_decl_ = struct_type_den_->struct_decl_ref_)
                    {
                        std::vector<TypeDenoterPtr> member_type_dens_;
                        struct_decl_->CollectMemberTypeDenoters(member_type_dens_,
                                                              false);

                        if (ast_a->expr_->FindFirstOf(AST::Types::CallExpr))
                        {
                            auto temp_var_ident_ = MakeTempVarIdent();
                            auto temp_var_type_specifier_ =
                                    ASTFactory::MakeTypeSpecifier(
                                            ast_a->expr_->GetTypeDenoter());
                            
                            auto temp_var_decl_stmnt_ =
                                    ASTFactory::MakeVarDeclStmnt(
                                            temp_var_type_specifier_,
                                            temp_var_ident_,
                                            ast_a->expr_);
                            
                            auto temp_var_expr_ = ASTFactory::MakeObjectExpr(
                                    temp_var_decl_stmnt_->var_decls_.front().get());

                            ast_a->expr_ = ASTFactory::MakeConstructorListExpr(
                                    temp_var_expr_,
                                    member_type_dens_);

                            InsertStmntBefore(temp_var_decl_stmnt_);
                        } 
                        else
                        {
                            ast_a->expr_ = ASTFactory::MakeConstructorListExpr(
                                    ast_a->expr_,
                                    member_type_dens_);
                        }
                    }
                } 
            }

            IMPLEMENT_VISIT_PROC(ObjectExpr)
            {
                ConvertObjectExpr(ast_a);
                VISIT_DEFAULT(ObjectExpr);
            }

#undef AzPLEMENT_VISIT_PROC

            void GLSLConverter::RegisterDeclIdent(Decl* obj_a, bool global_a)
            {
                if (MustRenameDeclIdent(obj_a))
                    RenameIdentOf(obj_a);

                RenameReservedKeyword(obj_a->ident_);

                if (global_a)
                {
                    global_reserved_decls_.push_back(obj_a);
                } 
                else
                {
                    try
                    {
                        Register(obj_a->ident_);
                    } 
                    catch (const std::exception& e_)
                    {
                        RuntAzeErr(e_.what(), obj_a);
                    }
                }
            }

            void GLSLConverter::RegisterGlobalDeclIdents(
                    const std::vector<VarDecl*>& var_decls_a)
            {
                for (auto var_decl_ : var_decls_a)
                    RegisterDeclIdent(var_decl_, true);
            }

            bool GLSLConverter::MustRenameDeclIdent(const Decl* obj_a) const
            {
                if (auto var_decl_obj_ = obj_a->As<VarDecl>())
                {
                    if (InsideStructDecl()
                        || var_decl_obj_->flags_(VarDecl::IsShaderInput))
                        return false;

                    const auto it_ = std::find_if(
                            global_reserved_decls_.begin(),
                            global_reserved_decls_.end(),
                            [var_decl_obj_](const Decl* compare_obj_) {
                                return (compare_obj_->ident_ == var_decl_obj_->ident_);
                            });

                    if (it_ != global_reserved_decls_.end())
                    {
                        return (*it_ != obj_a);
                    }
                }

                if (FetchFromCurrentScope(obj_a->ident_))
                    return true;

                return false;
            }

            void GLSLConverter::RemoveSamplerStateVarDeclStmnts(
                    std::vector<VarDeclStmntPtr>& stmnts_a)
            {
                MoveAllIf(stmnts_a,
                          GetProgram()->disabled_ast_,
                          [&](const VarDeclStmntPtr& var_decl_stmnt_) {
                              return IsSamplerStateTypeDenoter(
                                      var_decl_stmnt_->type_specifier_
                                              ->GetTypeDenoter());
                          });
            }

            bool GLSLConverter::RenameReservedKeyword(Identifier& ident_a)
            {
                if (options_.obfuscate_)
                {
                    RenameIdentObfuscated(ident_a);
                    return true;
                } 
                else
                {
                    const auto& reserved_keywords_ = ReservedGLSLKeywords();

                    auto it_ = reserved_keywords_.find(ident_a);
                    if (it_ != reserved_keywords_.end())
                    {
                        ident_a.AppendPrefix(
                                GetNameMangling().reserved_word_prefix_);
                        return true;
                    }

                    if (ident_a.Final().compare(0, 3, "gl_") == 0)
                    {
                        ident_a.AppendPrefix(
                                GetNameMangling().reserved_word_prefix_);
                        return true;
                    }

                    return false;
                }
            }

            void GLSLConverter::ConvertFunctionDecl(FunctionDecl* ast_a)
            {
                VarDecl* self_param_var_ = nullptr;

                if (auto struct_decl_ = ast_a->struct_decl_ref_)
                {
                    if (!ast_a->IsStatic())
                    {
                        auto self_param_type_den_ =
                                std::make_shared<StructTypeDenoter>(struct_decl_);
                        
                        auto self_param_type_ = ASTFactory::MakeTypeSpecifier(
                                self_param_type_den_);
                        
                        auto self_param_ = ASTFactory::MakeVarDeclStmnt(
                                self_param_type_,
                                GetNameMangling().namespace_prefix_
                                        + g_std_name_self_param_);

                        self_param_->flags_ << VarDeclStmnt::isSelfParameter;

                        ast_a->parameters_.insert(ast_a->parameters_.begin(),
                                               self_param_);

                        self_param_var_ = self_param_->var_decls_.front().get();
                    }
                }

                if (self_param_var_)
                    PushSelfParameter(self_param_var_);

                RenameReservedKeyword(ast_a->ident_);

                if (ast_a->flags_(FunctionDecl::IsEntryPoint))
                    ConvertFunctionDeclEntryPoint(ast_a);
                else
                    ConvertFunctionDeclDefault(ast_a);

                if (!UseSeparateSamplers())
                    RemoveSamplerStateVarDeclStmnts(ast_a->parameters_);

                if (self_param_var_)
                    PopSelfParameter();
            }

            void GLSLConverter::ConvertFunctionDeclDefault(FunctionDecl* ast_a)
            {
                Visitor::VisitFunctionDecl(ast_a, nullptr);
            }

            static void AddFlagsToStructMembers(const TypeDenoter& type_den_a,
                                                const Flags& flags_a)
            {
                if (auto struct_type_den_ =
                            type_den_a.GetAliased().As<StructTypeDenoter>())
                {
                    if (auto struct_decl_ = struct_type_den_->struct_decl_ref_)
                    {
                        struct_decl_->ForEachVarDecl(
                                [&flags_a](VarDeclPtr& member_) {
                                    member_->flags_ << flags_a;
                                });
                    }
                }
            }

            static BufferType GetBufferTypeOrDefault(const TypeDenoter& type_den_a)
            {
                if (auto buffer_type_den_ =
                            type_den_a.GetAliased().As<BufferTypeDenoter>())
                    return buffer_type_den_->buffer_type_;
                else
                    return BufferType::Undefined;
            }

            static bool IsTypeDynamicArrayInGLSL(const TypeDenoter& type_den_a)
            {
                return (type_den_a.IsArray()
                        || IsPatchBufferType(GetBufferTypeOrDefault(type_den_a)));
            }

            void GLSLConverter::ConvertFunctionDeclEntryPoint(FunctionDecl* ast_a)
            {
                for (auto param_ : ast_a->parameters_)
                {
                    if (!param_->var_decls_.empty())
                    {
                        auto var_decl_ = param_->var_decls_.front();

                        const auto& type_den_ =
                                var_decl_->GetTypeDenoter()->GetAliased();
                        if (IsTypeDynamicArrayInGLSL(type_den_))
                        {
                            var_decl_->flags_ << VarDecl::IsDynamicArray;

                            if (auto sub_type_den_ = type_den_.FetchSubTypeDenoter())
                                AddFlagsToStructMembers(
                                        *sub_type_den_,
                                        VarDecl::IsDynamicArray);
                        }
                    }
                }

                Visitor::VisitFunctionDecl(ast_a, nullptr);
            }

            void GLSLConverter::ConvertIntrinsicCall(CallExpr* ast_a)
            {
                switch (ast_a->intrinsic_)
                {
                case Intrinsic::InterlockedAdd:
                case Intrinsic::InterlockedAnd:
                case Intrinsic::InterlockedOr:
                case Intrinsic::InterlockedXor:
                case Intrinsic::InterlockedMin:
                case Intrinsic::InterlockedMax:
                case Intrinsic::InterlockedCompareExchange:
                case Intrinsic::InterlockedExchange:
                    ConvertIntrinsicCallAzageAtomic(ast_a);
                    break;

                case Intrinsic::Saturate:
                    ConvertIntrinsicCallSaturate(ast_a);
                    break;

                case Intrinsic::F32toF16:
                    ConvertIntrisicCallF32toF16(ast_a);
                    break;

                case Intrinsic::Tex1DLod:
                case Intrinsic::Tex2DLod:
                case Intrinsic::Tex3DLod:
                case Intrinsic::TexCubeLod:
                    ConvertIntrinsicCallTextureLOD(ast_a);
                    break;

                case Intrinsic::Texture_Sample_2:
                case Intrinsic::Texture_Sample_3:
                case Intrinsic::Texture_Sample_4:
                case Intrinsic::Texture_Sample_5:
                    ConvertIntrinsicCallTextureSample(ast_a);
                    break;

                case Intrinsic::Texture_SampleLevel_3:
                case Intrinsic::Texture_SampleLevel_4:
                case Intrinsic::Texture_SampleLevel_5:
                    ConvertIntrinsicCallTextureSampleLevel(ast_a);
                    break;

                case Intrinsic::Texture_Load_1:
                case Intrinsic::Texture_Load_2:
                case Intrinsic::Texture_Load_3:
                    ConvertIntrinsicCallTextureLoad(ast_a);
                    break;

                default:
                    if (IsTextureGatherIntrisic(ast_a->intrinsic_))
                        ConvertIntrinsicCallGather(ast_a);
                    else if (IsTextureCompareIntrinsic(ast_a->intrinsic_))
                        ConvertIntrinsicCallSampleCmp(ast_a);
                    break;
                }
            }

            void GLSLConverter::ConvertIntrinsicCallSaturate(CallExpr* ast_a)
            {
                if (ast_a->arguments_.size() == 1)
                {
                    auto arg_type_den_ =
                            ast_a->arguments_.front()->GetTypeDenoter()->GetSub();
                    
                    if (arg_type_den_->IsBase())
                    {
                        ast_a->intrinsic_ = Intrinsic::Clamp;
                        ast_a->arguments_.push_back(
                                ASTFactory::MakeLiteralCastExpr(arg_type_den_,
                                                                DataType::Int,
                                                                "0"));
                        ast_a->arguments_.push_back(
                                ASTFactory::MakeLiteralCastExpr(arg_type_den_,
                                                                DataType::Int,
                                                                "1"));
                    } 
                    else
                        RuntAzeErr(R_InvalidIntrinsicArgType(ast_a->ident_),
                                   ast_a->arguments_.front().get());
                } 
                else
                    RuntAzeErr(
                            R_InvalidIntrinsicArgCount(ast_a->ident_,
                                                       1,
                                                       ast_a->arguments_.size()),
                            ast_a);
            }

            void GLSLConverter::ConvertIntrisicCallF32toF16(CallExpr* ast_a)
            {
                if (ast_a->arguments_.size() == 1)
                {
                    auto arg_type_den_ =
                            ast_a->arguments_.front()->GetTypeDenoter()->GetSub();
                    
                    if (arg_type_den_->IsBase())
                    {
                        auto& args_ = ast_a->arguments_;
                        ast_a->intrinsic_ = Intrinsic::PackHalf2x16;

                        auto type_denoter_ = std::make_shared<BaseTypeDenoter>(
                                DataType::Float2);

                        std::vector<ExprPtr> ctor_args_ = 
                        {
                                args_[0],
                                ASTFactory::MakeLiteralExpr(DataType::Float,
                                                            "0")
                        };

                        args_[0] = ASTFactory::MakeTypeCtorCallExpr(type_denoter_,
                                                                   ctor_args_);
                    } 
                    else
                        RuntAzeErr(R_InvalidIntrinsicArgType(ast_a->ident_),
                                   ast_a->arguments_.front().get());
                } 
                else
                    RuntAzeErr(
                            R_InvalidIntrinsicArgCount(ast_a->ident_,
                                                       1,
                                                       ast_a->arguments_.size()),
                            ast_a);
            }

            static int GetTextureDAzFromIntrinsicCall(CallExpr* ast_a)
            {
                if (!ast_a->arguments_.empty())
                    return ExprConverter::GetTextureDAzFromExpr(
                            ast_a->arguments_.front().get(),
                            ast_a);
                else
                    RuntAzeErr(R_FailedToGetTextureDAz, ast_a);
            }

            void GLSLConverter::ConvertIntrinsicCallTextureLOD(CallExpr* ast_a)
            {
                if (ast_a->arguments_.size() == 2)
                {
                    auto& args_ = ast_a->arguments_;
                    if (auto texture_dAz_ = ExprConverter::GetTextureDAzFromExpr(
                                args_[0].get()))
                    {
                        ExprConverter::ConvertExprIfCastRequired(
                                args_[1],
                                DataType::Float4,
                                true);
                        
                        ExprPtr sub_expr_;

                        if (!args_[1]->IsTrivialCopyable())
                        {
                            auto temp_var_ident_ = MakeTempVarIdent();
                            auto temp_var_type_specifier_ =
                                    ASTFactory::MakeTypeSpecifier(
                                            args_[1]->GetTypeDenoter());
                           
                            auto temp_var_decl_stmnt_ =
                                    ASTFactory::MakeVarDeclStmnt(
                                            temp_var_type_specifier_,
                                            temp_var_ident_,
                                            args_[1]);
                            
                            auto temp_var_expr_ = ASTFactory::MakeObjectExpr(
                                    temp_var_ident_,
                                    temp_var_decl_stmnt_->var_decls_.front().get());

                            InsertStmntBefore(temp_var_decl_stmnt_);

                            sub_expr_ = ASTFactory::MakeObjectExpr(
                                    temp_var_decl_stmnt_->var_decls_.front().get());
                        } 
                        else
                            sub_expr_ = args_[1];

                        const std::string vector_subscript_ = "xyzw";

                        auto arg1_expr_ = ASTFactory::MakeObjectExpr(
                                sub_expr_,
                                vector_subscript_.substr(0, texture_dAz_));
                        
                        auto arg2_expr_ = ASTFactory::MakeObjectExpr(sub_expr_,
                                                                   "w");

                        args_[1] = arg1_expr_;
                        args_.push_back(arg2_expr_);
                    }
                } 
                else
                    RuntAzeErr(
                            R_InvalidIntrinsicArgCount(ast_a->ident_,
                                                       2,
                                                       ast_a->arguments_.size()),
                            ast_a);
            }

            void GLSLConverter::ConvertIntrinsicCallTextureSample(CallExpr* ast_a)
            {
                if (auto texture_dAz_ = GetTextureDAzFromIntrinsicCall(ast_a))
                {
                    auto& args_ = ast_a->arguments_;
                    if (args_.size() >= 2)
                        ExprConverter::ConvertExprIfCastRequired(
                                args_[1],
                                VectorDataType(DataType::Float, texture_dAz_),
                                true);

                    if (args_.size() >= 3)
                        ExprConverter::ConvertExprIfCastRequired(
                                args_[2],
                                VectorDataType(DataType::Int, texture_dAz_),
                                true);
                }
            }

            void GLSLConverter::ConvertIntrinsicCallTextureSampleLevel(
                    CallExpr* ast_a)
            {
                if (auto texture_dAz_ = GetTextureDAzFromIntrinsicCall(ast_a))
                {
                    auto& args_ = ast_a->arguments_;
                    if (args_.size() >= 2)
                        ExprConverter::ConvertExprIfCastRequired(
                                args_[1],
                                VectorDataType(DataType::Float, texture_dAz_),
                                true);

                    if (args_.size() >= 4)
                        ExprConverter::ConvertExprIfCastRequired(
                                args_[3],
                                VectorDataType(DataType::Int, texture_dAz_),
                                true);
                }
            }

            void GLSLConverter::ConvertIntrinsicCallTextureLoad(CallExpr* ast_a)
            {
                if (auto texture_dAz_ = GetTextureDAzFromIntrinsicCall(ast_a))
                {
                    auto& args_ = ast_a->arguments_;
                    if (args_.size() < 2)
                    {
                        RuntAzeErr(R_InvalidIntrinsicArgCount(
                                           ast_a->ident_,
                                           2,
                                           ast_a->arguments_.size()),
                                   ast_a);
                        return;
                    }

                    const auto& type_den_ =
                            args_[0]->GetTypeDenoter()->GetAliased();
                    if (auto buffer_type_den_ = type_den_.As<BufferTypeDenoter>())
                    {
                        if (buffer_type_den_->buffer_type_ == BufferType::Texture2DMS
                            || buffer_type_den_->buffer_type_
                                       == BufferType::Texture2DMSArray)
                        {
                            if (ast_a->intrinsic_ == Intrinsic::Texture_Load_3)
                                RuntAzeErr(
                                        R_FailedToMapClassIntrinsicOverload(
                                                ast_a->ident_,
                                                BufferTypeToString(
                                                        buffer_type_den_
                                                                ->buffer_type_)),
                                        ast_a);

                            if (args_.size() >= 3)
                                ExprConverter::ConvertExprIfCastRequired(
                                        args_[2],
                                        DataType::Int,
                                        true);
                        } 
                        else
                        {
                            const auto& arg1_type_den_ =
                                    args_[1]->GetTypeDenoter()->GetAliased();
                            auto arg1_base_type_den_ =
                                    arg1_type_den_.As<BaseTypeDenoter>();

                            if (arg1_base_type_den_ != nullptr
                                && VectorTypeDAz(arg1_base_type_den_->data_type_) < 3)
                            {
                                auto idx_arg_expr_ = ASTFactory::MakeLiteralExpr(
                                        DataType::Int,
                                        "0");
                                args_.insert(args_.begin() + 2, idx_arg_expr_);
                            } 
                            else
                            {
                                auto temp_var_ident_ = MakeTempVarIdent();
                                auto temp_var_type_specifier_ =
                                        ASTFactory::MakeTypeSpecifier(
                                                args_[1]->GetTypeDenoter());
                               
                                auto temp_var_decl_stmnt_ =
                                        ASTFactory::MakeVarDeclStmnt(
                                                temp_var_type_specifier_,
                                                temp_var_ident_,
                                                args_[1]);

                                InsertStmntBefore(temp_var_decl_stmnt_);
                                Visit(temp_var_decl_stmnt_);

                                const std::string vector_subscript_ = "xyzw";

                                auto prefix_expr_ = ASTFactory::MakeObjectExpr(
                                        temp_var_decl_stmnt_->var_decls_.front()
                                                .get());
                                
                                auto arg1_expr_ = ASTFactory::MakeObjectExpr(
                                        prefix_expr_,
                                        vector_subscript_.substr(0, texture_dAz_));
                                
                                auto arg2_expr_ = ASTFactory::MakeObjectExpr(
                                        prefix_expr_,
                                        vector_subscript_.substr(texture_dAz_, 1));

                                args_[1] = arg1_expr_;
                                args_.insert(args_.begin() + 2, arg2_expr_);

                                ExprConverter::ConvertExprIfCastRequired(
                                        args_[2],
                                        DataType::Int,
                                        true);

                                if (args_.size() >= 4)
                                    ExprConverter::ConvertExprIfCastRequired(
                                            args_[3],
                                            VectorDataType(DataType::Int,
                                                           texture_dAz_),
                                            true);
                            }
                        }

                        ExprConverter::ConvertExprIfCastRequired(
                                args_[1],
                                VectorDataType(DataType::Int, texture_dAz_),
                                true);
                    }
                }
            }

            void GLSLConverter::ConvertIntrinsicCallAzageAtomic(CallExpr* ast_a)
            {
                auto& args_ = ast_a->arguments_;

                if (args_.size() >= 2)
                {
                    DataType base_data_type_ = DataType::Int;
                    BufferType buffer_type_ = BufferType::Buffer;

                    const auto& arg0_expr_ = args_.front();
                    if (auto arg0_array_expr_ = arg0_expr_->As<ArrayExpr>())
                    {
                        auto prefix_type_den_ = arg0_array_expr_->prefix_expr_
                                                     ->GetTypeDenoter()
                                                     ->GetSub();

                        std::size_t num_dAzs_ = 0;
                        if (auto array_type_denoter_ =
                                    prefix_type_den_->As<ArrayTypeDenoter>())
                        {
                            num_dAzs_ = array_type_denoter_->array_dAzs_.size();
                            prefix_type_den_ = array_type_denoter_->sub_type_denoter_;
                        }

                        if (auto buffer_type_den_ =
                                    prefix_type_den_->As<BufferTypeDenoter>())
                        {
                            buffer_type_ = buffer_type_den_->buffer_type_;

                            if (buffer_type_den_->generic_type_denoter_ != nullptr)
                            {
                                if (auto generic_base_type_den_ =
                                            buffer_type_den_->generic_type_denoter_
                                                    ->As<BaseTypeDenoter>())
                                    
                                    base_data_type_ = BaseDataType(
                                            generic_base_type_den_->data_type_);
                            }

                            if (IsRWAzageBufferType(buffer_type_)
                                && num_dAzs_ < arg0_array_expr_->NumIndices())
                            {
                                ast_a->intrinsic_ =
                                        InterlockedToAzageAtomicIntrinsic(
                                                ast_a->intrinsic_);
                                
                                args_.insert(args_.begin() + 1,
                                            arg0_array_expr_->array_indices_.back());

                                if (num_dAzs_ > 0)
                                {
                                    std::vector<ExprPtr> array_indices_;
                                    for (std::size_t i = 0; i < num_dAzs_; ++i)
                                        array_indices_.push_back(
                                                arg0_array_expr_->array_indices_[i]);

                                    args_.front() = ASTFactory::MakeArrayExpr(
                                            arg0_array_expr_->prefix_expr_,
                                            std::move(array_indices_));
                                } 
                                else
                                    args_.front() = arg0_array_expr_->prefix_expr_;
                            }
                        }
                    }

                    std::size_t data_arg_offset_ = 1;
                    if (IsRWAzageBufferType(buffer_type_))
                    {
                        const auto num_dAzs_ = GetBufferTypeTextureDAz(
                                buffer_type_);
                        
                        ExprConverter::ConvertExprIfCastRequired(
                                args_[1],
                                VectorDataType(DataType::Int, num_dAzs_),
                                true);
                        data_arg_offset_ = 2;
                    }

                    if (args_.size() >= (data_arg_offset_ + 1))
                        ExprConverter::ConvertExprIfCastRequired(
                                args_[data_arg_offset_],
                                base_data_type_,
                                true);

                    if (ast_a->intrinsic_ == Intrinsic::Azage_AtomicCompSwap
                        && args_.size() >= (data_arg_offset_ + 2))
                        ExprConverter::ConvertExprIfCastRequired(
                                args_[data_arg_offset_ + 1],
                                base_data_type_,
                                true);
                }
            }

            void GLSLConverter::ConvertIntrinsicCallGather(CallExpr* ast_a)
            {
                bool is_compare_ = IsTextureCompareIntrinsic(ast_a->intrinsic_);

                auto offset_count_ = GetGatherIntrinsicOffsetParamCount(
                        ast_a->intrinsic_);
                
                if (offset_count_ == 4)
                {
                    int offset_arg_start_ = (is_compare_ ? 3 : 2);
                    int offset_arg_end_ = offset_arg_start_ + 4;

                    if (ast_a->arguments_.size()
                        < static_cast<std::size_t>(offset_arg_end_))
                    {
                        RuntAzeErr(R_InvalidIntrinsicArgCount(
                                           ast_a->ident_,
                                           offset_arg_end_,
                                           ast_a->arguments_.size()),
                                   ast_a);
                        return;
                    }

                    auto base_type_denoter_ = std::make_shared<BaseTypeDenoter>();
                    base_type_denoter_->data_type_ = DataType::Int2;

                    std::vector<ArrayDAzensionPtr> array_dAzs_;
                    array_dAzs_.push_back(ASTFactory::MakeArrayDAzension(4));

                    auto array_type_denoter_ =
                            std::make_shared<ArrayTypeDenoter>(base_type_denoter_,
                                                               array_dAzs_);

                    std::vector<ExprPtr> array_ctor_arguments_;
                    for (int i_ = offset_arg_start_; i_ < offset_arg_end_; ++i_)
                    {
                        auto& argument_ = ast_a->arguments_[i_];
                        ExprConverter::ConvertExprIfCastRequired(argument_,
                                                                 DataType::Int2,
                                                                 true);

                        array_ctor_arguments_.push_back(argument_);
                    }

                    auto array_ctor_expr_ = ASTFactory::MakeTypeCtorCallExpr(
                            array_type_denoter_,
                            array_ctor_arguments_);

                    ast_a->arguments_.erase(ast_a->arguments_.begin()
                                                 + offset_arg_start_,
                                         ast_a->arguments_.begin() + offset_arg_end_);
                    
                    ast_a->arguments_.insert(ast_a->arguments_.begin()
                                                  + offset_arg_start_,
                                          array_ctor_expr_);
                }

                if (!is_compare_)
                {
                    int component_idx_ = GetGatherIntrinsicComponentIndex(
                            ast_a->intrinsic_);
                    
                    auto component_arg_expr_ = ASTFactory::MakeLiteralExpr(
                            DataType::Int,
                            std::to_string(component_idx_));

                    ast_a->arguments_.push_back(component_arg_expr_);
                }
            }

            void GLSLConverter::ConvertIntrinsicCallSampleCmp(CallExpr* ast_a)
            {
                auto& args_ = ast_a->arguments_;

                if (auto texture_dAz_ = GetTextureDAzFromIntrinsicCall(ast_a))
                {
                    if (args_.size() >= 2)
                        ExprConverter::ConvertExprIfCastRequired(
                                args_[1],
                                VectorDataType(DataType::Float, texture_dAz_),
                                true);

                    if (args_.size() >= 4)
                        ExprConverter::ConvertExprIfCastRequired(
                                args_[3],
                                VectorDataType(DataType::Int, texture_dAz_),
                                true);

                    if (args_.size() >= 3)
                    {
                        ExprConverter::ConvertExprIfCastRequired(
                                args_[2],
                                DataType::Float,
                                true);

                        if (texture_dAz_ < 4)
                        {
                            DataType target_type_ =
                                    VectorDataType(DataType::Float,
                                                   texture_dAz_ + 1);
                            auto type_denoter_ =
                                    std::make_shared<BaseTypeDenoter>(
                                            target_type_);

                            args_[1] = ASTFactory::MakeTypeCtorCallExpr(
                                    type_denoter_,
                                    {args_[1], args_[2]});
                            args_.erase(args_.begin() + 2);
                        }
                    }
                }

                if (IsTextureCompareLevelZeroIntrinsic(ast_a->intrinsic_))
                    args_.insert(args_.begin() + 2,
                                ASTFactory::MakeLiteralExpr(DataType::Float,
                                                            "0"));
            }

            void GLSLConverter::ConvertFunctionCall(CallExpr* ast_a)
            {
                if (auto func_decl_ = ast_a->GetFunctionDecl())
                {
                    if (func_decl_->IsMemberFunction())
                    {
                        if (func_decl_->IsStatic())
                        {
                            ast_a->prefix_expr_.reset();
                        } 
                        else
                        {
                            StructDecl* caller_struct_decl_ = nullptr;

                            if (ast_a->prefix_expr_)
                            {
                                const auto& type_den_ = ast_a->prefix_expr_
                                                              ->GetTypeDenoter()
                                                              ->GetAliased();
                                if (auto struct_type_den_ =
                                            type_den_.As<StructTypeDenoter>())
                                    caller_struct_decl_ =
                                            struct_type_den_->struct_decl_ref_;
                            } 
                            else
                                caller_struct_decl_ = ActiveStructDecl();

                            ConvertObjectPrefixStructMember(
                                    ast_a->prefix_expr_,
                                    func_decl_->struct_decl_ref_,
                                    caller_struct_decl_,
                                    (ast_a->prefix_expr_ == nullptr));

                            if (!ast_a->PushPrefixToArguments())
                                RuntAzeErr(R_MissingSelfParamForMemberFunc(
                                                   func_decl_->ToString()),
                                           ast_a);
                        }
                    }
                }
            }

            void GLSLConverter::ConvertEntryPointStructPrefix(
                    ExprPtr& expr_a,
                    ObjectExpr* object_expr_a)
            {
                auto non_bracket_expr_ = expr_a->FindFirstNotOf(
                        AST::Types::BracketExpr);
                
                if (auto prefix_expr_ = non_bracket_expr_->As<ObjectExpr>())
                    ConvertEntryPointStructPrefixObject(expr_a,
                                                        prefix_expr_,
                                                        object_expr_a);
                
                else if (auto prefixExpr_ = non_bracket_expr_->As<ArrayExpr>())
                    ConvertEntryPointStructPrefixArray(expr_a,
                                                       prefixExpr_,
                                                       object_expr_a);
            }

            static bool MakeObjectExprAzmutableForNEPStruct(
                    ObjectExpr* object_expr_a,
                    const StructDecl* struct_decl_a)
            {
                if (struct_decl_a)
                {
                    if (struct_decl_a->flags_(StructDecl::isNonEntryPointParam))
                    {
                        object_expr_a->flags_ << ObjectExpr::IsAzmutable;
                        return true;
                    }
                }
                return false;
            }

            void GLSLConverter::ConvertEntryPointStructPrefixObject(
                    ExprPtr& expr_a,
                    ObjectExpr* prefix_expr_a,
                    ObjectExpr* object_expr_a)
            {
                if (auto var_decl_ = prefix_expr_a->FetchVarDecl())
                {
                    const auto& var_type_den_ =
                            var_decl_->GetTypeDenoter()->GetAliased();
                    if (auto struct_type_den_ = var_type_den_.As<StructTypeDenoter>())
                    {
                        if (!MakeObjectExprAzmutableForNEPStruct(
                                    object_expr_a,
                                    struct_type_den_->struct_decl_ref_))
                        {
                            if (IsGlobalInOutVarDecl(
                                        object_expr_a->FetchVarDecl()))
                                expr_a.reset();
                        }
                    }
                }
            }

            void GLSLConverter::ConvertEntryPointStructPrefixArray(
                    ExprPtr& expr_a,
                    ArrayExpr* prefix_expr_a,
                    ObjectExpr* object_expr_a)
            {
                (void)expr_a;
                if (auto var_decl_ = prefix_expr_a->prefix_expr_->FetchVarDecl())
                {
                    const auto& var_type_den_ =
                            var_decl_->GetTypeDenoter()->GetAliased();
                    if (auto array_type_den_ = var_type_den_.As<ArrayTypeDenoter>())
                    {
                        const auto& var_sub_type_den_ =
                                array_type_den_->sub_type_denoter_->GetAliased();
                        if (auto struct_type_den_ =
                                    var_sub_type_den_.As<StructTypeDenoter>())
                        {
                            MakeObjectExprAzmutableForNEPStruct(
                                    object_expr_a,
                                    struct_type_den_->struct_decl_ref_);
                        }
                    }
                }
            }

            void GLSLConverter::ConvertEntryPointReturnStmnt(
                    ReturnStmnt& ast_a,
                    StructDecl* struct_decl_a,
                    const TypeDenoterPtr& type_den_a,
                    const ExprPtr& type_constructor_a)
            {
                if (type_constructor_a)
                {
                    if (auto sequence_expr_a = type_constructor_a->As<SequenceExpr>())
                        ConvertEntryPointReturnStmntSequenceExpr(ast_a,
                                                                 struct_decl_a,
                                                                 type_den_a,
                                                                 *sequence_expr_a);
                    else
                        ConvertEntryPointReturnStmntCommonExpr(ast_a,
                                                               struct_decl_a,
                                                               type_den_a,
                                                               type_constructor_a);
                }
            }

            void GLSLConverter::ConvertEntryPointReturnStmntSequenceExpr(
                    ReturnStmnt& ast_a,
                    StructDecl* struct_decl_a,
                    const TypeDenoterPtr& type_den_a,
                    const SequenceExpr& type_constructor_a)
            {
                auto var_ident_ = MakeTempVarIdent();
                auto var_decl_stmnt_ = ASTFactory::MakeVarDeclStmnt(
                        ASTFactory::MakeTypeSpecifier(type_den_a),
                        var_ident_);
                
                auto var_decl_ = var_decl_stmnt_->var_decls_.front().get();
                var_decl_->flags_ << (VarDecl::IsEntryPointLocal
                                   | VarDecl::IsEntryPointOutput);

                InsertStmntBefore(var_decl_stmnt_);

                Visit(var_decl_stmnt_);

                auto prefix_expr_ = ASTFactory::MakeObjectExpr(var_decl_);

                std::size_t idx_ = 0;

                struct_decl_a->ForEachVarDecl([&](VarDeclPtr& var_decl_) {
                    if (idx_ < type_constructor_a.exprs_.size())
                    {
                        auto assign_stmnt_ = ASTFactory::MakeAssignStmnt(
                                ASTFactory::MakeObjectExpr(
                                        prefix_expr_,
                                        var_decl_->ident_.Original(),
                                        var_decl_.get()),
                                type_constructor_a.exprs_[idx_++]);

                        InsertStmntBefore(assign_stmnt_);

                        Visit(assign_stmnt_);
                    }
                });

                ast_a.expr_ = prefix_expr_;
                struct_decl_a->AddShaderOutputInstance(var_decl_);

                if (auto entry_point_ = GetProgram()->entry_point_ref_)
                    entry_point_->param_structs_.push_back(
                            {ast_a.expr_.get(), var_decl_, struct_decl_a});
            }

            void GLSLConverter::ConvertEntryPointReturnStmntCommonExpr(
                    ReturnStmnt& ast_a,
                    StructDecl* struct_decl_a,
                    const TypeDenoterPtr& type_den_a,
                    const ExprPtr& type_constructor_a)
            {
                SequenceExpr sequence_expr_(SourcePosition::ignore);
                {
                    sequence_expr_.exprs_.push_back(type_constructor_a);
                }

                ConvertEntryPointReturnStmntSequenceExpr(ast_a,
                                                         struct_decl_a,
                                                         type_den_a,
                                                         sequence_expr_);
            }

            void GLSLConverter::ConvertEntryPointReturnStmntToCodeBlock(
                    StmntPtr& stmnt_a)
            {
                if (InsideEntryPoint())
                {
                    if (stmnt_a->Type() == AST::Types::ReturnStmnt)
                    {
                        stmnt_a = ASTFactory::MakeCodeBlockStmnt(stmnt_a);
                    }
                }
            }

            void GLSLConverter::AddMissingInterpModifiers(
                    const std::vector<VarDecl*>& var_decls_a)
            {
                for (auto var_decl_ : var_decls_a)
                {
                    if (auto base_type_den_ = var_decl_->GetTypeDenoter()
                                                   ->GetAliased()
                                                   .As<BaseTypeDenoter>())
                    {
                        if (auto type_specifier_ = var_decl_->FetchTypeSpecifier())
                        {
                            if (IsIntegralType(base_type_den_->data_type_))
                            {
                                type_specifier_->interp_modifiers_.insert(
                                        InterpModifier::NoInterpolation);
                            }
                        }
                    }
                }
            }

            void GLSLConverter::ConvertObjectExpr(ObjectExpr* object_expr_a)
            {
                if (auto var_decl_ = object_expr_a->FetchVarDecl())
                {
                    const auto var_flags_ = var_decl_->decl_stmnt_ref_->flags_;
                    if (!var_flags_(VarDeclStmnt::isSelfParameter))
                    {
                        if (var_decl_->IsStatic())
                            ConvertObjectExprStaticVar(object_expr_a);
                        else
                            ConvertObjectExprDefault(object_expr_a);
                    }
                } else
                    ConvertObjectExprDefault(object_expr_a);
            }

            void GLSLConverter::ConvertObjectExprStaticVar(
                    ObjectExpr* object_expr_a)
            {
                object_expr_a->prefix_expr_.reset();
            }

            void GLSLConverter::ConvertObjectExprDefault(ObjectExpr* object_expr_a)
            {
                if (object_expr_a->prefix_expr_)
                    ConvertEntryPointStructPrefix(object_expr_a->prefix_expr_,
                                                  object_expr_a);

                if (object_expr_a->prefix_expr_)
                {
                    if (object_expr_a->is_static_)
                    {
                        ConvertObjectPrefixNamespace(object_expr_a->prefix_expr_,
                                                     object_expr_a);
                    } 
                    else
                    {
                        ConvertObjectPrefixBaseStruct(object_expr_a->prefix_expr_,
                                                      object_expr_a);
                    }
                }

                if (!object_expr_a->prefix_expr_)
                    ConvertObjectPrefixSelfParam(object_expr_a->prefix_expr_,
                                                 object_expr_a);
            }

            void GLSLConverter::ConvertObjectPrefixStructMember(
                    ExprPtr& prefix_expr_a,
                    const StructDecl* owner_struct_decl_a,
                    const StructDecl* caller_struct_decl_a,
                    bool use_self_param_a)
            {
                if (owner_struct_decl_a && caller_struct_decl_a)
                {
                    if (owner_struct_decl_a->IsBaseOf(caller_struct_decl_a, true))
                    {
                        if (use_self_param_a)
                        {
                            if (auto self_param_ = ActiveSelfParameter())
                            {
                                prefix_expr_a = ASTFactory::MakeObjectExpr(
                                        self_param_);
                            }
                        }
                    }

                    InsertBaseMemberPrefixes(prefix_expr_a,
                                             owner_struct_decl_a,
                                             caller_struct_decl_a);
                }
            }

            void GLSLConverter::ConvertObjectPrefixSelfParam(
                    ExprPtr& prefix_expr_a,
                    ObjectExpr* object_expr_a)
            {
                if (auto active_struct_decl_ = ActiveStructDecl())
                {
                    if (auto self_param_ = ActiveSelfParameter())
                    {
                        if (auto var_decl_ = object_expr_a->FetchVarDecl())
                        {
                            if (auto owner_struct_decl_ = var_decl_->struct_decl_ref_)
                            {
                                prefix_expr_a = ASTFactory::MakeObjectExpr(
                                        self_param_);

                                InsertBaseMemberPrefixes(prefix_expr_a,
                                                         owner_struct_decl_,
                                                         active_struct_decl_);
                            }
                        }
                    }
                }
            }

            void GLSLConverter::ConvertObjectPrefixBaseStruct(
                    ExprPtr& prefix_expr_a,
                    ObjectExpr* object_expr_a)
            {
                const auto& prefix_type_den_ =
                        prefix_expr_a->GetTypeDenoter()->GetAliased();
                
                if (auto prefix_struct_type_den_ =
                            prefix_type_den_.As<StructTypeDenoter>())
                {
                    if (auto active_struct_decl_ =
                                prefix_struct_type_den_->struct_decl_ref_)
                    {
                        if (auto var_decl_ = object_expr_a->FetchVarDecl())
                        {
                            ConvertObjectPrefixStructMember(
                                    prefix_expr_a,
                                    var_decl_->struct_decl_ref_,
                                    active_struct_decl_,
                                    false);
                        }
                    }
                }
            }

            void GLSLConverter::ConvertObjectPrefixNamespace(
                    ExprPtr& prefix_expr_a,
                    ObjectExpr* object_expr_a)
            {
                if (auto prefix_object_expr_ = prefix_expr_a->As<ObjectExpr>())
                {
                    if (auto base_struct_decl_ =
                                prefix_object_expr_->FetchSymbol<StructDecl>())
                    {
                        if (prefix_object_expr_->prefix_expr_)
                        {
                            const auto& prefix_type_den_ =
                                    prefix_object_expr_->prefix_expr_
                                            ->GetTypeDenoter()
                                            ->GetAliased();
                            
                            if (auto prefix_struct_type_den_ =
                                        prefix_type_den_.As<StructTypeDenoter>())
                            {
                                if (auto active_struct_decl_ =
                                            prefix_struct_type_den_->struct_decl_ref_)
                                    ConvertObjectPrefixNamespaceStruct(
                                            prefix_object_expr_,
                                            object_expr_a,
                                            base_struct_decl_,
                                            active_struct_decl_);
                            }
                        } 
                        else
                        {
                            if (auto active_struct_decl_ = ActiveStructDecl())
                                ConvertObjectPrefixNamespaceStruct(
                                        prefix_object_expr_,
                                        object_expr_a,
                                        base_struct_decl_,
                                        active_struct_decl_);
                        }
                    }
                }
            }

            void GLSLConverter::ConvertObjectPrefixNamespaceStruct(
                    ObjectExpr* prefix_object_expr_a,
                    ObjectExpr* object_expr_a,
                    const StructDecl* base_struct_decl_a,
                    const StructDecl* active_struct_decl_a)
            {
                if (active_struct_decl_a == base_struct_decl_a)
                {
                    object_expr_a->is_static_ = false;
                    object_expr_a->prefix_expr_ = prefix_object_expr_a->prefix_expr_;
                } 
                else
                {
                    if (auto base_member_ = active_struct_decl_a->FetchBaseMember())
                    {
                        object_expr_a->is_static_ = false;
                        prefix_object_expr_a->symbol_ref_ = base_member_;
                        prefix_object_expr_a->ident_ = base_member_->ident_.Original();
                    }

                    while (true)
                    {
                        active_struct_decl_a = active_struct_decl_a->base_struct_ref_;
                        if (!active_struct_decl_a
                            || active_struct_decl_a == base_struct_decl_a)
                            break;

                        if (auto base_member_ =
                                    active_struct_decl_a->FetchBaseMember())
                        {
                            object_expr_a->prefix_expr_ = ASTFactory::MakeObjectExpr(
                                    object_expr_a->prefix_expr_,
                                    base_member_->ident_,
                                    base_member_);
                        } 
                        else
                            break;
                    }
                }
            }

            void GLSLConverter::InsertBaseMemberPrefixes(
                    ExprPtr& prefix_expr_a,
                    const StructDecl* owner_struct_decl_a,
                    const StructDecl* caller_struct_decl_a)
            {
                if (owner_struct_decl_a->IsBaseOf(caller_struct_decl_a))
                {
                    while (caller_struct_decl_a
                           && caller_struct_decl_a != owner_struct_decl_a)
                    {
                        if (auto base_member_ =
                                    caller_struct_decl_a->FetchBaseMember())
                            prefix_expr_a = ASTFactory::MakeObjectExpr(
                                    prefix_expr_a,
                                    base_member_->ident_.Original(),
                                    base_member_);

                        caller_struct_decl_a = caller_struct_decl_a->base_struct_ref_;
                    }
                }
            }

            void GLSLConverter::UnrollStmnts(std::vector<StmntPtr>& stmnts_a)
            {
                for (auto it_ = stmnts_a.begin(); it_ != stmnts_a.end();)
                {
                    std::vector<StmntPtr> unrolled_stmnts_;

                    auto ast_ = it_->get();
                    if (auto var_decl_stmnt_ = ast_->As<VarDeclStmnt>())
                    {
                        if (options_.unroll_array_initializers_)
                            UnrollStmntsVarDecl(unrolled_stmnts_, var_decl_stmnt_);
                    }

                    ++it_;

                    if (!unrolled_stmnts_.empty())
                    {
                        it_ = stmnts_a.insert(it_,
                                           unrolled_stmnts_.begin(),
                                           unrolled_stmnts_.end());
                        it_ += unrolled_stmnts_.size();
                    }
                }
            }

            void GLSLConverter::UnrollStmntsVarDecl(
                    std::vector<StmntPtr>& unrolled_stmnts_a,
                    VarDeclStmnt* ast_a)
            {
                for (const auto& var_decl_ : ast_a->var_decls_)
                {
                    if (var_decl_->initializer_)
                        UnrollStmntsVarDeclInitializer(unrolled_stmnts_a,
                                                       var_decl_.get());
                }
            }

            void GLSLConverter::UnrollStmntsVarDeclInitializer(
                    std::vector<StmntPtr>& unrolled_stmnts_a,
                    VarDecl* var_decl_a)
            {
                const auto& type_den_ = var_decl_a->GetTypeDenoter()->GetAliased();
                if (auto array_type_den_ = type_den_.As<ArrayTypeDenoter>())
                {
                    if (auto init_expr_ =
                                var_decl_a->initializer_->As<InitializerExpr>())
                    {
                        auto dAz_sizes_ = array_type_den_->GetDAzensionSizes();
                        std::vector<int> array_indices_(dAz_sizes_.size(), 0);

                        do
                        {
                            auto sub_expr_ = init_expr_->FetchSubExpr(array_indices_);
                            auto assign_stmnt_ = ASTFactory::MakeArrayAssignStmnt(
                                    var_decl_a,
                                    array_indices_,
                                    sub_expr_);

                            unrolled_stmnts_a.push_back(assign_stmnt_);
                        } 
                        while (init_expr_->NextArrayIndices(array_indices_));
                                var_decl_a->initializer_.reset();
                    }
                }
            }

            void GLSLConverter::ConvertSlotRegisters(
                    std::vector<RegisterPtr>& slot_registers_a)
            {
                if (auto_binding_)
                {
                    slot_registers_a.clear();
                    slot_registers_a.push_back(
                            ASTFactory::MakeRegister(auto_binding_slot_++));
                }
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
