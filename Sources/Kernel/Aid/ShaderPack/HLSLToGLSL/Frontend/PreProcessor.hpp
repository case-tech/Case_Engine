// Copyright (c) 2025 Case Technologies

#pragma once
#include "../AST/ASTEnums.hpp"
#include "../AST/TokenString.hpp"
#include "../HTG.hpp"
#include "../Log.hpp"
#include "../SourceCode.hpp"
#include "Parser.hpp"
#include "PreProcessorScanner.hpp"

#include <functional>
#include <initializer_list>
#include <iostream>
#include <map>
#include <set>
#include <stack>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class PreProcessor : public Parser
            {
            public:
                PreProcessor(IncludeHandler& include_handler_a,
                             Log* log_a = nullptr);

                std::unique_ptr<std::iostream> Process(
                        const SourceCodePtr& input_a,
                        const std::string& filename_a = "",
                        bool write_line_marks_a = true,
                        bool write_line_mark_filenames_a = true,
                        bool enable_warnings_a = false);

                std::vector<std::string> ListDefinedMacroIdents() const;

            protected:
                struct Macro
                {
                    Macro() = default;
                    Macro(const Macro&) = default;
                    Macro& operator=(const Macro&) = default;

                    Macro(const TokenPtr& ident_tkn_a);
                    Macro(const TokenPtr& ident_tkn_a,
                          const TokenPtrString& value_a);

                    Macro(const TokenPtr& ident_tkn_a,
                          const TokenPtrString& value_a,
                          const std::vector<std::string>& parameters_a,
                          bool var_args_a = false,
                          bool std_macro_a = false,
                          bool empty_param_list_a = false);

                    bool HasParameterList() const;

                    TokenPtr ident_tkn_;
                    TokenPtrString token_string_;
                    std::vector<std::string> parameters_;

                    bool var_args_ = false;
                    bool std_macro_ = false;
                    bool empty_param_list_ = false;
                };

                virtual void ParseDirective(const std::string& directive_a,
                                            bool ignore_unknown_a);
                
                virtual void WriteLineDirective(unsigned int line_no_a,
                                                const std::string& filename_a);

                void IgnoreDirective();
                void DefineMacro(const Macro& macro_a);
                void DefineStandardMacro(const std::string& ident_a,
                                         int int_value_a = 1);
               
                void UndefineMacro(const std::string& ident_a,
                                   const Token* tkn_a = nullptr);

                bool IsDefined(const std::string& ident_a) const;

                virtual bool OnDefineMacro(const Macro& macro_a);
                virtual bool OnRedefineMacro(const Macro& macro_a,
                                             const Macro& previous_macro_a);
                
                virtual bool OnUndefineMacro(const Macro& macro_a);
                virtual bool OnSubstitueStdMacro(const Token& ident_tkn_a,
                                                 TokenPtrString& token_string_a);

                Variant EvaluateExpr(const TokenPtrString& token_string_a,
                                     const Token* tkn_a = nullptr);
                
                Variant ParseAndEvaluateExpr(const Token* tkn_a = nullptr);
                Variant ParseAndEvaluateArgumentExpr(
                        const Token* tkn_a = nullptr);

                inline std::stringstream& Out()
                {
                    return *output_;
                }

            private:
                struct IfBlock
                {
                    void SetActive(bool activate);

                    TokenPtr directive_token_;
                    SourceCodePtr directive_source_;
                    bool parent_active_ = true;
                    bool active_ = true;
                    bool was_active_ = false;
                    bool else_allowed_ = true;
                };

                using MacroPtr = std::shared_ptr<Macro>;

                ScannerPtr MakeScanner() override;

                void PushScannerSource(
                        const SourceCodePtr& source_a,
                        const std::string& filename_a = "") override;
                
                bool PopScannerSource() override;
                void PushIfBlock(const TokenPtr& directive_token_a,
                                 bool active_a = false,
                                 bool else_allowed_a = true);
                
                void SetIfBlock(const TokenPtr& directive_token_a,
                                bool active_a = false,
                                bool else_allowed_a = true);
                
                void PopIfBlock();
                IfBlock TopIfBlock() const;
                TokenPtrString ExpandMacro(
                        const Macro& macro_a,
                        const std::vector<TokenPtrString>& arguments_a);

                void WritePosToLineDirective();
                void ParseProgram();

                void ParesComment();
                void ParseIdent();
                TokenPtrString ParseIdentAsTokenString();
                TokenPtrString ParseIdentArgumentsForMacro(
                        const TokenPtr& ident_token_a,
                        const Macro& macro_a);
                
                void ParseMisc();
                void ParseDirective();
                void ParseAnyIfDirectiveAndSkipValidation();
                void ParseDirectiveDefine();
                void ParseDirectiveUndef();
                void ParseDirectiveInclude();
                void ParseDirectiveIf(bool skip_evaluation_a = false);
                void ParseDirectiveIfdef(bool skip_evaluation_a = false);
                void ParseDirectiveIfndef(bool skip_evaluation_a = false);
                void ParseDirectiveElif(bool skip_evaluation_a = false);
                void ParseDirectiveIfOrElifCondition(
                        bool is_else_branch_a,
                        bool skip_evaluation_a = false);
                
                void ParseDirectiveElse();
                void ParseDirectiveEndif();
                void ParseDirectivePragma();
                void ParseDirectiveLine();
                void ParseDirectiveError();

                ExprPtr ParseExpr();
                ExprPtr ParsePrimaryExpr() override;

                TokenPtrString ParseDirectiveTokenString(
                        bool expand_defined_directive_a = false,
                        bool ignore_comments_a = false);
               
                TokenPtrString ParseArgumentTokenString();
                std::string ParseDefinedMacro();

            private:
                IncludeHandler& include_handler_;

                std::unique_ptr<std::stringstream> output_;

                std::map<std::string, MacroPtr> macros_;
                std::set<std::string> once_included_;
                std::map<std::string, std::size_t> include_counter_;

                std::stack<IfBlock> if_block_stack_;

                bool write_line_marks_ = true;
                bool write_line_mark_filenames_ = true;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel