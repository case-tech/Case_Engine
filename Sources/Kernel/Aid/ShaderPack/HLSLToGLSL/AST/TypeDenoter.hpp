// Copyright (c) 2025 Case Technologies

#pragma once
#include "../AST/Visitor/Visitor.hpp"
#include "../CiString.hpp"
#include "../Flags.hpp"
#include "ASTEnums.hpp"

#include <memory>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
#define DECL_PTR(CLASS_NAME)                                                   \
    struct CLASS_NAME;                                                         \
    using CLASS_NAME##Ptr = std::shared_ptr<CLASS_NAME>

            DECL_PTR(TypeDenoter);
            DECL_PTR(VoidTypeDenoter);
            DECL_PTR(NullTypeDenoter);
            DECL_PTR(BaseTypeDenoter);
            DECL_PTR(BufferTypeDenoter);
            DECL_PTR(SamplerTypeDenoter);
            DECL_PTR(StructTypeDenoter);
            DECL_PTR(AliasTypeDenoter);
            DECL_PTR(ArrayTypeDenoter);
            DECL_PTR(FunctionTypeDenoter);

#undef DECL_PTR

            struct TypeDenoter : std::enable_shared_from_this<TypeDenoter>
            {
                enum class Types
                {
                    Void,
                    Null,
                    Base,
                    Buffer,
                    Sampler,
                    Struct,
                    Alias,
                    Array,
                    Function,
                };

                enum : unsigned int
                {
                    IgnoreGenericSubType = (1 << 0),
                };

                virtual ~TypeDenoter();
                virtual Types Type() const = 0;
                virtual std::string ToString() const = 0;
                virtual TypeDenoterPtr Copy() const = 0;
                virtual bool Equals(const TypeDenoter& rhs_a,
                                    const Flags& compare_flags_a = 0) const;
               
                virtual bool IsCastableTo(const TypeDenoter& target_type_a) const;
                virtual bool AccumAlignedVectorSize(
                        unsigned int& size_a,
                        unsigned int& padding_a,
                        unsigned int* offset_a = nullptr) const;

                bool IsVoid() const;
                bool IsNull() const;
                bool IsBase() const;
                bool IsScalar() const;
                bool IsVector() const;
                bool IsMatrix() const;
                bool IsSampler() const;
                bool IsBuffer() const;
                bool IsStruct() const;
                bool IsAlias() const;
                bool IsArray() const;
                bool IsFunction() const;

                template <typename T>
                T* As()
                {
                    return (Type() == T::class_type_ ? static_cast<T*>(this)
                                                   : nullptr);
                }

                template <typename T>
                const T* As() const
                {
                    return (Type() == T::class_type_ ? static_cast<const T*>(this)
                                                   : nullptr);
                }

                virtual TypeDenoterPtr GetSub(const Expr* expr_a = nullptr);
                virtual TypeDenoterPtr GetSubObject(const std::string& ident_a,
                                                    const AST* ast_a = nullptr);
                virtual TypeDenoterPtr GetSubArray(
                        const std::size_t num_array_indices_a,
                        const AST* ast_a = nullptr);
                
                virtual const TypeDenoter& GetAliased() const;
                virtual std::string Ident() const;

                virtual void SetIdentIfAnonymous(const std::string& ident_a);
                virtual unsigned int NumDAzensions() const;
                virtual AST* SymbolRef() const;
                virtual TypeDenoterPtr AsArray(
                        const std::vector<ArrayDAzensionPtr>& array_dAzs_a);
                virtual TypeDenoter* FetchSubTypeDenoter() const;

                static TypeDenoterPtr FindCommonTypeDenoter(
                        const TypeDenoterPtr& lhs_type_den_a,
                        const TypeDenoterPtr& rhs_type_den_a,
                        bool use_min_dAzension_a = false);
                
                static TypeDenoterPtr FindCommonTypeDenoterFrom(
                        const ExprPtr& lhs_expr_a,
                        const ExprPtr& rhs_expr_a,
                        bool use_min_dAzension_a = false,
                        const AST* ast_a = nullptr);
                
                static BaseTypeDenoterPtr MakeBoolTypeWithDAzensionOf(
                        const TypeDenoter& type_den_a);

                static int FindVectorTruncation(
                        const TypeDenoter& source_type_den_a,
                        const TypeDenoter& dest_type_den_a,
                        int& source_vec_size_a,
                        int& dest_vec_size_a);
            };

            struct VoidTypeDenoter : public TypeDenoter
            {
                static const Types class_type_ = Types::Void;

                Types Type() const override;
                std::string ToString() const override;
                TypeDenoterPtr Copy() const override;

                bool IsCastableTo(const TypeDenoter& target_type_a) const override;
            };

            struct NullTypeDenoter : public TypeDenoter
            {
                static const Types class_type_ = Types::Null;

                Types Type() const override;
                std::string ToString() const override;
                TypeDenoterPtr Copy() const override;

                bool IsCastableTo(const TypeDenoter& target_type_a) const override;
            };

            struct BaseTypeDenoter : public TypeDenoter
            {
                static const Types class_type_ = Types::Base;

                BaseTypeDenoter() = default;
                BaseTypeDenoter(const DataType data_type_a);

                Types Type() const override;
                std::string ToString() const override;
                TypeDenoterPtr Copy() const override;

                bool Equals(const TypeDenoter& rhs_a,
                            const Flags& compare_flags_a = 0) const override;
                
                bool IsCastableTo(const TypeDenoter& target_type_a) const override;
                bool AccumAlignedVectorSize(
                        unsigned int& size_a,
                        unsigned int& padding_a,
                        unsigned int* offset_a = nullptr) const override;

                TypeDenoterPtr GetSubObject(const std::string& ident_a,
                                            const AST* ast_a = nullptr) override;
              
                TypeDenoterPtr GetSubArray(const std::size_t num_array_indices_a,
                                           const AST* ast_a = nullptr) override;

                DataType data_type_ = DataType::Undefined;
            };

            struct BufferTypeDenoter : public TypeDenoter
            {
                static const Types class_type_ = Types::Buffer;

                BufferTypeDenoter() = default;
                BufferTypeDenoter(const BufferType buffer_type_a);
                BufferTypeDenoter(BufferDecl* buffer_decl_ref_a);

                Types Type() const override;
                std::string ToString() const override;
                TypeDenoterPtr Copy() const override;

                bool Equals(const TypeDenoter& rhs_a,
                            const Flags& compare_flags_a = 0) const override;

                TypeDenoterPtr GetSubObject(const std::string& ident_a,
                                            const AST* ast_a = nullptr) override;
               
                TypeDenoterPtr GetSubArray(const std::size_t num_array_indices_a,
                                           const AST* ast_a = nullptr) override;

                AST* SymbolRef() const override;

                TypeDenoter* FetchSubTypeDenoter() const override;
                TypeDenoterPtr GetGenericTypeDenoter() const;

                BufferType buffer_type_ = BufferType::Undefined;
                TypeDenoterPtr generic_type_denoter_;
                int generic_size_ = 1;

                BufferDecl* buffer_decl_ref_ = nullptr;
            };

            struct SamplerTypeDenoter : public TypeDenoter
            {
                static const Types class_type_ = Types::Sampler;

                SamplerTypeDenoter() = default;
                SamplerTypeDenoter(const SamplerType sampler_type_a);
                SamplerTypeDenoter(SamplerDecl* sampler_decl_ref_a);

                Types Type() const override;
                std::string ToString() const override;
                TypeDenoterPtr Copy() const override;

                bool Equals(const TypeDenoter& rhs_a,
                            const Flags& compare_flags_a = 0) const override;

                AST* SymbolRef() const override;

                SamplerType sampler_type_ = SamplerType::Undefined;
                SamplerDecl* sampler_decl_ref_ = nullptr;
            };

            struct StructTypeDenoter : public TypeDenoter
            {
                static const Types class_type_ = Types::Struct;

                StructTypeDenoter() = default;
                StructTypeDenoter(const std::string& ident_a);
                StructTypeDenoter(StructDecl* struct_decl_ref_a);

                Types Type() const override;
                std::string ToString() const override;
                TypeDenoterPtr Copy() const override;

                void SetIdentIfAnonymous(const std::string& ident_a) override;

                bool Equals(const TypeDenoter& rhs_a,
                            const Flags& compare_flags_a = 0) const override;
                
                bool IsCastableTo(const TypeDenoter& target_type_a) const override;
                bool AccumAlignedVectorSize(
                        unsigned int& size_a,
                        unsigned int& padding_a,
                        unsigned int* offset_a = nullptr) const override;

                std::string Ident() const override;

                AST* SymbolRef() const override;

                TypeDenoterPtr GetSubObject(const std::string& ident_a,
                                            const AST* ast_a = nullptr) override;

                StructDecl* GetStructDeclOrThrow(
                        const AST* ast_a = nullptr) const;

                std::string ident_;
                StructDecl* struct_decl_ref_ = nullptr;
            };

            struct AliasTypeDenoter : public TypeDenoter
            {
                static const Types class_type_ = Types::Alias;

                AliasTypeDenoter() = default;
                AliasTypeDenoter(const std::string& ident_a);
                AliasTypeDenoter(AliasDecl* alias_decl_ref_a);

                Types Type() const override;
                std::string ToString() const override;
                TypeDenoterPtr Copy() const override;

                void SetIdentIfAnonymous(const std::string& ident_a) override;

                bool Equals(const TypeDenoter& rhs_a,
                            const Flags& compare_flags_a = 0) const override;
                bool IsCastableTo(const TypeDenoter& target_type_a) const override;
                bool AccumAlignedVectorSize(
                        unsigned int& size_a,
                        unsigned int& padding_a,
                        unsigned int* offset_a = nullptr) const override;

                std::string Ident() const override;

                TypeDenoterPtr GetSub(const Expr* expr_a = nullptr) override;
                TypeDenoterPtr GetSubObject(const std::string& ident_a,
                                            const AST* ast_a = nullptr) override;
                
                TypeDenoterPtr GetSubArray(const std::size_t num_array_indices_a,
                                           const AST* ast_a = nullptr) override;

                const TypeDenoter& GetAliased() const override;
                const TypeDenoterPtr& GetAliasedTypeOrThrow(
                        const AST* ast_a = nullptr) const;

                unsigned int NumDAzensions() const override;

                AST* SymbolRef() const override;

                std::string ident_;

                AliasDecl* alias_decl_ref_ = nullptr;
            };

            struct ArrayTypeDenoter : public TypeDenoter
            {
                static const Types class_type_ = Types::Array;

                ArrayTypeDenoter() = default;

                ArrayTypeDenoter(const TypeDenoterPtr& sub_type_denoter_a);
                ArrayTypeDenoter(
                        const TypeDenoterPtr& sub_type_denoter_a,
                        const std::vector<ArrayDAzensionPtr>& array_dAzs_a);

                ArrayTypeDenoter(
                        const TypeDenoterPtr& sub_type_denoter_a,
                        const std::vector<ArrayDAzensionPtr>& base_array_dAzs_a,
                        const std::vector<ArrayDAzensionPtr>& sub_array_dAzs_a);

                Types Type() const override;

                std::string ToString() const override;

                TypeDenoterPtr Copy() const override;
                TypeDenoterPtr GetSubArray(const std::size_t num_array_indices_a,
                                           const AST* ast_a = nullptr) override;

                bool Equals(const TypeDenoter& rhs_a,
                            const Flags& compare_flags_a = 0) const override;
               
                bool IsCastableTo(const TypeDenoter& target_type_a) const override;
                bool AccumAlignedVectorSize(
                        unsigned int& size_a,
                        unsigned int& padding_a,
                        unsigned int* offset_a = nullptr) const override;

                unsigned int NumDAzensions() const override;
                AST* SymbolRef() const override;

                bool EqualsDAzensions(const ArrayTypeDenoter& rhs_a) const;

                TypeDenoterPtr AsArray(const std::vector<ArrayDAzensionPtr>&
                                               sub_array_dAzs_a) override;
                
                TypeDenoter* FetchSubTypeDenoter() const override;
                void InsertSubArray(
                        const ArrayTypeDenoter& sub_array_type_denoter_a);

                std::vector<int> GetDAzensionSizes() const;

                int NumArrayElements() const;

                TypeDenoterPtr sub_type_denoter_;
                std::vector<ArrayDAzensionPtr> array_dAzs_;
            };

            struct FunctionTypeDenoter : public TypeDenoter
            {
                static const Types class_type_ = Types::Function;

                FunctionTypeDenoter() = default;
                FunctionTypeDenoter(FunctionDecl* func_decl_ref_a);
                FunctionTypeDenoter(
                        const std::string& ident_a,
                        const std::vector<FunctionDecl*>& func_decl_refs_a);

                Types Type() const override;
                std::string ToString() const override;
                TypeDenoterPtr Copy() const override;

                bool Equals(const TypeDenoter& rhs_a,
                            const Flags& compare_flags_a = 0) const override;
                
                bool IsCastableTo(const TypeDenoter& targetType) const override;
                std::string Ident() const override;

                std::string ident_;
                std::vector<FunctionDecl*> func_decl_refs_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel