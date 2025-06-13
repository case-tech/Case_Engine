#include "PreProcessor.hpp"

#include "../AST/AST.hpp"
#include "../AST/ASTFactory.hpp"
#include "../AST/Visitor/ExprEvaluator.hpp"
#include "../Exception.hpp"
#include "../Helper.hpp"
#include "../Report/ReportIdents.hpp"

#include <sstream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            PreProcessor::PreProcessor(IncludeHandler& include_handler_a, Log* log_a)
                : Parser {log_a}, include_handler_ {include_handler_a}
            {}

            std::unique_ptr<std::iostream> PreProcessor::Process(
                    const SourceCodePtr& input_a,
                    const std::string& filename_a,
                    bool write_line_marks_a,
                    bool write_line_mark_filenames_a,
                    bool enable_warnings_a)
            {
                output_ = MakeUnique<std::stringstream>();
                write_line_marks_ = write_line_marks_a;
                write_line_mark_filenames_ = write_line_mark_filenames_a;

                EnableWarnings(enable_warnings_a);

                PushScannerSource(input_a, filename_a);

                try
                {
                    ParseProgram();
                    return (GetReportHandler().HasErrors()
                                    ? nullptr
                                    : std::move(output_));
                } 
                catch (const Report& err_)
                {
                    if (GetLog())
                        GetLog()->SubmitReport(err_);
                }

                return nullptr;
            }

            std::vector<std::string> PreProcessor::ListDefinedMacroIdents()
                    const
            {
                std::vector<std::string> idents_;
                idents_.reserve(macros_.size());

                for (const auto& macro_ : macros_)
                    idents_.push_back(macro_.first);

                return idents_;
            }

            void PreProcessor::ParseDirective(const std::string& directive_a,
                                              bool ignore_unknown_a)
            {
                if (directive_a == "define")
                    ParseDirectiveDefine();
                else if (directive_a == "undef")
                    ParseDirectiveUndef();
                else if (directive_a == "include")
                    ParseDirectiveInclude();
                else if (directive_a == "if")
                    ParseDirectiveIf();
                else if (directive_a == "ifdef")
                    ParseDirectiveIfdef();
                else if (directive_a == "ifndef")
                    ParseDirectiveIfndef();
                else if (directive_a == "elif")
                    ParseDirectiveElif();
                else if (directive_a == "else")
                    ParseDirectiveElse();
                else if (directive_a == "endif")
                    ParseDirectiveEndif();
                else if (directive_a == "pragma")
                    ParseDirectivePragma();
                else if (directive_a == "line")
                    ParseDirectiveLine();
                else if (directive_a == "error")
                    ParseDirectiveError();
                else
                {
                    if (ignore_unknown_a)
                        Warning(R_UnknownPPDirective(directive_a));
                    else
                        Error(R_UnknownPPDirective(directive_a), true, false);
                    IgnoreDirective();
                }
            }

            void PreProcessor::WriteLineDirective(unsigned int line_no_a,
                                                  const std::string& filename_a)
            {
                if (write_line_mark_filenames_)
                    Out() << "#line " << line_no_a << " \"" << filename_a << '\"'
                          << std::endl;
                else
                    Out() << "#line " << line_no_a << std::endl;
            }

            void PreProcessor::IgnoreDirective()
            {
                ParseDirectiveTokenString();
            }

            void PreProcessor::DefineMacro(const Macro& macro_a)
            {
                if (!macro_a.ident_tkn_)
                    throw std::invalid_argument(R_InvalidMacroIdentTokenArg);

                if (OnDefineMacro(macro_a))
                {
                    const auto& ident_ = macro_a.ident_tkn_->Spell();

                    auto previous_macro_it_ = macros_.find(ident_);
                    if (previous_macro_it_ != macros_.end())
                    {
                        if (!OnRedefineMacro(macro_a, *previous_macro_it_->second))
                            return;
                    }

                    macros_[ident_] = std::make_shared<Macro>(macro_a);
                }
            }

            void PreProcessor::DefineStandardMacro(const std::string& ident_a,
                                                   int int_value_a)
            {
                auto ident_tkn_ = std::make_shared<Token>(SourcePosition::ignore,
                                                        Token::Types::Ident,
                                                        ident_a);
                auto value_tkn_ =
                        std::make_shared<Token>(SourcePosition::ignore,
                                                Token::Types::IntLiteral,
                                                std::to_string(int_value_a));

                TokenPtrString value_token_string_;
                value_token_string_.PushBack(value_tkn_);

                DefineMacro(
                        {ident_tkn_, value_token_string_, {}, false, true, false});
            }

            void PreProcessor::UndefineMacro(const std::string& ident_a,
                                             const Token* tkn_a)
            {
                auto it_ = macros_.find(ident_a);
                if (it_ != macros_.end())
                {
                    if (OnUndefineMacro(*it_->second))
                        macros_.erase(it_);
                } 
                else
                    Warning(R_FailedToUndefMacro(ident_a), tkn_a);
            }

            bool PreProcessor::IsDefined(const std::string& ident_a) const
            {
                return (macros_.find(ident_a) != macros_.end());
            }

            bool PreProcessor::OnDefineMacro(const Macro& macro_a)
            {
                (void)macro_a;
                return true;
            }

            bool PreProcessor::OnRedefineMacro(const Macro& macro_a,
                                               const Macro& previous_macro_a)
            {
                auto mismatch_param_ = (previous_macro_a.parameters_
                                              != macro_a.parameters_
                                      || previous_macro_a.var_args_
                                                 != macro_a.var_args_);
                auto mismatch_body_ = (previous_macro_a.token_string_
                                     != macro_a.token_string_);

                std::string context_desc_;

                if (mismatch_param_ && mismatch_body_)
                    context_desc_ = R_WithMismatchInParamListAndBody;
                else if (mismatch_param_)
                    context_desc_ = R_WithMismatchInParamList;
                else if (mismatch_body_)
                    context_desc_ = R_WithMismatchInBody;

                if (auto previous_macro_pos_ = previous_macro_a.ident_tkn_->Pos())
                    ReportHandler::HintForNextReport(
                            R_PrevDefinitionAt(previous_macro_pos_.ToString()));

                Warning(R_MacroRedef(macro_a.ident_tkn_->Spell(), context_desc_),
                        macro_a.ident_tkn_.get());

                return true;
            }

            bool PreProcessor::OnUndefineMacro(const Macro& macro_a)
            {
                (void)macro_a;
                return true;
            }

            bool PreProcessor::OnSubstitueStdMacro(const Token& ident_tkn_a,
                                                   TokenPtrString& token_string_a)
            {
                const auto& ident_ = ident_tkn_a.Spell();

                if (ident_ == "__FILE__")
                {
                    token_string_a.PushBack(
                            Make<Token>(Tokens::StringLiteral,
                                        '\"' + GetCurrentFilename() + '\"'));
                    return true;
                } 
                else if (ident_ == "__LINE__")
                {
                    token_string_a.PushBack(Make<Token>(
                            Tokens::IntLiteral,
                            std::to_string(GetScanner().Pos().Row())));
                    return true;
                }
                else if (ident_ == "__EVAL__")
                {
                    auto argument_ = ParseAndEvaluateExpr(&ident_tkn_a);
                    token_string_a.PushBack(
                            Make<Token>(Tokens::IntLiteral,
                                        std::to_string(argument_.ToInt())));
                    return true;
                }

                return false;
            }

            Variant PreProcessor::EvaluateExpr(
                    const TokenPtrString& token_string_a,
                    const Token* tkn_a)
            {
                Variant value_;
                PushTokenString(token_string_a);
                {
                    auto condition_expr_ = ParseExpr();

                    try
                    {
                        ExprEvaluator expr_avaluator_ {
                                ExprEvaluator::EvaluateReducedBinaryExpr};
                        value_ = expr_avaluator_.Evaluate(
                                *condition_expr_,
                                [](ObjectExpr* expr_) -> Variant {
                                    RuntAzeErr(R_UndeclaredIdent(expr_->ident_),
                                               expr_);
                                    return {};
                                });
                    } 
                    catch (const std::exception& e_)
                    {
                        Error(e_.what(), tkn_a);
                    }
                }
                PopTokenString();

                return value_;
            }

            Variant PreProcessor::ParseAndEvaluateExpr(const Token* tkn_a)
            {
                TokenPtrString token_string_;

                token_string_.PushBack(Make<Token>(Tokens::LBracket, "("));
                token_string_.PushBack(ParseDirectiveTokenString(true));
                token_string_.PushBack(Make<Token>(Tokens::RBracket, ")"));

                return EvaluateExpr(token_string_, tkn_a);
            }

            Variant PreProcessor::ParseAndEvaluateArgumentExpr(const Token* tkn_a)
            {
                return EvaluateExpr(ParseArgumentTokenString(), tkn_a);
            }

            ScannerPtr PreProcessor::MakeScanner()
            {
                return std::make_shared<PreProcessorScanner>(GetLog());
            }

            void PreProcessor::PushScannerSource(const SourceCodePtr& source_a,
                                                 const std::string& filename_a)
            {
                static const std::size_t include_counter_lAzit_ = 500;
                auto& counter_ = include_counter_[filename_a];
                if (counter_ >= include_counter_lAzit_)
                    Error(R_TooManyRecursiveIncludesOfFile(filename_a));
                else
                    ++counter_;

                Parser::PushScannerSource(source_a, filename_a);
                GetScanner().Source()->NextSourceOrigin(filename_a, 0);

                WritePosToLineDirective();
            }

            bool PreProcessor::PopScannerSource()
            {
                const auto filename_ = GetCurrentFilename();
                if (!filename_.empty())
                {
                    auto& counter_ = include_counter_[filename_];
                    if (counter_ > 0)
                        --counter_;
                }

                if (Parser::PopScannerSource())
                {
                    WritePosToLineDirective();
                    return true;
                }

                return false;
            }

            void PreProcessor::PushIfBlock(const TokenPtr& directive_token_a,
                                           bool active_a,
                                           bool else_allowed_a)
            {
                IfBlock if_block_;
                {
                    if_block_.directive_token_ = directive_token_a;
                    if_block_.directive_source_ = GetScanner().GetSharedSource();
                    if_block_.parent_active_ = TopIfBlock().active_;
                    if_block_.else_allowed_ = else_allowed_a;
                    if_block_.SetActive(active_a);
                }
                if_block_stack_.push(if_block_);

                WritePosToLineDirective();
            }

            void PreProcessor::SetIfBlock(const TokenPtr& directive_token_a,
                                          bool active_a,
                                          bool else_allowed_a)
            {
                if (!if_block_stack_.empty())
                {
                    auto& if_block_ = if_block_stack_.top();

                    if_block_.directive_token_ = directive_token_a;
                    if_block_.else_allowed_ = else_allowed_a;
                    if_block_.SetActive(active_a);

                    WritePosToLineDirective();
                }
            }

            void PreProcessor::PopIfBlock()
            {
                if (if_block_stack_.empty())
                    Error(R_MissingIfDirective, true);
                else
                    if_block_stack_.pop();

                WritePosToLineDirective();
            }

            PreProcessor::IfBlock PreProcessor::TopIfBlock() const
            {
                return (if_block_stack_.empty() ? IfBlock()
                                              : if_block_stack_.top());
            }

            TokenPtrString PreProcessor::ExpandMacro(
                    const Macro& macro_a,
                    const std::vector<TokenPtrString>& arguments_a)
            {
                TokenPtrString expanded_string_;

                if (macro_a.parameters_.size() > arguments_a.size())
                    return expanded_string_;

                auto ExpandTokenString =
                        [&](TokenPtrString::Container::const_iterator& tkn_it_,
                            const TokenPtrString::Container::const_iterator&
                                    tkn_it_end_) -> bool {
                    const auto& tkn_ = **tkn_it_;

                    switch (tkn_.Type())
                    {
                    case Tokens::Ident: 
                    {
                        auto ident_ = tkn_.Spell();
                        if (ident_ == "__VA_ARGS__")
                        {
                            for (std::size_t i_ = macro_a.parameters_.size();
                                 i_ < arguments_a.size();
                                 ++i_)
                            {
                                expanded_string_.PushBack(arguments_a[i_]);
                                if (i_ + 1 < arguments_a.size())
                                    expanded_string_.PushBack(
                                            Make<Token>(Tokens::Comma, ","));
                            }
                            return true;
                        } 
                        else
                        {
                            for (std::size_t i_ = 0; i_ < macro_a.parameters_.size();
                                 ++i_)
                            {
                                if (ident_ == macro_a.parameters_[i_])
                                {
                                    expanded_string_.PushBack(arguments_a[i_]);
                                    return true;
                                }
                            }
                        }
                    }
                    break;

                    case Tokens::Directive: {
                        auto ident_ = tkn_.Spell();
                        for (std::size_t i = 0; i < macro_a.parameters_.size();
                             ++i)
                        {
                            if (ident_ == macro_a.parameters_[i])
                            {
                                std::stringstream string_literal_;
                                string_literal_ << '\"' << arguments_a[i] << '\"';
                                expanded_string_.PushBack(
                                        Make<Token>(Tokens::StringLiteral,
                                                    string_literal_.str()));
                                return true;
                            }
                        }
                    }
                    break;

                    case Tokens::DirectiveConcat: {
                        expanded_string_.TrAzBack();
                        ++tkn_it_;

                        while (tkn_it_ != tkn_it_end_
                               && !DefaultTokenOfInterestFunctor::IsOfInterest(
                                       *tkn_it_))
                            ++tkn_it_;

                        --tkn_it_;
                        return true;
                    }
                    break;

                    default:
                        break;
                    }

                    return false;
                };

                const auto& tokens_ = macro_a.token_string_.GetTokens();
                for (auto it_ = tokens_.begin(); it_ != tokens_.end(); ++it_)
                {
                    if (!ExpandTokenString(it_, tokens_.end()))
                        expanded_string_.PushBack(*it_);
                }

                return expanded_string_;
            }

            void PreProcessor::WritePosToLineDirective()
            {
                if (write_line_marks_)
                {
                    auto pos_ = GetScanner().ActiveToken()->Pos();
                    WriteLineDirective(pos_.Row(), GetCurrentFilename());
                }
            }

            void PreProcessor::ParseProgram()
            {
                do
                {
                    while (!Is(Tokens::EndOfStream))
                    {
                        if (TopIfBlock().active_)
                        {
                            switch (TknType())
                            {
                            case Tokens::Directive:
                                ParseDirective();
                                break;
                            case Tokens::Comment:
                                ParesComment();
                                break;
                            case Tokens::Ident:
                                ParseIdent();
                                break;
                            default:
                                ParseMisc();
                                break;
                            }
                        } else
                        {
                            if (TknType() == Tokens::Directive)
                                ParseAnyIfDirectiveAndSkipValidation();
                            else
                                AcceptIt();
                        }
                    }
                } while (PopScannerSource());

                while (!if_block_stack_.empty())
                {
                    const auto& if_block_ = if_block_stack_.top();
                    GetReportHandler().SubmitReport(
                            false,
                            ReportTypes::Error,
                            R_SyntaxError,
                            R_MissingEndIfDirective,
                            if_block_.directive_source_.get(),
                            if_block_.directive_token_->Area());
                    if_block_stack_.pop();
                }
            }

            void PreProcessor::ParesComment()
            {
                Out() << Accept(Tokens::Comment)->Spell();
            }

            void PreProcessor::ParseIdent()
            {
                Out() << ParseIdentAsTokenString();
            }

            TokenPtrString PreProcessor::ParseIdentAsTokenString()
            {
                TokenPtrString token_string_;

                auto ident_tkn_ = Accept(Tokens::Ident);
                if (!OnSubstitueStdMacro(*ident_tkn_, token_string_))
                {
                    auto it_ = macros_.find(ident_tkn_->Spell());
                    if (it_ != macros_.end())
                    {
                        auto& macro_ = *it_->second;
                        if (macro_.HasParameterList())
                        {
                            token_string_.PushBack(
                                    ParseIdentArgumentsForMacro(ident_tkn_,
                                                                macro_));
                        } 
                        else if (macro_.token_string_.Empty())
                        {
                            token_string_.PushBack(
                                    Make<Token>(Tokens::WhiteSpace, " "));
                        }
                        else
                        {
                            token_string_.PushBack(macro_.token_string_);
                        }
                    } 
                    else
                        token_string_.PushBack(ident_tkn_);
                }

                return token_string_;
            }

            TokenPtrString PreProcessor::ParseIdentArgumentsForMacro(
                    const TokenPtr& ident_token_a,
                    const Macro& macro_a)
            {
                IgnoreWhiteSpaces();

                if (!Is(Tokens::LBracket))
                {
                    TokenPtrString token_str_;
                    token_str_.PushBack(ident_token_a);
                    token_str_.PushBack(Make<Token>(Tokens::WhiteSpace, " "));
                    return token_str_;
                }

                AcceptIt();
                IgnoreWhiteSpaces();

                std::vector<TokenPtrString> arguments_;

                while (!Is(Tokens::RBracket))
                {
                    auto arg_ = ParseArgumentTokenString();
                    arg_.TrAzBack();
                    arg_.TrAzFront();

                    arguments_.push_back(arg_);

                    if (Is(Tokens::Comma))
                    {
                        AcceptIt();

                        if (Is(Tokens::RBracket))
                            arguments_.push_back({});
                    }
                }

                AcceptIt();

                if ((!macro_a.var_args_
                     && arguments_.size() != macro_a.parameters_.size())
                    || (macro_a.var_args_
                        && arguments_.size() < macro_a.parameters_.size()))
                {
                    if (macro_a.parameters_.size() == 1 && arguments_.size() == 0)
                    {
                        arguments_.push_back({});
                    } 
                    else
                    {
                        std::string error_msg_;

                        if (arguments_.size() > macro_a.parameters_.size())
                            error_msg_ = R_TooManyArgsForMacro(
                                    ident_token_a->Spell(),
                                    macro_a.parameters_.size(),
                                    arguments_.size());
                      
                        if (arguments_.size() < macro_a.parameters_.size())
                            error_msg_ = R_TooFewArgsForMacro(
                                    ident_token_a->Spell(),
                                    macro_a.parameters_.size(),
                                    arguments_.size());

                        Error(error_msg_, ident_token_a.get());
                    }
                }

                return ExpandMacro(macro_a, arguments_);
            }

            void PreProcessor::ParseMisc()
            {
                Out() << AcceptIt()->Spell();
            }

            void PreProcessor::ParseDirective()
            {
                const auto directive_ = Accept(Tokens::Directive)->Spell();
                ParseDirective(directive_, true);
            }

            void PreProcessor::ParseAnyIfDirectiveAndSkipValidation()
            {
                auto tkn_l_ = Accept(Tokens::Directive);
                auto directive_ = tkn_l_->Spell();

                if (directive_ == "if")
                    ParseDirectiveIf(true);
                else if (directive_ == "ifdef")
                    ParseDirectiveIfdef(true);
                else if (directive_ == "ifndef")
                    ParseDirectiveIfndef(true);
                else if (directive_ == "elif")
                    ParseDirectiveElif(true);
                else if (directive_ == "else")
                    ParseDirectiveElse();
                else if (directive_ == "endif")
                    ParseDirectiveEndif();
            }

            void PreProcessor::ParseDirectiveDefine()
            {
                IgnoreWhiteSpaces();
                auto ident_tkn_ = Accept(Tokens::Ident);

                Macro macro_ {ident_tkn_};
                if (Is(Tokens::LBracket))
                {
                    AcceptIt();
                    IgnoreWhiteSpaces();

                    if (!Is(Tokens::RBracket))
                    {
                        while (true)
                        {
                            IgnoreWhiteSpaces(false, true);

                            if (Is(Tokens::LineBreak))
                            {
                                AcceptIt();
                                IgnoreWhiteSpaces(false, true);
                                if (Is(Tokens::NewLine))
                                {
                                    AcceptIt();
                                    IgnoreWhiteSpaces(false, true);
                                }
                            }

                            if (Is(Tokens::VarArg))
                            {
                                AcceptIt();
                                macro_.var_args_ = true;
                                IgnoreWhiteSpaces();
                                break;
                            }
                            else
                            {
                                auto param_ident_ =
                                        Accept(Tokens::Ident)->Spell();
                                IgnoreWhiteSpaces();

                                macro_.parameters_.push_back(param_ident_);
                                if (!Is(Tokens::Comma))
                                    break;

                                AcceptIt();
                            }
                        }
                    }

                    if (macro_.parameters_.empty())
                        macro_.empty_param_list_ = true;

                    Accept(Tokens::RBracket);
                }

                IgnoreWhiteSpaces();
                if (!Is(Tokens::NewLine))
                {
                    macro_.token_string_ = ParseDirectiveTokenString(false, true);

                    for (const auto& tkn_l_ : macro_.token_string_.GetTokens())
                    {
                        if (tkn_l_->Type() == Tokens::NewLine)
                            Out() << std::endl;
                    }
                }

                DefineMacro(macro_);
            }

            void PreProcessor::ParseDirectiveUndef()
            {
                IgnoreWhiteSpaces();
                auto ident_tkn_ = Accept(Tokens::Ident);

                UndefineMacro(ident_tkn_->Spell(), ident_tkn_.get());
            }

            void PreProcessor::ParseDirectiveInclude()
            {
                bool use_search_paths_ = false;
                std::string filename_;

                IgnoreWhiteSpaces();

                if (Is(Tokens::BinaryOp, "<"))
                {
                    AcceptIt();

                    use_search_paths_ = true;
                    while (!Is(Tokens::BinaryOp, ">"))
                        filename_ += AcceptIt()->Spell();

                    AcceptIt();
                } 
                else
                {
                    filename_ = Accept(Tokens::StringLiteral)->SpellContent();
                }

                if (once_included_.find(filename_) == once_included_.end())
                {
                    std::unique_ptr<std::istream> include_stream_;

                    try
                    {
                        include_stream_ = include_handler_.Include(filename_,
                                                                use_search_paths_);
                    } 
                    catch (const std::exception& e_)
                    {
                        Error(e_.what());
                    }

                    auto source_code_ = std::make_shared<SourceCode>(
                            std::move(include_stream_));
                    PushScannerSource(source_code_, filename_);
                }
            }

            void PreProcessor::ParseDirectiveIf(bool skip_evaluation_a)
            {
                ParseDirectiveIfOrElifCondition(false, skip_evaluation_a);
            }

            void PreProcessor::ParseDirectiveIfdef(bool skip_evaluation_a)
            {
                auto tkn_l_ = GetScanner().PreviousToken();

                if (skip_evaluation_a)
                {
                    PushIfBlock(tkn_l_);
                } 
                else
                {
                    IgnoreWhiteSpaces();
                    auto ident_ = Accept(Tokens::Ident)->Spell();

                    PushIfBlock(tkn_l_, IsDefined(ident_));
                }
            }

            void PreProcessor::ParseDirectiveIfndef(bool skip_evaluation_a)
            {
                (void)skip_evaluation_a;
                auto tkn_l_ = GetScanner().PreviousToken();

                IgnoreWhiteSpaces();
                auto ident_ = Accept(Tokens::Ident)->Spell();

                PushIfBlock(tkn_l_, !IsDefined(ident_));
            }

            void PreProcessor::ParseDirectiveElif(bool skip_evaluation_a)
            {
                if (!TopIfBlock().else_allowed_)
                    Error(R_ExpectedEndIfDirective("#elif"), true);

                auto parent_if_condition_ = TopIfBlock().parent_active_;
                ParseDirectiveIfOrElifCondition(true,
                                                skip_evaluation_a
                                                        && !parent_if_condition_);
            }

            void PreProcessor::ParseDirectiveIfOrElifCondition(
                    bool is_else_branch_a,
                    bool skip_evaluation_a)
            {
                auto tkn_l_ = GetScanner().PreviousToken();

                if (skip_evaluation_a)
                {
                    ParseDirectiveTokenString(true);
                    if (is_else_branch_a)
                        SetIfBlock(tkn_l_);
                    else
                        PushIfBlock(tkn_l_);
                } 
                else
                {
                    auto condition_ = ParseAndEvaluateExpr(tkn_l_.get());

                    if (is_else_branch_a)
                        SetIfBlock(tkn_l_, condition_.ToBool());
                    else
                        PushIfBlock(tkn_l_, condition_.ToBool());
                }
            }

            void PreProcessor::ParseDirectiveElse()
            {
                auto tkn_l_ = TopIfBlock().directive_token_;
                if (!TopIfBlock().else_allowed_)
                    Error(R_ExpectedEndIfDirective("#else"), true);

                SetIfBlock(tkn_l_, true, false);
            }

            void PreProcessor::ParseDirectiveEndif()
            {
                PopIfBlock();
            }

            void PreProcessor::ParseDirectivePragma()
            {
                auto tkn_l_ = GetScanner().PreviousToken();
                IgnoreWhiteSpaces();

                auto token_string_ = ParseDirectiveTokenString();
                if (!token_string_.Empty())
                {
                    auto token_it_ = token_string_.Begin();
                    if ((*token_it_)->Type() == Tokens::Ident)
                    {
                        auto command_ = (*token_it_)->Spell();
                        if (command_ == "once")
                        {
                            auto filename_ = GetCurrentFilename();
                            if (!filename_.empty())
                                once_included_.insert(std::move(filename_));
                        } 
                        else if (command_ == "message")
                        {
                            auto prev_token_ = token_it_->get();
                            if (!(++token_it_).ReachedEnd())
                            {
                                if ((*token_it_)->Type() == Tokens::StringLiteral)
                                {
                                    GetReportHandler().SubmitReport(
                                            false,
                                            ReportTypes::Info,
                                            R_Message,
                                            (*token_it_)->SpellContent(),
                                            nullptr,
                                            (*token_it_)->Area());
                                } 
                                else
                                    ErrorUnexpected(Tokens::StringLiteral,
                                                    token_it_->get());
                            } 
                            else
                                Error(R_UnexpectedEndOfTokenString, prev_token_);
                        } 
                        else if (command_ == "pack_matrix")
                        {
                            auto prev_token_ = token_it_->get();
                            try
                            {
                                token_it_.AcceptIt();
                                token_it_.Accept(Tokens::LBracket);
                                auto alignment_tkn_ = token_it_.Accept(
                                        Tokens::Ident);
                                token_it_.Accept(Tokens::RBracket);

                                auto alignment_ = alignment_tkn_->Spell();
                                if (alignment_ == "row_major"
                                    || alignment_ == "column_major")
                                    Out() << "#pragma pack_matrix(" << alignment_
                                          << ")";
                                else
                                    Warning(R_UnknownMatrixPackAlignment(
                                                    alignment_),
                                            alignment_tkn_.get());
                            } 
                            catch (const std::exception& e_)
                            {
                                Warning(e_.what(), prev_token_);
                                return;
                            }
                        } 
                        else if (command_ == "def" || command_ == "warning")
                        {
                            Warning(R_PragmaCantBeHandled(command_),
                                    token_it_->get());
                            return;
                        } 
                        else
                            Warning(R_UnknownPragma(command_), token_it_->get());
                    } 
                    else
                        Warning(R_UnexpectedTokenInPragma, token_it_->get());

                    if (!(++token_it_).ReachedEnd())
                        Warning(R_RemainingTokensInPragma, token_it_->get());
                } 
                else
                    Warning(R_EmptyPragma, tkn_l_.get());
            }

            void PreProcessor::ParseDirectiveLine()
            {
                IgnoreWhiteSpaces();
                auto line_number_ = Accept(Tokens::IntLiteral)->Spell();

                Out() << "#line " << line_number_;

                IgnoreWhiteSpaces();

                if (Is(Tokens::StringLiteral))
                {
                    auto filename_ = AcceptIt()->SpellContent();
                    Out() << " \"" << filename_ << '\"';
                }

                Out() << std::endl;
            }

            void PreProcessor::ParseDirectiveError()
            {
                auto tkn_l_ = GetScanner().PreviousToken();
                auto token_string_ = ParseDirectiveTokenString();

                std::string error_msg_;

                for (const auto& ctkn_ : token_string_.GetTokens())
                    error_msg_ += ctkn_->Spell();

                GetReportHandler().SubmitReport(true,
                                                ReportTypes::Error,
                                                R_Error,
                                                error_msg_,
                                                GetScanner().Source(),
                                                tkn_l_->Area());
            }

            ExprPtr PreProcessor::ParseExpr()
            {
                return ParseGenericExpr();
            }

            ExprPtr PreProcessor::ParsePrAzaryExpr()
            {
                switch (TknType())
                {
                case Tokens::Ident: {
                    if (Tkn()->Spell() == "defined")
                    {
                        return ASTFactory::MakeLiteralExpr(DataType::Int,
                                                           ParseDefinedMacro());
                    } else
                    {
                        return ASTFactory::MakeObjectExpr(AcceptIt()->Spell());
                    }
                }
                break;

                case Tokens::UnaryOp: {
                    auto ast_ = Make<UnaryExpr>();
                    {
                        ast_->op_ = StringToUnaryOp(AcceptIt()->Spell());
                        ast_->expr_ = ParseValueExpr();
                    }
                    return ast_;
                }
                break;

                case Tokens::BoolLiteral:
                case Tokens::IntLiteral:
                case Tokens::FloatLiteral: {
                    auto ast_ = Make<LiteralExpr>();
                    {
                        ast_->data_type_ = TokenToDataType(*Tkn());
                        ast_->value_ = AcceptIt()->Spell();
                    }
                    return ast_;
                }
                break;

                case Tokens::LBracket: {
                    AcceptIt();
                    auto ast_ = ParseGenericExpr();
                    Accept(Tokens::RBracket);
                    return ast_;
                }
                break;

                default: {
                    ErrorUnexpected(R_ExpectedConstExpr, nullptr, true);
                }
                break;
                }
                return nullptr;
            }

            TokenPtrString PreProcessor::ParseDirectiveTokenString(
                    bool expand_defined_directive_a,
                    bool ignore_comments_a)
            {
                TokenPtrString token_string_;

                IgnoreWhiteSpaces();

                while (!Is(Tokens::NewLine))
                {
                    switch (TknType())
                    {
                    case Tokens::LineBreak: {
                        AcceptIt();
                        IgnoreWhiteSpaces();
                        if (Is(Tokens::NewLine))
                            token_string_.PushBack(AcceptIt());
                    }
                    break;

                    case Tokens::Ident: {
                        if (expand_defined_directive_a
                            && Tkn()->Spell() == "defined")
                        {
                            auto defined_macro_ = ParseDefinedMacro();
                            token_string_.PushBack(Make<Token>(Tokens::IntLiteral,
                                                             defined_macro_));
                        } 
                        else
                        {
                            token_string_.PushBack(ParseIdentAsTokenString());
                        }
                    }
                    break;

                    case Tokens::Comment: {
                        if (ignore_comments_a)
                            AcceptIt();
                        else
                            token_string_.PushBack(AcceptIt());
                    }
                    break;

                    default: {
                        token_string_.PushBack(AcceptIt());
                    }
                    break;
                    }
                }

                return token_string_;
            }

            TokenPtrString PreProcessor::ParseArgumentTokenString()
            {
                TokenPtrString token_string_;

                int bracket_level_ = 0;
                while (bracket_level_ > 0
                       || !(Is(Tokens::RBracket) || Is(Tokens::Comma)))
                {
                    if (Is(Tokens::LBracket))
                        ++bracket_level_;
                    else if (bracket_level_ > 0 && Is(Tokens::RBracket))
                        --bracket_level_;

                    if (Is(Tokens::Ident))
                        token_string_.PushBack(ParseIdentAsTokenString());
                    else
                        token_string_.PushBack(AcceptIt());
                }

                return token_string_;
            }

            std::string PreProcessor::ParseDefinedMacro()
            {
                Accept(Tokens::Ident, "defined");
                IgnoreWhiteSpaces();

                std::string macro_ident_;
                if (Is(Tokens::LBracket))
                {
                    AcceptIt();
                    IgnoreWhiteSpaces();
                    macro_ident_ = Accept(Tokens::Ident)->Spell();
                    IgnoreWhiteSpaces();
                    Accept(Tokens::RBracket);
                } else
                    macro_ident_ = Accept(Tokens::Ident)->Spell();

                return (IsDefined(macro_ident_) ? "1" : "0");
            }

            PreProcessor::Macro::Macro(const TokenPtr& ident_tkn_a)
                : ident_tkn_ {ident_tkn_a}
            {}

            PreProcessor::Macro::Macro(const TokenPtr& ident_tkn_a,
                                       const TokenPtrString& value_a)
                : ident_tkn_ {ident_tkn_a}, token_string_ {value_a}
            {}

            PreProcessor::Macro::Macro(
                    const TokenPtr& ident_tkn_a,
                    const TokenPtrString& value_a,
                    const std::vector<std::string>& parameters_a,
                    bool var_args_a,
                    bool std_macro_a,
                    bool empty_param_list_a)
                : ident_tkn_ {ident_tkn_a}, token_string_ {value_a},
                  parameters_ {parameters_a}, var_args_ {var_args_a},
                  std_macro_ {std_macro_a}, empty_param_list_ {empty_param_list_a}
            {}

            bool PreProcessor::Macro::HasParameterList() const
            {
                return (!parameters_.empty() || empty_param_list_);
            }

            void PreProcessor::IfBlock::SetActive(bool activate_a)
            {
                active_ = (parent_active_ && !was_active_ && activate_a);
                if (active_)
                    was_active_ = true;
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel