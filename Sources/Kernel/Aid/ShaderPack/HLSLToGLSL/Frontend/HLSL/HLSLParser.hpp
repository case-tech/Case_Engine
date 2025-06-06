// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../SymbolTable.hpp"
#include "../SLParser.hpp"
#include "HLSLScanner.hpp"

#include <map>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class HLSLParser : public SLParser
            {
            public:
                HLSLParser(Log* log_a = nullptr);

                ProgramPtr ParseSource(const SourceCodePtr& source_a,
                                       const NameMangling& name_mangling_a,
                                       const InputShaderVersion version_in_a,
                                       bool row_major_alignment_a = false,
                                       bool enable_warnings_a = false);

            private:
                ScannerPtr MakeScanner() override;

                bool IsDataType() const;
                bool IsBaseDataType() const;
                bool IsLiteral() const;
                bool IsArithmeticUnaryExpr() const;
                bool IsModifier() const;

                TypeSpecifierPtr MakeTypeSpecifierIfLhsOfCastExpr(
                        const ExprPtr& expr_);
                TokenPtr AcceptIt() override;

                void ProcessDirective(const std::string& ident_a);
                void ProcessDirectiveLine();
                void ProcessDirectivePragma();

                void OpenScope();
                void CloseScope();
                void RegisterTypeName(const std::string& ident_a);

                bool IsRegisteredTypeName(const std::string& ident_a) const;

                AliasDeclStmntPtr MakeAndRegisterBuildinAlias(
                        const DataType data_type_a,
                        const std::string& ident_a);

                void GeneratePreDefinedTypeAliases(Program& ast_a);
                TypeSpecifierPtr MakeTypeSpecifierWithPackAlignment();

                ProgramPtr ParseProgram(const SourceCodePtr& source_a);

                CodeBlockPtr ParseCodeBlock() override;
                VarDeclStmntPtr ParseParameter() override;
                StmntPtr ParseLocalStmnt() override;
                StmntPtr ParseForLoopInitializer() override;
                SwitchCasePtr ParseSwitchCase() override;
                VarDeclPtr ParseVarDecl(
                        VarDeclStmnt* decl_stmnt_ref_a,
                        const TokenPtr& ident_tkn_a = nullptr) override;

                SamplerValuePtr ParseSamplerValue();
                AttributePtr ParseAttribute();
                RegisterPtr ParseRegister(bool parse_colon_a = true);
                PackOffsetPtr ParsePackOffset(bool parse_colon_a = true);
                TypeSpecifierPtr ParseTypeSpecifier(bool parse_void_type_a = false);

                BufferDeclPtr ParseBufferDecl(
                        BufferDeclStmnt* decl_stmnt_ref_a,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                SamplerDeclPtr ParseSamplerDecl(
                        SamplerDeclStmnt* decl_stmnt_ref_a,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                StructDeclPtr ParseStructDecl(
                        bool parse_struct_tkn_a = true,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                AliasDeclPtr ParseAliasDecl(TypeDenoterPtr type_denoter_a);
                FunctionDeclPtr ParseFunctionDecl(
                        BasicDeclStmnt* decl_stmnt_ref_a,
                        const TypeSpecifierPtr& return_type_a = nullptr,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                UniformBufferDeclPtr ParseUniformBufferDecl();
                StmntPtr ParseGlobalStmnt();
                StmntPtr ParseGlobalStmntPrimary();
                StmntPtr ParseGlobalStmntWithTypeSpecifier();
                StmntPtr ParseGlobalStmntWithSamplerTypeDenoter();
                StmntPtr ParseGlobalStmntWithBufferTypeDenoter();
                BasicDeclStmntPtr ParseFunctionDeclStmnt(
                        const TypeSpecifierPtr& return_type_a = nullptr,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                BasicDeclStmntPtr ParseUniformBufferDeclStmnt();
                BufferDeclStmntPtr ParseBufferDeclStmnt(
                        const BufferTypeDenoterPtr& type_denoter_a = nullptr,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                SamplerDeclStmntPtr ParseSamplerDeclStmnt(
                        const SamplerTypeDenoterPtr& type_denoter_a = nullptr,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                VarDeclStmntPtr ParseVarDeclStmnt();
                AliasDeclStmntPtr ParseAliasDeclStmnt();

                StmntPtr ParseStmnt(bool allow_attributes_a = true);
                StmntPtr ParseStmntPrimary();
                StmntPtr ParseStmntWithStructDecl();
                StmntPtr ParseStmntWithIdent();

                ExprPtr ParsePrimaryExpr() override;
                ExprPtr ParsePrimaryExprPrefix();
                ExprPtr ParseExprWithSuffixOpt(ExprPtr expr_a);
                LiteralExprPtr ParseLiteralExpr();
                ExprPtr ParseTypeSpecifierOrCallExpr();
                TypeSpecifierExprPtr ParseTypeSpecifierExpr();
                UnaryExprPtr ParseUnaryExpr();
                PostUnaryExprPtr ParsePostUnaryExpr(const ExprPtr& expr_a);
                ExprPtr ParseExprWithBracketPrefix();
                ObjectExprPtr ParseObjectExpr(const ExprPtr& expr_a = nullptr);
                AssignExprPtr ParseAssignExpr(const ExprPtr& expr_a);
                ExprPtr ParseObjectOrCallExpr(const ExprPtr& expr_a = nullptr);
                CallExprPtr ParseCallExpr(
                        const ObjectExprPtr& object_expr_a = nullptr,
                        const TypeDenoterPtr& type_denoter_a = nullptr);
               
                CallExprPtr ParseCallExprWithPrefixOpt(
                        const ExprPtr& prefix_expr_a = nullptr,
                        bool is_static_a = false,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                CallExprPtr ParseCallExprAsTypeCtor(
                        const TypeDenoterPtr& type_denoter_a);

                std::vector<StmntPtr> ParseGlobalStmntList();
                std::vector<VarDeclStmntPtr> ParseAnnotationList();
                std::vector<RegisterPtr> ParseRegisterList(
                        bool parse_first_colon_a = true);
                
                std::vector<AttributePtr> ParseAttributeList();
                std::vector<BufferDeclPtr> ParseBufferDeclList(
                        BufferDeclStmnt* decl_stmnt_ref_a,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                std::vector<SamplerDeclPtr> ParseSamplerDeclList(
                        SamplerDeclStmnt* decl_stmnt_ref_a,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                std::vector<SamplerValuePtr> ParseSamplerValueList();
                std::vector<AliasDeclPtr> ParseAliasDeclList(
                        TypeDenoterPtr type_denoter_a);

                std::string ParseIdentWithNamespaceOpt(
                        ObjectExprPtr& namespace_expr_a,
                        TokenPtr ident_tkn_a = nullptr,
                        SourceArea* area_a = nullptr);

                TypeDenoterPtr ParseTypeDenoter(
                        bool allow_void_type_a = true,
                        StructDeclPtr* struct_decl_a = nullptr);
                
                TypeDenoterPtr ParseTypeDenoterPrimary(
                        StructDeclPtr* struct_decl_a = nullptr);
                
                TypeDenoterPtr ParseTypeDenoterWithStructDeclOpt(
                        StructDeclPtr& struct_decl_a,
                        bool allow_void_type_a = true);
                
                VoidTypeDenoterPtr ParseVoidTypeDenoter();
                BaseTypeDenoterPtr ParseBaseTypeDenoter();
                BaseTypeDenoterPtr ParseBaseVectorTypeDenoter();
                BaseTypeDenoterPtr ParseBaseMatrixTypeDenoter();
                BufferTypeDenoterPtr ParseBufferTypeDenoter();
                SamplerTypeDenoterPtr ParseSamplerTypeDenoter();
                StructTypeDenoterPtr ParseStructTypeDenoter();
                StructTypeDenoterPtr ParseStructTypeDenoterWithStructDeclOpt(
                        StructDeclPtr& struct_decl_a);
                
                AliasTypeDenoterPtr ParseAliasTypeDenoter(
                        std::string ident_a = "");

                void ParseAndIgnoreTechniquesAndNullStmnts();
                void ParseAndIgnoreTechnique();

                void ParseVarDeclSemantic(VarDecl& var_decl_a,
                                          bool allow_pack_offset_a = true);
                void ParseFunctionDeclSemantic(FunctionDecl& func_decl_a);

                DataType ParseDataType(const std::string& keyword_a);
                PrimitiveType ParsePrimitiveType();
                InterpModifier ParseInterpModifier();
                TypeModifier ParseTypeModifier();
                StorageClass ParseStorageClass();
                UniformBufferType ParseUniformBufferType();
                BufferType ParseBufferType();
                SamplerType ParseSamplerType();
                IndexedSemantic ParseSemantic(bool parse_colon_a = true);

                std::string ParseSamplerStateTextureIdent();

                bool ParseModifiers(TypeSpecifier* type_specifier_a,
                                    bool allow_primitive_type_a = false);

            private:
                using TypeNameSymbolTable = SymbolTable<bool>;
                TypeNameSymbolTable type_name_symbol_table_;
                bool use_d3d10_semantics_ = true;
                bool enable_cg_keywords_ = false;
                bool row_major_alignment_ = false;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
