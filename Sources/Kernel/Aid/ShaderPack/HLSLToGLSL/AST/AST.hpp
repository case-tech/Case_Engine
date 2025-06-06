// Copyright (c) 2025 Case Technologies

#pragma once
#include "../AST/Visitor/Visitor.hpp"
#include "../Flags.hpp"
#include "../SourceCode.hpp"
#include "../Targets.hpp"
#include "../Variant.hpp"
#include "ASTEnums.hpp"
#include "Identifier.hpp"
#include "Token.hpp"
#include "TypeDenoter.hpp"

#include <functional>
#include <initializer_list>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class Visitor;

            enum SearchFlags : unsigned int
            {
                SearchLValue = (1 << 0),
                SearchRValue = (1 << 1),
                SearchAll = (~0u),
            };

            using VarDeclIteratorFunctor =
                    std::function<void(VarDeclPtr& var_decl_a)>;

            using ExprIteratorFunctor =
                    std::function<void(ExprPtr& expr_a, VarDecl* param_a)>;

            using ArgumentParameterTypeFunctor = std::function<
                    void(ExprPtr& argument_a, const TypeDenoter& param_type_den_a)>;

            using FindPredicateConstFunctor =
                    std::function<bool(const Expr& expr_a)>;
           
            using MergeExprFunctor = std::function<
                    ExprPtr(const ExprPtr& expr0_a, const ExprPtr& expr1_a)>;

#define AST_INTERFACE(CLASS_NAME)                                              \
    static const Types class_type_ = Types::CLASS_NAME;                        \
    CLASS_NAME(const SourcePosition& ast_pos_a)                                \
    {                                                                          \
        area_ = SourceArea(ast_pos_a, 1);                                      \
    }                                                                          \
    CLASS_NAME(const SourceArea& ast_area_a)                                   \
    {                                                                          \
        area_ = ast_area_a;                                                    \
    }                                                                          \
    Types Type() const override                                                \
    {                                                                          \
        return Types::CLASS_NAME;                                              \
    }                                                                          \
    void Visit(Visitor* visitor_a, void* args_a = nullptr) override            \
    {                                                                          \
        visitor_a->Visit##CLASS_NAME(this, args_a);                            \
    }

#define DECL_AST_ALIAS(ALIAS, BASE)                                            \
    using ALIAS = BASE;                                                        \
    using ALIAS##Ptr = BASE##Ptr

#define FLAG_ENUM enum : unsigned int

#define FLAG(IDENT, INDEX) IDENT = (1u << (INDEX))

            struct AST
            {
                enum class Types
                {
                    Program,
                    CodeBlock,
                    Attribute,
                    SwitchCase,
                    SamplerValue,
                    Register,
                    PackOffset,
                    ArrayDimension,
                    TypeSpecifier,

                    VarDecl,
                    BufferDecl,
                    SamplerDecl,
                    StructDecl,
                    AliasDecl,
                    FunctionDecl,
                    UniformBufferDecl,

                    VarDeclStmnt,
                    BufferDeclStmnt,
                    SamplerDeclStmnt,
                    AliasDeclStmnt,
                    BasicDeclStmnt,

                    NullStmnt,
                    CodeBlockStmnt,
                    ForLoopStmnt,
                    WhileLoopStmnt,
                    DoWhileLoopStmnt,
                    IfStmnt,
                    ElseStmnt,
                    SwitchStmnt,
                    ExprStmnt,
                    ReturnStmnt,
                    CtrlTransferStmnt,
                    LayoutStmnt,

                    NullExpr,
                    SequenceExpr,
                    LiteralExpr,
                    TypeSpecifierExpr,
                    TernaryExpr,
                    BinaryExpr,
                    UnaryExpr,
                    PostUnaryExpr,
                    CallExpr,
                    BracketExpr,
                    ObjectExpr,
                    AssignExpr,
                    ArrayExpr,
                    CastExpr,
                    InitializerExpr,
                };

                virtual Types Type() const = 0;
                virtual void Visit(Visitor* visitor_a, void* args_a = nullptr) = 0;

                FLAG_ENUM 
                {
                        FLAG(IsReachable, 30),
                        FLAG(IsDeadCode, 29),
                        FLAG(IsBuiltin, 28),
                };

                template <typename T>
                static T* GetAs(AST* ast_a)
                {
                    return (ast_a != nullptr && ast_a->Type() == T::class_type_
                                    ? static_cast<T*>(ast_a)
                                    : nullptr);
                }

                template <typename T>
                static const T* GetAs(const AST* ast_a)
                {
                    return (ast_a != nullptr && ast_a->Type() == T::class_type_
                                    ? static_cast<const T*>(ast_a)
                                    : nullptr);
                }

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

                SourceArea area_;
                Flags flags_;
            };

            bool IsDeclAST(const AST::Types t_a);
            bool IsExprAST(const AST::Types t_a);
            bool IsStmntAST(const AST::Types t_a);
            bool IsDeclStmntAST(const AST::Types t_a);

            struct Stmnt : public AST
            {
                virtual void CollectDeclIdents(
                        std::map<const AST*, std::string>& decl_ast_idents_a) const;

                std::string comment_;
                std::vector<AttributePtr> attribs_;
            };

            struct TypedAST : public AST
            {
            public:
                const TypeDenoterPtr& GetTypeDenoter(
                        const TypeDenoter* expectedTypeDenoter = nullptr);
                void ResetTypeDenoter();

            protected:
                virtual TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) = 0;

            private:
                TypeDenoterPtr buffered_type_denoter_;
            };

            struct Expr : public TypedAST
            {
                FLAG_ENUM 
                {
                        FLAG(WasConverted, 0),
                };

                VarDecl* FetchVarDecl() const;

                virtual const ObjectExpr* FetchLValueExpr() const;
                ObjectExpr* FetchLValueExpr();

                virtual IndexedSemantic FetchSemantic() const;
                virtual bool IsTrivialCopyable(
                        unsigned int max_tree_depth_a = 3) const;
                
                virtual const Expr* Find(
                        const FindPredicateConstFunctor& predicate_a,
                        unsigned int flags_a = SearchAll) const;

                const Expr* FindFirstOf(const Types expr_type_a,
                                        unsigned int flags_a = SearchAll) const;
                
                const Expr* FindFirstNotOf(
                        const Types expr_type_a,
                        unsigned int flags_a = SearchAll) const;

                Expr* Find(const FindPredicateConstFunctor& predicate_a,
                           unsigned int flags_a = SearchAll);
                
                Expr* FindFirstOf(const Types expr_type_a,
                                  unsigned int flags_a = SearchAll);
                
                Expr* FindFirstNotOf(const Types expr_type_a,
                                     unsigned int flags_a = SearchAll);
            };

            struct Decl : public TypedAST
            {
                FLAG_ENUM 
                {
                        FLAG(IsWrittenTo, 0),
                        FLAG(IsReadFrom, 1),
                };

                virtual std::string ToString() const;
                virtual TypeSpecifier* FetchTypeSpecifier() const;

                bool IsAnonymous() const;

                Identifier ident_;
            };

            struct Program : public AST
            {
                AST_INTERFACE(Program);

                struct LayoutTessControlShader
                {
                    unsigned int outputControlPoints = 0;
                    float maxTessFactor = 0.0f;
                    FunctionDecl* patch_const_function_ref_ = nullptr;
                };

                struct LayoutTessEvaluationShader
                {
                    AttributeValue domain_type_ = AttributeValue::Undefined;
                    AttributeValue partitioning_ = AttributeValue::Undefined;
                    AttributeValue output_topology_ = AttributeValue::Undefined;
                };

                struct LayoutGeometryShader
                {
                    PrimitiveType input_primitive_ = PrimitiveType::Undefined;
                    BufferType output_primitive_ = BufferType::Undefined;
                    unsigned int max_vertices_ = 0;
                };

                struct LayoutFragmentShader
                {
                    bool frag_coord_used_ = false;
                    bool pixel_center_integer_ = false;
                    bool early_depth_stencil_ = false;
                };

                struct LayoutComputeShader
                {
                    unsigned int num_threads_[3] = {0};
                };

                void RegisterIntrinsicUsage(
                        const Intrinsic intrinsic_,
                        const std::vector<DataType>& argumentDataTypes);
                void RegisterIntrinsicUsage(
                        const Intrinsic intrinsic_,
                        const std::vector<ExprPtr>& arguments_);
                const IntrinsicUsage* FetchIntrinsicUsage(
                        const Intrinsic intrinsic_) const;

                std::vector<StmntPtr> global_stmnts_;
                std::vector<ASTPtr> disabled_ast_;
                SourceCodePtr source_code_;
                FunctionDecl* entry_point_ref_ = nullptr;
                std::map<Intrinsic, IntrinsicUsage> used_intrinsics_;
                std::set<MatrixSubscriptUsage> used_matrix_subscripts_;

                LayoutTessControlShader layout_tess_control_;
                LayoutTessEvaluationShader layoutTessEvaluation;
                LayoutGeometryShader layout_geometry_;
                LayoutFragmentShader layout_fragment_;
                LayoutComputeShader layoutCompute;
            };

            struct CodeBlock : public AST
            {
                AST_INTERFACE(CodeBlock);

                std::vector<StmntPtr> stmnts_;
            };

            struct SamplerValue : public AST
            {
                AST_INTERFACE(SamplerValue);

                std::string name_;
                ExprPtr value_;
            };

            struct Attribute : public AST
            {
                AST_INTERFACE(Attribute);

                std::string ToString() const;

                AttributeType attribute_type_ = AttributeType::Undefined;
                std::vector<ExprPtr> arguments_;
            };

            struct SwitchCase : public AST
            {
                AST_INTERFACE(SwitchCase);

                bool IsDefaultCase() const;

                ExprPtr expr_;
                std::vector<StmntPtr> stmnts_;
            };

            struct Register : public AST
            {
                AST_INTERFACE(Register);

                std::string ToString() const;
                static Register* GetForTarget(
                        const std::vector<RegisterPtr>& registers_a,
                        const ShaderTarget shader_target_a);

                ShaderTarget shader_target_ = ShaderTarget::Undefined;
                RegisterType register_type_ = RegisterType::Undefined;
                int slot_ = 0;
            };

            struct PackOffset : public AST
            {
                AST_INTERFACE(PackOffset);

                std::string ToString() const;

                std::string register_name_;
                std::string vector_component_;
            };

            struct ArrayDimension : public TypedAST
            {
                AST_INTERFACE(ArrayDimension);

                std::string ToString() const;

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                bool HasDynamicSize() const;
                void ValidateIndexBoundary(int idx_a) const;

#if 0 
    static std::vector<int> GetArrayDimensionSizes(const std::vector<ArrayDimensionPtr>& array_dims_a);
#endif

                ExprPtr expr_;
                int size_ = 0;
            };

            struct TypeSpecifier : public TypedAST
            {
                AST_INTERFACE(TypeSpecifier);
                std::string ToString() const;

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                StructDecl* GetStructDeclRef();

                bool IsInput() const;
                bool IsOutput() const;
                bool IsConst() const;
                bool IsConstOrUniform() const;

                void SetTypeModifier(const TypeModifier modifier_a);
                bool HasAnyTypeModifierOf(
                        const std::initializer_list<TypeModifier>& modifiers_a)
                        const;
                
                bool HasAnyStorageClassOf(
                        const std::initializer_list<StorageClass>& modifiers_a)
                        const;

                void SwapMatrixStorageLayout(
                        const TypeModifier default_storge_layout_a);

                bool is_input_ = false;
                bool is_output_ = false;
                bool is_uniform_ = false;

                std::set<StorageClass> storage_ñlasses_;
                std::set<InterpModifier> interp_modifiers_;
                std::set<TypeModifier> type_modifiers_;
                PrimitiveType primitive_type_ = PrimitiveType::Undefined;
                StructDeclPtr struct_decl_;

                TypeDenoterPtr type_denoter_;
            };

            struct VarDecl : public Decl
            {
                AST_INTERFACE(VarDecl);

                FLAG_ENUM {
                        FLAG(IsShaderInput, 2),
                        FLAG(IsShaderOutput, 3),
                        FLAG(IsSystemValue, 4),
                        FLAG(IsDynamicArray, 5),
                        FLAG(IsEntryPointOutput, 6),
                        FLAG(IsEntryPointLocal, 7),

                        IsShaderInputSV = (IsShaderInput | IsSystemValue),
                        IsShaderOutputSV = (IsShaderOutput | IsSystemValue),
                };

                std::string ToString() const override;

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;
                
                TypeSpecifier* FetchTypeSpecifier() const override;

                VarDecl* FetchStaticVarDeclRef() const;
                VarDecl* FetchStaticVarDefRef() const;

                bool IsStatic() const;
                bool IsParameter() const;
                bool HasStaticConstInitializer() const;

                void SetCustomTypeDenoter(const TypeDenoterPtr& type_denoter_a);
                void AddFlagsRecursive(unsigned int var_flags_a);

                bool AccumAlignedVectorSize(unsigned int& size_a,
                                            unsigned int& padding_a,
                                            unsigned int* offset_a = nullptr);

                ObjectExprPtr namespace_expr_;
                std::vector<ArrayDimensionPtr> array_dims_;
                std::vector<RegisterPtr> slot_registers_;
                IndexedSemantic semantic_;
                PackOffsetPtr pack_offset_;
                std::vector<VarDeclStmntPtr> annotations_;
                ExprPtr initializer_;

                TypeDenoterPtr custom_type_denoter_;
                Variant initializer_value_;

                VarDeclStmnt* decl_stmnt_ref_ = nullptr;
                UniformBufferDecl* buffer_decl_ref_ = nullptr;
                StructDecl* struct_decl_ref_ = nullptr;
                VarDecl* static_member_var_ref_ = nullptr;
            };

            struct BufferDecl : public Decl
            {
                AST_INTERFACE(BufferDecl);

                FLAG_ENUM 
                {
                        FLAG(IsUsedForCompare, 2),
                        FLAG(IsUsedForImageRead, 3),
                };

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                BufferType GetBufferType() const;

                std::vector<ArrayDimensionPtr> array_dims_;
                std::vector<RegisterPtr> slot_registers_;
                std::vector<VarDeclStmntPtr> annotations_;
                BufferDeclStmnt* decl_stmnt_ref_ = nullptr;
            };

            struct SamplerDecl : public Decl
            {
                AST_INTERFACE(SamplerDecl);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                SamplerType GetSamplerType() const;

                std::vector<ArrayDimensionPtr> array_dims_;
                std::vector<RegisterPtr> slot_registers_;
                std::string textureIdent;
                std::vector<SamplerValuePtr> sampler_values_;

                SamplerDeclStmnt* decl_stmnt_ref_ = nullptr;
            };

            struct StructDecl : public Decl
            {
                AST_INTERFACE(StructDecl);

                FLAG_ENUM 
                {
                        FLAG(IsShaderInput, 2),
                        FLAG(IsShaderOutput, 3),
                        FLAG(isNestedStruct, 4),

                        FLAG(isNonEntryPointParam, 5),
                };

                std::string ToString() const override;

                bool EqualsMemberTypes(const StructDecl& rhs_a,
                                       const Flags& compare_flags_a = 0) const;
                bool IsCastableTo(const BaseTypeDenoter& rhs_a) const;

                VarDecl* FetchVarDecl(const std::string& ident_a,
                                      const StructDecl** owner_a = nullptr) const;
                VarDecl* FetchBaseMember() const;

                StructDecl* FetchBaseStructDecl(const std::string& ident_a);

                FunctionDecl* FetchFunctionDecl(
                        const std::string& ident_a,
                        const std::vector<TypeDenoterPtr>& arg_type_denoters_a,
                        const StructDecl** owner_a = nullptr,
                        bool throw_error_if_no_match_a = false) const;

                std::string FetchSimilar(const std::string& ident_a);
                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                bool HasNonSystemValueMembers() const;
                std::size_t NumMemberVariables(
                        bool only_non_static_members_a = false) const;
               
                std::size_t NumMemberFunctions(
                        bool only_non_static_members_a = false) const;

                void CollectMemberTypeDenoters(
                        std::vector<TypeDenoterPtr>& member_type_dens_a,
                        bool include_base_structs_a = true) const;

                void ForEachVarDecl(const VarDeclIteratorFunctor& iterator_a,
                                    bool include_base_structs_a = true);
               
                void AddShaderOutputInstance(VarDecl* var_decl_a);
                bool HasMultipleShaderOutputInstances() const;
                bool IsBaseOf(const StructDecl* sub_struct_decl_a,
                              bool include_self_a = false) const;

                void AddFlagsRecursive(unsigned int struct_flags_a);
                void AddFlagsRecursiveParents(unsigned int struct_flags_a);

                std::size_t MemberVarToIndex(
                        const VarDecl* var_decl_a,
                        bool include_base_structs_a = true) const;
                
                VarDecl* IndexToMemberVar(std::size_t idx_a,
                                          bool include_base_structs_a = true) const;

                bool AccumAlignedVectorSize(unsigned int& size_a,
                                            unsigned int& padding_a,
                                            unsigned int* offset_a = nullptr);

                bool is_class_ = false;
                std::string base_struct_name_;
                std::vector<StmntPtr> local_stmnts_;

                std::vector<VarDeclStmntPtr> var_members_;
                std::vector<FunctionDeclPtr> func_members_;

                BasicDeclStmnt* decl_stmnt_ref_ = nullptr;
                StructDecl* base_struct_ref_ = nullptr;
                StructDecl* compatible_struct_ref_ = nullptr;
                std::map<std::string, VarDecl*> system_values_ref_;
                std::set<StructDecl*> parent_struct_decl_refs_;
                std::set<VarDecl*> shader_output_var_decl_refs_;
            };

            struct AliasDecl : public Decl
            {
                AST_INTERFACE(AliasDecl);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;
                
                TypeDenoterPtr type_denoter_;
                AliasDeclStmnt* decl_stmnt_ref_ = nullptr;
            };

            struct FunctionDecl : public Decl
            {
                AST_INTERFACE(FunctionDecl);

                struct ParameterSemantics
                {
                    using IteratorFunc = std::function<void(VarDecl* var_decl_a)>;

                    void Add(VarDecl* var_decl_a);
                    bool Contains(VarDecl* var_decl_a) const;
                    void ForEach(const IteratorFunc& iterator_a);

                    bool Empty() const;
                    void UpdateDistribution();

                    std::vector<VarDecl*> var_decl_refs_;
                    std::vector<VarDecl*> var_decl_refs_sv_;
                };

                struct ParameterStructure
                {
                    Expr* expr_;
                    VarDecl* var_decl_;
                    StructDecl* struct_decl_;
                };

                FLAG_ENUM 
                {
                        FLAG(IsEntryPoint, 0),
                        FLAG(IsSecondaryEntryPoint, 1),
                        FLAG(HasNonReturnControlPath, 2),
                };

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                bool IsForwardDecl() const;
                bool HasVoidReturnType() const;
                bool IsMemberFunction() const;
                bool IsStatic() const;

                std::string ToString() const override;
                std::string ToString(bool use_param_names_a) const;
                std::string ToTypeDenoterString() const;

                bool EqualsSignature(const FunctionDecl& rhs_a,
                                     const Flags& compare_flags_a = 0) const;
                
                std::size_t NumMinArgs() const;
                std::size_t NumMaxArgs() const;

                void SetFuncImplRef(FunctionDecl* func_decl_a);

                bool MatchParameterWithTypeDenoter(
                        std::size_t param_index_a,
                        const TypeDenoter& arg_type_a,
                        bool implicit_conversion_a) const;

                static FunctionDecl* FetchFunctionDeclFromList(
                        const std::vector<FunctionDecl*>& func_decl_list_a,
                        const std::string& ident_a,
                        const std::vector<TypeDenoterPtr>& arg_type_denoters_a,
                        bool throw_error_if_no_match_a = true);

                TypeSpecifierPtr return_type_;
                std::vector<VarDeclStmntPtr> parameters_;
                IndexedSemantic semantic_ = Semantic::Undefined;
                std::vector<VarDeclStmntPtr> annotations_;
                CodeBlockPtr code_block_;

                ParameterSemantics input_semantics_;
                ParameterSemantics output_semantics_;

                BasicDeclStmnt* decl_stmnt_ref_ = nullptr;
                FunctionDecl* func_impl_ref_ = nullptr;
                std::vector<FunctionDecl*> func_forward_decl_refs_;
                StructDecl* struct_decl_ref_ = nullptr;

                std::vector<ParameterStructure> param_structs_;
            };

            struct UniformBufferDecl : public Decl
            {
                AST_INTERFACE(UniformBufferDecl);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                std::string ToString() const override;
                TypeModifier DeriveCommonStorageLayout(
                        const TypeModifier default_storge_layout_a =
                                TypeModifier::Undefined);

                bool AccumAlignedVectorSize(unsigned int& size_a,
                                            unsigned int& padding_a,
                                            unsigned int* offset_a = nullptr);

                UniformBufferType buffer_type_ = UniformBufferType::Undefined;
                std::vector<RegisterPtr> slot_registers_;
                std::vector<StmntPtr> local_stmnts_;

                std::vector<VarDeclStmntPtr> var_members_;
                TypeModifier common_storage_layout_ = TypeModifier::ColumnMajor;

                BasicDeclStmnt* decl_stmnt_ref_ = nullptr;
            };

            struct BufferDeclStmnt : public Stmnt
            {
                AST_INTERFACE(BufferDeclStmnt);

                void CollectDeclIdents(std::map<const AST*, std::string>&
                                               decl_ast_idents_a) const override;

                BufferTypeDenoterPtr type_denoter_;
                std::vector<BufferDeclPtr> buffer_decls_;
            };

            struct SamplerDeclStmnt : public Stmnt
            {
                AST_INTERFACE(SamplerDeclStmnt);

                void CollectDeclIdents(std::map<const AST*, std::string>&
                                               decl_ast_idents_a) const override;

                SamplerTypeDenoterPtr type_denoter_;
                std::vector<SamplerDeclPtr> sampler_decls_;
            };

            struct BasicDeclStmnt : public Stmnt
            {
                AST_INTERFACE(BasicDeclStmnt);

                DeclPtr decl_object_;
            };

            struct VarDeclStmnt : public Stmnt
            {
                AST_INTERFACE(VarDeclStmnt);

                FLAG_ENUM 
                {
                        FLAG(IsShaderInput, 0),
                        FLAG(IsShaderOutput, 1),
                        FLAG(IsParameter, 2),
                        FLAG(isSelfParameter, 3),
                        FLAG(IsBaseMember, 4),
                        FLAG(isImplicitConst, 5),
                };

                void CollectDeclIdents(std::map<const AST*, std::string>&
                                               decl_ast_idents_a) const override;

                std::string ToString(bool use_var_names_a = true) const;

                VarDecl* FetchVarDecl(const std::string& ident_a) const;
                VarDecl* FetchUniqueVarDecl() const;

                bool IsInput() const;
                bool IsOutput() const;
                bool IsUniform() const;
                bool IsConstOrUniform() const;
                void SetTypeModifier(const TypeModifier modifier_a);
                bool HasAnyTypeModifierOf(
                        const std::initializer_list<TypeModifier>& modifiers_a)
                        const;
               
                void ForEachVarDecl(const VarDeclIteratorFunctor& iterator_a);
                void MakeImplicitConst();

                StructDecl* FetchStructDeclRef() const;
                bool AccumAlignedVectorSize(unsigned int& size_a,
                                            unsigned int& padding_a,
                                            unsigned int* offset_a = nullptr);

                TypeSpecifierPtr type_specifier_;
                std::vector<VarDeclPtr> var_decls_;
            };

            struct AliasDeclStmnt : public Stmnt
            {
                AST_INTERFACE(AliasDeclStmnt);

                StructDeclPtr struct_decl_;
                std::vector<AliasDeclPtr> alias_decls_;
            };

            struct NullStmnt : public Stmnt
            {
                AST_INTERFACE(NullStmnt);
            };

            struct CodeBlockStmnt : public Stmnt
            {
                AST_INTERFACE(CodeBlockStmnt);

                CodeBlockPtr code_block_;
            };

            struct ForLoopStmnt : public Stmnt
            {
                AST_INTERFACE(ForLoopStmnt);

                StmntPtr init_stmnt_;
                ExprPtr condition_;
                ExprPtr iteration_;
                StmntPtr body_stmnt_;
            };

            struct WhileLoopStmnt : public Stmnt
            {
                AST_INTERFACE(WhileLoopStmnt);

                ExprPtr condition_;
                StmntPtr body_stmnt_;
            };

            struct DoWhileLoopStmnt : public Stmnt
            {
                AST_INTERFACE(DoWhileLoopStmnt);

                StmntPtr body_stmnt_;
                ExprPtr condition_;
            };

            struct IfStmnt : public Stmnt
            {
                AST_INTERFACE(IfStmnt);

                ExprPtr condition_;
                StmntPtr body_stmnt_;
                ElseStmntPtr else_stmnt_;
            };

            struct ElseStmnt : public Stmnt
            {
                AST_INTERFACE(ElseStmnt);

                StmntPtr body_stmnt_;
            };

            struct SwitchStmnt : public Stmnt
            {
                AST_INTERFACE(SwitchStmnt);

                ExprPtr selector_;
                std::vector<SwitchCasePtr> cases_;
            };

            struct ExprStmnt : public Stmnt
            {
                AST_INTERFACE(ExprStmnt);

                ExprPtr expr_;
            };

            struct ReturnStmnt : public Stmnt
            {
                AST_INTERFACE(ReturnStmnt);

                FLAG_ENUM {
                        FLAG(IsEndOfFunction, 0),
                };

                ExprPtr expr_;
            };

            struct CtrlTransferStmnt : public Stmnt
            {
                AST_INTERFACE(CtrlTransferStmnt);

                CtrlTransfer transfer_ = CtrlTransfer::Undefined;
            };

            struct LayoutStmnt : public Stmnt
            {
                AST_INTERFACE(LayoutStmnt);

                bool is_input_ = false;
                bool is_output_ = false;
            };

            struct NullExpr : public Expr
            {
                AST_INTERFACE(NullExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;
            };

            struct SequenceExpr : public Expr
            {
                AST_INTERFACE(SequenceExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                const Expr* Find(const FindPredicateConstFunctor& predicate_a,
                                 unsigned int flags_a = SearchAll) const override;

                void Append(const ExprPtr& expr_a);

                std::vector<ExprPtr> exprs_;
            };

            struct LiteralExpr : public Expr
            {
                AST_INTERFACE(LiteralExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                void ConvertDataType(const DataType type_a);
                std::string GetStringValue() const;
                std::string GetLiteralValue(bool enable_suffix_a = true) const;

                bool IsNull() const;
                bool IsSpaceRequiredForSubscript() const;

                std::string value_;
                DataType data_type_ = DataType::Undefined;
            };

            struct TypeSpecifierExpr : public Expr
            {
                AST_INTERFACE(TypeSpecifierExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                TypeSpecifierPtr type_specifier_;
            };

            struct TernaryExpr : public Expr
            {
                AST_INTERFACE(TernaryExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                const Expr* Find(const FindPredicateConstFunctor& predicate_a,
                                 unsigned int flags_a = SearchAll) const override;

                bool IsVectorCondition() const;

                ExprPtr cond_expr_;
                ExprPtr then_expr_;
                ExprPtr else_expr_;
            };

            struct BinaryExpr : public Expr
            {
                AST_INTERFACE(BinaryExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                const Expr* Find(const FindPredicateConstFunctor& predicate_a,
                                 unsigned int flags_a = SearchAll) const override;

                ExprPtr lhs_expr_;
                BinaryOp op_ = BinaryOp::Undefined;
                ExprPtr rhs_expr_;
            };

            struct UnaryExpr : public Expr
            {
                AST_INTERFACE(UnaryExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                const Expr* Find(const FindPredicateConstFunctor& predicate_a,
                                 unsigned int flags_a = SearchAll) const override;

                const ObjectExpr* FetchLValueExpr() const override;

                UnaryOp op_ = UnaryOp::Undefined;
                ExprPtr expr_;
            };

            struct PostUnaryExpr : public Expr
            {
                AST_INTERFACE(PostUnaryExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                const Expr* Find(const FindPredicateConstFunctor& predicate_a,
                                 unsigned int flags_a = SearchAll) const override;

                ExprPtr expr_;
                UnaryOp op_ = UnaryOp::Undefined;
            };

            struct CallExpr : public Expr
            {
                AST_INTERFACE(CallExpr);

                FLAG_ENUM 
                {
                        FLAG(CanInlineIntrinsicWrapper, 0),
                        FLAG(IsWrapperCall, 1),
                };

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                const Expr* Find(const FindPredicateConstFunctor& predicate_a,
                                 unsigned int flags_a = SearchAll) const override;

                IndexedSemantic FetchSemantic() const override;

#if 0
    std::vector<Expr*> GetArguments() const;
#endif

                FunctionDecl* GetFunctionDecl() const;
                FunctionDecl* GetFunctionImpl() const;

                void ForEachOutputArgument(const ExprIteratorFunctor& iterator_a);
                void ForEachArgumentWithParameterType(
                        const ArgumentParameterTypeFunctor& iterator_a);
                void PushArgumentFront(const ExprPtr& expr_a);

                bool PushPrefixToArguments();
                bool PopPrefixFromArguments();
                bool MergeArguments(std::size_t first_arg_index_a,
                                    const MergeExprFunctor& merge_functor_a);

                Expr* GetMemberFuncObjectExpr() const;

                ExprPtr prefix_expr_;
                bool is_static_ = false;
                std::string ident_;
                TypeDenoterPtr type_denoter_;
                std::vector<ExprPtr> arguments_;

                FunctionDecl* func_decl_ref_ = nullptr;
                Intrinsic intrinsic_ = Intrinsic::Undefined;
                std::vector<VarDecl*> default_param_refs_;
            };

            struct BracketExpr : public Expr
            {
                AST_INTERFACE(BracketExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                const Expr* Find(const FindPredicateConstFunctor& predicate_a,
                                 unsigned int flags_a = SearchAll) const override;

                const ObjectExpr* FetchLValueExpr() const override;
                IndexedSemantic FetchSemantic() const override;

                ExprPtr expr_;
            };

            struct AssignExpr : public Expr
            {
                AST_INTERFACE(AssignExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                const Expr* Find(const FindPredicateConstFunctor& predicate_a,
                                 unsigned int flags_a = SearchAll) const override;

                const ObjectExpr* FetchLValueExpr() const override;

                ExprPtr lvalue_expr_;
                AssignOp op_ = AssignOp::Undefined;
                ExprPtr rvalue_expr_;
            };

            struct ObjectExpr : public Expr
            {
                AST_INTERFACE(ObjectExpr);

                FLAG_ENUM 
                {
                        FLAG(IsImmutable, 1),
                        FLAG(IsBaseStructNamespace, 2),
                };

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                const Expr* Find(const FindPredicateConstFunctor& predicate_a,
                                 unsigned int flags_a = SearchAll) const override;

                const ObjectExpr* FetchLValueExpr() const override;

                IndexedSemantic FetchSemantic() const override;

                bool IsTrivialCopyable(
                        unsigned int max_tree_depth_a = 3) const override;

                BaseTypeDenoterPtr GetTypeDenoterFromSubscript() const;

                std::string ToStringAsNamespace() const;
                void ReplaceSymbol(Decl* symbol_a);

                template <typename T>
                T* FetchSymbol() const
                {
                    if (symbol_ref_)
                    {
                        if (auto ast_ = symbol_ref_->As<T>())
                            return ast_;
                    }
                    return nullptr;
                }

                VarDecl* FetchVarDecl() const;

                ExprPtr prefix_expr_;
                bool is_static_ = false;
                std::string ident_;

                Decl* symbol_ref_ = nullptr;
            };

            struct ArrayExpr : public Expr
            {
                AST_INTERFACE(ArrayExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                const Expr* Find(const FindPredicateConstFunctor& predicate_a,
                                 unsigned int flags_a = SearchAll) const override;

                const ObjectExpr* FetchLValueExpr() const override;
                std::size_t NumIndices() const;

                ExprPtr prefix_expr_;
                std::vector<ExprPtr> array_indices_;
            };

            struct CastExpr : public Expr
            {
                AST_INTERFACE(CastExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                const Expr* Find(const FindPredicateConstFunctor& predicate_a,
                                 unsigned int flags_a = SearchAll) const override;

                TypeSpecifierPtr type_specifier_;
                ExprPtr expr_;
            };

            struct InitializerExpr : public Expr
            {
                AST_INTERFACE(InitializerExpr);

                TypeDenoterPtr DeriveTypeDenoter(
                        const TypeDenoter* expected_type_denoter_a) override;

                const Expr* Find(const FindPredicateConstFunctor& predicate_a,
                                 unsigned int flags_a = SearchAll) const override;

                std::size_t NumElementsUnrolled() const;

                void CollectElements(std::vector<ExprPtr>& elements_a) const;
                void UnrollElements();

                ExprPtr FetchSubExpr(
                        const std::vector<int>& array_indices_a) const;

                bool NextArrayIndices(std::vector<int>& array_indices_a) const;

                std::vector<ExprPtr> exprs_;
            };

#undef AST_INTERFACE
#undef DECL_AST_ALIAS
#undef FLAG_ENUM
#undef FLAG
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
