// Copyright (c) 2025 Case Technologies

#pragma once
#include "../AST/AST.hpp"
#include "../AST/Token.hpp"
#include "../AST/Visitor/VisitorTracker.hpp"
#include "../HTG.hpp"
#include "../Report/ReportHandler.hpp"
#include "../SymbolTable.hpp"
#include "../Variant.hpp"

#include <stack>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            struct StructTypeDenoter;

            class Analyzer : protected VisitorTracker
            {
            public:
                Analyzer(Log* log_a = nullptr);

                bool DecorateAST(Program& program_a,
                                 const ShaderInput& input_desc_a,
                                 const ShaderOutput& output_desc_a);

            protected:
                using OnOverrideProc = ASTSymbolTable::OnOverrideProc;

                virtual void DecorateASTPrimary(
                        Program& program_a,
                        const ShaderInput& input_desc_a,
                        const ShaderOutput& output_desc_a) = 0;

                void SubmitReport(
                        bool is_error_a,
                        const std::string& msg_a,
                        const AST* ast_a = nullptr,
                        const std::vector<const AST*>& ast_appendices_a = {});

                void Error(const std::string& msg_a,
                           const AST* ast_a = nullptr,
                           const std::vector<const AST*>& ast_appendices_a = {});
                
                void ErrorUndeclaredIdent(const std::string& ident_a,
                                          const AST* ast_a = nullptr);
                
                void ErrorUndeclaredIdent(const std::string& ident_a,
                                          const std::string& context_name_a,
                                          const AST* ast_a = nullptr);
                
                void ErrorUndeclaredIdent(const std::string& ident_a,
                                          const std::string& context_name_a,
                                          const std::string& similar_ident_a,
                                          const AST* ast_a = nullptr);
                void ErrorInternal(const std::string& msg_a,
                                   const AST* ast_a = nullptr);

                void Warning(const std::string& msg_a, const AST* ast_a = nullptr);
                void WarningOnNullStmnt(const StmntPtr& ast_a,
                                        const std::string& stmnt_type_name_a);

                inline ReportHandler& GetReportHandler()
                {
                    return report_handler_;
                }

                bool WarnEnabled(unsigned int flags_a) const;

                void OpenScope();
                void CloseScope();
                void Register(const std::string& ident_a, AST* ast_a);

                AST* Fetch(const std::string& ident_a, const AST* ast_a = nullptr);
                AST* FetchFromCurrentScopeOrNull(
                        const std::string& ident_a) const;

                Decl* FetchDecl(const std::string& ident_a,
                                const AST* ast_a = nullptr);
                
                Decl* FetchType(const std::string& ident_a,
                                const AST* ast_a = nullptr);

                VarDecl* FetchVarDecl(const std::string& ident_a,
                                      const AST* ast_a = nullptr);
                
                FunctionDecl* FetchFunctionDecl(
                        const std::string& ident_a,
                        const std::vector<ExprPtr>& args_a,
                        const AST* ast_a = nullptr);
                
                FunctionDecl* FetchFunctionDecl(const std::string& ident_a,
                                                const AST* ast_a = nullptr);
                
                VarDecl* FetchVarDeclFromStruct(
                        const StructTypeDenoter& struct_type_denoter_a,
                        const std::string& ident_a,
                        const AST* ast_a = nullptr);

                FunctionDecl* FetchFunctionDeclFromStruct(
                        const StructTypeDenoter& struct_type_denoter_a,
                        const std::string& ident_a,
                        const std::vector<ExprPtr>& args_a,
                        const AST* ast_a = nullptr);

                StructDecl* FetchStructDeclFromIdent(const std::string& ident_a,
                                                     const AST* ast_a = nullptr);
                
                StructDecl* FetchStructDeclFromTypeDenoter(
                        const TypeDenoter& type_denoter_a);
                
                StructDecl* FindCompatibleStructDecl(const StructDecl& rhs_a);

                bool InsideGlobalScope() const;
                void AnalyzeTypeDenoter(TypeDenoterPtr& type_denoter_a,
                                        const AST* ast_a);
                
                void AnalyzeBufferTypeDenoter(BufferTypeDenoter& buffer_type_den_a,
                                              const AST* ast_a);
                
                void AnalyzeStructTypeDenoter(StructTypeDenoter& struct_type_den_a,
                                              const AST* ast_a);
                
                void AnalyzeAliasTypeDenoter(TypeDenoterPtr& type_denoter_a,
                                             const AST* ast_a);

                void AnalyzeTypeSpecifier(TypeSpecifier* type_specifier_a);
                void AnalyzeTypeSpecifierForParameter(
                        TypeSpecifier* type_specifier_a);

                void AnalyzeFunctionEndOfScopes(FunctionDecl& func_decl_a);
                void AnalyzeFunctionControlPath(FunctionDecl& func_decl_a);

                TypeDenoterPtr GetTypeDenoterFrom(
                        TypedAST* ast_a,
                        const TypeDenoter* expected_type_denoter_a = nullptr);

                void ValidateTypeCast(const TypeDenoter& source_type_den_a,
                                      const TypeDenoter& dest_type_den_a,
                                      const std::string& context_desc_a,
                                      const AST* ast_a = nullptr);
                
                void ValidateTypeCastFrom(TypedAST* source_ast_a,
                                          TypedAST* dest_ast_a,
                                          const std::string& context_desc_a);

                void AnalyzeConditionalExpression(Expr* expr_a);

                Variant EvaluateConstExpr(Expr& expr_a);
                Variant EvaluateConstExprObject(const ObjectExpr& expr_a);
                Variant EvaluateOrDefault(Expr& expr_a,
                                          const Variant& default_value_a = {});

                int EvaluateConstExprInt(Expr& expr_a);
                float EvaluateConstExprFloat(Expr& expr_a);

            private:
                bool CollectArgumentTypeDenoters(
                        const std::vector<ExprPtr>& args_a,
                        std::vector<TypeDenoterPtr>& arg_type_dens_a);

                std::string FetchSimilarIdent(
                        const std::string& ident_a,
                        StructDecl* struct_decl_a = nullptr) const;

                void OnReleaseSymbol(const ASTSymbolOverloadPtr& symbol_a);

                ReportHandler report_handler_;
                SourceCode* source_code_ = nullptr;

                ASTSymbolOverloadTable sym_table_;
                Flags warnings_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
