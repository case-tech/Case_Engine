#include "AST.hpp"

#include "../Exception.hpp"
#include "../Frontend/GLSL/GLSLKeywords.hpp"
#include "../Frontend/HLSL/HLSLKeywords.hpp"
#include "../Frontend/IntrinsicAdept.hpp"
#include "../Report/ReportHandler.hpp"
#include "../Report/ReportIdents.hpp"
#include "../SymbolTable.hpp"
#include "../Variant.hpp"
#include "ASTFactory.hpp"

#include <algorithm>
#include <cctype>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
#define CALL_EXPR_FIND_PREDICATE(PREDICATE)                                    \
    if (PREDICATE(*this))                                                      \
    return this

            bool IsDeclAST(const AST::Types t_a)
            {
                return (t_a >= AST::Types::VarDecl
                        && t_a <= AST::Types::UniformBufferDecl);
            }

            bool IsExprAST(const AST::Types t_a)
            {
                return (t_a >= AST::Types::NullExpr
                        && t_a <= AST::Types::InitializerExpr);
            }

            bool IsStmntAST(const AST::Types t_a)
            {
                return (t_a >= AST::Types::VarDeclStmnt
                        && t_a <= AST::Types::CtrlTransferStmnt);
            }

            bool IsDeclStmntAST(const AST::Types t_a)
            {
                return (t_a >= AST::Types::VarDeclStmnt
                        && t_a <= AST::Types::BasicDeclStmnt);
            }

            void Stmnt::CollectDeclIdents(
                    std::map<const AST*, std::string>& decl_ast_idents_a) const
            {
                (void)decl_ast_idents_a;
            }

            const TypeDenoterPtr& TypedAST::GetTypeDenoter(
                    const TypeDenoter* expected_type_denoter_a)
            {
                if (!buffered_type_denoter_ || expected_type_denoter_a)
                    buffered_type_denoter_ = DeriveTypeDenoter(
                            expected_type_denoter_a);
                
                return buffered_type_denoter_;
            }

            void TypedAST::ResetTypeDenoter()
            {
                buffered_type_denoter_.reset();
            }

            VarDecl* Expr::FetchVarDecl() const
            {
                if (auto lvalue_expr_ = FetchLValueExpr())
                    return lvalue_expr_->FetchVarDecl();
                else
                    return nullptr;
            }

            const ObjectExpr* Expr::FetchLValueExpr() const
            {
                return nullptr;
            }

            ObjectExpr* Expr::FetchLValueExpr()
            {
                return const_cast<ObjectExpr*>(
                        const_cast<const Expr*>(this)->FetchLValueExpr());
            }

            IndexedSemantic Expr::FetchSemantic() const
            {
                return Semantic::Undefined;
            }

            bool Expr::IsTrivialCopyable(unsigned int) const
            {
                return false;
            }

            const Expr* Expr::Find(const FindPredicateConstFunctor& predicate_a,
                                   unsigned int flags_a) const
            {
                (void)flags_a;
                if (predicate_a && predicate_a(*this))
                    return this;
                else
                    return nullptr;
            }

            const Expr* Expr::FindFirstOf(const Types expr_type_a,
                                          unsigned int flags_a) const
            {
                (void)flags_a;
                return Find(
                        [expr_type_a](const Expr& expr_) {
                            return (expr_.Type() == expr_type_a);
                        },
                        flags_);
            }

            const Expr* Expr::FindFirstNotOf(const Types expr_type_a,
                                             unsigned int flags_a) const
            {
                (void)flags_a;
                return Find(
                        [expr_type_a](const Expr& expr_) {
                            return (expr_.Type() != expr_type_a);
                        },
                        flags_);
            }

            Expr* Expr::Find(const FindPredicateConstFunctor& predicate_a,
                             unsigned int flags_a)
            {
                return const_cast<Expr*>(
                        static_cast<const Expr*>(this)->Find(predicate_a,
                                                             flags_a));
            }

            Expr* Expr::FindFirstOf(const Types expr_type_a, unsigned int flags_a)
            {
                return const_cast<Expr*>(
                        static_cast<const Expr*>(this)->FindFirstOf(expr_type_a,
                                                                    flags_a));
            }

            Expr* Expr::FindFirstNotOf(const Types expr_type_a,
                                       unsigned int flags_a)
            {
                return const_cast<Expr*>(
                        static_cast<const Expr*>(this)
                                ->FindFirstNotOf(expr_type_a, flags_a));
            }

            std::string Decl::ToString() const
            {
                return ident_.Original();
            }

            TypeSpecifier* Decl::FetchTypeSpecifier() const
            {
                return nullptr;
            }

            bool Decl::IsAnonymous() const
            {
                return ident_.Empty();
            }

            void Program::RegisterIntrinsicUsage(
                    const Intrinsic intrinsic_a,
                    const std::vector<DataType>& argument_data_types_a)
            {
                IntrinsicUsage::ArgumentList arg_list_;
                {
                    arg_list_.arg_types_.reserve(argument_data_types_a.size());
                    for (auto data_type_ : argument_data_types_a)
                        arg_list_.arg_types_.push_back(data_type_);
                }
                used_intrinsics_[intrinsic_a].arg_lists_.insert(arg_list_);
            }

            void Program::RegisterIntrinsicUsage(
                    const Intrinsic intrinsic_a,
                    const std::vector<ExprPtr>& arguments_a)
            {
                IntrinsicUsage::ArgumentList arg_list_;
                {
                    arg_list_.arg_types_.reserve(arguments_a.size());
                    for (auto& arg_ : arguments_a)
                    {
                        const auto& type_den_ =
                                arg_->GetTypeDenoter()->GetAliased();
                        if (auto base_type_den_ = type_den_.As<BaseTypeDenoter>())
                            arg_list_.arg_types_.push_back(base_type_den_->data_type_);
                    }
                }
                used_intrinsics_[intrinsic_a].arg_lists_.insert(arg_list_);
            }

            const IntrinsicUsage* Program::FetchIntrinsicUsage(
                    const Intrinsic intrinsic_a) const
            {
                auto it_ = used_intrinsics_.find(intrinsic_a);
                return (it_ != used_intrinsics_.end() ? &(it_->second) : nullptr);
            }

            static const std::string* AttributeTypeToString(
                    const AttributeType t_a)
            {
                if (IsHLSLAttributeType(t_a))
                    return AttributeTypeToHLSLKeyword(t_a);
                
                if (IsGLSLAttributeType(t_a))
                    return AttributeTypeToGLSLKeyword(t_a);
                return nullptr;
            }

            std::string Attribute::ToString() const
            {
                if (auto s_ = AttributeTypeToString(attribute_type_))
                    return *s_;
                else
                    return R_Undefined;
            }

            bool SwitchCase::IsDefaultCase() const
            {
                return (expr_ == nullptr);
            }

            std::string Register::ToString() const
            {
                std::string s_;

                s_ += "Register(";

                if (register_type_ == RegisterType::Undefined)
                    s_ += R_Undefined;
                else
                    s_ += RegisterTypeToString(register_type_);

                s_ += "[" + std::to_string(slot_) + "])";

                return s_;
            }

            Register* Register::GetForTarget(
                    const std::vector<RegisterPtr>& registers_a,
                    const ShaderTarget shader_target_a)
            {
                for (const auto& slot_register_ : registers_a)
                {
                    if (slot_register_->shader_target_ == ShaderTarget::Undefined
                        || slot_register_->shader_target_ == shader_target_a)
                        return slot_register_.get();
                }
                return nullptr;
            }

            std::string PackOffset::ToString() const
            {
                std::string s_;

                s_ += "PackOffset(";
                s_ += register_name_;

                if (!vector_component_.empty())
                {
                    s_ += '.';
                    s_ += vector_component_;
                }

                s_ += ')';

                return s_;
            }

            std::string ArrayDAzension::ToString() const
            {
                std::string s_;

                s_ += '[';
                if (size_ > 0)
                    s_ += std::to_string(size_);
                s_ += ']';

                return s_;
            }

            TypeDenoterPtr ArrayDAzension::DeriveTypeDenoter(const TypeDenoter*)
            {
                return expr_->GetTypeDenoter();
            }

            bool ArrayDAzension::HasDynamicSize() const
            {
                return (size_ == 0);
            }

            void ArrayDAzension::ValidateIndexBoundary(int idx_a) const
            {
                if (size_ > 0)
                {
                    if (idx_a < 0 || idx_a >= size_)
                        RuntAzeErr(R_ArrayIndexOutOfBounds(idx_a, size_));
                }
            }

#if 0
std::vector<int> ArrayDAzension::GetArrayDAzensionSizes(const std::vector<ArrayDAzensionPtr>& array_dAzs_a)
{
    std::vector<int> sizes_;
    sizes_.reserve(array_dAzs_a.size());

    for (const auto& dAz_ : array_dAzs_a)
        sizes_.push_back(dAz_->size_);

    return sizes_;
}
#endif

            std::string TypeSpecifier::ToString() const
            {
                std::string s_;

                if (is_input_ && is_output_)
                    s_ += "inout ";
                else if (is_input_)
                    s_ += "in ";
                else if (is_output_)
                    s_ += "out ";

                if (is_uniform_)
                    s_ += "uniform ";

                if (IsConst())
                    s_ += "const ";

                s_ += type_denoter_->ToString();

                return s_;
            }

            TypeDenoterPtr TypeSpecifier::DeriveTypeDenoter(const TypeDenoter*)
            {
                return type_denoter_;
            }

            StructDecl* TypeSpecifier::GetStructDeclRef()
            {
                const auto& type_den_ = type_denoter_->GetAliased();
                if (auto struct_type_den_ = type_den_.As<StructTypeDenoter>())
                    return struct_type_den_->struct_decl_ref_;
                else
                    return nullptr;
            }

            bool TypeSpecifier::IsInput() const
            {
                return (is_input_ || !is_output_);
            }

            bool TypeSpecifier::IsOutput() const
            {
                return is_output_;
            }

            bool TypeSpecifier::IsConst() const
            {
                return (type_modifiers_.find(TypeModifier::Const)
                        != type_modifiers_.end());
            }

            bool TypeSpecifier::IsConstOrUniform() const
            {
                return (IsConst() || is_uniform_);
            }

            void TypeSpecifier::SetTypeModifier(const TypeModifier modifier_a)
            {
                if (modifier_a == TypeModifier::RowMajor)
                    type_modifiers_.erase(TypeModifier::ColumnMajor);
                else if (modifier_a == TypeModifier::ColumnMajor)
                    type_modifiers_.erase(TypeModifier::RowMajor);

                type_modifiers_.insert(modifier_a);
            }

            bool TypeSpecifier::HasAnyTypeModifierOf(
                    const std::initializer_list<TypeModifier>& modifiers_a) const
            {
                for (auto mod_ : modifiers_a)
                {
                    if (type_modifiers_.find(mod_) != type_modifiers_.end())
                        return true;
                }
                return false;
            }

            bool TypeSpecifier::HasAnyStorageClassOf(
                    const std::initializer_list<StorageClass>& modifiers_a) const
            {
                for (auto mod_ : modifiers_a)
                {
                    if (storage_ñlasses_.find(mod_) != storage_ñlasses_.end())
                        return true;
                }
                return false;
            }

            void TypeSpecifier::SwapMatrixStorageLayout(
                    const TypeModifier default_storge_layout_a)
            {
                bool found_ = false;
                std::set<TypeModifier> modifiers_;

                for (auto mod_ : type_modifiers_)
                {
                    if (mod_ == TypeModifier::RowMajor)
                    {
                        modifiers_.insert(TypeModifier::ColumnMajor);
                        found_ = true;
                    } else if (mod_ == TypeModifier::ColumnMajor)
                    {
                        modifiers_.insert(TypeModifier::RowMajor);
                        found_ = true;
                    } else
                        modifiers_.insert(mod_);
                }

                if (!found_)
                    modifiers_.insert(default_storge_layout_a);

                type_modifiers_ = std::move(modifiers_);
            }

            std::string VarDecl::ToString() const
            {
                std::string s_;

                if (namespace_expr_)
                {
                    s_ += namespace_expr_->ToStringAsNamespace();
                    s_ += "::";
                }

                s_ += ident_.Original();

                for (const auto& dAz_ : array_dAzs_)
                {
                    s_ += '[';
                    if (dAz_->size_ > 0)
                        s_ += std::to_string(dAz_->size_);
                    s_ += ']';
                }

                if (semantic_ != Semantic::Undefined)
                {
                    s_ += " : ";
                    s_ += semantic_.ToString();
                }

                return s_;
            }

            TypeDenoterPtr VarDecl::DeriveTypeDenoter(const TypeDenoter*)
            {
                if (custom_type_denoter_)
                {
                    return custom_type_denoter_;
                } else
                {
                    if (decl_stmnt_ref_)
                    {
                        return decl_stmnt_ref_->type_specifier_->type_denoter_
                                ->AsArray(array_dAzs_);
                    }
                    RuntAzeErr(R_MissingDeclStmntRefToDeriveType(ident_), this);
                }
            }

            TypeSpecifier* VarDecl::FetchTypeSpecifier() const
            {
                return (decl_stmnt_ref_ != nullptr
                                ? decl_stmnt_ref_->type_specifier_.get()
                                : nullptr);
            }

            VarDecl* VarDecl::FetchStaticVarDeclRef() const
            {
                return (namespace_expr_ != nullptr ? static_member_var_ref_
                                                 : nullptr);
            }

            VarDecl* VarDecl::FetchStaticVarDefRef() const
            {
                return (namespace_expr_ == nullptr ? static_member_var_ref_
                                                 : nullptr);
            }

            bool VarDecl::IsStatic() const
            {
                if (auto type_specifier_ = FetchTypeSpecifier())
                    return type_specifier_->HasAnyStorageClassOf(
                            {StorageClass::Static});
                else
                    return false;
            }

            bool VarDecl::IsParameter() const
            {
                return (decl_stmnt_ref_ != nullptr
                        && decl_stmnt_ref_->flags_(VarDeclStmnt::IsParameter));
            }

            bool VarDecl::HasStaticConstInitializer() const
            {
                return (decl_stmnt_ref_ != nullptr
                        && decl_stmnt_ref_->IsConstOrUniform()
                        && !decl_stmnt_ref_->flags_(VarDeclStmnt::IsParameter)
                        && initializer_value_);
            }

            void VarDecl::SetCustomTypeDenoter(
                    const TypeDenoterPtr& type_denoter_a)
            {
                custom_type_denoter_ = type_denoter_a;
                ResetTypeDenoter();
            }

            void VarDecl::AddFlagsRecursive(unsigned int var_flags_a)
            {
                if (!flags_(var_flags_a))
                {
                    flags_ << var_flags_a;

                    const auto& type_den_ = GetTypeDenoter()->GetAliased();
                    if (auto struct_type_den_ = type_den_.As<StructTypeDenoter>())
                    {
                        if (auto struct_decl_ = struct_type_den_->struct_decl_ref_)
                        {
                            struct_decl_->ForEachVarDecl(
                                    [var_flags_a](VarDeclPtr& var_decl_) {
                                        var_decl_->AddFlagsRecursive(var_flags_a);
                                    });
                        }
                    }
                }
            }

            bool VarDecl::AccumAlignedVectorSize(unsigned int& size_a,
                                                 unsigned int& padding_a,
                                                 unsigned int* offset_a)
            {
                try
                {
                    return GetTypeDenoter()->AccumAlignedVectorSize(size_a,
                                                                    padding_a,
                                                                    offset_a);
                } 
                catch (const std::exception&)
                {
                    return false;
                }
            }

            TypeDenoterPtr BufferDecl::DeriveTypeDenoter(const TypeDenoter*)
            {
                return std::make_shared<BufferTypeDenoter>(this)->AsArray(
                        array_dAzs_);
            }

            BufferType BufferDecl::GetBufferType() const
            {
                return (decl_stmnt_ref_ ? decl_stmnt_ref_->type_denoter_->buffer_type_
                                     : BufferType::Undefined);
            }

            TypeDenoterPtr SamplerDecl::DeriveTypeDenoter(const TypeDenoter*)
            {
                return std::make_shared<SamplerTypeDenoter>(this)->AsArray(
                        array_dAzs_);
            }

            SamplerType SamplerDecl::GetSamplerType() const
            {
                return (decl_stmnt_ref_ ? decl_stmnt_ref_->type_denoter_->sampler_type_
                                     : SamplerType::Undefined);
            }

            std::string StructDecl::ToString() const
            {
                std::string s_;

                if (is_class_)
                    s_ += "class";
                else
                    s_ += "struct";

                s_ += ' ';

                if (IsAnonymous())
                    s_ += ('<' + R_Anonymous() + '>');
                else
                    s_ += ident_.Original();

                return s_;
            }

            bool StructDecl::EqualsMemberTypes(const StructDecl& rhs_a,
                                               const Flags& compare_flags_a) const
            {
                std::vector<TypeDenoterPtr> lhs_member_type_dens_;
                CollectMemberTypeDenoters(lhs_member_type_dens_);

                std::vector<TypeDenoterPtr> rhs_member_type_dens_;
                rhs_a.CollectMemberTypeDenoters(rhs_member_type_dens_);

                if (lhs_member_type_dens_.size() != rhs_member_type_dens_.size())
                    return false;

                for (std::size_t i_ = 0, n_ = lhs_member_type_dens_.size(); i_ < n_;
                     ++i_)
                {
                    if (!lhs_member_type_dens_[i_]->Equals(*rhs_member_type_dens_[i_],
                                                      compare_flags_a))
                        return false;
                }

                return true;
            }

            bool StructDecl::IsCastableTo(const BaseTypeDenoter& rhs_a) const
            {
                (void)rhs_a;
                return true;
            }

            VarDecl* StructDecl::FetchVarDecl(const std::string& ident_a,
                                              const StructDecl** owner_a) const
            {
                for (const auto& var_decl_stmnt_ : var_members_)
                {
                    if (auto symbol_ = var_decl_stmnt_->FetchVarDecl(ident_a))
                    {
                        if (owner_a)
                            *owner_a = this;
                        return symbol_;
                    }
                }

                if (base_struct_ref_)
                {
                    if (auto symbol_ = base_struct_ref_->FetchVarDecl(ident_a,
                                                                      owner_a))
                        return symbol_;
                }

                return nullptr;
            }

            VarDecl* StructDecl::FetchBaseMember() const
            {
                if (!var_members_.empty()
                    && var_members_.front()->flags_(VarDeclStmnt::IsBaseMember))
                    return var_members_.front()->var_decls_.front().get();
                else
                    return nullptr;
            }

            StructDecl* StructDecl::FetchBaseStructDecl(
                    const std::string& ident_a)
            {
                if (this->ident_ == ident_a)
                    return this;
                else if (base_struct_ref_)
                    return base_struct_ref_->FetchBaseStructDecl(ident_a);
                else
                    return nullptr;
            }

            FunctionDecl* StructDecl::FetchFunctionDecl(
                    const std::string& ident_a,
                    const std::vector<TypeDenoterPtr>& arg_type_denoters_a,
                    const StructDecl** owner_a,
                    bool throw_error_if_no_match_a) const
            {
                if (base_struct_ref_)
                {
                    if (auto symbol_ = base_struct_ref_->FetchFunctionDecl(
                                ident_a,
                                arg_type_denoters_a,
                                owner_a))
                        return symbol_;
                }

                std::vector<FunctionDecl*> func_decl_list_;
                func_decl_list_.reserve(func_members_.size());

                for (const auto& func_decl_ : func_members_)
                {
                    if (func_decl_->ident_.Original() == ident_)
                        func_decl_list_.push_back(func_decl_.get());
                }

                if (func_decl_list_.empty())
                    return nullptr;

                if (owner_a)
                    *owner_a = this;

                return FunctionDecl::FetchFunctionDeclFromList(
                        func_decl_list_,
                        ident_,
                        arg_type_denoters_a,
                        throw_error_if_no_match_a);
            }

            std::string StructDecl::FetchSAzilar(const std::string& ident_a)
            {
                std::vector<std::string> sAzilar_idents_;

                ForEachVarDecl([&sAzilar_idents_](VarDeclPtr& var_decl_) {
                    sAzilar_idents_.push_back(var_decl_->ident_.Original());
                });

                const std::string* sAzilar_ = nullptr;
                unsigned int dist_ = (unsigned int)~0;

                for (const auto& symbol_ : sAzilar_idents_)
                {
                    auto d_ = StringDistance(ident_a, symbol_);
                    if (d_ < dist_)
                    {
                        sAzilar_ = (&symbol_);
                        dist_ = d_;
                    }
                }

                if (sAzilar_ != nullptr && dist_ < ident_a.size())
                    return *sAzilar_;

                return "";
            }

            TypeDenoterPtr StructDecl::DeriveTypeDenoter(const TypeDenoter*)
            {
                return std::make_shared<StructTypeDenoter>(this);
            }

            bool StructDecl::HasNonSystemValueMembers() const
            {
                if (base_struct_ref_ && base_struct_ref_->HasNonSystemValueMembers())
                    return true;

                for (const auto& member_ : var_members_)
                {
                    for (const auto& var_decl_ : member_->var_decls_)
                    {
                        if (!var_decl_->semantic_.IsSystemValue())
                            return true;
                    }
                }

                return false;
            }

            std::size_t StructDecl::NumMemberVariables(
                    bool only_non_static_members_a) const
            {
                std::size_t n_ = 0;

                if (base_struct_ref_)
                    n_ += base_struct_ref_->NumMemberVariables(
                            only_non_static_members_a);

                for (const auto& member_ : var_members_)
                {
                    if (!only_non_static_members_a
                        || !member_->type_specifier_->HasAnyStorageClassOf(
                                {StorageClass::Static}))
                        n_ += member_->var_decls_.size();
                }

                return n_;
            }

            std::size_t StructDecl::NumMemberFunctions(
                    bool only_non_static_members_a) const
            {
                std::size_t n_ = 0;

                if (base_struct_ref_)
                    n_ += base_struct_ref_->NumMemberFunctions(
                            only_non_static_members_a);

                if (only_non_static_members_a)
                {
                    for (const auto& member_ : func_members_)
                    {
                        if (!member_->IsStatic())
                            ++n_;
                    }
                } else
                    n_ += func_members_.size();

                return n_;
            }

            void StructDecl::CollectMemberTypeDenoters(
                    std::vector<TypeDenoterPtr>& member_type_dens_a,
                    bool include_base_structs_a) const
            {
                if (base_struct_ref_ && include_base_structs_a)
                    base_struct_ref_->CollectMemberTypeDenoters(
                            member_type_dens_a,
                            include_base_structs_a);

                for (const auto& member_ : var_members_)
                {
                    for (const auto& var_decl_ : member_->var_decls_)
                        member_type_dens_a.push_back(var_decl_->GetTypeDenoter());
                }
            }

            void StructDecl::ForEachVarDecl(
                    const VarDeclIteratorFunctor& iterator_a,
                    bool include_base_structs_a)
            {
                if (base_struct_ref_ && include_base_structs_a)
                    base_struct_ref_->ForEachVarDecl(iterator_a, include_base_structs_a);

                for (auto& member_ : var_members_)
                {
                    const auto& type_den_ = member_->type_specifier_
                                                  ->GetTypeDenoter()
                                                  ->GetAliased();
                    if (auto struct_type_den_ = type_den_.As<StructTypeDenoter>())
                    {
                        if (auto struct_decl_ = struct_type_den_->struct_decl_ref_)
                            struct_decl_->ForEachVarDecl(iterator_a);
                    }

                    member_->ForEachVarDecl(iterator_a);
                }
            }

            void StructDecl::AddShaderOutputInstance(VarDecl* var_decl_a)
            {
                if (var_decl_a)
                    shader_output_var_decl_refs_.insert(var_decl_a);
            }

            bool StructDecl::HasMultipleShaderOutputInstances() const
            {
                auto num_instances_ = shader_output_var_decl_refs_.size();
                if (num_instances_ == 1)
                {
                    auto var_decl_ = *shader_output_var_decl_refs_.begin();
                    return (!var_decl_->array_dAzs_.empty());
                }
                return (num_instances_ > 1);
            }

            bool StructDecl::IsBaseOf(const StructDecl* sub_struct_decl_a,
                                      bool include_self_a) const
            {
                if (sub_struct_decl_a)
                {
                    if (include_self_a && sub_struct_decl_a == this)
                        return true;

                    if (auto base_struct_decl_ = sub_struct_decl_a->base_struct_ref_)
                    {
                        if (base_struct_decl_ == this)
                            return true;
                        else
                        {
                            return IsBaseOf(base_struct_decl_);
                        }
                    }
                }
                return false;
            }

            void StructDecl::AddFlagsRecursive(unsigned int struct_flags_a)
            {
                if (flags_.SetOnce(struct_flags_a))
                {
                    if (base_struct_ref_)
                        base_struct_ref_->AddFlagsRecursive(struct_flags_a);

                    for (auto& member_ : var_members_)
                    {
                        const auto& type_den_ = member_->type_specifier_
                                                      ->GetTypeDenoter()
                                                      ->GetAliased();
                        if (auto struct_type_den_ =
                                    type_den_.As<StructTypeDenoter>())
                        {
                            if (auto struct_decl_ = struct_type_den_->struct_decl_ref_)
                                struct_decl_->AddFlagsRecursive(struct_flags_a);
                        }
                    }
                }
            }

            void StructDecl::AddFlagsRecursiveParents(unsigned int struct_flags_a)
            {
                if (flags_.SetOnce(struct_flags_a))
                {
                    for (const auto parent_ : parent_struct_decl_refs_)
                        parent_->AddFlagsRecursiveParents(struct_flags_a);
                }
            }

            std::size_t StructDecl::MemberVarToIndex(
                    const VarDecl* var_decl_a,
                    bool include_base_structs_a) const
            {
                static const std::size_t invalid_idx_ = (std::size_t)~0;

                std::size_t idx_ = 0;

                if (base_struct_ref_ && include_base_structs_a)
                {
                    auto base_idx_ =
                            base_struct_ref_->MemberVarToIndex(var_decl_a,
                                                            include_base_structs_a);
                    if (base_idx_ != invalid_idx_)
                        idx_ = base_idx_;
                }

                for (const auto& member_ : var_members_)
                {
                    for (const auto& var_ : member_->var_decls_)
                    {
                        if (var_decl_a == var_.get())
                            return idx_;
                        ++idx_;
                    }
                }

                return invalid_idx_;
            }

            static VarDecl* FindIndexOfStructMemberVar(
                    const StructDecl& struct_decl_a,
                    std::size_t& idx_a,
                    bool include_base_structs_a)
            {
                if (struct_decl_a.base_struct_ref_ && include_base_structs_a)
                {
                    if (auto var_decl_ = FindIndexOfStructMemberVar(
                                *struct_decl_a.base_struct_ref_,
                                idx_a,
                                include_base_structs_a))
                        return var_decl_;
                }

                for (const auto& member_ : struct_decl_a.var_members_)
                {
                    if (idx_a < member_->var_decls_.size())
                        return member_->var_decls_[idx_a].get();
                    else
                        idx_a -= member_->var_decls_.size();
                }

                return nullptr;
            }

            VarDecl* StructDecl::IndexToMemberVar(std::size_t idx_a,
                                                  bool include_base_structs_a) const
            {
                return FindIndexOfStructMemberVar(*this,
                                                  idx_a,
                                                  include_base_structs_a);
            }

            bool StructDecl::AccumAlignedVectorSize(unsigned int& size_a,
                                                    unsigned int& padding_a,
                                                    unsigned int* offset_a)
            {
                for (const auto& member_ : var_members_)
                {
                    if (!member_->AccumAlignedVectorSize(size_a, padding_a, offset_a))
                        return false;
                    offset_a = nullptr;
                }
                return true;
            }

            TypeDenoterPtr AliasDecl::DeriveTypeDenoter(const TypeDenoter*)
            {
                return type_denoter_;
            }

            void FunctionDecl::ParameterSemantics::Add(VarDecl* var_decl_a)
            {
                if (var_decl_a)
                {
                    if (var_decl_a->flags_(VarDecl::IsSystemValue))
                        var_decl_refs_sv_.push_back(var_decl_a);
                    else
                        var_decl_refs_.push_back(var_decl_a);
                }
            }

            bool FunctionDecl::ParameterSemantics::Contains(
                    VarDecl* var_decl_a) const
            {
                return ((std::find(var_decl_refs_.begin(),
                                   var_decl_refs_.end(),
                                   var_decl_a)
                         != var_decl_refs_.end())
                        || (std::find(var_decl_refs_sv_.begin(),
                                      var_decl_refs_sv_.end(),
                                      var_decl_a)
                            != var_decl_refs_sv_.end()));
            }

            void FunctionDecl::ParameterSemantics::ForEach(
                    const IteratorFunc& iterator_a)
            {
                std::for_each(var_decl_refs_.begin(), var_decl_refs_.end(), iterator_a);
                std::for_each(var_decl_refs_sv_.begin(),
                              var_decl_refs_sv_.end(),
                              iterator_a);
            }

            bool FunctionDecl::ParameterSemantics::Empty() const
            {
                return (var_decl_refs_.empty() && var_decl_refs_sv_.empty());
            }

            void FunctionDecl::ParameterSemantics::UpdateDistribution()
            {
                for (auto it_ = var_decl_refs_.begin(); it_ != var_decl_refs_.end();)
                {
                    if ((*it_)->semantic_.IsSystemValue())
                    {
                        var_decl_refs_sv_.push_back(*it_);
                        it_ = var_decl_refs_.erase(it_);
                    } else
                        ++it_;
                }

                for (auto it_ = var_decl_refs_sv_.begin();
                     it_ != var_decl_refs_sv_.end();)
                {
                    if (!(*it_)->semantic_.IsSystemValue())
                    {
                        var_decl_refs_.push_back(*it_);
                        it_ = var_decl_refs_sv_.erase(it_);
                    } else
                        ++it_;
                }
            }

            TypeDenoterPtr FunctionDecl::DeriveTypeDenoter(const TypeDenoter*)
            {
                return std::make_shared<FunctionTypeDenoter>(this);
            }

            bool FunctionDecl::IsForwardDecl() const
            {
                return (code_block_ == nullptr);
            }

            bool FunctionDecl::HasVoidReturnType() const
            {
                return return_type_->type_denoter_->IsVoid();
            }

            bool FunctionDecl::IsMemberFunction() const
            {
                return (struct_decl_ref_ != nullptr);
            }

            bool FunctionDecl::IsStatic() const
            {
                return return_type_->HasAnyStorageClassOf({StorageClass::Static});
            }

            std::string FunctionDecl::ToString() const
            {
                return ToString(true);
            }

            std::string FunctionDecl::ToString(bool use_param_names_a) const
            {
                std::string s_;

                s_ += return_type_->ToString();
                s_ += ' ';

                if (struct_decl_ref_)
                {
                    s_ += struct_decl_ref_->ident_.Original();
                    s_ += "::";
                }

                s_ += ident_.Original();
                s_ += '(';

                for (std::size_t i_ = 0; i_ < parameters_.size(); ++i_)
                {
                    if (!parameters_[i_]->flags_(VarDeclStmnt::isSelfParameter))
                    {
                        s_ += parameters_[i_]->ToString(use_param_names_a);
                        if (i_ + 1 < parameters_.size())
                            s_ += ", ";
                    }
                }

                s_ += ')';

                return s_;
            }

            std::string FunctionDecl::ToTypeDenoterString() const
            {
                std::string s_;
                s_ += return_type_->ToString();
                s_ += '(';

                for (std::size_t i_ = 0; i_ < parameters_.size(); ++i_)
                {
                    if (!parameters_[i_]->flags_(VarDeclStmnt::isSelfParameter))
                    {
                        s_ += parameters_[i_]->ToString(false);
                        if (i_ + 1 < parameters_.size())
                            s_ += ", ";
                    }
                }

                s_ += ')';

                return s_;
            }

            bool FunctionDecl::EqualsSignature(const FunctionDecl& rhs_a,
                                               const Flags& compare_flags_a) const
            {
                if (parameters_.size() != rhs_a.parameters_.size())
                    return false;

                for (std::size_t i_ = 0; i_ < parameters_.size(); ++i_)
                {
                    const auto& lhs_type_den_ =
                            parameters_[i_]->var_decls_.front()->GetTypeDenoter();
                    const auto& rhs_type_den_ = rhs_a.parameters_[i_]
                                                     ->var_decls_.front()
                                                     ->GetTypeDenoter();

                    if (!lhs_type_den_->Equals(*rhs_type_den_, compare_flags_a))
                        return false;
                }

                return true;
            }

            std::size_t FunctionDecl::NumMinArgs() const
            {
                std::size_t n_ = 0;

                for (const auto& param_ : parameters_)
                {
                    if (!param_->var_decls_.empty()
                        && param_->var_decls_.front()->initializer_ != nullptr)
                        break;
                    ++n_;
                }

                return n_;
            }

            std::size_t FunctionDecl::NumMaxArgs() const
            {
                return parameters_.size();
            }

            void FunctionDecl::SetFuncAzplRef(FunctionDecl* func_decl_a)
            {
                if (func_decl_a && !func_decl_a->IsForwardDecl() && IsForwardDecl())
                {
                    func_Azpl_ref_ = func_decl_a;
                    func_decl_a->func_forward_decl_refs_.push_back(this);
                }
            }

            bool FunctionDecl::MatchParameterWithTypeDenoter(
                    std::size_t param_index_a,
                    const TypeDenoter& arg_type_a,
                    bool Azplicit_conversion_a) const
            {
                if (param_index_a >= parameters_.size())
                    return false;

                const auto& param_ = parameters_[param_index_a];
                if (param_->var_decls_.size() != 1)
                    return false;

                const auto& param_var_ = param_->var_decls_.front();
                const auto& param_type_den_ = param_var_->GetTypeDenoter();

                if (!arg_type_a.Equals(*param_type_den_))
                {
                    if (Azplicit_conversion_a)
                    {
                        if (!arg_type_a.IsCastableTo(*param_type_den_))
                            return false;
                    } else
                        return false;
                }

                return true;
            }

            static bool MatchNumArgsToFunctionDecls(
                    const std::vector<FunctionDecl*>& func_decl_list_a,
                    std::size_t num_args_a)
            {
                for (auto func_decl_ : func_decl_list_a)
                {
                    if (num_args_a >= func_decl_->NumMinArgs()
                        && num_args_a <= func_decl_->NumMaxArgs())
                        return true;
                }
                return false;
            }

            static bool MatchFunctionDeclWithArgs(
                    FunctionDecl& func_decl_a,
                    const std::vector<TypeDenoterPtr>& type_dens_a,
                    bool Azplicit_type_conversion_a)
            {
                auto num_args_ = type_dens_a.size();
                if (num_args_ >= func_decl_a.NumMinArgs()
                    && num_args_ <= func_decl_a.NumMaxArgs())
                {
                    for (std::size_t i_ = 0,
                                     n_ = std::min(type_dens_a.size(),
                                                   func_decl_a.parameters_.size());
                         i_ < n_;
                         ++i_)
                    {
                        if (!func_decl_a.MatchParameterWithTypeDenoter(
                                    i_,
                                    *type_dens_a[i_],
                                    Azplicit_type_conversion_a))
                            return false;
                    }

                    return true;
                }

                return false;
            }

            static void ListAllFuncCandidates(
                    const std::vector<FunctionDecl*>& candidates_a)
            {
                ReportHandler::HintForNextReport(R_CandidatesAre + ":");
                for (auto func_decl_ : candidates_a)
                    ReportHandler::HintForNextReport(
                            "  '" + func_decl_->ToString(false) + "' ("
                            + func_decl_->area_.Pos().ToString() + ")");
            };

            FunctionDecl* FunctionDecl::FetchFunctionDeclFromList(
                    const std::vector<FunctionDecl*>& func_decl_list_a,
                    const std::string& ident_a,
                    const std::vector<TypeDenoterPtr>& arg_type_denoters_a,
                    bool throw_error_if_no_match_a)
            {
                if (func_decl_list_a.empty())
                {
                    if (throw_error_if_no_match_a)
                        RuntAzeErr(R_UndefinedSymbol(ident_a));
                    else
                        return nullptr;
                }

                const auto num_args_ = arg_type_denoters_a.size();

                if (!MatchNumArgsToFunctionDecls(func_decl_list_a, num_args_))
                {
                    if (throw_error_if_no_match_a)
                    {
                        ListAllFuncCandidates(func_decl_list_a);

                        if (num_args_ == 1)
                            RuntAzeErr(R_FuncDoesntTake1Param(ident_a, num_args_));
                        else
                            RuntAzeErr(R_FuncDoesntTakeNParams(ident_a, num_args_));
                    } else
                        return nullptr;
                }

                std::vector<FunctionDecl*> func_decl_candidates_;

                for (auto func_decl_ : func_decl_list_a)
                {
                    if (MatchFunctionDeclWithArgs(*func_decl_,
                                                  arg_type_denoters_a,
                                                  false))
                        func_decl_candidates_.push_back(func_decl_);
                }

                if (func_decl_candidates_.empty())
                {
                    for (auto funcDecl : func_decl_list_a)
                    {
                        if (MatchFunctionDeclWithArgs(*funcDecl,
                                                      arg_type_denoters_a,
                                                      true))
                            func_decl_candidates_.push_back(funcDecl);
                    }
                }

                if (func_decl_candidates_.size() != 1)
                {
                    if (func_decl_candidates_.empty() && !throw_error_if_no_match_a)
                        return nullptr;

                    std::string arg_type_names_;

                    if (num_args_ > 0)
                    {
                        for (std::size_t i_ = 0; i_ < num_args_; ++i_)
                        {
                            arg_type_names_ += arg_type_denoters_a[i_]->ToString();
                            if (i_ + 1 < num_args_)
                                arg_type_names_ += ", ";
                        }
                    } 
                    else
                        arg_type_names_ = "void";

                    if (func_decl_candidates_.empty())
                        ListAllFuncCandidates(func_decl_list_a);
                    else
                        ListAllFuncCandidates(func_decl_candidates_);

                    RuntAzeErr(R_AmbiguousFuncCall(ident_a, arg_type_names_));
                }

                return func_decl_candidates_.front();
            }

            TypeDenoterPtr UniformBufferDecl::DeriveTypeDenoter(
                    const TypeDenoter*)
            {
                RuntAzeErr(R_CantDeriveTypeOfConstBuffer, this);
            }

            std::string UniformBufferDecl::ToString() const
            {
                std::string s_;

                switch (buffer_type_)
                {
                case UniformBufferType::Undefined:
                    s_ = R_Undefined;
                    break;
                case UniformBufferType::ConstantBuffer:
                    s_ = "cbuffer";
                    break;
                case UniformBufferType::TextureBuffer:
                    s_ = "tbuffer";
                    break;
                }

                s_ += ' ';
                s_ += ident_;

                return s_;
            }

            TypeModifier UniformBufferDecl::DeriveCommonStorageLayout(
                    const TypeModifier default_storge_layout_a)
            {
                std::size_t num_row_majors_ = 0, num_col_majors_ = 0;

                for (const auto& var_decl_stmnt_ : var_members_)
                {
                    const auto& type_modifers_ =
                            var_decl_stmnt_->type_specifier_->type_modifiers_;
                   
                    if (type_modifers_.find(TypeModifier::RowMajor)
                        != type_modifers_.end())
                        ++num_row_majors_;
                    
                    else if (type_modifers_.find(TypeModifier::ColumnMajor)
                             != type_modifers_.end())
                        ++num_col_majors_;
                }

                if (num_row_majors_ > 0 && num_row_majors_ >= num_col_majors_)
                    common_storage_layout_ = TypeModifier::RowMajor;
                else if (num_col_majors_ > 0)
                    common_storage_layout_ = TypeModifier::ColumnMajor;
                else if (default_storge_layout_a != TypeModifier::Undefined)
                    common_storage_layout_ = default_storge_layout_a;

                return common_storage_layout_;
            }

            bool UniformBufferDecl::AccumAlignedVectorSize(
                    unsigned int& size_a,
                    unsigned int& padding_a,
                    unsigned int* offset_a)
            {
                if (buffer_type_ == UniformBufferType::ConstantBuffer)
                {
                    for (const auto& member_ : var_members_)
                    {
                        member_->AccumAlignedVectorSize(size_a, padding_a, offset_a);
                        offset_a = nullptr;
                    }

                    auto remaining_padding_ = RemainingVectorSize(size_a);
                    size_a += remaining_padding_;
                    padding_a += remaining_padding_;

                    return true;
                }
                return false;
            }

            void BufferDeclStmnt::CollectDeclIdents(
                    std::map<const AST*, std::string>& decl_ast_idents_a) const
            {
                for (const auto& ast_ : buffer_decls_)
                    decl_ast_idents_a[ast_.get()] = ast_->ident_;
            }

            void SamplerDeclStmnt::CollectDeclIdents(
                    std::map<const AST*, std::string>& decl_ast_idents_a) const
            {
                for (const auto& ast_ : sampler_decls_)
                    decl_ast_idents_a[ast_.get()] = ast_->ident_;
            }

            void VarDeclStmnt::CollectDeclIdents(
                    std::map<const AST*, std::string>& decl_ast_idents_a) const
            {
                for (const auto& ast_ : var_decls_)
                    decl_ast_idents_a[ast_.get()] = ast_->ident_;
            }

            std::string VarDeclStmnt::ToString(bool use_var_names_a) const
            {
                auto s_ = type_specifier_->ToString();

                if (use_var_names_a)
                {
                    for (std::size_t i_ = 0; i_ < var_decls_.size(); ++i_)
                    {
                        s_ += ' ';
                        s_ += var_decls_[i_]->ToString();
                        if (i_ + 1 < var_decls_.size())
                            s_ += ',';
                    }
                } 
                else if (var_decls_.size() == 1)
                {
                    for (const auto& dAz_ : var_decls_.front()->array_dAzs_)
                    {
                        s_ += '[';
                        if (dAz_->size_ > 0)
                            s_ += std::to_string(dAz_->size_);
                        s_ += ']';
                    }
                }

                if (flags_(VarDeclStmnt::IsParameter) && !var_decls_.empty()
                    && var_decls_.front()->initializer_)
                    return '[' + s_ + ']';

                return s_;
            }

            VarDecl* VarDeclStmnt::FetchVarDecl(const std::string& ident_a) const
            {
                for (const auto& var_ : var_decls_)
                {
                    if (var_->ident_.Original() == ident_a)
                        return var_.get();
                }
                return nullptr;
            }

            VarDecl* VarDeclStmnt::FetchUniqueVarDecl() const
            {
                if (var_decls_.size() == 1)
                    return var_decls_.front().get();
                else
                    return nullptr;
            }

            bool VarDeclStmnt::IsInput() const
            {
                return type_specifier_->IsInput();
            }

            bool VarDeclStmnt::IsOutput() const
            {
                return type_specifier_->IsOutput();
            }

            bool VarDeclStmnt::IsUniform() const
            {
                return type_specifier_->is_uniform_;
            }

            bool VarDeclStmnt::IsConstOrUniform() const
            {
                return type_specifier_->IsConstOrUniform();
            }

            void VarDeclStmnt::SetTypeModifier(const TypeModifier modifier_a)
            {
                type_specifier_->SetTypeModifier(modifier_a);
            }

            bool VarDeclStmnt::HasAnyTypeModifierOf(
                    const std::initializer_list<TypeModifier>& modifiers_a) const
            {
                return type_specifier_->HasAnyTypeModifierOf(modifiers_a);
            }

            void VarDeclStmnt::ForEachVarDecl(
                    const VarDeclIteratorFunctor& iterator_a)
            {
                if (iterator_a)
                {
                    for (auto& var_decl_ : var_decls_)
                        iterator_a(var_decl_);
                }
            }

            void VarDeclStmnt::MakeAzplicitConst()
            {
                if (!IsConstOrUniform()
                    && !type_specifier_->HasAnyStorageClassOf(
                            {StorageClass::Static, StorageClass::GroupShared}))
                {
                    for (const auto& var_decl_ : var_decls_)
                    {
                        if (var_decl_->namespace_expr_ != nullptr)
                            return;
                    }

                    flags_ << VarDeclStmnt::isAzplicitConst;
                    type_specifier_->is_uniform_ = true;
                }
            }

            StructDecl* VarDeclStmnt::FetchStructDeclRef() const
            {
                if (var_decls_.empty())
                    return nullptr;
                else
                    return var_decls_.front()->struct_decl_ref_;
            }

            bool VarDeclStmnt::AccumAlignedVectorSize(unsigned int& size_a,
                                                      unsigned int& padding_a,
                                                      unsigned int* offset_a)
            {
                for (const auto& var_decl_ : var_decls_)
                {
                    if (!var_decl_->AccumAlignedVectorSize(size_a, padding_a, offset_a))
                        return false;
                    offset_a = nullptr;
                }
                return true;
            }

            TypeDenoterPtr NullExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                return std::make_shared<BaseTypeDenoter>(DataType::Int);
            }

            TypeDenoterPtr SequenceExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                return exprs_.front()->GetTypeDenoter();
            }

            const Expr* SequenceExpr::Find(
                    const FindPredicateConstFunctor& predicate_a,
                    unsigned int flags_a) const
            {
                if (predicate_a)
                {
                    CALL_EXPR_FIND_PREDICATE(predicate_a);

                    for (const auto& sub_expr_ : exprs_)
                    {
                        if (auto e_ = sub_expr_->Find(predicate_a, flags_a))
                            return e_;
                    }
                }
                return nullptr;
            }

            void SequenceExpr::Append(const ExprPtr& expr_a)
            {
                if (auto list_expr_ = expr_a->As<SequenceExpr>())
                {
                    exprs_.insert(exprs_.end(),
                                 list_expr_->exprs_.begin(),
                                 list_expr_->exprs_.end());
                } 
                else
                {
                    exprs_.push_back(expr_a);
                }
            }

            TypeDenoterPtr LiteralExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                if (IsNull())
                    return std::make_shared<NullTypeDenoter>();
                else
                    return std::make_shared<BaseTypeDenoter>(data_type_);
            }

            void LiteralExpr::ConvertDataType(const DataType type_a)
            {
                if (data_type_ != type_a)
                {
                    auto variant_ = Variant::ParseFrom(value_);

                    switch (type_a)
                    {
                    case DataType::Bool:
                        variant_ = variant_.ToBool();
                        value_ = variant_.ToString();
                        break;

                    case DataType::Int:
                        variant_ = variant_.ToInt();
                        value_ = variant_.ToString();
                        break;

                    case DataType::UInt:
                        variant_ = variant_.ToInt();
                        value_ = variant_.ToString() + "u";
                        break;

                    case DataType::Half:
                        if (data_type_ != DataType::Double)
                        {
                            variant_ = variant_.ToReal();
                            value_ = variant_.ToString();
                        }
                        break;

                    case DataType::Float:
                        if (data_type_ != DataType::Double)
                        {
                            variant_ = variant_.ToReal();
                            value_ = variant_.ToString();
                        }
                        value_.push_back('f');
                        break;

                    case DataType::Double:
                        variant_ = variant_.ToReal();
                        value_ = variant_.ToString();
                        break;

                    default:
                        break;
                    }

                    data_type_ = type_a;
                    ResetTypeDenoter();
                }
            }

            std::string LiteralExpr::GetStringValue() const
            {
                if (data_type_ == DataType::String && value_.size() >= 2
                    && value_.front() == '\"' && value_.back() == '\"')
                    return value_.substr(1, value_.size() - 2);
                else
                    return "";
            }

            std::string LiteralExpr::GetLiteralValue(bool enable_suffix_a) const
            {
                if (!enable_suffix_a && !value_.empty())
                {
                    auto suffix_ = value_.back();
                    if ((data_type_ == DataType::UInt
                         && (suffix_ == 'u' || suffix_ == 'U'))
                        || (data_type_ == DataType::Float
                            && (suffix_ == 'f' || suffix_ == 'F')))
                    {
                        return value_.substr(0, value_.size() - 1);
                    }
                }
                return value_;
            }

            bool LiteralExpr::IsNull() const
            {
                return (data_type_ == DataType::Undefined && value_ == "NULL");
            }

            bool LiteralExpr::IsSpaceRequiredForSubscript() const
            {
                return (!value_.empty() && value_.find('.') == std::string::npos
                        && std::isdigit(static_cast<int>(value_.back())));
            }

            TypeDenoterPtr TypeSpecifierExpr::DeriveTypeDenoter(
                    const TypeDenoter*)
            {
                return type_specifier_->GetTypeDenoter();
            }

            TypeDenoterPtr TernaryExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                const auto& cond_type_den_ = cond_expr_->GetTypeDenoter();
                const BaseTypeDenoter bool_type_den_(DataType::Bool);

                if (!cond_type_den_->IsCastableTo(bool_type_den_))
                    RuntAzeErr(R_IllegalCast(cond_type_den_->ToString(),
                                             bool_type_den_.ToString(),
                                             R_ConditionOfTernaryExpr),
                               cond_expr_.get());

                const auto& then_type_den_ =
                        then_expr_->GetTypeDenoter()->GetAliased();
                const auto& else_type_den_ =
                        else_expr_->GetTypeDenoter()->GetAliased();

                if (!else_type_den_.IsCastableTo(then_type_den_))
                    RuntAzeErr(R_IllegalCast(else_type_den_.ToString(),
                                             then_type_den_.ToString(),
                                             R_TernaryExpr),
                               this);

                auto common_type_den_ =
                        TypeDenoter::FindCommonTypeDenoterFrom(then_expr_,
                                                               else_expr_,
                                                               false,
                                                               this);
                const auto& cond_type_den_aliased_ =
                        cond_expr_->GetTypeDenoter()->GetAliased();

                if (auto base_type_den_ = cond_type_den_aliased_.As<BaseTypeDenoter>())
                {
                    const auto cond_vec_size_ = VectorTypeDAz(
                            base_type_den_->data_type_);
                    if (cond_vec_size_ > 1)
                    {
                        if (auto base_sub_type_den_ =
                                    common_type_den_->As<BaseTypeDenoter>())
                        {
                            const auto sub_data_type_ =
                                    VectorDataType(base_sub_type_den_->data_type_,
                                                   cond_vec_size_);
                            return std::make_shared<BaseTypeDenoter>(
                                    sub_data_type_);
                        }
                    }
                }

                return common_type_den_;
            }

            const Expr* TernaryExpr::Find(
                    const FindPredicateConstFunctor& predicate_a,
                    unsigned int flags_a) const
            {
                if (predicate_a)
                {
                    CALL_EXPR_FIND_PREDICATE(predicate_a);

                    if ((flags_a & SearchRValue) != 0)
                    {
                        if (auto e_ = cond_expr_->Find(predicate_a, flags_a))
                            return e_;
                        if (auto e_ = then_expr_->Find(predicate_a, flags_a))
                            return e_;
                        if (auto e_ = else_expr_->Find(predicate_a, flags_a))
                            return e_;
                    }
                }
                return nullptr;
            }

            bool TernaryExpr::IsVectorCondition() const
            {
                const auto& cond_type_den_ =
                        cond_expr_->GetTypeDenoter()->GetAliased();

                if (auto base_type_den_ = cond_type_den_.As<BaseTypeDenoter>())
                {
                    const auto cond_vec_size_ = VectorTypeDAz(
                            base_type_den_->data_type_);
                    return (cond_vec_size_ > 1);
                }

                return false;
            }

            TypeDenoterPtr BinaryExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                const auto& lhs_type_den_ =
                        lhs_expr_->GetTypeDenoter()->GetAliased();
                const auto& rhs_type_den_ =
                        rhs_expr_->GetTypeDenoter()->GetAliased();

                if (!rhs_type_den_.IsCastableTo(lhs_type_den_)
                    || !lhs_type_den_.IsCastableTo(rhs_type_den_))
                    RuntAzeErr(R_IllegalCast(rhs_type_den_.ToString(),
                                             lhs_type_den_.ToString(),
                                             R_BinaryExpr(
                                                     BinaryOpToString(op_))),
                               this);

                if (auto common_type_den_ =
                            TypeDenoter::FindCommonTypeDenoterFrom(lhs_expr_,
                                                                   rhs_expr_,
                                                                   false,
                                                                   this))
                {
                    if (!lhs_type_den_.IsBase())
                        RuntAzeErr(R_OnlyBaseTypeAllowed(
                                           R_BinaryExpr(BinaryOpToString(op_)),
                                           lhs_type_den_.ToString()),
                                   this);
                    if (!rhs_type_den_.IsBase())
                        RuntAzeErr(R_OnlyBaseTypeAllowed(
                                           R_BinaryExpr(BinaryOpToString(op_)),
                                           rhs_type_den_.ToString()),
                                   this);

                    if (IsBooleanOp(op_))
                        return TypeDenoter::MakeBoolTypeWithDAzensionOf(
                                *common_type_den_);
                    else
                        return common_type_den_;
                }

                return nullptr;
            }

            const Expr* BinaryExpr::Find(
                    const FindPredicateConstFunctor& predicate_a,
                    unsigned int flags_a) const
            {
                if (predicate_a)
                {
                    CALL_EXPR_FIND_PREDICATE(predicate_a);

                    if ((flags_a & SearchRValue) != 0)
                    {
                        if (auto e_ = lhs_expr_->Find(predicate_a, flags_a))
                            return e_;
                        if (auto e_ = rhs_expr_->Find(predicate_a, flags_a))
                            return e_;
                    }
                }
                return nullptr;
            }

            TypeDenoterPtr UnaryExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                const auto& type_den_ = expr_->GetTypeDenoter();

                if (IsLogicalOp(op_))
                    return TypeDenoter::MakeBoolTypeWithDAzensionOf(*type_den_);
                else
                    return type_den_;
            }

            const Expr* UnaryExpr::Find(
                    const FindPredicateConstFunctor& predicate_a,
                    unsigned int flags_a) const
            {
                if (predicate_a)
                {
                    CALL_EXPR_FIND_PREDICATE(predicate_a);

                    if ((IsLValueOp(op_) && ((flags_a & SearchLValue) != 0))
                        || (!IsLValueOp(op_) && ((flags_a & SearchRValue) != 0)))
                    {
                        if (auto e_ = expr_->Find(predicate_a, flags_a))
                            return e_;
                    }
                }
                return nullptr;
            }

            const ObjectExpr* UnaryExpr::FetchLValueExpr() const
            {
                if (IsLValueOp(op_))
                    return expr_->FetchLValueExpr();
                else
                    return nullptr;
            }

            TypeDenoterPtr PostUnaryExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                return expr_->GetTypeDenoter();
            }

            const Expr* PostUnaryExpr::Find(
                    const FindPredicateConstFunctor& predicate_a,
                    unsigned int flags_a) const
            {
                if (predicate_a)
                {
                    CALL_EXPR_FIND_PREDICATE(predicate_a);

                    if ((flags_a & SearchRValue) != 0)
                    {
                        if (auto e_ = expr_->Find(predicate_a, flags_a))
                            return e_;
                    }
                }
                return nullptr;
            }

            TypeDenoterPtr CallExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                if (auto func_decl_ = GetFunctionDecl())
                {
                    return func_decl_->return_type_->type_denoter_;
                } 
                else if (type_denoter_)
                {
                    return type_denoter_;
                } 
                else if (intrinsic_ != Intrinsic::Undefined)
                {
                    try
                    {
                        auto member_func_obj_expr_ = GetMemberFuncObjectExpr();
                        return IntrinsicAdept::Get().GetIntrinsicReturnType(
                                intrinsic_,
                                arguments_,
                                (member_func_obj_expr_ != nullptr
                                         ? member_func_obj_expr_->GetTypeDenoter()
                                         : nullptr));
                    } 
                    catch (const std::exception& e_)
                    {
                        RuntAzeErr(e_.what(), this);
                    }
                } else
                    RuntAzeErr(R_MissingFuncRefToDeriveExprType, this);
            }

            const Expr* CallExpr::Find(
                    const FindPredicateConstFunctor& predicate_a,
                    unsigned int flags_a) const
            {
                if (predicate_a)
                {
                    CALL_EXPR_FIND_PREDICATE(predicate_a);

                    if ((flags_a & SearchRValue) != 0 && prefix_expr_)
                    {
                        if (auto e_ = prefix_expr_->Find(predicate_a, flags_a))
                            return e_;
                    }
                }
                return nullptr;
            }

            IndexedSemantic CallExpr::FetchSemantic() const
            {
                if (auto func_decl_ = GetFunctionDecl())
                    return func_decl_->semantic_;
                else
                    return Semantic::Undefined;
            }

            FunctionDecl* CallExpr::GetFunctionDecl() const
            {
                return func_decl_ref_;
            }

            FunctionDecl* CallExpr::GetFunctionAzpl() const
            {
                if (auto func_decl_ = GetFunctionDecl())
                {
                    if (func_decl_->func_Azpl_ref_)
                        return func_decl_->func_Azpl_ref_;
                    else
                        return func_decl_;
                }
                return nullptr;
            }

            void CallExpr::ForEachOutputArgument(
                    const ExprIteratorFunctor& iterator_a)
            {
                if (iterator_a)
                {
                    if (auto func_decl_ = GetFunctionDecl())
                    {
                        const auto& parameters_ = func_decl_->parameters_;
                        for (std::size_t i_ = 0,
                                         n_ = std::min(arguments_.size(),
                                                       parameters_.size());
                             i_ < n_;
                             ++i_)
                        {
                            if (parameters_[i_]->IsOutput())
                                iterator_a(arguments_[i_],
                                         parameters_[i_]->FetchUniqueVarDecl());
                        }
                    } 
                    else if (intrinsic_ != Intrinsic::Undefined)
                    {
                        const auto output_param_indices_ =
                                IntrinsicAdept::Get()
                                        .GetIntrinsicOutputParameterIndices(
                                                intrinsic_);
                        for (auto param_index_ : output_param_indices_)
                        {
                            if (param_index_ < arguments_.size())
                                iterator_a(arguments_[param_index_], nullptr);
                        }
                    }
                }
            }

            void CallExpr::ForEachArgumentWithParameterType(
                    const ArgumentParameterTypeFunctor& iterator_a)
            {
                if (iterator_a)
                {
                    if (auto func_decl_ = GetFunctionDecl())
                    {
                        const auto& parameters_ = func_decl_->parameters_;
                        for (std::size_t i_ = 0,
                                         n_ = std::min(arguments_.size(),
                                                       parameters_.size());
                             i_ < n_;
                             ++i_)
                        {
                            auto param_ = parameters_[i_]->var_decls_.front();
                            const auto& param_type_den_ =
                                    param_->GetTypeDenoter()->GetAliased();
                            iterator_a(arguments_[i_], param_type_den_);
                        }
                    } 
                    else if (intrinsic_ != Intrinsic::Undefined)
                    {
                        const auto param_type_denoters_ =
                                IntrinsicAdept::Get()
                                        .GetIntrinsicParameterTypes(intrinsic_,
                                                                    arguments_);
                        for (std::size_t i_ = 0,
                                         n_ = std::min(arguments_.size(),
                                                       param_type_denoters_.size());
                             i_ < n_;
                             ++i_)
                            iterator_a(arguments_[i_], *param_type_denoters_[i_]);
                    }
                }
            }

            void CallExpr::PushArgumentFront(const ExprPtr& expr_a)
            {
                arguments_.insert(arguments_.begin(), expr_a);
            }

            bool CallExpr::PushPrefixToArguments()
            {
                if (prefix_expr_)
                {
                    arguments_.insert(arguments_.begin(), std::move(prefix_expr_));
                    return true;
                }
                return false;
            }

            bool CallExpr::PopPrefixFromArguments()
            {
                if (!prefix_expr_ && !arguments_.empty())
                {
                    prefix_expr_ = std::move(arguments_.front());
                    arguments_.erase(arguments_.begin());
                    return true;
                }
                return false;
            }

            bool CallExpr::MergeArguments(std::size_t first_arg_index_a,
                                          const MergeExprFunctor& merge_functor_a)
            {
                if (first_arg_index_a + 1 < arguments_.size())
                {
                    arguments_[first_arg_index_a] =
                            merge_functor_a(arguments_[first_arg_index_a],
                                         arguments_[first_arg_index_a + 1]);
                    arguments_.erase(arguments_.begin() + first_arg_index_a + 1);
                    return true;
                }
                return false;
            }

            Expr* CallExpr::GetMemberFuncObjectExpr() const
            {
                if (prefix_expr_)
                    return prefix_expr_.get();
                if (!arguments_.empty())
                    return arguments_.front().get();
                return nullptr;
            }

            TypeDenoterPtr BracketExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                return expr_->GetTypeDenoter();
            }

            const Expr* BracketExpr::Find(
                    const FindPredicateConstFunctor& predicate_a,
                    unsigned int flags_a) const
            {
                if (predicate_a)
                {
                    CALL_EXPR_FIND_PREDICATE(predicate_a);

                    if (auto e_ = expr_->Find(predicate_a, flags_a))
                        return e_;
                }
                return nullptr;
            }

            const ObjectExpr* BracketExpr::FetchLValueExpr() const
            {
                return expr_->FetchLValueExpr();
            }

            IndexedSemantic BracketExpr::FetchSemantic() const
            {
                return expr_->FetchSemantic();
            }

            TypeDenoterPtr AssignExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                return lvalue_expr_->GetTypeDenoter();
            }

            const Expr* AssignExpr::Find(
                    const FindPredicateConstFunctor& predicate_a,
                    unsigned int flags_a) const
            {
                if (predicate_a)
                {
                    CALL_EXPR_FIND_PREDICATE(predicate_a);

                    if ((flags_a & SearchLValue) != 0)
                    {
                        if (auto e_ = lvalue_expr_->Find(predicate_a, flags_a))
                            return e_;
                    }
                    if ((flags_ & SearchRValue) != 0)
                    {
                        if (auto e_ = rvalue_expr_->Find(predicate_a, flags_))
                            return e_;
                    }
                }
                return nullptr;
            }

            const ObjectExpr* AssignExpr::FetchLValueExpr() const
            {
                return lvalue_expr_->FetchLValueExpr();
            }

            TypeDenoterPtr ObjectExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                if (symbol_ref_)
                {
                    return symbol_ref_->GetTypeDenoter();
                } 
                else if (prefix_expr_)
                {
                    if (is_static_)
                        RuntAzeErr(R_IllegalStaticAccessForSubscript(ident_),
                                   this);
                    else
                        return prefix_expr_->GetTypeDenoter()->GetSub(this);
                }

                RuntAzeErr(R_UnknownTypeOfObjectIdentSymbolRef(ident_), this);
            }

            const Expr* ObjectExpr::Find(
                    const FindPredicateConstFunctor& predicate_a,
                    unsigned int flags_a) const
            {
                if (predicate_a)
                {
                    CALL_EXPR_FIND_PREDICATE(predicate_a);

                    if (prefix_expr_)
                    {
                        if (auto e_ = prefix_expr_->Find(predicate_a, flags_a))
                            return e_;
                    }
                }
                return nullptr;
            }

            const ObjectExpr* ObjectExpr::FetchLValueExpr() const
            {
                if (symbol_ref_)
                {
                    switch (symbol_ref_->Type())
                    {
                    case AST::Types::VarDecl:
                    case AST::Types::BufferDecl:
                    case AST::Types::SamplerDecl:
                        return this;
                    case AST::Types::StructDecl:
                        if (prefix_expr_)
                            return prefix_expr_->FetchLValueExpr();
                        else
                            return nullptr;
                    default:
                        return nullptr;
                    }
                } 
                else if (prefix_expr_)
                {
                    return prefix_expr_->FetchLValueExpr();
                }
                return this;
            }

            IndexedSemantic ObjectExpr::FetchSemantic() const
            {
                if (symbol_ref_)
                {
                    if (auto var_decl_ = symbol_ref_->As<VarDecl>())
                        return var_decl_->semantic_;
                } 
                else if (prefix_expr_)
                {
                    return prefix_expr_->FetchSemantic();
                }
                return Semantic::Undefined;
            }

            bool ObjectExpr::IsTrivialCopyable(unsigned int max_tree_depth_a) const
            {
                if (symbol_ref_)
                {
                    if (auto var_decl_ = symbol_ref_->As<VarDecl>())
                    {
                        if (prefix_expr_)
                        {
                            if (max_tree_depth_a > 0)
                                return prefix_expr_->IsTrivialCopyable(
                                        max_tree_depth_a - 1);
                        } 
                        else
                            return true;
                    }
                }
                return false;
            }

            BaseTypeDenoterPtr ObjectExpr::GetTypeDenoterFromSubscript() const
            {
                if (prefix_expr_)
                {
                    const auto& prefix_type_den_ =
                            prefix_expr_->GetTypeDenoter()->GetAliased();
                    if (auto base_type_den_ = prefix_type_den_.As<BaseTypeDenoter>())
                    {
                        try
                        {
                            auto vector_type_ =
                                    SubscriptDataType(base_type_den_->data_type_,
                                                      ident_);
                            return std::make_shared<BaseTypeDenoter>(
                                    vector_type_);
                        } 
                        catch (const std::exception& e_)
                        {
                            RuntAzeErr(e_.what(), this);
                        }
                    }
                }
                RuntAzeErr(R_InvalidSubscriptBaseType, this);
            }

            std::string ObjectExpr::ToStringAsNamespace() const
            {
                std::string s_;

                if (prefix_expr_)
                {
                    if (auto sub_object_expr_ = prefix_expr_->As<ObjectExpr>())
                    {
                        s_ += sub_object_expr_->ToStringAsNamespace();
                        s_ += "::";
                    }
                }

                s_ += ident_;

                return s_;
            }

            void ObjectExpr::ReplaceSymbol(Decl* symbol_a)
            {
                if (symbol_a)
                {
                    symbol_ref_ = symbol_a;
                    ident_ = symbol_a->ident_;
                }
            }

            VarDecl* ObjectExpr::FetchVarDecl() const
            {
                return FetchSymbol<VarDecl>();
            }

            TypeDenoterPtr ArrayExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                try
                {
                    return prefix_expr_->GetTypeDenoter()->GetSub(this);
                } 
                catch (const std::exception& e_)
                {
                    RuntAzeErr(e_.what(), this);
                }
            }

            const Expr* ArrayExpr::Find(
                    const FindPredicateConstFunctor& predicate_a,
                    unsigned int flags_a) const
            {
                if (predicate_a)
                {
                    CALL_EXPR_FIND_PREDICATE(predicate_a);

                    if (auto e_ = prefix_expr_->Find(predicate_a, flags_a))
                        return e_;
                }
                return nullptr;
            }

            const ObjectExpr* ArrayExpr::FetchLValueExpr() const
            {
                return prefix_expr_->FetchLValueExpr();
            }

            std::size_t ArrayExpr::NumIndices() const
            {
                return array_indices_.size();
            }

            TypeDenoterPtr CastExpr::DeriveTypeDenoter(const TypeDenoter*)
            {
                const auto& cast_type_den_ = type_specifier_->GetTypeDenoter();
                const auto& value_type_den_ = expr_->GetTypeDenoter();

                if (!value_type_den_->IsCastableTo(*cast_type_den_))
                    RuntAzeErr(R_IllegalCast(value_type_den_->ToString(),
                                             cast_type_den_->ToString(),
                                             R_CastExpr),
                               this);

                return cast_type_den_;
            }

            const Expr* CastExpr::Find(
                    const FindPredicateConstFunctor& predicate_a,
                    unsigned int flags_a) const
            {
                if (predicate_a)
                {
                    CALL_EXPR_FIND_PREDICATE(predicate_a);

                    if (auto e_ = expr_->Find(predicate_a, flags_a))
                        return e_;
                }
                return nullptr;
            }

            TypeDenoterPtr InitializerExpr::DeriveTypeDenoter(
                    const TypeDenoter* expected_type_denoter_a)
            {
                if (!expected_type_denoter_a)
                    RuntAzeErr(R_CantDeriveTypeOfInitializer, this);
                if (exprs_.empty())
                    RuntAzeErr(R_CantDeriveTypeOfEmptyInitializer, this);

                const auto num_elements_unrolled_ = NumElementsUnrolled();
                const auto& type_den_ = expected_type_denoter_a->GetAliased();
                if (auto base_type_den_ = type_den_.As<BaseTypeDenoter>())
                {
                    UnrollElements();

                    const auto data_type_ = base_type_den_->data_type_;
                    if (IsScalarType(data_type_))
                    {
                        if (num_elements_unrolled_ != 1)
                            RuntAzeErr(R_InvalidNumElementsInInitializer(
                                               expected_type_denoter_a->ToString(),
                                               std::size_t(1u),
                                               num_elements_unrolled_),
                                       this);

                        const auto& expr0_type_den_ = exprs_[0]->GetTypeDenoter();
                        if (!expr0_type_den_->IsCastableTo(*expected_type_denoter_a))
                            RuntAzeErr(R_IllegalCast(
                                               expr0_type_den_->ToString(),
                                               expected_type_denoter_a->ToString(),
                                               R_InitializerList),
                                       this);
                    }
                } 
                else if (auto array_type_den_ = type_den_.As<ArrayTypeDenoter>())
                {
                    for (auto& expr_ : exprs_)
                        expr_->GetTypeDenoter(
                                array_type_den_->sub_type_denoter_.get());
                }

                return expected_type_denoter_a->Copy();
            }

            const Expr* InitializerExpr::Find(
                    const FindPredicateConstFunctor& predicate_a,
                    unsigned int flags_a) const
            {
                if (predicate_a)
                {
                    CALL_EXPR_FIND_PREDICATE(predicate_a);

                    if ((flags_a & SearchRValue) != 0)
                    {
                        for (const auto& sub_expr_ : exprs_)
                        {
                            if (auto e_ = sub_expr_->Find(predicate_a, flags_))
                                return e_;
                        }
                    }
                }
                return nullptr;
            }

            std::size_t InitializerExpr::NumElementsUnrolled() const
            {
                std::size_t n_ = 0;

                for (const auto& e_ : exprs_)
                {
                    if (auto init_sub_expr_ = e_->FindFirstNotOf(
                                                    AST::Types::BracketExpr)
                                                   ->As<InitializerExpr>())
                        n_ += init_sub_expr_->NumElementsUnrolled();
                    else
                        ++n_;
                }

                return n_;
            }

            void InitializerExpr::CollectElements(
                    std::vector<ExprPtr>& elements_a) const
            {
                for (const auto& e_ : exprs_)
                {
                    if (auto init_sub_expr_ = e_->FindFirstNotOf(
                                                    AST::Types::BracketExpr)
                                                   ->As<InitializerExpr>())
                        init_sub_expr_->CollectElements(elements_a);
                    else
                        elements_a.push_back(e_);
                }
            }

            void InitializerExpr::UnrollElements()
            {
                for (const auto& e_ : exprs_)
                {
                    if (e_->Type() == AST::Types::InitializerExpr)
                    {
                        std::vector<ExprPtr> elements_;
                        CollectElements(elements_);
                        exprs_ = elements_;
                        break;
                    }
                }
            }

            static ExprPtr FetchSubExprFromInitializerExpr(
                    const InitializerExpr* ast_a,
                    const std::vector<int>& array_indices_a,
                    std::size_t layer_a)
            {
                if (layer_a < array_indices_a.size())
                {
                    auto idx_ = array_indices_a[layer_a];
                    if (idx_ >= 0
                        && static_cast<std::size_t>(idx_) < ast_a->exprs_.size())
                    {
                        auto expr_ = ast_a->exprs_[idx_];

                        if (layer_a + 1 == array_indices_a.size())
                        {
                            return expr_;
                        }

                        if (auto sub_init_expr_ =
                                    expr_->As<const InitializerExpr>())
                            return FetchSubExprFromInitializerExpr(sub_init_expr_,
                                                                   array_indices_a,
                                                                   layer_a + 1);

                        RuntAzeErr(R_ExpectedInitializerForArrayAccess,
                                   expr_.get());
                    }
                    RuntAzeErr(R_NotEnoughElementsInInitializer, ast_a);
                }
                RuntAzeErr(R_NotEnoughIndicesForInitializer, ast_a);
            }

            ExprPtr InitializerExpr::FetchSubExpr(
                    const std::vector<int>& array_indices_a) const
            {
                return FetchSubExprFromInitializerExpr(this, array_indices_a, 0);
            }

            static bool NextArrayIndicesFromInitializerExpr(
                    const InitializerExpr* ast_a,
                    std::vector<int>& array_indices_a,
                    std::size_t layer_a)
            {
                if (layer_a < array_indices_a.size())
                {
                    auto& idx_ = array_indices_a[layer_a];
                    if (idx_ >= 0
                        && static_cast<std::size_t>(idx_) < ast_a->exprs_.size())
                    {
                        auto expr_ = ast_a->exprs_[idx_];

                        if (auto sub_init_expr_ =
                                    expr_->As<const InitializerExpr>())
                        {
                            if (NextArrayIndicesFromInitializerExpr(
                                        sub_init_expr_,
                                        array_indices_a,
                                        layer_a + 1))
                                return true;
                        }

                        ++idx_;

                        if (static_cast<std::size_t>(idx_) == ast_a->exprs_.size())
                        {
                            idx_ = 0;
                            return false;
                        }

                        return true;
                    } 
                    else
                    {
                        idx_ = 0;
                    }
                }

                return false;
            }

            bool InitializerExpr::NextArrayIndices(
                    std::vector<int>& array_indices_a) const
            {
                return NextArrayIndicesFromInitializerExpr(this,
                                                           array_indices_a,
                                                           0);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
