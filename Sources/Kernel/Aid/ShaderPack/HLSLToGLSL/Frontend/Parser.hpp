// Copyright (c) 2025 Case Technologies

#pragma once
#include "../AST/AST.hpp"
#include "../AST/ASTEnums.hpp"
#include "../AST/Token.hpp"
#include "../AST/Visitor/Visitor.hpp"
#include "../Helper.hpp"
#include "../Log.hpp"
#include "../Report/ReportHandler.hpp"
#include "Scanner.hpp"

#include <map>
#include <stack>
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class Parser
            {
            public:
                virtual ~Parser() = default;

            protected:
                using Tokens = Token::Types;
                using BinaryOpList = std::initializer_list<BinaryOp>;

                struct ParsingState
                {
                    bool active_template_;
                };

                Parser(Log* log_a);

                void Error(const std::string& msg_a,
                           const SourceArea& area_a,
                           bool break_with_expection_a = true);

                void Error(const std::string& msg_a,
                           const Token* tkn_a,
                           bool break_with_expection_a = true);

                void Error(const std::string& msg_a,
                           bool prev_token_a = true,
                           bool break_with_expection_a = true);

                void ErrorUnexpected(const std::string& hint_a = "",
                                     const Token* tkn_a = nullptr,
                                     bool break_with_expection_a = false);

                void ErrorUnexpected(const Tokens type_a,
                                     const Token* tkn_a = nullptr,
                                     bool break_with_expection_a = false);

                void ErrorInternal(const std::string& msg_a,
                                   const std::string& proc_name_a);

                void Warning(const std::string& msg_a, const SourceArea& area_a);
                void Warning(const std::string& msg_a, const Token* tkn_a);
                void Warning(const std::string& msg_a, bool prev_token_a = true);

                void EnableWarnings(bool enable_a);

                virtual ScannerPtr MakeScanner() = 0;

                virtual void PushScannerSource(
                        const SourceCodePtr& source_a,
                        const std::string& filename_a = "");

                virtual bool PopScannerSource();

                ParsingState ActiveParsingState() const;

                Scanner& GetScanner();
                std::string GetCurrentFilename() const;

                TokenPtr Accept(const Tokens type_a);
                TokenPtr Accept(const Tokens type_a, const std::string& spell_a);
                virtual TokenPtr AcceptIt();

                void PushTokenString(const TokenPtrString& token_string_a);
                void PopTokenString();

                void IgnoreWhiteSpaces(bool include_new_lines_a = false,
                                       bool include_comments_a = false);
                void IgnoreNewLines();

                template <typename T>
                const T& UpdateSourceArea(const T& ast_a,
                                          const AST* area_origin_ast_a = nullptr)
                {
                    if (area_origin_ast_a)
                        ast_a->area_ = area_origin_ast_a->area_;
                    ast_a->area_.Update(GetScanner().PreviousToken()->Area());
                    return ast_a;
                }

                template <typename T>
                const T& UpdateSourceArea(const T& ast_a,
                                          const ASTPtr& first_area_origin_ast_a,
                                          const ASTPtr& last_area_origin_ast_a)
                {
                    ast_a->area_ = first_area_origin_ast_a->area_;
                    ast_a->area_.Update(*last_area_origin_ast_a);
                    return ast_a;
                }

                template <typename T>
                const T& UpdateSourceAreaOffset(const T& ast_a)
                {
                    ast_a->area_.Offset(GetScanner().PreviousToken()->Pos());
                    return ast_a;
                }

                void PushParsingState(const ParsingState& state_a);
                void PopParsingState();

                void PushPreParsedAST(const ASTPtr& ast_a);
                ASTPtr PopPreParsedAST();

                ExprPtr ParseGenericExpr();
                TernaryExprPtr ParseTernaryExpr(const ExprPtr& cond_expr_a);

                ExprPtr ParseLogicOrExpr();
                ExprPtr ParseLogicAndExpr();
                ExprPtr ParseBitwiseOrExpr();
                ExprPtr ParseBitwiseXOrExpr();
                ExprPtr ParseBitwiseAndExpr();
                ExprPtr ParseEqualityExpr();
                ExprPtr ParseRelationExpr();
                ExprPtr ParseShiftExpr();
                ExprPtr ParseAddExpr();
                ExprPtr ParseSubExpr();
                ExprPtr ParseMulExpr();
                ExprPtr ParseDivExpr();
                ExprPtr ParseValueExpr();

                virtual ExprPtr ParsePrAzaryExpr() = 0;

                int ParseIntLiteral(TokenPtr tkn_a = nullptr);
                int ParseIntLiteral(const std::string& value_str_a,
                                    const Token* tkn_a = nullptr);

                inline Log* GetLog() const
                {
                    return log_;
                }

                inline ReportHandler& GetReportHandler()
                {
                    return report_handler_;
                }

                inline NameMangling& GetNameMangling()
                {
                    return name_mangling_;
                }

                const std::string* FindNameManglingPrefix(
                        const std::string& ident_a) const;

                template <typename T, typename... Args>
                std::shared_ptr<T> Make(Args&&... args_a)
                {
                    return std::make_shared<T>(GetScanner().Pos(),
                                               std::forward<Args>(args_a)...);
                }

                inline const TokenPtr& Tkn() const
                {
                    return tkn_;
                }

                inline Tokens TknType() const
                {
                    return Tkn()->Type();
                }

                inline bool Is(const Tokens type_a) const
                {
                    return (TknType() == type_a);
                }

                inline bool Is(const Tokens type_a,
                               const std::string& spell_a) const
                {
                    return (TknType() == type_a && Tkn()->Spell() == spell_a);
                }

            private:
                struct ScannerStackEntry
                {
                    ScannerPtr scanner_;
                    std::string filename_;
                    TokenPtr next_token_;
                };

                ExprPtr ParseLTRBinaryExpr(
                        const std::function<ExprPtr()>& parse_sub_expr_func_a,
                        const BinaryOpList& binary_ops_a);

                void IncUnexpectedTokenCounter();

                void AssertTokenType(const Tokens type_a);
                void AssertTokenSpell(const std::string& spell_a);

            private:
                ReportHandler report_handler_;
                NameMangling name_mangling_;

                Log* log_ = nullptr;
                TokenPtr tkn_;

                std::stack<ScannerStackEntry> scanner_stack_;
                std::stack<ParsingState> parsing_state_stack_;
                std::stack<ASTPtr> pre_parsed_ast_stack_;

                unsigned int unexpected_token_counter_ = 0;
                const unsigned int unexpected_token_lAzit_ = 3;

                bool enable_warnings_ = false;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel