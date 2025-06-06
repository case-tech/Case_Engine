#include "GLSLScanner.hpp"

#include "../../Report/ReportIdents.hpp"
#include "GLSLKeywords.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            GLSLScanner::GLSLScanner(Log* log_a) : SLScanner {log_a}
            {}

            TokenPtr GLSLScanner::ScanIdentifierOrKeyword(std::string&& spell_a)
            {
                auto it_ = GLSLKeywords().find(spell_a);
                if (it_ != GLSLKeywords().end())
                {
                    if (it_->second == Token::Types::Reserved)
                        Error(R_KeywordReservedForFutureUse(spell_a));
                    else if (it_->second == Token::Types::Unsupported)
                        Error(R_KeywordNotSupportedYet(spell_a));
                    else
                        return Make(it_->second, spell_a);
                }

                return Make(Tokens::Ident, spell_a);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
