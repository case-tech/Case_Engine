#include "TypeDenoter.hpp"

#include "../Exception.hpp"
#include "../Report/ReportIdents.hpp"
#include "AST.hpp"

#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            TypeDenoter::~TypeDenoter()
            {}

            bool TypeDenoter::Equals(const TypeDenoter& rhs_a,
                                     const Flags&) const
            {
                return (GetAliased().Type() == rhs_a.GetAliased().Type());
            }

            bool TypeDenoter::IsCastableTo(const TypeDenoter& target_type_a) const
            {
                return (GetAliased().Type() == target_type_a.GetAliased().Type());
            }

            bool TypeDenoter::AccumAlignedVectorSize(
                    unsigned int&,
                    unsigned int&,
                    unsigned int*) const
            {
                return false;
            }

            bool TypeDenoter::IsVoid() const
            {
                return (Type() == Types::Void);
            }

            bool TypeDenoter::IsNull() const
            {
                return (Type() == Types::Null);
            }

            bool TypeDenoter::IsBase() const
            {
                return (Type() == Types::Base);
            }

            bool TypeDenoter::IsScalar() const
            {
                if (auto base_type_den_ = As<BaseTypeDenoter>())
                    return IsScalarType(base_type_den_->data_type_);
                else
                    return false;
            }

            bool TypeDenoter::IsVector() const
            {
                if (auto base_type_den_ = As<BaseTypeDenoter>())
                    return IsVectorType(base_type_den_->data_type_);
                else
                    return false;
            }

            bool TypeDenoter::IsMatrix() const
            {
                if (auto base_type_den_ = As<BaseTypeDenoter>())
                    return IsMatrixType(base_type_den_->data_type_);
                else
                    return false;
            }

            bool TypeDenoter::IsSampler() const
            {
                return (Type() == Types::Sampler);
            }

            bool TypeDenoter::IsBuffer() const
            {
                return (Type() == Types::Buffer);
            }

            bool TypeDenoter::IsStruct() const
            {
                return (Type() == Types::Struct);
            }

            bool TypeDenoter::IsAlias() const
            {
                return (Type() == Types::Alias);
            }

            bool TypeDenoter::IsArray() const
            {
                return (Type() == Types::Array);
            }

            bool TypeDenoter::IsFunction() const
            {
                return (Type() == Types::Function);
            }

            TypeDenoterPtr TypeDenoter::GetSub(const Expr* expr_a)
            {
                if (expr_a)
                {
                    if (auto obj_expr_ = expr_a->As<ObjectExpr>())
                        return GetSubObject(obj_expr_->ident_, expr_a);
                    if (auto array_expr_ = expr_a->As<ArrayExpr>())
                        return GetSubArray(array_expr_->NumIndices(), expr_a);
                    RuntAzeErr(R_InvalidExprForSubTypeDen(ToString()), expr_a);
                }
                return shared_from_this();
            }

            TypeDenoterPtr TypeDenoter::GetSubObject(const std::string& ident_a,
                                                     const AST* ast_a)
            {
                RuntAzeErr(R_TypeHasNoSuchObject(ToString(), ident_a), ast_a);
            }

            TypeDenoterPtr TypeDenoter::GetSubArray(
                    const std::size_t num_array_indices_a,
                    const AST* ast_a)
            {
                if (num_array_indices_a > 0)
                    RuntAzeErr(R_IllegalArrayAccess(ToString()), ast_a);
                else
                    return shared_from_this();
            }

            const TypeDenoter& TypeDenoter::GetAliased() const
            {
                return *this;
            }

            std::string TypeDenoter::Ident() const
            {
                return "";
            }

            void TypeDenoter::SetIdentIfAnonymous(const std::string& ident_a)
            {
                (void)ident_a;
            }

            unsigned int TypeDenoter::NumDAzensions() const
            {
                return 0;
            }

            AST* TypeDenoter::SymbolRef() const
            {
                return nullptr;
            }

            TypeDenoterPtr TypeDenoter::AsArray(
                    const std::vector<ArrayDAzensionPtr>& array_dAzs_a)
            {
                if (array_dAzs_a.empty())
                    return shared_from_this();
                else
                    return std::make_shared<ArrayTypeDenoter>(
                            shared_from_this(),
                            array_dAzs_a);
            }

            TypeDenoter* TypeDenoter::FetchSubTypeDenoter() const
            {
                return nullptr;
            }

            static DataType HighestOrderDataType(
                    DataType lhs_a,
                    DataType rhs_a,
                    DataType highest_type_a = DataType::Float)
            {
                auto highest_order_ = std::max(
                        {static_cast<int>(lhs_a), static_cast<int>(rhs_a)});
                auto clamped_order_ = std::min(
                        {highest_order_, static_cast<int>(highest_type_a)});
                return static_cast<DataType>(clamped_order_);
            }

            static TypeDenoterPtr FindCommonTypeDenoterScalarAndScalar(
                    BaseTypeDenoter* lhs_type_den_a,
                    BaseTypeDenoter* rhs_type_den_a)
            {
                auto common_type_ = HighestOrderDataType(lhs_type_den_a->data_type_,
                                                       rhs_type_den_a->data_type_);
                return std::make_shared<BaseTypeDenoter>(common_type_);
            }

            static TypeDenoterPtr FindCommonTypeDenoterScalarAndVector(
                    BaseTypeDenoter* lhs_type_den_a,
                    BaseTypeDenoter* rhs_type_den_a,
                    bool use_min_dAzension_a)
            {
                auto common_type_ = HighestOrderDataType(
                        lhs_type_den_a->data_type_,
                        BaseDataType(rhs_type_den_a->data_type_));
                if (use_min_dAzension_a)
                {
                    return std::make_shared<BaseTypeDenoter>(common_type_);
                } 
                else
                {
                    auto rhs_dAz_ = VectorTypeDAz(rhs_type_den_a->data_type_);
                    return std::make_shared<BaseTypeDenoter>(
                            VectorDataType(common_type_, rhs_dAz_));
                }
            }

            static TypeDenoterPtr FindCommonTypeDenoterScalarAndMatrix(
                    BaseTypeDenoter* lhs_type_den_a,
                    BaseTypeDenoter* rhs_type_den_a,
                    bool use_min_dAzension_a)
            {
                auto common_type_ = HighestOrderDataType(
                        lhs_type_den_a->data_type_,
                        BaseDataType(rhs_type_den_a->data_type_));
                if (use_min_dAzension_a)
                {
                    return std::make_shared<BaseTypeDenoter>(common_type_);
                } 
                else
                {
                    auto rhs_dAz_ = MatrixTypeDAz(rhs_type_den_a->data_type_);
                    return std::make_shared<BaseTypeDenoter>(
                            MatrixDataType(common_type_,
                                           rhs_dAz_.first,
                                           rhs_dAz_.second));
                }
            }

            static TypeDenoterPtr FindCommonTypeDenoterVectorAndVector(
                    BaseTypeDenoter* lhs_type_den_a,
                    BaseTypeDenoter* rhs_type_den_a)
            {
                auto common_type_ = HighestOrderDataType(
                        BaseDataType(lhs_type_den_a->data_type_),
                        BaseDataType(rhs_type_den_a->data_type_));

                auto lhs_dAz_ = VectorTypeDAz(lhs_type_den_a->data_type_);
                auto rhs_dAz_ = VectorTypeDAz(rhs_type_den_a->data_type_);
                auto common_dAz_ = std::min(lhs_dAz_, rhs_dAz_);

                return std::make_shared<BaseTypeDenoter>(
                        VectorDataType(common_type_, common_dAz_));
            }

            static TypeDenoterPtr FindCommonTypeDenoterVectorAndMatrix(
                    BaseTypeDenoter* lhs_type_den_a,
                    BaseTypeDenoter* rhs_type_den_a,
                    bool row_vector_a)
            {
                auto common_type_ = HighestOrderDataType(
                        BaseDataType(lhs_type_den_a->data_type_),
                        BaseDataType(rhs_type_den_a->data_type_));

                auto matrix_dAz_ = MatrixTypeDAz(rhs_type_den_a->data_type_);
                auto common_dAz_ = (row_vector_a ? matrix_dAz_.first
                                            : matrix_dAz_.second);

                return std::make_shared<BaseTypeDenoter>(
                        VectorDataType(common_type_, common_dAz_));
            }

            static TypeDenoterPtr FindCommonTypeDenoterAnyAndAny(
                    TypeDenoter* lhs_type_den_a,
                    TypeDenoter* rhs_type_den_a)
            {
                (void)rhs_type_den_a;
                return lhs_type_den_a->GetSub();
            }

            TypeDenoterPtr TypeDenoter::FindCommonTypeDenoter(
                    const TypeDenoterPtr& lhs_type_den_a,
                    const TypeDenoterPtr& rhs_type_den_a,
                    bool use_min_dAzension_a)
            {
                if (lhs_type_den_a->IsScalar())
                {
                    if (rhs_type_den_a->IsScalar())
                        return FindCommonTypeDenoterScalarAndScalar(
                                lhs_type_den_a->As<BaseTypeDenoter>(),
                                rhs_type_den_a->As<BaseTypeDenoter>());

                    if (rhs_type_den_a->IsVector())
                        return FindCommonTypeDenoterScalarAndVector(
                                lhs_type_den_a->As<BaseTypeDenoter>(),
                                rhs_type_den_a->As<BaseTypeDenoter>(),
                                use_min_dAzension_a);

                    if (rhs_type_den_a->IsMatrix())
                        return FindCommonTypeDenoterScalarAndMatrix(
                                lhs_type_den_a->As<BaseTypeDenoter>(),
                                rhs_type_den_a->As<BaseTypeDenoter>(),
                                use_min_dAzension_a);
                } else if (lhs_type_den_a->IsVector())
                {
                    if (rhs_type_den_a->IsScalar())
                        return FindCommonTypeDenoterScalarAndVector(
                                rhs_type_den_a->As<BaseTypeDenoter>(),
                                lhs_type_den_a->As<BaseTypeDenoter>(),
                                use_min_dAzension_a);

                    if (rhs_type_den_a->IsVector())
                        return FindCommonTypeDenoterVectorAndVector(
                                lhs_type_den_a->As<BaseTypeDenoter>(),
                                rhs_type_den_a->As<BaseTypeDenoter>());

                    if (rhs_type_den_a->IsMatrix())
                        return FindCommonTypeDenoterVectorAndMatrix(
                                lhs_type_den_a->As<BaseTypeDenoter>(),
                                rhs_type_den_a->As<BaseTypeDenoter>(),
                                true);
                } else if (lhs_type_den_a->IsMatrix())
                {
                    if (rhs_type_den_a->IsScalar())
                        return FindCommonTypeDenoterScalarAndMatrix(
                                rhs_type_den_a->As<BaseTypeDenoter>(),
                                lhs_type_den_a->As<BaseTypeDenoter>(),
                                use_min_dAzension_a);

                    if (rhs_type_den_a->IsVector())
                        return FindCommonTypeDenoterVectorAndMatrix(
                                rhs_type_den_a->As<BaseTypeDenoter>(),
                                lhs_type_den_a->As<BaseTypeDenoter>(),
                                false);
                }

                return FindCommonTypeDenoterAnyAndAny(lhs_type_den_a.get(),
                                                      rhs_type_den_a.get());
            }

            TypeDenoterPtr TypeDenoter::FindCommonTypeDenoterFrom(
                    const ExprPtr& lhs_expr_a,
                    const ExprPtr& rhs_expr_a,
                    bool use_min_dAzension_a,
                    const AST* ast_a)
            {
                (void)ast_a;
                auto lhs_type_den_ = lhs_expr_a->GetTypeDenoter()->GetSub();
                auto rhs_type_den_ = rhs_expr_a->GetTypeDenoter()->GetSub();

                auto common_type_denoter_ =
                        TypeDenoter::FindCommonTypeDenoter(lhs_type_den_,
                                                           rhs_type_den_,
                                                           use_min_dAzension_a);

                return common_type_denoter_;
            }

            BaseTypeDenoterPtr TypeDenoter::MakeBoolTypeWithDAzensionOf(
                    const TypeDenoter& type_den_a)
            {
                if (auto base_type_den_ =
                            type_den_a.GetAliased().As<BaseTypeDenoter>())
                {
                    auto vec_bool_type_ = VectorDataType(
                            DataType::Bool,
                            VectorTypeDAz(base_type_den_->data_type_));
                    return std::make_shared<BaseTypeDenoter>(vec_bool_type_);
                } 
                else
                {
                    return std::make_shared<BaseTypeDenoter>(DataType::Bool);
                }
            }

            int TypeDenoter::FindVectorTruncation(
                    const TypeDenoter& source_type_den_a,
                    const TypeDenoter& dest_type_den_a,
                    int& source_vec_size_a,
                    int& dest_vec_size_a)
            {
                if (auto source_base_type_den_a =
                            source_type_den_a.As<BaseTypeDenoter>())
                {
                    if (auto dest_base_type_den_ =
                                dest_type_den_a.As<BaseTypeDenoter>())
                    {
                        source_vec_size_a = VectorTypeDAz(
                                source_base_type_den_a->data_type_);
                        dest_vec_size_a = VectorTypeDAz(dest_base_type_den_->data_type_);

                        if (source_vec_size_a > 0 && dest_vec_size_a > 0)
                        {
                            if (dest_vec_size_a < source_vec_size_a)
                                return (dest_vec_size_a - source_vec_size_a);

                            if (source_vec_size_a > 1
                                && dest_vec_size_a > source_vec_size_a)
                                return (dest_vec_size_a - source_vec_size_a);
                        }
                    }
                }
                return 0;
            }

            TypeDenoter::Types VoidTypeDenoter::Type() const
            {
                return Types::Void;
            }

            std::string VoidTypeDenoter::ToString() const
            {
                return "void";
            }

            TypeDenoterPtr VoidTypeDenoter::Copy() const
            {
                return std::make_shared<VoidTypeDenoter>();
            }

            bool VoidTypeDenoter::IsCastableTo(
                    const TypeDenoter& target_type_a) const
            {
                (void)target_type_a;
                return false;
            }

            TypeDenoter::Types NullTypeDenoter::Type() const
            {
                return Types::Null;
            }

            std::string NullTypeDenoter::ToString() const
            {
                return "NULL";
            }

            TypeDenoterPtr NullTypeDenoter::Copy() const
            {
                return std::make_shared<NullTypeDenoter>();
            }

            bool NullTypeDenoter::IsCastableTo(
                    const TypeDenoter& target_type_a) const
            {
                auto target_ = target_type_a.GetAliased().Type();
                return (target_ == TypeDenoter::Types::Buffer
                        || target_ == TypeDenoter::Types::Sampler);
            }

            BaseTypeDenoter::BaseTypeDenoter(const DataType data_type_a)
                : data_type_ {data_type_a}
            {}

            TypeDenoter::Types BaseTypeDenoter::Type() const
            {
                return Types::Base;
            }

            std::string BaseTypeDenoter::ToString() const
            {
                return DataTypeToString(data_type_);
            }

            TypeDenoterPtr BaseTypeDenoter::Copy() const
            {
                return std::make_shared<BaseTypeDenoter>(data_type_);
            }

            bool BaseTypeDenoter::Equals(const TypeDenoter& rhs_a,
                                         const Flags&) const
            {
                if (auto rhs_base_type_den_ = rhs_a.As<BaseTypeDenoter>())
                    return (data_type_ == rhs_base_type_den_->data_type_);
                else
                    return false;
            }

            bool BaseTypeDenoter::IsCastableTo(
                    const TypeDenoter& target_type_a) const
            {
                const auto& target_type_aliased_ = target_type_a.GetAliased();
                const auto target_type_class_ = target_type_aliased_.Type();
                return (target_type_class_ == Types::Base
                        || target_type_class_ == Types::Struct
                        || target_type_class_ == Types::Array);
            }

            bool BaseTypeDenoter::AccumAlignedVectorSize(
                    unsigned int& size_a,
                    unsigned int& padding_a,
                    unsigned int* offset_a) const
            {
                return CE_Kernel::Aid::ShaderPack::AccumAlignedVectorSize(data_type_,
                                                   size_a,
                                                   padding_a,
                                                   offset_a);
            }

            TypeDenoterPtr BaseTypeDenoter::GetSubObject(
                    const std::string& ident_a,
                    const AST* ast_a)
            {
                try
                {
                    auto subscript_data_type_ = SubscriptDataType(data_type_, ident_a);
                    auto sub_type_den_ = std::make_shared<BaseTypeDenoter>(
                            subscript_data_type_);

                    return sub_type_den_;
                } 
                catch (const std::exception& e_)
                {
                    RuntAzeErr(e_.what(), ast_a);
                }
            }

            TypeDenoterPtr BaseTypeDenoter::GetSubArray(
                    const std::size_t num_array_indices_a,
                    const AST* ast_a)
            {
                if (num_array_indices_a > 0)
                {
                    if (IsVectorType(data_type_))
                    {
                        if (num_array_indices_a > 1)
                            RuntAzeErr(R_TooManyArrayDAzensions(
                                               R_VectorTypeDen),
                                       ast_a);
                        else
                            return std::make_shared<BaseTypeDenoter>(
                                    BaseDataType(data_type_));
                    } 
                    else if (IsMatrixType(data_type_))
                    {
                        if (num_array_indices_a == 1)
                        {
                            auto matrix_dAz_ = MatrixTypeDAz(data_type_);
                            return std::make_shared<BaseTypeDenoter>(
                                    VectorDataType(BaseDataType(data_type_),
                                                   matrix_dAz_.second));
                        } 
                        else if (num_array_indices_a == 2)
                            return std::make_shared<BaseTypeDenoter>(
                                    BaseDataType(data_type_));
                        else if (num_array_indices_a > 2)
                            RuntAzeErr(R_TooManyArrayDAzensions(
                                               R_MatrixTypeDen),
                                       ast_a);
                    } 
                    else
                        return TypeDenoter::GetSubArray(num_array_indices_a, ast_a);
                }

                return GetSub();
            }

            BufferTypeDenoter::BufferTypeDenoter(const BufferType buffer_type_a)
                : buffer_type_ {buffer_type_a}
            {}

            BufferTypeDenoter::BufferTypeDenoter(BufferDecl* buffer_decl_ref_a)
                : buffer_decl_ref_ {buffer_decl_ref_a}
            {
                if (buffer_decl_ref_a && buffer_decl_ref_a->decl_stmnt_ref_)
                {
                    auto source_type_den_ =
                            buffer_decl_ref_a->decl_stmnt_ref_->type_denoter_.get();
                    buffer_type_ = source_type_den_->buffer_type_;
                    generic_type_denoter_ = source_type_den_->generic_type_denoter_;
                    generic_size_ = source_type_den_->generic_size_;
                }
            }

            TypeDenoter::Types BufferTypeDenoter::Type() const
            {
                return Types::Buffer;
            }

            std::string BufferTypeDenoter::ToString() const
            {
                auto s_ = BufferTypeToString(buffer_type_);

                if (generic_type_denoter_)
                {
                    s_ += '<';
                    s_ += generic_type_denoter_->ToString();
                    s_ += '>';
                }

                return s_;
            }

            TypeDenoterPtr BufferTypeDenoter::Copy() const
            {
                auto copy_ = std::make_shared<BufferTypeDenoter>();
                {
                    copy_->buffer_type_ = buffer_type_;
                    copy_->generic_type_denoter_ = generic_type_denoter_;
                    copy_->generic_size_ = generic_size_;
                    copy_->buffer_decl_ref_ = buffer_decl_ref_;
                }
                return copy_;
            }

            bool BufferTypeDenoter::Equals(const TypeDenoter& rhs_a,
                                           const Flags& compare_flags_a) const
            {
                if (auto rhs_buffer_type_den_ =
                            rhs_a.GetAliased().As<BufferTypeDenoter>())
                {
                    if (buffer_type_ == rhs_buffer_type_den_->buffer_type_)
                    {
                        if (!compare_flags_a(IgnoreGenericSubType))
                        {
                            if (generic_type_denoter_
                                && rhs_buffer_type_den_->generic_type_denoter_)
                                return generic_type_denoter_->Equals(
                                        *rhs_buffer_type_den_->generic_type_denoter_,
                                        compare_flags_a);
                            if (!generic_type_denoter_
                                && !rhs_buffer_type_den_->generic_type_denoter_)
                                return true;
                        } else
                            return true;
                    }
                }
                return false;
            }

            TypeDenoterPtr BufferTypeDenoter::GetSubObject(
                    const std::string& ident_a,
                    const AST* ast_a)
            {
                if (ident_a == "mips")
                {
                    RuntAzeErr(R_NotAzplementedYet(ToString() + ".mips",
                                                   __FUNCTION__),
                               ast_a);
                }
                return TypeDenoter::GetSubObject(ident_a, ast_a);
            }

            TypeDenoterPtr BufferTypeDenoter::GetSubArray(
                    const std::size_t num_array_indices_a,
                    const AST* ast_a)
            {
                if (num_array_indices_a > 0)
                    return GetGenericTypeDenoter()->GetSubArray(num_array_indices_a
                                                                        - 1,
                                                                ast_a);
                else
                    return shared_from_this();
            }

            TypeDenoter* BufferTypeDenoter::FetchSubTypeDenoter() const
            {
                return generic_type_denoter_.get();
            }

            TypeDenoterPtr BufferTypeDenoter::GetGenericTypeDenoter() const
            {
                if (generic_type_denoter_)
                    return generic_type_denoter_;
                else
                    return std::make_shared<BaseTypeDenoter>(DataType::Float4);
            }

            AST* BufferTypeDenoter::SymbolRef() const
            {
                return buffer_decl_ref_;
            }

            SamplerTypeDenoter::SamplerTypeDenoter(
                    const SamplerType sampler_type__a)
                : sampler_type_ {sampler_type__a}
            {}

            SamplerTypeDenoter::SamplerTypeDenoter(SamplerDecl* sampler_decl_ref_a)
                : sampler_decl_ref_ {sampler_decl_ref_a}
            {
                if (sampler_decl_ref_a)
                    sampler_type_ = sampler_decl_ref_a->GetSamplerType();
            }

            TypeDenoter::Types SamplerTypeDenoter::Type() const
            {
                return Types::Sampler;
            }

            std::string SamplerTypeDenoter::ToString() const
            {
                return (IsSamplerStateType(sampler_type_) ? "SamplerState"
                                                        : "Sampler");
            }

            TypeDenoterPtr SamplerTypeDenoter::Copy() const
            {
                auto copy_ = std::make_shared<SamplerTypeDenoter>();
                {
                    copy_->sampler_type_ = sampler_type_;
                    copy_->sampler_decl_ref_ = sampler_decl_ref_;
                }
                return copy_;
            }

            bool SamplerTypeDenoter::Equals(const TypeDenoter& rhs_a,
                                            const Flags& compare_flags_a) const
            {
                (void)compare_flags_a;
                if (auto rhs_sampler_type_den_ =
                            rhs_a.GetAliased().As<SamplerTypeDenoter>())
                    return (sampler_type_ == rhs_sampler_type_den_->sampler_type_);
                else
                    return false;
            }

            AST* SamplerTypeDenoter::SymbolRef() const
            {
                return sampler_decl_ref_;
            }

            StructTypeDenoter::StructTypeDenoter(const std::string& ident_a)
                : ident_ {ident_a}
            {}

            StructTypeDenoter::StructTypeDenoter(StructDecl* struct_decl_ref_a)
                : ident_ {struct_decl_ref_a ? struct_decl_ref_a->ident_.Original() : ""},
                  struct_decl_ref_ {struct_decl_ref_a}
            {}

            TypeDenoter::Types StructTypeDenoter::Type() const
            {
                return Types::Struct;
            }

            std::string StructTypeDenoter::ToString() const
            {
                return (struct_decl_ref_ ? struct_decl_ref_->ToString()
                                      : "struct " + R_Undefined);
            }

            TypeDenoterPtr StructTypeDenoter::Copy() const
            {
                auto copy_ = std::make_shared<StructTypeDenoter>();
                {
                    copy_->ident_ = ident_;
                    copy_->struct_decl_ref_ = struct_decl_ref_;
                }
                return copy_;
            }

            void StructTypeDenoter::SetIdentIfAnonymous(
                    const std::string& ident_a)
            {
                if (this->ident_.empty())
                    this->ident_ = ident_a;
            }

            bool StructTypeDenoter::Equals(const TypeDenoter& rhs_a,
                                           const Flags& compare_flags_a) const
            {
                if (auto rhs_struct_type_den_ =
                            rhs_a.GetAliased().As<StructTypeDenoter>())
                {
                    return GetStructDeclOrThrow()->EqualsMemberTypes(
                            *rhs_struct_type_den_->GetStructDeclOrThrow(),
                            compare_flags_a);
                }
                return false;
            }

            bool StructTypeDenoter::IsCastableTo(
                    const TypeDenoter& target_type_a) const
            {
                auto struct_decl_ = GetStructDeclOrThrow();

                const auto& target_aliased_type_ = target_type_a.GetAliased();
                if (auto target_struct_type_den_ =
                            target_aliased_type_.As<StructTypeDenoter>())
                {
                    return struct_decl_->EqualsMemberTypes(
                            *target_struct_type_den_->GetStructDeclOrThrow());
                }
                else if (auto target_base_type_den_ =
                                   target_aliased_type_.As<BaseTypeDenoter>())
                {
                    return struct_decl_->IsCastableTo(*target_base_type_den_);
                }

                return false;
            }

            bool StructTypeDenoter::AccumAlignedVectorSize(
                    unsigned int& size_a,
                    unsigned int& padding_a,
                    unsigned int* offset_a) const
            {
                if (struct_decl_ref_)
                    return struct_decl_ref_->AccumAlignedVectorSize(size_a,
                                                                 padding_a,
                                                                 offset_a);
                else
                    return false;
            }

            std::string StructTypeDenoter::Ident() const
            {
                return ident_;
            }

            AST* StructTypeDenoter::SymbolRef() const
            {
                return struct_decl_ref_;
            }

            TypeDenoterPtr StructTypeDenoter::GetSubObject(
                    const std::string& ident_a,
                    const AST* ast_a)
            {
                auto struct_decl_ = GetStructDeclOrThrow(ast_a);

                if (auto var_decl_ = struct_decl_->FetchVarDecl(ident_a))
                {
                    return var_decl_->GetTypeDenoter();
                }

                RuntAzeErr(R_UndeclaredIdent(ident_a,
                                             struct_decl_->ToString(),
                                             struct_decl_->FetchSAzilar(ident_a)),
                           ast_a);
            }

            StructDecl* StructTypeDenoter::GetStructDeclOrThrow(
                    const AST* ast_a) const
            {
                if (struct_decl_ref_)
                    return struct_decl_ref_;
                else
                    RuntAzeErr(R_MissingRefToStructDecl(ident_), ast_a);
            }

            AliasTypeDenoter::AliasTypeDenoter(const std::string& ident_a)
                : ident_ {ident_a}
            {}

            AliasTypeDenoter::AliasTypeDenoter(AliasDecl* alias_decl_ref_a)
                : ident_ {alias_decl_ref_a ? alias_decl_ref_a->ident_.Original() : ""},
                  alias_decl_ref_ {alias_decl_ref_a}
            {}

            TypeDenoter::Types AliasTypeDenoter::Type() const
            {
                return Types::Alias;
            }

            std::string AliasTypeDenoter::ToString() const
            {
                return ident_;
            }

            TypeDenoterPtr AliasTypeDenoter::Copy() const
            {
                auto copy_ = std::make_shared<AliasTypeDenoter>();
                {
                    copy_->ident_ = ident_;
                    copy_->alias_decl_ref_ = alias_decl_ref_;
                }
                return copy_;
            }

            void AliasTypeDenoter::SetIdentIfAnonymous(
                    const std::string& ident_a)
            {
                if (this->ident_.empty())
                    this->ident_ = ident_a;
            }

            bool AliasTypeDenoter::Equals(const TypeDenoter& rhs_a,
                                          const Flags& compare_flags_a) const
            {
                return GetAliasedTypeOrThrow()->Equals(rhs_a, compare_flags_a);
            }

            bool AliasTypeDenoter::IsCastableTo(
                    const TypeDenoter& target_type_a) const
            {
                return GetAliasedTypeOrThrow()->IsCastableTo(target_type_a);
            }

            bool AliasTypeDenoter::AccumAlignedVectorSize(
                    unsigned int& size_a,
                    unsigned int& padding_a,
                    unsigned int* offset_a) const
            {
                return GetAliasedTypeOrThrow()->AccumAlignedVectorSize(size_a,
                                                                       padding_a,
                                                                       offset_a);
            }

            std::string AliasTypeDenoter::Ident() const
            {
                return ident_;
            }

            TypeDenoterPtr AliasTypeDenoter::GetSub(const Expr* expr_a)
            {
                return GetAliasedTypeOrThrow(expr_a)->GetSub(expr_a);
            }

            TypeDenoterPtr AliasTypeDenoter::GetSubObject(
                    const std::string& ident_a,
                    const AST* ast_a)
            {
                return GetAliasedTypeOrThrow(ast_a)->GetSubObject(ident_a, ast_a);
            }

            TypeDenoterPtr AliasTypeDenoter::GetSubArray(
                    const std::size_t num_array_indices_a,
                    const AST* ast_a)
            {
                return GetAliasedTypeOrThrow(ast_a)->GetSubArray(num_array_indices_a,
                                                               ast_a);
            }

            const TypeDenoter& AliasTypeDenoter::GetAliased() const
            {
                return GetAliasedTypeOrThrow()->GetAliased();
            }

            const TypeDenoterPtr& AliasTypeDenoter::GetAliasedTypeOrThrow(
                    const AST* ast_a) const
            {
                if (alias_decl_ref_)
                    return alias_decl_ref_->GetTypeDenoter();
                else
                    RuntAzeErr(R_MissingRefToAliasDecl(ident_), ast_a);
            }

            unsigned int AliasTypeDenoter::NumDAzensions() const
            {
                return GetAliasedTypeOrThrow()->NumDAzensions();
            }

            AST* AliasTypeDenoter::SymbolRef() const
            {
                return alias_decl_ref_;
            }

            ArrayTypeDenoter::ArrayTypeDenoter(
                    const TypeDenoterPtr& sub_type_denoter_a)
                : sub_type_denoter_ {sub_type_denoter_a}
            {}

            ArrayTypeDenoter::ArrayTypeDenoter(
                    const TypeDenoterPtr& sub_type_denoter_a,
                    const std::vector<ArrayDAzensionPtr>& array_dAzs_a)
                : sub_type_denoter_ {sub_type_denoter_a}, array_dAzs_ {array_dAzs_a}
            {}

            ArrayTypeDenoter::ArrayTypeDenoter(
                    const TypeDenoterPtr& sub_type_denoter_a,
                    const std::vector<ArrayDAzensionPtr>& base_array_dAzs_a,
                    const std::vector<ArrayDAzensionPtr>& sub_array_dAzs_a)
                : sub_type_denoter_ {sub_type_denoter_a}, array_dAzs_ {base_array_dAzs_a}
            {
                array_dAzs_.insert(array_dAzs_.end(),
                                 sub_array_dAzs_a.begin(),
                                 sub_array_dAzs_a.end());
            }

            TypeDenoter::Types ArrayTypeDenoter::Type() const
            {
                return Types::Array;
            }

            std::string ArrayTypeDenoter::ToString() const
            {
                if (!sub_type_denoter_)
                    throw std::runtime_error(R_MissingBaseTypeInArray);

                auto type_name_ = sub_type_denoter_->ToString();

                for (const auto& dAz_ : array_dAzs_)
                    type_name_ += dAz_->ToString();

                return type_name_;
            }

            TypeDenoterPtr ArrayTypeDenoter::Copy() const
            {
                return std::make_shared<ArrayTypeDenoter>(sub_type_denoter_,
                                                          array_dAzs_);
            }

            TypeDenoterPtr ArrayTypeDenoter::GetSubArray(
                    const std::size_t num_array_indices_a,
                    const AST* ast_a)
            {
                const auto num_dAzs_ = array_dAzs_.size();

                if (num_array_indices_a == 0)
                {
                    return shared_from_this();
                }

                if (num_array_indices_a < num_dAzs_)
                {
                    auto sub_array_dAzs_ = array_dAzs_;
                    sub_array_dAzs_.resize(num_dAzs_ - num_array_indices_a);
                    return std::make_shared<ArrayTypeDenoter>(sub_type_denoter_,
                                                              sub_array_dAzs_);
                }

                return sub_type_denoter_->GetSubArray(num_array_indices_a - num_dAzs_,
                                                   ast_a);
            }

            bool ArrayTypeDenoter::Equals(const TypeDenoter& rhs_a,
                                          const Flags& compare_flags_a) const
            {
                if (auto rhs_array_type_den_a =
                            rhs_a.GetAliased().As<ArrayTypeDenoter>())
                {
                    if (sub_type_denoter_ && rhs_array_type_den_a->sub_type_denoter_
                        && EqualsDAzensions(*rhs_array_type_den_a))
                        return sub_type_denoter_->Equals(
                                *rhs_array_type_den_a->sub_type_denoter_,
                                compare_flags_a);
                }
                return false;
            }

            bool ArrayTypeDenoter::IsCastableTo(
                    const TypeDenoter& target_type_a) const
            {
                if (auto target_array_type_den_ =
                            target_type_a.GetAliased().As<ArrayTypeDenoter>())
                {
                    if (sub_type_denoter_ && target_array_type_den_->sub_type_denoter_
                        && EqualsDAzensions(*target_array_type_den_))
                        return sub_type_denoter_->IsCastableTo(
                                *target_array_type_den_->sub_type_denoter_);
                }
                return false;
            }

            bool ArrayTypeDenoter::AccumAlignedVectorSize(
                    unsigned int& size_a,
                    unsigned int& padding_a,
                    unsigned int* offset_a) const
            {
                unsigned int sub_size_ = 0, sub_padding_ = 0;
                if (sub_type_denoter_->AccumAlignedVectorSize(sub_size_, sub_padding_))
                {
                    unsigned int linear_array_size_ = 1;
                    auto dAz_sizes_ = GetDAzensionSizes();

                    for (auto size_ : dAz_sizes_)
                    {
                        if (size_ > 0)
                            linear_array_size_ *= static_cast<unsigned int>(size_);
                        else
                            return false;
                    }

                    auto remaining_size_ = RemainingVectorSize(size_a);
                    size_a += remaining_size_;
                    padding_a += remaining_size_;

                    if (offset_a != nullptr)
                        *offset_a = size_a;

                    sub_padding_ = RemainingVectorSize(sub_size_);
                    sub_size_ += sub_padding_;

                    size_a += (linear_array_size_ * sub_size_);
                    padding_a += (linear_array_size_ * sub_padding_);

                    return true;
                }
                return false;
            }

            unsigned int ArrayTypeDenoter::NumDAzensions() const
            {
                return (static_cast<unsigned int>(array_dAzs_.size())
                        + sub_type_denoter_->NumDAzensions());
            }

            AST* ArrayTypeDenoter::SymbolRef() const
            {
                return (sub_type_denoter_ ? sub_type_denoter_->SymbolRef() : nullptr);
            }

            bool ArrayTypeDenoter::EqualsDAzensions(
                    const ArrayTypeDenoter& rhs_a) const
            {
                if (array_dAzs_.size() == rhs_a.array_dAzs_.size())
                {
                    for (std::size_t i_ = 0, n_ = array_dAzs_.size(); i_ < n_; ++i_)
                    {
                        if (array_dAzs_[i_]->size_ != rhs_a.array_dAzs_[i_]->size_)
                            return false;
                    }
                    return true;
                }
                return false;
            }

            TypeDenoterPtr ArrayTypeDenoter::AsArray(
                    const std::vector<ArrayDAzensionPtr>& sub_array_dAzs_a)
            {
                if (sub_array_dAzs_a.empty())
                    return shared_from_this();
                else
                    return std::make_shared<ArrayTypeDenoter>(sub_type_denoter_,
                                                              array_dAzs_,
                                                              sub_array_dAzs_a);
            }

            TypeDenoter* ArrayTypeDenoter::FetchSubTypeDenoter() const
            {
                return sub_type_denoter_.get();
            }

            void ArrayTypeDenoter::InsertSubArray(
                    const ArrayTypeDenoter& sub_array_type_denoter_a)
            {
                array_dAzs_.insert(array_dAzs_.end(),
                                 sub_array_type_denoter_a.array_dAzs_.begin(),
                                 sub_array_type_denoter_a.array_dAzs_.end());

                sub_type_denoter_ = sub_array_type_denoter_a.sub_type_denoter_;
            }

            std::vector<int> ArrayTypeDenoter::GetDAzensionSizes() const
            {
                std::vector<int> sizes_;

                for (const auto& dAz_ : array_dAzs_)
                    sizes_.push_back(dAz_ != nullptr ? dAz_->size_ : -1);

                return sizes_;
            }

            int ArrayTypeDenoter::NumArrayElements() const
            {
                int n_ = 1;

                for (const auto& dAz_ : array_dAzs_)
                    n_ *= dAz_->size_;

                return n_;
            }

            FunctionTypeDenoter::FunctionTypeDenoter(FunctionDecl* func_decl_ref_a)
                : ident_ {func_decl_ref_a ? func_decl_ref_a->ident_.Original() : ""},
                  func_decl_refs_ {std::vector<FunctionDecl*> {func_decl_ref_a}}
            {}

            FunctionTypeDenoter::FunctionTypeDenoter(
                    const std::string& ident_a,
                    const std::vector<FunctionDecl*>& func_decl_refs_a)
                : ident_ {ident_a}, func_decl_refs_ {func_decl_refs_a}
            {}

            TypeDenoter::Types FunctionTypeDenoter::Type() const
            {
                return Types::Struct;
            }

            std::string FunctionTypeDenoter::ToString() const
            {
                if (func_decl_refs_.size() == 1)
                    return func_decl_refs_.front()->ToTypeDenoterString();
                else
                    return R_OverloadedFunction;
            }

            TypeDenoterPtr FunctionTypeDenoter::Copy() const
            {
                return std::make_shared<FunctionTypeDenoter>(ident_,
                                                             func_decl_refs_);
            }

            bool FunctionTypeDenoter::Equals(const TypeDenoter& rhs_a,
                                             const Flags& compare_flags_a) const
            {
                (void)compare_flags_a;
                if (auto rhs_func_type_den_ =
                            rhs_a.GetAliased().As<FunctionTypeDenoter>())
                {
                    return (func_decl_refs_ == rhs_func_type_den_->func_decl_refs_);
                }
                return false;
            }

            bool FunctionTypeDenoter::IsCastableTo(
                    const TypeDenoter& target_type_a) const
            {
                (void)target_type_a;
                return false;
            }

            std::string FunctionTypeDenoter::Ident() const
            {
                return ident_;
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel