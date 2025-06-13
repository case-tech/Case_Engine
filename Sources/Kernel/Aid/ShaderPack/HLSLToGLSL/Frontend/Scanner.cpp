#include "Scanner.hpp"

#include "../Helper.hpp"
#include "../Report/ReportIdents.hpp"

#include <cctype>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            Scanner::Scanner(Log* log_a) : log_ {log_a}
            {}

            bool Scanner::ScanSource(const SourceCodePtr& source_a)
            {
                if (source_a && source_a->IsValid())
                {
                    source_ = source_a;
                    TakeIt();
                    return true;
                }
                return false;
            }

            void Scanner::PushTokenString(const TokenPtrString& token_string_a)
            {
                token_string_it_stack_.push_back(token_string_a.Begin());
            }

            void Scanner::PopTokenString()
            {
                token_string_it_stack_.pop_back();
            }

            TokenPtrString::ConstIterator Scanner::TopTokenStringIterator()
                    const
            {
                for (auto it_ = token_string_it_stack_.rbegin();
                     it_ != token_string_it_stack_.rend();
                     ++it_)
                {
                    if (!(*it_).ReachedEnd())
                        return *it_;
                }
                return TokenPtrString::ConstIterator();
            }

            TokenPtr Scanner::ActiveToken() const
            {
                return active_token_;
            }

            TokenPtr Scanner::PreviousToken() const
            {
                return prev_token_;
            }

            TokenPtr Scanner::NextToken(bool scan_comments_a, bool scan_white_spaces_a)
            {
                TokenPtr tkn_;

                prev_token_ = active_token_;

                for (auto it_ = token_string_it_stack_.rbegin();
                     it_ != token_string_it_stack_.rend();
                     ++it_)
                {
                    if (!it_->ReachedEnd())
                    {
                        auto& token_string_it_ = token_string_it_stack_.back();
                        tkn_ = *(token_string_it_++);
                        break;
                    }
                }

                if (!tkn_)
                {
                    tkn_ = NextTokenScan(scan_comments_a, scan_white_spaces_a);
                }

                active_token_ = tkn_;

                return tkn_;
            }

            TokenPtr Scanner::NextTokenScan(bool scan_comments_a,
                                            bool scan_white_spaces_a)
            {
                while (true)
                {
                    try
                    {
                        comment_.clear();
                        comment_first_line_ = true;
                        bool has_comments_ = true;

                        do
                        {
                            if (scan_white_spaces_a && std::isspace(UChr()))
                            {
                                StoreStartPos();
                                return ScanWhiteSpaces(false);
                            } 
                            else
                                IgnoreWhiteSpaces();

                            if (Is(0))
                            {
                                StoreStartPos();
                                return Make(Tokens::EndOfStream);
                            }

                            if (Is('/'))
                            {
                                StoreStartPos();
                                comment_start_pos_ = next_start_pos_.Column();

                                auto prev_chr_ = TakeIt();

                                if (Is('/'))
                                {
                                    auto tkn_ = ScanCommentLine(scan_comments_a);
                                    if (tkn_)
                                        return tkn_;
                                } 
                                else if (Is('*'))
                                {
                                    auto tkn_ = ScanCommentBlock(scan_comments_a);
                                    if (tkn_)
                                        return tkn_;
                                } 
                                else
                                {
                                    std::string spell_;
                                    spell_ += prev_chr_;

                                    if (Is('='))
                                    {
                                        spell_ += TakeIt();
                                        return Make(Tokens::AssignOp, spell_);
                                    }

                                    return Make(Tokens::BinaryOp, spell_);
                                }
                            } 
                            else
                                has_comments_ = false;
                        } while (has_comments_);

                        StoreStartPos();
                        return ScanToken();
                    }
                    catch (const Report& err_)
                    {
                        if (log_)
                            log_->SubmitReport(err_);
                    }
                }

                return nullptr;
            }

            void Scanner::StoreStartPos()
            {
                next_start_pos_ = source_->Pos();
            }

            char Scanner::Take(char chr_a)
            {
                if (chr_ != chr_a)
                    ErrorUnexpected(chr_a);
                return TakeIt();
            }

            char Scanner::TakeIt()
            {
                auto prev_chr_ = chr_;
                chr_ = source_->Next();
                return prev_chr_;
            }

            TokenPtr Scanner::Make(const Token::Types& type_a, bool take_chr_a)
            {
                if (take_chr_a)
                {
                    std::string spell_;
                    spell_ += TakeIt();
                    return std::make_shared<Token>(Pos(),
                                                   type_a,
                                                   std::move(spell_));
                }
                return std::make_shared<Token>(Pos(), type_a);
            }

            TokenPtr Scanner::Make(const Token::Types& type_a,
                                   std::string& spell_a,
                                   bool take_chr_a)
            {
                if (take_chr_a)
                    spell_a += TakeIt();
                return std::make_shared<Token>(Pos(), type_a, std::move(spell_a));
            }

            TokenPtr Scanner::Make(const Token::Types& type_a,
                                   std::string& spell_a,
                                   const SourcePosition& pos_a,
                                   bool take_chr_a)
            {
                if (take_chr_a)
                    spell_a += TakeIt();
                return std::make_shared<Token>(pos_a, type_a, std::move(spell_a));
            }

            [[noreturn]]
            void Scanner::Error(const std::string& msg_a)
            {
                throw Report(ReportTypes::Error,
                             R_LexicalError + " (" + Pos().ToString()
                                     + ") : " + msg_a);
            }

            [[noreturn]]
            void Scanner::ErrorUnexpected()
            {
                auto chr1_ = TakeIt();
                Error(R_UnexpectedChar(std::string(1, chr1_)));
            }

            [[noreturn]]
            void Scanner::ErrorUnexpected(char expectedChar)
            {
                auto chr1_ = TakeIt();
                Error(R_UnexpectedChar(std::string(1, chr1_),
                                       std::string(1, expectedChar)));
            }

            [[noreturn]]
            void Scanner::ErrorUnexpectedEOS()
            {
                Error(R_UnexpectedEndOfStream);
            }

            void Scanner::Ignore(const std::function<bool(char)>& pred_a)
            {
                while (pred_a(Chr()))
                    TakeIt();
            }

            void Scanner::IgnoreWhiteSpaces(bool include_new_lines_a)
            {
                while (std::isspace(UChr())
                       && (include_new_lines_a || !IsNewLine()))
                    TakeIt();
            }

            TokenPtr Scanner::ScanWhiteSpaces(bool include_new_lines_a)
            {
                if (!include_new_lines_a && IsNewLine())
                    return Make(Tokens::NewLine, true);

                std::string spell_;

                while (std::isspace(UChr())
                       && (include_new_lines_a || !IsNewLine()))
                    spell_ += TakeIt();

                return Make(Tokens::WhiteSpace, spell_);
            }

            TokenPtr Scanner::ScanCommentLine(bool scan_comments_a)
            {
                std::string spell_;

                TakeIt();

                while (!IsNewLine())
                    spell_ += TakeIt();

                AppendComment(spell_);

                if (scan_comments_a)
                {
                    spell_ = "//" + spell_;
                    return Make(Tokens::Comment, spell_);
                }

                return nullptr;
            }

            TokenPtr Scanner::ScanCommentBlock(bool scan_comments_a)
            {
                std::string spell_;

                TakeIt();

                while (!Is(0))
                {
                    if (Is('*'))
                    {
                        TakeIt();
                        if (Is('/'))
                        {
                            TakeIt();
                            break;
                        } else
                            spell_ += '*';
                    } else
                        spell_ += TakeIt();
                }

                AppendMultiLineComment(spell_);

                if (scan_comments_a)
                {
                    spell_ = "/*" + spell_ + "*/";
                    return Make(Tokens::Comment, spell_);
                }

                return nullptr;
            }

            TokenPtr Scanner::ScanStringLiteral()
            {
                std::string spell_;

                spell_ += Take('\"');

                while (!Is('\"'))
                {
                    if (Is(0))
                        ErrorUnexpectedEOS();
                    spell_ += TakeIt();
                }

                spell_ += Take('\"');

                return Make(Tokens::StringLiteral, spell_);
            }

            TokenPtr Scanner::ScanCharLiteral()
            {
                std::string spell_;

                spell_ += Take('\'');

                while (!Is('\''))
                {
                    if (Is(0))
                        ErrorUnexpectedEOS();
                    spell_ += TakeIt();
                }

                spell_ += Take('\'');

                return Make(Tokens::CharLiteral, spell_);
            }

            TokenPtr Scanner::ScanNumber(bool start_with_period_a)
            {
                std::string spell_;

                auto type_ = Tokens::IntLiteral;
                auto pre_digits_ = false;
                auto post_digits_ = false;

                if (!start_with_period_a)
                    pre_digits_ = ScanDigitSequence(spell_);

                if (!start_with_period_a && (Is('e') || Is('E')))
                    start_with_period_a = true;

                if (start_with_period_a || Is('.'))
                {
                    type_ = Tokens::FloatLiteral;

                    if (start_with_period_a)
                        spell_ += '.';
                    else
                        spell_ += TakeIt();

                    post_digits_ = ScanDigitSequence(spell_);

                    if (!pre_digits_ && !post_digits_)
                        Error(R_MissingDecAzalPartInFloat);

                    if (Is('e') || Is('E'))
                    {
                        spell_ += TakeIt();

                        if (Is('-') || Is('+'))
                            spell_ += TakeIt();

                        if (!ScanDigitSequence(spell_))
                            Error(R_MissingDigitSequenceAfterExpr);
                    }

                    if (Is('f') || Is('F') || Is('h') || Is('H') || Is('l')
                        || Is('L'))
                        spell_ += TakeIt();
                } 
                else
                {
                    if (spell_ == "0" && Is('x'))
                    {
                        spell_ += TakeIt();
                        while (std::isdigit(UChr())
                               || (Chr() >= 'a' && Chr() <= 'f')
                               || (Chr() >= 'A' && Chr() <= 'F'))
                            spell_ += TakeIt();
                    }

                    if (Is('u') || Is('U') || Is('l') || Is('L'))
                        spell_ += TakeIt();
                }

                return Make(type_, spell_);
            }

            TokenPtr Scanner::ScanNumberOrDot()
            {
                std::string spell_;

                spell_ += Take('.');

                if (Is('.'))
                    return ScanVarArg(spell_);
                if (std::isdigit(UChr()))
                    return ScanNumber(true);

                return Make(Tokens::Dot, spell_);
            }

            TokenPtr Scanner::ScanVarArg(std::string& spell_a)
            {
                spell_a += Take('.');
                spell_a += Take('.');
                return Make(Tokens::VarArg, spell_a);
            }

            bool Scanner::ScanDigitSequence(std::string& spell_a)
            {
                bool result_ = (std::isdigit(UChr()) != 0);

                while (std::isdigit(UChr()))
                    spell_a += TakeIt();

                return result_;
            }

            void Scanner::AppendComment(const std::string& s_a)
            {
                if (comment_first_line_)
                    comment_first_line_ = false;
                else
                    comment_ += '\n';

                if (comment_start_pos_ > 0)
                {
                    auto first_not_ = s_a.find_first_not_of(" \t");
                    if (first_not_ == std::string::npos)
                        comment_ += s_a;
                    else
                        comment_ += s_a.substr(
                                std::min(first_not_,
                                         static_cast<std::size_t>(
                                                 comment_start_pos_ - 1)));
                } 
                else
                {
                    comment_ += s_a;
                }
            }

            void Scanner::AppendMultiLineComment(const std::string& s_a)
            {
                std::size_t start_ = 0, end_ = 0;

                while (end_ < s_a.size())
                {
                    end_ = s_a.find('\n', start_);

                    AppendComment(end_ < s_a.size() ? s_a.substr(start_, end_ - start_)
                                                 : s_a.substr(start_));

                    start_ = end_ + 1;
                }
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel