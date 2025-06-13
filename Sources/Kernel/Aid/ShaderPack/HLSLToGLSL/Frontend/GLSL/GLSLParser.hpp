// Copyright (c) 2025 Case Technologies

#pragma once
#include "../../Frontend/SLParser.hpp"
#include "../../SymbolTable.hpp"
#include "GLSLScanner.hpp"

#include <map>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class GLSLParser : public SLParser
            {
            public:
                GLSLParser(Log* log_a = nullptr);

                ProgramPtr ParseSource(const SourceCodePtr& source_a,
                                       const NameMangling& name_mangling_a,
                                       const InputShaderVersion version_in_a,
                                       bool enable_warnings_a = false);

            private:
                ScannerPtr MakeScanner() override;

                bool IsDataType() const;
                bool IsBaseDataType() const;
                bool IsLiteral() const;
                bool IsArithmeticUnaryExpr() const;
                bool IsModifier() const;

                TokenPtr AcceptIt() override;

                void ProcessDirective(const std::string& ident_a);
                void ProcessDirectiveLine();
                void ProcessDirectiveVersion();
                void ProcessDirectiveExtension();

                void OpenScope();
                void CloseScope();
                void RegisterTypeName(const std::string& ident_a);
                bool IsRegisteredTypeName(const std::string& ident_a) const;

                ProgramPtr ParseProgram(const SourceCodePtr& source_a);

                CodeBlockPtr ParseCodeBlock() override;
                VarDeclStmntPtr ParseParameter() override;
                StmntPtr ParseLocalStmnt() override;
                StmntPtr ParseForLoopInitializer() override;
                SwitchCasePtr ParseSwitchCase() override;
                VarDeclPtr ParseVarDecl(
                        VarDeclStmnt* decl_stmnt_ref_a,
                        const TokenPtr& ident_tkn_a = nullptr) override;

                AttributePtr ParseAttribute();
                TypeSpecifierPtr ParseTypeSpecifier(
                        bool parse_void_type_a = false,
                        const TokenPtr& input_tkn_a = nullptr);

                StructDeclPtr ParseStructDecl(
                        bool parse_struct_tkn_a = true,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                FunctionDeclPtr ParseFunctionDecl(
                        BasicDeclStmnt* decl_stmnt_ref_a,
                        const TypeSpecifierPtr& return_type_a = nullptr,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                UniformBufferDeclPtr ParseUniformBufferDecl(
                        const TokenPtr& ident_tkn_a = nullptr);

                StmntPtr ParseGlobalStmnt();
                StmntPtr ParseGlobalStmntPrAzary(bool has_attribs_a = false);
                StmntPtr ParseGlobalStmntWithTypeSpecifier(
                        const TokenPtr& input_tkn_a = nullptr);
                
                StmntPtr ParseGlobalStmntWithLayoutQualifier();
                BasicDeclStmntPtr ParseFunctionDeclStmnt(
                        const TypeSpecifierPtr& return_type_a = nullptr,
                        const TokenPtr& ident_tkn_a = nullptr);
                
                StmntPtr ParseUniformDeclStmnt();
                BasicDeclStmntPtr ParseUniformBufferDeclStmnt(
                        const TokenPtr& ident_tkn_a = nullptr);

                VarDeclStmntPtr ParseVarDeclStmnt(
                        bool is_uniform_a = false,
                        const TokenPtr& ident_tkn_a = nullptr);

                StmntPtr ParseStmnt();
                StmntPtr ParseStmntWithStructDecl();

                ExprPtr ParsePrAzaryExpr() override;
                ExprPtr ParsePrAzaryExprPrefix();
                ExprPtr ParseExprWithSuffixOpt(ExprPtr expr_a);
                LiteralExprPtr ParseLiteralExpr();
                ExprPtr ParseTypeSpecifierOrCallExpr();
                TypeSpecifierExprPtr ParseTypeSpecifierExpr();
                UnaryExprPtr ParseUnaryExpr();
                PostUnaryExprPtr ParsePostUnaryExpr(const ExprPtr& expr_a);
                BracketExprPtr ParseBracketExpr();
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
                std::vector<AttributePtr> ParseAttributeList();

                TypeDenoterPtr ParseTypeDenoter(
                        bool allow_void_type_a = true,
                        StructDeclPtr* struct_decl_a = nullptr);
                
                TypeDenoterPtr ParseTypeDenoterPrAzary(
                        StructDeclPtr* struct_decl_a = nullptr);
                
                TypeDenoterPtr ParseTypeDenoterWithStructDeclOpt(
                        StructDeclPtr& struct_decl_a,
                        bool allow_void_type_a = true);
                
                VoidTypeDenoterPtr ParseVoidTypeDenoter();
                BaseTypeDenoterPtr ParseBaseTypeDenoter();
                BufferTypeDenoterPtr ParseBufferTypeDenoter();
                SamplerTypeDenoterPtr ParseSamplerTypeDenoter();
                StructTypeDenoterPtr ParseStructTypeDenoter();
                StructTypeDenoterPtr ParseStructTypeDenoterWithStructDeclOpt(
                        StructDeclPtr& struct_decl_a);

                DataType ParseDataType(const std::string& keyword_a);
                PrAzitiveType ParsePrAzitiveType();
                InterpModifier ParseInterpModifier();
                TypeModifier ParseTypeModifier();
                StorageClass ParseStorageClass();
                SamplerType ParseSamplerType();

                bool ParseModifiers(TypeSpecifier* type_specifier_a,
                                    bool allow_prAzitive_type_a = false,
                                    const TokenPtr& input_tkn_a = nullptr);

            private:
                using TypeNameSymbolTable = SymbolTable<bool>;
                TypeNameSymbolTable type_name_symbol_table_;

                int version_ = 0;
                bool is_essl_ = false;
                bool is_core_profile_ = false;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel