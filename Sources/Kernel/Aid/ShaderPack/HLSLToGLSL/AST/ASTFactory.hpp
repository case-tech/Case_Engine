// Copyright (c) 2025 Case Technologies

#pragma once
#include "AST.hpp"
#include "TypeDenoter.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class Variant;

            namespace ASTFactory
            {
                CallExprPtr MakeIntrinsicCallExpr(
                        const Intrinsic intrinsic_a,
                        const std::string& ident_a,
                        const TypeDenoterPtr& type_denoter_a,
                        const std::vector<ExprPtr>& arguments_a);

                CallExprPtr MakeTextureSamplerBindingCallExpr(
                        const ExprPtr& texture_object_expr_a,
                        const ExprPtr& sampler_object_expr_a);
               
                CallExprPtr MakeWrapperCallExpr(
                        const std::string& func_ident_a,
                        const TypeDenoterPtr& type_denoter_a,
                        const std::vector<ExprPtr>& arguments_a);

                InitializerExprPtr MakeInitializerExpr(
                        const std::vector<ExprPtr>& exprs_a);

                CallExprPtr MakeTypeCtorCallExpr(
                        const TypeDenoterPtr& type_denoter_a,
                        const std::vector<ExprPtr>& arguments_a);

                CastExprPtr MakeCastExpr(const TypeDenoterPtr& type_denoter_a,
                                         const ExprPtr& value_expr_a);
                
                CastExprPtr MakeLiteralCastExpr(
                        const TypeDenoterPtr& type_denoter_a,
                        const DataType literal_type_a,
                        const std::string& literal_value_a);

                BinaryExprPtr MakeBinaryExpr(const ExprPtr& lhs_expr_a,
                                             const BinaryOp op_a,
                                             const ExprPtr& rhs_expr_a);
               
                LiteralExprPtr MakeLiteralExpr(const DataType literal_type_a,
                                               const std::string& literal_value_a);

                LiteralExprPtr MakeLiteralExprOrNull(
                        const Variant& literal_value_a);

                AliasDeclStmntPtr MakeBaseTypeAlias(const DataType data_type_a,
                                                    const std::string& ident_a);

                TypeSpecifierPtr MakeTypeSpecifier(
                        const StructDeclPtr& struct_decl_a);
                
                TypeSpecifierPtr MakeTypeSpecifier(
                        const TypeDenoterPtr& type_denoter_a);
                
                TypeSpecifierPtr MakeTypeSpecifier(const DataType data_type_a);

                VarDeclStmntPtr MakeVarDeclStmnt(
                        const TypeSpecifierPtr& type_specifier_a,
                        const std::string& ident_a,
                        const ExprPtr& initializer_a = nullptr);
                
                VarDeclStmntPtr MakeVarDeclStmnt(
                        const DataType data_type_a,
                        const std::string& ident_a,
                        const ExprPtr& initializer_a = nullptr);

                VarDeclStmntPtr MakeVarDeclStmntSplit(
                        const VarDeclStmntPtr& var_decl_stmnt_a,
                        std::size_t idx_a);

                ObjectExprPtr MakeObjectExpr(const ExprPtr& prefix_expr_a,
                                             const std::string& ident_a,
                                             Decl* symbol_ref_a = nullptr);
                
                ObjectExprPtr MakeObjectExpr(const std::string& ident_a,
                                             Decl* symbol_ref_a = nullptr);
                
                ObjectExprPtr MakeObjectExpr(Decl* symbol_ref_a);

                ArrayExprPtr MakeArrayExpr(const ExprPtr& prefix_expr_a,
                                           std::vector<ExprPtr>&& array_indices_a);
                
                ArrayExprPtr MakeArrayExpr(
                        const ExprPtr& prefix_expr_a,
                        const std::vector<int>& array_indices_a);
                
                ArrayExprPtr MakeArrayExpr(
                        const ExprPtr& prefix_expr_a,
                        const std::vector<ExprPtr>::const_iterator&
                                array_indices_begin_a,
                        const std::vector<ExprPtr>::const_iterator&
                                array_indices_end_a);

                ArrayExprPtr MakeArrayExprSplit(const ArrayExprPtr& array_expr_a,
                                                std::size_t split_array_index_a);
                
                RegisterPtr MakeRegister(int slot_a,
                                         const RegisterType register_type_a =
                                                 RegisterType::Undefined);
                
                BracketExprPtr MakeBracketExpr(const ExprPtr& expr_a);

                ExprPtr MakeConstructorListExpr(
                        const ExprPtr& expr_a,
                        const std::vector<TypeDenoterPtr>& list_type_dens_a);
                
                ExprStmntPtr MakeAssignStmnt(const ExprPtr& lvalue_expr_a,
                                             const ExprPtr& rvalue_expr_a,
                                             const AssignOp op_a = AssignOp::Set);
                
                ExprStmntPtr MakeArrayAssignStmnt(
                        VarDecl* var_decl_a,
                        const std::vector<int>& array_indices_a,
                        const ExprPtr& assign_expr_a);

                ArrayDimensionPtr MakeArrayDimension(int array_size_a);
                CodeBlockStmntPtr MakeCodeBlockStmnt(const StmntPtr& stmnt_a);

                BasicDeclStmntPtr MakeStructDeclStmnt(
                        const StructDeclPtr& struct_decl_a);
                
                UniformBufferDeclPtr MakeUniformBufferDecl(
                        const std::string& ident_a,
                        int binding_slot_a = 0,
                        const UniformBufferType buffer_type_a =
                                UniformBufferType::ConstantBuffer);

                std::vector<ExprPtr> MakeArrayIndices(
                        const std::vector<int>& array_indices_a);
                
                std::vector<ArrayDimensionPtr> MakeArrayDimensionList(
                        const std::vector<int>& array_sizes_a);

                ExprPtr ConvertExprBaseType(const DataType data_type_a,
                                            const ExprPtr& sub_expr_a);
                
                ArrayDimensionPtr ConvertExprToArrayDimension(
                        const ExprPtr& expr_a);
                
                std::vector<ArrayDimensionPtr>
                ConvertExprListToArrayDimensionList(
                        const std::vector<ExprPtr>& exprs_a);
            } // namespace ASTFactory
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
