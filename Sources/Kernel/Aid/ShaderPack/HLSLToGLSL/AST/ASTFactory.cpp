#include "ASTFactory.hpp"

#include "../Exception.hpp"
#include "../Helper.hpp"
#include "../Variant.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            namespace ASTFactory
            {
                template <typename T, typename... Args>
                std::shared_ptr<T> MakeAST(Args&&... args_a)
                {
                    return std::make_shared<T>(SourcePosition::ignore,
                                               std::forward<Args>(args_a)...);
                }

                template <typename T, typename Origin, typename... Args>
                std::shared_ptr<T> MakeASTWithOrigin(const Origin& origin_a,
                                                     Args&&... args_a)
                {
                    return std::make_shared<T>(origin_a->area_,
                                               std::forward<Args>(args_a)...);
                }

                CallExprPtr MakeIntrinsicCallExpr(
                        const Intrinsic intrinsic_a,
                        const std::string& ident_a,
                        const TypeDenoterPtr& type_denoter_a,
                        const std::vector<ExprPtr>& arguments_a)
                {
                    auto ast_ = MakeAST<CallExpr>();
                    {
                        ast_->ident_ = ident_a;
                        ast_->type_denoter_ = type_denoter_a;
                        ast_->arguments_ = arguments_a;
                        ast_->intrinsic_ = intrinsic_a;
                    }

                    return ast_;
                }

                CallExprPtr MakeTextureSamplerBindingCallExpr(
                        const ExprPtr& texture_object_expr_a,
                        const ExprPtr& sampler_object_expr_a)
                {
                    auto ast_ = MakeAST<CallExpr>();
                    {
                        const auto& type_den_ = texture_object_expr_a
                                                      ->GetTypeDenoter()
                                                      ->GetAliased();
                        if (auto buffer_type_den_ =
                                    type_den_.As<BufferTypeDenoter>())
                        {
                            ast_->type_denoter_ =
                                    std::make_shared<SamplerTypeDenoter>(
                                            TextureTypeToSamplerType(
                                                    buffer_type_den_->buffer_type_));
                            
                            ast_->arguments_ = {texture_object_expr_a,
                                              sampler_object_expr_a};
                        }
                    }
                    return ast_;
                }

                CallExprPtr MakeTypeCtorCallExpr(
                        const TypeDenoterPtr& type_denoter_a,
                        const std::vector<ExprPtr>& arguments_a)
                {
                    auto ast_ = MakeAST<CallExpr>();
                    {
                        ast_->type_denoter_ = type_denoter_a;
                        ast_->arguments_ = arguments_a;
                    }
                    return ast_;
                }

                CallExprPtr MakeWrapperCallExpr(
                        const std::string& func_ident_a,
                        const TypeDenoterPtr& type_denoter_a,
                        const std::vector<ExprPtr>& arguments_a)
                {
                    auto ast_ = MakeAST<CallExpr>();
                    {
                        ast_->ident_ = func_ident_a;
                        ast_->type_denoter_ = type_denoter_a;
                        ast_->arguments_ = arguments_a;
                        ast_->flags_ << CallExpr::IsWrapperCall;
                    }

                    return ast_;
                }

                InitializerExprPtr MakeInitializerExpr(
                        const std::vector<ExprPtr>& exprs_a)
                {
                    auto ast_ = MakeAST<InitializerExpr>();
                    {
                        ast_->exprs_ = exprs_a;
                    }
                    return ast_;
                }

                CastExprPtr MakeCastExpr(const TypeDenoterPtr& type_denoter_a,
                                         const ExprPtr& value_expr_a)
                {
                    auto ast_ = MakeAST<CastExpr>();
                    {
                        ast_->type_specifier_ = MakeTypeSpecifier(type_denoter_a);
                        ast_->type_specifier_->area_ = value_expr_a->area_;
                        ast_->expr_ = value_expr_a;
                    }

                    return ast_;
                }

                CastExprPtr MakeLiteralCastExpr(
                        const TypeDenoterPtr& type_denoter_a,
                        const DataType literal_type_a,
                        const std::string& literal_value_a)
                {
                    return MakeCastExpr(type_denoter_a,
                                        MakeLiteralExpr(literal_type_a,
                                                        literal_value_a));
                }

                BinaryExprPtr MakeBinaryExpr(const ExprPtr& lhs_expr_a,
                                             const BinaryOp op_a,
                                             const ExprPtr& rhs_expr_a)
                {
                    auto ast_ = MakeAST<BinaryExpr>();
                    {
                        ast_->lhs_expr_ = lhs_expr_a;
                        ast_->op_ = op_a;
                        ast_->rhs_expr_ = rhs_expr_a;
                    }

                    return ast_;
                }

                LiteralExprPtr MakeLiteralExpr(const DataType literal_type_a,
                                               const std::string& literal_value_a)
                {
                    auto ast_ = MakeAST<LiteralExpr>();
                    {
                        ast_->data_type_ = literal_type_a;
                        ast_->value_ = literal_value_a;
                    }

                    return ast_;
                }

                LiteralExprPtr MakeLiteralExprOrNull(
                        const Variant& literal_value_a)
                {
                    switch (literal_value_a.Type())
                    {
                    case Variant::Types::Bool:
                        return MakeLiteralExpr(DataType::Bool,
                                               std::to_string(
                                                       literal_value_a.Bool()));
                    case Variant::Types::Int:
                        return MakeLiteralExpr(DataType::Int,
                                               std::to_string(
                                                       literal_value_a.Int()));
                    case Variant::Types::Real:
                        return MakeLiteralExpr(DataType::Float,
                                               std::to_string(
                                                       literal_value_a.Real()));
                    default:
                        return nullptr;
                    }
                }

                AliasDeclStmntPtr MakeBaseTypeAlias(const DataType data_type_a,
                                                    const std::string& ident_a)
                {
                    auto ast_ = MakeAST<AliasDeclStmnt>();
                    {
                        auto alias_decl_ = MakeAST<AliasDecl>();
                        {
                            alias_decl_->ident_ = ident_a;
                            alias_decl_->type_denoter_ =
                                    std::make_shared<BaseTypeDenoter>(data_type_a);
                            alias_decl_->decl_stmnt_ref_ = ast_.get();
                        }

                        ast_->alias_decls_.push_back(alias_decl_);
                    }
                    return ast_;
                }

                TypeSpecifierPtr MakeTypeSpecifier(
                        const StructDeclPtr& struct_decl_a)
                {
                    auto ast_ = MakeAST<TypeSpecifier>();
                    {
                        ast_->struct_decl_ = struct_decl_a;
                        ast_->type_denoter_ = std::make_shared<StructTypeDenoter>(
                                struct_decl_a.get());
                    }

                    ast_->area_ = ast_->struct_decl_->area_;
                    return ast_;
                }

                TypeSpecifierPtr MakeTypeSpecifier(
                        const TypeDenoterPtr& type_denoter_a)
                {
                    auto ast_ = MakeAST<TypeSpecifier>();
                    {
                        ast_->type_denoter_ = type_denoter_a;
                    }

                    return ast_;
                }

                TypeSpecifierPtr MakeTypeSpecifier(const DataType data_type_a)
                {
                    return MakeTypeSpecifier(
                            std::make_shared<BaseTypeDenoter>(data_type_a));
                }

                VarDeclStmntPtr MakeVarDeclStmnt(
                        const TypeSpecifierPtr& type_specifier_a,
                        const std::string& ident_a,
                        const ExprPtr& initializer_a)
                {
                    auto ast_ = MakeAST<VarDeclStmnt>();
                    {
                        ast_->type_specifier_ = type_specifier_a;

                        auto var_decl_ = MakeAST<VarDecl>();
                        {
                            var_decl_->ident_ = ident_a;
                            var_decl_->initializer_ = initializer_a;
                            var_decl_->decl_stmnt_ref_ = ast_.get();
                        }

                        ast_->var_decls_.push_back(var_decl_);
                    }

                    return ast_;
                }

                VarDeclStmntPtr MakeVarDeclStmnt(const DataType data_type_a,
                                                 const std::string& ident_a,
                                                 const ExprPtr& initializer_a)
                {
                    return MakeVarDeclStmnt(MakeTypeSpecifier(data_type_a),
                                            ident_a,
                                            initializer_a);
                }

                VarDeclStmntPtr MakeVarDeclStmntSplit(
                        const VarDeclStmntPtr& var_decl_stmnt_a,
                        std::size_t idx_a)
                {
                    if (var_decl_stmnt_a->var_decls_.size() >= 2
                        && idx_a < var_decl_stmnt_a->var_decls_.size())
                    {
                        auto var_decl_ = var_decl_stmnt_a->var_decls_[idx_a];
                        var_decl_stmnt_a->var_decls_.erase(
                                var_decl_stmnt_a->var_decls_.begin() + idx_a);

                        auto ast_ = MakeAST<VarDeclStmnt>();
                        {
                            ast_->flags_ = var_decl_stmnt_a->flags_;
                            ast_->type_specifier_ = var_decl_stmnt_a->type_specifier_;
                            ast_->var_decls_.push_back(var_decl_);
                        }

                        return ast_;
                    }
                    return var_decl_stmnt_a;
                }

                ObjectExprPtr MakeObjectExpr(const ExprPtr& prefix_expr_a,
                                             const std::string& ident_a,
                                             Decl* symbol_ref_a)
                {
                    auto ast_ = MakeAST<ObjectExpr>();
                    {
                        ast_->prefix_expr_ = prefix_expr_a;
                        ast_->ident_ = ident_a;
                        ast_->symbol_ref_ = symbol_ref_a;
                    }

                    return ast_;
                }

                ObjectExprPtr MakeObjectExpr(const std::string& ident_a,
                                             Decl* symbol_ref_a)
                {
                    return MakeObjectExpr(nullptr, ident_a, symbol_ref_a);
                }

                ObjectExprPtr MakeObjectExpr(Decl* symbol_ref_a)
                {
                    return MakeObjectExpr(symbol_ref_a->ident_.Original(),
                                          symbol_ref_a);
                }

                ArrayExprPtr MakeArrayExpr(const ExprPtr& prefix_expr_a,
                                           std::vector<ExprPtr>&& array_indices_a)
                {
                    auto ast_ = MakeAST<ArrayExpr>();
                    {
                        ast_->prefix_expr_ = prefix_expr_a;
                        ast_->array_indices_ = std::move(array_indices_a);
                    }

                    return ast_;
                }

                ArrayExprPtr MakeArrayExpr(const ExprPtr& prefix_expr_a,
                                           const std::vector<int>& array_indices_a)
                {
                    return MakeArrayExpr(prefix_expr_a,
                                         MakeArrayIndices(array_indices_a));
                }

                ArrayExprPtr MakeArrayExpr(
                        const ExprPtr& prefix_expr_a,
                        const std::vector<ExprPtr>::const_iterator&
                                array_indices_begin_a,
                        const std::vector<ExprPtr>::const_iterator&
                                array_indices_end_a)
                {
                    auto ast_ = MakeAST<ArrayExpr>();
                    {
                        ast_->prefix_expr_ = prefix_expr_a;
                        ast_->array_indices_.insert(ast_->array_indices_.end(),
                                                 array_indices_begin_a,
                                                 array_indices_end_a);
                    }

                    return ast_;
                }

                ArrayExprPtr MakeArrayExprSplit(const ArrayExprPtr& array_expr_a,
                                                std::size_t split_array_index_a)
                {
                    if (array_expr_a != nullptr && split_array_index_a > 0
                        && split_array_index_a < array_expr_a->NumIndices())
                    {
                        auto ast_ = MakeArrayExpr(
                                MakeArrayExpr(array_expr_a->prefix_expr_,
                                              array_expr_a->array_indices_.begin(),
                                              array_expr_a->array_indices_.begin()
                                                      + split_array_index_a),
                                array_expr_a->array_indices_.begin()
                                        + split_array_index_a,
                                array_expr_a->array_indices_.end());

                        ast_->area_ = array_expr_a->area_;
                        return ast_;
                    }
                    return array_expr_a;
                }

                RegisterPtr MakeRegister(int slot_a,
                                         const RegisterType register_type_a)
                {
                    auto ast_ = MakeAST<Register>();
                    {
                        ast_->register_type_ = register_type_a;
                        ast_->slot_ = slot_a;
                    }

                    return ast_;
                }

                BracketExprPtr MakeBracketExpr(const ExprPtr& expr_a)
                {
                    auto ast_ = MakeASTWithOrigin<BracketExpr>(expr_a);
                    {
                        ast_->expr_ = expr_a;
                    }
                    return ast_;
                }

                static ExprPtr MakeConstructorListExprPrAzarySingle(
                        const ExprPtr& expr_a,
                        const TypeDenoterPtr& type_den_a)
                {
                    if (auto struct_type_den_ = type_den_a->As<StructTypeDenoter>())
                    {
                        if (auto struct_decl_ = struct_type_den_->struct_decl_ref_)
                        {
                            std::vector<TypeDenoterPtr> member_type_dens_;
                            struct_decl_->CollectMemberTypeDenoters(
                                    member_type_dens_,
                                    false);

                            return MakeCastExpr(
                                    type_den_a,
                                    MakeConstructorListExpr(expr_a,
                                                            member_type_dens_));
                        }
                    } 
                    else if (auto base_type_den_ =
                                       type_den_a->As<BaseTypeDenoter>())
                    {
                        if (!base_type_den_->IsScalar())
                        {
                            return MakeCastExpr(type_den_a, expr_a);
                        }
                    }

                    return expr_a;
                }

                static ExprPtr MakeConstructorListExprPrAzary(
                        const ExprPtr& expr_a,
                        std::vector<TypeDenoterPtr>::const_iterator
                                type_dens_begin_a,
                        std::vector<TypeDenoterPtr>::const_iterator type_dens_end_a)
                {
                    if (type_dens_begin_a + 1 != type_dens_end_a)
                    {
                        auto ast_ = MakeAST<SequenceExpr>();
                        {
                            ast_->Append(MakeConstructorListExprPrAzarySingle(
                                    expr_a,
                                    (*type_dens_begin_a)->GetSub()));
                            ast_->Append(MakeConstructorListExprPrAzary(
                                    expr_a,
                                    type_dens_begin_a + 1,
                                    type_dens_end_a));
                        }
                        return ast_;
                    } 
                    else
                        return MakeConstructorListExprPrAzarySingle(
                                expr_a,
                                (*type_dens_begin_a)->GetSub());
                }

                ExprPtr MakeConstructorListExpr(
                        const ExprPtr& expr_a,
                        const std::vector<TypeDenoterPtr>& list_type_dens_a)
                {
                    if (list_type_dens_a.empty())
                        return expr_a;
                    else
                        return MakeConstructorListExprPrAzary(
                                expr_a,
                                list_type_dens_a.begin(),
                                list_type_dens_a.end());
                }

                ExprStmntPtr MakeAssignStmnt(const ExprPtr& lvalue_expr_a,
                                             const ExprPtr& rvalue_expr_a,
                                             const AssignOp op_a)
                {
                    auto ast_ = MakeAST<ExprStmnt>();
                    {
                        auto assign_expr_ = MakeAST<AssignExpr>();
                        {
                            assign_expr_->lvalue_expr_ = lvalue_expr_a;
                            assign_expr_->op_ = op_a;
                            assign_expr_->rvalue_expr_ = rvalue_expr_a;
                        }
                        ast_->expr_ = assign_expr_;
                    }

                    return ast_;
                }

                ExprStmntPtr MakeArrayAssignStmnt(
                        VarDecl* var_decl_a,
                        const std::vector<int>& array_indices_a,
                        const ExprPtr& assign_expr_a)
                {
                    return MakeAssignStmnt(MakeArrayExpr(MakeObjectExpr(
                                                                 var_decl_a),
                                                         array_indices_a),
                                           assign_expr_a);
                }

                ArrayDAzensionPtr MakeArrayDAzension(int array_size_a)
                {
                    auto ast_ = MakeAST<ArrayDAzension>();
                    {
                        if (array_size_a > 0)
                        {
                            ast_->expr_ = MakeLiteralExpr(DataType::Int,
                                                        std::to_string(
                                                                array_size_a));
                            ast_->size_ = array_size_a;
                        } 
                        else
                        {
                            ast_->expr_ = MakeAST<NullExpr>();
                            ast_->size_ = 0;
                        }
                    }

                    return ast_;
                }

                CodeBlockStmntPtr MakeCodeBlockStmnt(const StmntPtr& stmnt_a)
                {
                    auto ast_ = MakeASTWithOrigin<CodeBlockStmnt>(stmnt_a);
                    {
                        ast_->code_block_ = MakeASTWithOrigin<CodeBlock>(stmnt_a);
                        ast_->code_block_->stmnts_.push_back(stmnt_a);
                    }

                    return ast_;
                }

                BasicDeclStmntPtr MakeStructDeclStmnt(
                        const StructDeclPtr& struct_decl_a)
                {
                    auto ast_ = MakeAST<BasicDeclStmnt>();
                    {
                        ast_->decl_object_ = struct_decl_a;
                        struct_decl_a->decl_stmnt_ref_ = ast_.get();
                    }

                    return ast_;
                }

                UniformBufferDeclPtr MakeUniformBufferDecl(
                        const std::string& ident_a,
                        int binding_slot_a,
                        const UniformBufferType buffer_type_a)
                {
                    auto ast_ = MakeAST<UniformBufferDecl>();
                    {
                        ast_->ident_ = ident_a;
                        ast_->buffer_type_ = buffer_type_a;
                        ast_->slot_registers_.push_back(
                                MakeRegister(binding_slot_a,
                                             RegisterType::ConstantBuffer));
                    }

                    return ast_;
                }

                std::vector<ExprPtr> MakeArrayIndices(
                        const std::vector<int>& array_indices_a)
                {
                    std::vector<ExprPtr> exprs_;

                    for (auto index_ : array_indices_a)
                        exprs_.push_back(MakeLiteralExpr(DataType::Int,
                                                        std::to_string(index_)));

                    return exprs_;
                }

                std::vector<ArrayDAzensionPtr> MakeArrayDAzensionList(
                        const std::vector<int>& array_sizes_a)
                {
                    std::vector<ArrayDAzensionPtr> array_dAzs_;

                    for (auto dAz_ : array_sizes_a)
                        array_dAzs_.push_back(MakeArrayDAzension(dAz_));

                    return array_dAzs_;
                }

                ExprPtr ConvertExprBaseType(const DataType data_type_a,
                                            const ExprPtr& sub_expr_a)
                {
                    if (sub_expr_a->Type() == AST::Types::LiteralExpr
                        && IsScalarType(data_type_a))
                    {
                        auto ast_ = std::static_pointer_cast<LiteralExpr>(
                                sub_expr_a);
                        {
                            ast_->ConvertDataType(data_type_a);
                        }
                        return ast_;
                    } 
                    else
                    {
                        auto ast_ = MakeASTWithOrigin<CastExpr>(sub_expr_a);
                        {
                            ast_->type_specifier_ = MakeTypeSpecifier(
                                    std::make_shared<BaseTypeDenoter>(
                                            data_type_a));
                            ast_->type_specifier_->area_ = sub_expr_a->area_;
                            ast_->expr_ = sub_expr_a;
                        }

                        return ast_;
                    }
                }

                ArrayDAzensionPtr ConvertExprToArrayDAzension(
                        const ExprPtr& expr_a)
                {
                    auto ast_ = MakeAST<ArrayDAzension>();
                    {
                        if (expr_a)
                        {
                            ast_->area_ = expr_a->area_;
                            ast_->expr_ = expr_a;
                        }
                    }

                    return ast_;
                }

                std::vector<ArrayDAzensionPtr>
                ConvertExprListToArrayDAzensionList(
                        const std::vector<ExprPtr>& exprs_a)
                {
                    std::vector<ArrayDAzensionPtr> array_dAzs_;

                    for (const auto& expr_ : exprs_a)
                        array_dAzs_.push_back(ConvertExprToArrayDAzension(expr_));

                    return array_dAzs_;
                }
            } // namespace ASTFactory
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel