#include "TokenString.hpp"

#include "../Report/ReportIdents.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void AssertReachedEnd(bool reached_end_a)
            {
                if (reached_end_a)
                    throw std::runtime_error(R_UnexpectedEndOfStream);
            }

            void AssertCurrentTokenType(const Token::Types type_a,
                                        const Token::Types expected_type_a)
            {
                if (type_a != expected_type_a)
                {
                    throw std::runtime_error(R_UnexpectedToken(
                            Token::TypeToString(type_a),
                            (R_Expected(Token::TypeToString(expected_type_a)))));
                }
            }

            bool DefaultTokenOfInterestFunctor::IsOfInterest(
                    const TokenPtr& token_a)
            {
                auto type_ = token_a->Type();
                return (type_ != Token::Types::Comment
                        && type_ != Token::Types::WhiteSpace
                        && type_ != Token::Types::NewLine);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel