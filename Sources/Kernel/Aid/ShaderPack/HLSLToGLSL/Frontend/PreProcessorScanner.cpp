#include "PreProcessorScanner.hpp"

#include <cctype>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            PreProcessorScanner::PreProcessorScanner(Log* log_a) : Scanner {log_a}
            {}

            TokenPtr PreProcessorScanner::Next()
            {
                return NextToken(true, true);
            }

            TokenPtr PreProcessorScanner::ScanToken()
            {
                std::string spell_;

                if (Is('#'))
                    return ScanDirectiveOrDirectiveConcat();

                if (std::isalpha(UChr()) || Is('_'))
                    return ScanIdentifier();

                if (Is('.'))
                    return ScanNumberOrDot();
                if (std::isdigit(UChr()))
                    return ScanNumber();

                if (Is('\"'))
                    return ScanStringLiteral();

                if (Is('\''))
                    return ScanCharLiteral();

                if (Is('='))
                {
                    spell_ += TakeIt();
                    if (Is('='))
                        return Make(Tokens::BinaryOp, spell_, true);
                    return Make(Tokens::Misc, spell_);
                }

                if (Is('!'))
                {
                    spell_ += TakeIt();
                    if (Is('='))
                        return Make(Tokens::BinaryOp, spell_, true);
                    return Make(Tokens::UnaryOp, spell_);
                }

                if (Is('<'))
                {
                    spell_ += TakeIt();
                    if (Is('<'))
                        spell_ += TakeIt();
                    else if (Is('='))
                        spell_ += TakeIt();
                    return Make(Tokens::BinaryOp, spell_);
                }

                if (Is('>'))
                {
                    spell_ += TakeIt();
                    if (Is('>'))
                        spell_ += TakeIt();
                    else if (Is('='))
                        spell_ += TakeIt();
                    return Make(Tokens::BinaryOp, spell_);
                }

                if (Is('&'))
                {
                    spell_ += TakeIt();
                    if (Is('&'))
                        spell_ += TakeIt();
                    return Make(Tokens::BinaryOp, spell_);
                }

                if (Is('|'))
                {
                    spell_ += TakeIt();
                    if (Is('|'))
                        spell_ += TakeIt();
                    return Make(Tokens::BinaryOp, spell_);
                }

                switch (Chr())
                {
                case ':':
                    return Make(Token::Types::Colon, true);
                    break;
                case ',':
                    return Make(Token::Types::Comma, true);
                    break;
                case '?':
                    return Make(Token::Types::TernaryOp, true);
                    break;
                case '(':
                    return Make(Token::Types::LBracket, true);
                    break;
                case ')':
                    return Make(Token::Types::RBracket, true);
                    break;
                case '~':
                    return Make(Token::Types::UnaryOp, true);
                    break;
                case '^':
                    return Make(Token::Types::BinaryOp, true);
                    break;
                case '%':
                    return Make(Token::Types::BinaryOp, true);
                    break;
                case '+':
                    return Make(Token::Types::BinaryOp, true);
                    break;
                case '-':
                    return Make(Token::Types::BinaryOp, true);
                    break;
                case '*':
                    return Make(Token::Types::BinaryOp, true);
                    break;
                case '/':
                    return Make(Token::Types::BinaryOp, true);
                    break;
                case '\\':
                    return Make(Token::Types::LineBreak, true);
                    break;
                }

                return Make(Token::Types::Misc, true);
            }

            TokenPtr PreProcessorScanner::ScanDirectiveOrDirectiveConcat()
            {
                std::string spell_;

                Take('#');

                if (Is('#'))
                {
                    TakeIt();
                    spell_ = "##";
                    return Make(Token::Types::DirectiveConcat, spell_);
                }

                IgnoreWhiteSpaces(false);
                StoreStartPos();

                while (std::isalpha(UChr()))
                    spell_ += TakeIt();

                return Make(Token::Types::Directive, spell_);
            }

            TokenPtr PreProcessorScanner::ScanIdentifier()
            {
                std::string spell_;
                spell_ += TakeIt();

                while (std::isalnum(UChr()) || Is('_'))
                    spell_ += TakeIt();

                return Make(Token::Types::Ident, spell_);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel