#include "Parser.hpp"

#include "../AST/AST.hpp"
#include "../Exception.hpp"
#include "../Report/ReportIdents.hpp"

#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            Parser::Parser(Log* log_a) : report_handler_ {log_a}, log_ {log_a}
            {}

            static SourceArea GetTokenArea(const Token* tkn_a)
            {
                return (tkn_a != nullptr ? tkn_a->Area() : SourceArea::ignore);
            }

            void Parser::Error(const std::string& msg_a,
                               const SourceArea& area_a,
                               bool break_with_expection_a)
            {
                report_handler_.SubmitReport(break_with_expection_a,
                                            ReportTypes::Error,
                                            R_SyntaxError,
                                            msg_a,
                                            GetScanner().Source(),
                                            area_a);
            }

            void Parser::Error(const std::string& msg_a,
                               const Token* tkn_a,
                               bool break_with_expection_a)
            {
                if (tkn_a != nullptr && tkn_a->Type() == Tokens::EndOfStream)
                    break_with_expection_a = true;

                Error(msg_a, GetTokenArea(tkn_a), break_with_expection_a);
            }

            void Parser::Error(const std::string& msg_a,
                               bool prev_token_a,
                               bool break_with_expection_a)
            {
                auto tkn_l_ = (prev_token_a ? GetScanner().PreviousToken().get()
                                      : GetScanner().ActiveToken().get());
                
                Error(msg_a, tkn_l_, break_with_expection_a);
            }

            void Parser::ErrorUnexpected(const std::string& hint_a,
                                         const Token* tkn_a,
                                         bool break_with_expection_a)
            {
                if (!tkn_a)
                    tkn_a = tkn_.get();

                IncUnexpectedTokenCounter();

                Error(R_UnexpectedToken(Token::TypeToString(tkn_a->Type()), hint_a),
                      tkn_a,
                      break_with_expection_a);

                AcceptIt();
            }

            void Parser::ErrorUnexpected(const Tokens type_a,
                                         const Token* tkn_a,
                                         bool break_with_expection_a)
            {
                auto type_name_ = Token::TypeToString(type_a);
                if (type_name_.empty())
                    ErrorUnexpected("", tkn_a, break_with_expection_a);
                else
                    ErrorUnexpected(R_Expected(type_name_),
                                    tkn_a,
                                    break_with_expection_a);
            }

            void Parser::ErrorInternal(const std::string& msg_a,
                                       const std::string& proc_name_a)
            {
                report_handler_.SubmitReport(true,
                                            ReportTypes::Error,
                                            R_InternalError,
                                            msg_a + R_InFunction(proc_name_a),
                                            nullptr,
                                            SourceArea::ignore);
            }

            void Parser::Warning(const std::string& msg_a, const SourceArea& area_a)
            {
                if (enable_warnings_)
                    report_handler_.Warning(false,
                                           msg_a,
                                           GetScanner().Source(),
                                           area_a);
            }

            void Parser::Warning(const std::string& msg_a, const Token* tkn_a)
            {
                Warning(msg_a, GetTokenArea(tkn_a));
            }

            void Parser::Warning(const std::string& msg_a, bool prev_token_a)
            {
                Warning(msg_a,
                        prev_token_a ? GetScanner().PreviousToken().get()
                                  : GetScanner().ActiveToken().get());
            }

            void Parser::EnableWarnings(bool enable_a)
            {
                enable_warnings_ = enable_a;
            }

            void Parser::PushScannerSource(const SourceCodePtr& source_a,
                                           const std::string& filename_a)
            {
                if (!scanner_stack_.empty())
                    scanner_stack_.top().next_token_ = tkn_;

                auto scanner_ = MakeScanner();
                if (!scanner_)
                    RuntimeErr(R_FailedToCreateScanner);

                scanner_stack_.push({scanner_, filename_a, nullptr});

                if (!scanner_->ScanSource(source_a))
                    RuntimeErr(R_FailedToScanSource);

                scanner_->Source()->NextSourceOrigin(filename_a, 0);

                AcceptIt();
            }

            bool Parser::PopScannerSource()
            {
                if (scanner_stack_.empty())
                    return false;

                scanner_stack_.pop();

                if (scanner_stack_.empty())
                    return false;

                tkn_ = scanner_stack_.top().next_token_;

                return (tkn_ != nullptr);
            }

            Scanner& Parser::GetScanner()
            {
                if (scanner_stack_.empty())
                    RuntimeErr(R_MissingScanner);
                return *(scanner_stack_.top().scanner_);
            }

            std::string Parser::GetCurrentFilename() const
            {
                return (!scanner_stack_.empty() ? scanner_stack_.top().filename_
                                               : "");
            }

            TokenPtr Parser::Accept(const Tokens type_)
            {
                AssertTokenType(type_);
                unexpected_token_counter_ = 0;
                return AcceptIt();
            }

            TokenPtr Parser::Accept(const Tokens type_a, const std::string& spell_a)
            {
                AssertTokenType(type_a);
                AssertTokenSpell(spell_a);
                unexpected_token_counter_ = 0;
                return AcceptIt();
            }

            TokenPtr Parser::AcceptIt()
            {
                if (tkn_ && tkn_->Type() == Tokens::EndOfStream)
                    Error(R_UnexpectedEndOfStream, tkn_.get());

                auto prev_tkn_ = tkn_;
                tkn_ = GetScanner().Next();

                return prev_tkn_;
            }

            void Parser::PushTokenString(const TokenPtrString& token_string_a)
            {
                GetScanner().PushTokenString(token_string_a);
                AcceptIt();
            }

            void Parser::PopTokenString()
            {
                GetScanner().PopTokenString();
            }

            void Parser::IgnoreWhiteSpaces(bool include_new_lines_a,
                                           bool include_comments_a)
            {
                while (Is(Tokens::WhiteSpace)
                       || (include_new_lines_a && Is(Tokens::NewLine))
                       || (include_comments_a && Is(Tokens::Comment)))
                    AcceptIt();
            }

            void Parser::IgnoreNewLines()
            {
                while (Is(Tokens::NewLine))
                    AcceptIt();
            }

            void Parser::PushParsingState(const ParsingState& state_a)
            {
                parsing_state_stack_.push(state_a);
            }

            void Parser::PopParsingState()
            {
                parsing_state_stack_.pop();
            }

            void Parser::PushPreParsedAST(const ASTPtr& ast_a)
            {
                pre_parsed_ast_stack_.push(ast_a);
            }

            ASTPtr Parser::PopPreParsedAST()
            {
                if (!pre_parsed_ast_stack_.empty())
                {
                    auto ast_ = pre_parsed_ast_stack_.top();
                    pre_parsed_ast_stack_.pop();
                    return ast_;
                }
                return nullptr;
            }

            Parser::ParsingState Parser::ActiveParsingState() const
            {
                return (parsing_state_stack_.empty() ? ParsingState {false}
                                                   : parsing_state_stack_.top());
            }

            ExprPtr Parser::ParseGenericExpr()
            {
                auto ast_ = ParseLogicOrExpr();
                if (Is(Tokens::TernaryOp))
                    return ParseTernaryExpr(ast_);

                return ast_;
            }

            TernaryExprPtr Parser::ParseTernaryExpr(const ExprPtr& cond_expr_a)
            {
                auto ast_ = Make<TernaryExpr>();
                ast_->cond_expr_ = cond_expr_a;
                ast_->area_ = cond_expr_a->area_;

                Accept(Tokens::TernaryOp);

                UpdateSourceAreaOffset(ast_);

                ast_->then_expr_ = ParseGenericExpr();
                Accept(Tokens::Colon);
                ast_->else_expr_ = ParseGenericExpr();

                return UpdateSourceArea(ast_);
            }

            ExprPtr Parser::ParseLogicOrExpr()
            {
                return ParseLTRBinaryExpr(std::bind(&Parser::ParseLogicAndExpr,
                                                    this),
                                          {BinaryOp::LogicalOr});
            }

            ExprPtr Parser::ParseLogicAndExpr()
            {
                return ParseLTRBinaryExpr(std::bind(&Parser::ParseBitwiseOrExpr,
                                                    this),
                                          {BinaryOp::LogicalAnd});
            }

            ExprPtr Parser::ParseBitwiseOrExpr()
            {
                return ParseLTRBinaryExpr(
                        std::bind(&Parser::ParseBitwiseXOrExpr, this),
                        {BinaryOp::Or});
            }

            ExprPtr Parser::ParseBitwiseXOrExpr()
            {
                return ParseLTRBinaryExpr(
                        std::bind(&Parser::ParseBitwiseAndExpr, this),
                        {BinaryOp::Xor});
            }

            ExprPtr Parser::ParseBitwiseAndExpr()
            {
                return ParseLTRBinaryExpr(std::bind(&Parser::ParseEqualityExpr,
                                                    this),
                                          {BinaryOp::And});
            }

            ExprPtr Parser::ParseEqualityExpr()
            {
                return ParseLTRBinaryExpr(std::bind(&Parser::ParseRelationExpr,
                                                    this),
                                          {BinaryOp::Equal,
                                           BinaryOp::NotEqual});
            }

            ExprPtr Parser::ParseRelationExpr()
            {
                if (ActiveParsingState().active_template_)
                    return ParseLTRBinaryExpr(std::bind(&Parser::ParseShiftExpr,
                                                        this),
                                              {BinaryOp::LessEqual,
                                               BinaryOp::GreaterEqual});
                else
                    return ParseLTRBinaryExpr(std::bind(&Parser::ParseShiftExpr,
                                                        this),
                                              {BinaryOp::Less,
                                               BinaryOp::LessEqual,
                                               BinaryOp::Greater,
                                               BinaryOp::GreaterEqual});
            }

            ExprPtr Parser::ParseShiftExpr()
            {
                return ParseLTRBinaryExpr(std::bind(&Parser::ParseAddExpr,
                                                    this),
                                          {BinaryOp::LShift, BinaryOp::RShift});
            }

            ExprPtr Parser::ParseAddExpr()
            {
                return ParseLTRBinaryExpr(std::bind(&Parser::ParseSubExpr,
                                                    this),
                                          {BinaryOp::Add});
            }

            ExprPtr Parser::ParseSubExpr()
            {
                return ParseLTRBinaryExpr(std::bind(&Parser::ParseMulExpr,
                                                    this),
                                          {BinaryOp::Sub});
            }

            ExprPtr Parser::ParseMulExpr()
            {
                return ParseLTRBinaryExpr(std::bind(&Parser::ParseDivExpr,
                                                    this),
                                          {BinaryOp::Mul});
            }

            ExprPtr Parser::ParseDivExpr()
            {
                return ParseLTRBinaryExpr(std::bind(&Parser::ParseValueExpr,
                                                    this),
                                          {BinaryOp::Div, BinaryOp::Mod});
            }

            ExprPtr Parser::ParseValueExpr()
            {
                return ParsePrimaryExpr();
            }

            int Parser::ParseIntLiteral(TokenPtr tkn_a)
            {
                if (!tkn_a)
                    tkn_a = Accept(Tokens::IntLiteral);

                return ParseIntLiteral(tkn_a->Spell(), tkn_a.get());
            }

            int Parser::ParseIntLiteral(const std::string& value_str_a,
                                        const Token* tkn_a)
            {
                try
                {
                    return std::stoi(value_str_a);
                } 
                catch (const std::invalid_argument&)
                {
                    Error(R_ExpectedIntLiteral(value_str_a), tkn_a, false);
                } 
                catch (const std::out_of_range&)
                {
                    Error(R_IntLiteralOutOfRange(value_str_a), tkn_a, false);
                }
                return 0;
            }

            const std::string* Parser::FindNameManglingPrefix(
                    const std::string& ident_a) const
            {
                auto FindPrefix = [&ident_a](const std::string& prefix_)
                        -> const std::string* {
                    if (ident_a.compare(0, prefix_.size(), prefix_) == 0)
                        return &prefix_;
                    else
                        return nullptr;
                };

                if (auto prefix_ = FindPrefix(name_mangling_.input_prefix_))
                    return prefix_;

                if (auto prefix_ = FindPrefix(name_mangling_.output_prefix_))
                    return prefix_;

                if (auto prefix_ = FindPrefix(name_mangling_.reserved_word_prefix_))
                    return prefix_;

                if (auto prefix_ = FindPrefix(name_mangling_.temporary_prefix_))
                    return prefix_;

                return nullptr;
            }

            ExprPtr Parser::ParseLTRBinaryExpr(
                    const std::function<ExprPtr()>& parse_sub_expr_func_a,
                    const BinaryOpList& binary_ops_a)
            {
                ExprPtr ast_ = parse_sub_expr_func_a();

                while (Is(Tokens::BinaryOp))
                {
                    auto op_ = StringToBinaryOp(Tkn()->Spell());

                    if (std::find(binary_ops_a.begin(), binary_ops_a.end(), op_)
                        == binary_ops_a.end())
                        break;

                    AcceptIt();

                    auto binary_expr_ = Make<BinaryExpr>();
                    binary_expr_->lhs_expr_ = ast_;
                    binary_expr_->op_ = op_;
                    binary_expr_->rhs_expr_ = parse_sub_expr_func_a();

                    ast_ = binary_expr_;
                }

                return ast_;
            }

            void Parser::IncUnexpectedTokenCounter()
            {
                ++unexpected_token_counter_;

                if (unexpected_token_counter_ > unexpected_token_limit_)
                    report_handler_.SubmitReport(true,
                                                ReportTypes::Error,
                                                R_Error,
                                                R_TooManySyntaxErrors);
            }

            void Parser::AssertTokenType(const Tokens type_a)
            {
                while (tkn_->Type() != type_a)
                {
                    IncUnexpectedTokenCounter();

                    ErrorUnexpected(type_a);

                    AcceptIt();
                }
            }

            void Parser::AssertTokenSpell(const std::string& spell_a)
            {
                while (tkn_->Spell() != spell_a)
                {
                    IncUnexpectedTokenCounter();

                    Error(R_UnexpectedTokenSpell(tkn_->Spell(), spell_a),
                          true,
                          false);

                    AcceptIt();
                }
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel