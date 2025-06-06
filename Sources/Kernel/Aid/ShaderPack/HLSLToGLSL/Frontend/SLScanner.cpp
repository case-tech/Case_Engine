#include "SLScanner.hpp"

#include <cctype>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            SLScanner::SLScanner(Log* log_a) : Scanner {log_a}
            {}

            TokenPtr SLScanner::Next()
            {
                return NextToken(false, false);
            }

            TokenPtr SLScanner::ScanToken()
            {
                std::string spell_;

                if (Is('#'))
                    return ScanDirective();

                if (std::isalpha(UChr()) || Is('_'))
                    return ScanIdentifier();

                if (Is('.'))
                    return ScanNumberOrDot();
                if (std::isdigit(UChr()))
                    return ScanNumber();

                if (Is('\"'))
                    return ScanStringLiteral();

                if (Is('='))
                {
                    spell_ += TakeIt();
                    if (Is('='))
                        return Make(Tokens::BinaryOp, spell_, true);
                    return Make(Tokens::AssignOp, spell_);
                }

                if (Is('~'))
                    return Make(Tokens::UnaryOp, spell_, true);

                if (Is('!'))
                {
                    spell_ += TakeIt();
                    if (Is('='))
                        return Make(Tokens::BinaryOp, spell_, true);
                    return Make(Tokens::UnaryOp, spell_);
                }

                if (Is('%'))
                {
                    spell_ += TakeIt();
                    if (Is('='))
                        return Make(Tokens::AssignOp, spell_, true);
                    return Make(Tokens::BinaryOp, spell_);
                }

                if (Is('*'))
                {
                    spell_ += TakeIt();
                    if (Is('='))
                        return Make(Tokens::AssignOp, spell_, true);
                    return Make(Tokens::BinaryOp, spell_);
                }

                if (Is('^'))
                {
                    spell_ += TakeIt();
                    if (Is('='))
                        return Make(Tokens::AssignOp, spell_, true);
                    return Make(Tokens::BinaryOp, spell_);
                }

                if (Is('+'))
                    return ScanPlusOp();
                if (Is('-'))
                    return ScanMinusOp();

                if (Is('<') || Is('>'))
                    return ScanAssignShiftRelationOp(Chr());

                if (Is('&'))
                {
                    spell_ += TakeIt();
                    if (Is('='))
                        return Make(Tokens::AssignOp, spell_, true);
                    if (Is('&'))
                        return Make(Tokens::BinaryOp, spell_, true);
                    return Make(Tokens::BinaryOp, spell_);
                }

                if (Is('|'))
                {
                    spell_ += TakeIt();
                    if (Is('='))
                        return Make(Tokens::AssignOp, spell_, true);
                    if (Is('|'))
                        return Make(Tokens::BinaryOp, spell_, true);
                    return Make(Tokens::BinaryOp, spell_);
                }

                if (Is(':'))
                {
                    spell_ += TakeIt();
                    if (Is(':'))
                        return Make(Tokens::DColon, spell_, true);
                    return Make(Tokens::Colon, spell_);
                }

                switch (Chr())
                {
                case ';':
                    return Make(Tokens::Semicolon, true);
                    break;
                case ',':
                    return Make(Tokens::Comma, true);
                    break;
                case '?':
                    return Make(Tokens::TernaryOp, true);
                    break;
                case '(':
                    return Make(Tokens::LBracket, true);
                    break;
                case ')':
                    return Make(Tokens::RBracket, true);
                    break;
                case '{':
                    return Make(Tokens::LCurly, true);
                    break;
                case '}':
                    return Make(Tokens::RCurly, true);
                    break;
                case '[':
                    return Make(Tokens::LParen, true);
                    break;
                case ']':
                    return Make(Tokens::RParen, true);
                    break;
                }

                ErrorUnexpected();

                return nullptr;
            }

            TokenPtr SLScanner::ScanDirective()
            {
                std::string spell_;
                Take('#');

                IgnoreWhiteSpaces(false);

                StoreStartPos();

                while (std::isalpha(UChr()))
                    spell_ += TakeIt();

                return Make(Token::Types::Directive, spell_);
            }

            TokenPtr SLScanner::ScanIdentifier()
            {
                std::string spell_;
                spell_ += TakeIt();

                while (std::isalnum(UChr()) || Is('_'))
                    spell_ += TakeIt();

                return ScanIdentifierOrKeyword(std::move(spell_));
            }

            TokenPtr SLScanner::ScanAssignShiftRelationOp(const char chr_a)
            {
                std::string spell_;
                spell_ += TakeIt();

                if (Is(chr_a))
                {
                    spell_ += TakeIt();

                    if (Is('='))
                        return Make(Tokens::AssignOp, spell_, true);

                    return Make(Tokens::BinaryOp, spell_);
                }

                if (Is('='))
                    spell_ += TakeIt();

                return Make(Tokens::BinaryOp, spell_);
            }

            TokenPtr SLScanner::ScanPlusOp()
            {
                std::string spell_;
                spell_ += TakeIt();

                if (Is('+'))
                    return Make(Tokens::UnaryOp, spell_, true);
                else if (Is('='))
                    return Make(Tokens::AssignOp, spell_, true);

                return Make(Tokens::BinaryOp, spell_);
            }

            TokenPtr SLScanner::ScanMinusOp()
            {
                std::string spell_;
                spell_ += TakeIt();

                if (Is('-'))
                    return Make(Tokens::UnaryOp, spell_, true);
                else if (Is('='))
                    return Make(Tokens::AssignOp, spell_, true);

                return Make(Tokens::BinaryOp, spell_);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel