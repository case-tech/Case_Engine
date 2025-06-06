#include "HLSLScanner.hpp"

#include "../../Report/ReportIdents.hpp"
#include "HLSLKeywords.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            HLSLScanner::HLSLScanner(bool enable_cg_keywords_a, Log* log_a)
                : SLScanner {log_a}, enable_cg_keywords_ {enable_cg_keywords_a}
            {}

            TokenPtr HLSLScanner::ScanIdentifierOrKeyword(std::string&& spell_a)
            {
                auto it_ = HLSLKeywords().find(spell_a);
                if (it_ != HLSLKeywords().end())
                {
                    if (it_->second == Token::Types::Reserved)
                        Error(R_KeywordReservedForFutureUse(spell_a));
                    else if (it_->second == Token::Types::Unsupported)
                        Error(R_KeywordNotSupportedYet(spell_a));
                    else
                        return Make(it_->second, spell_a);
                }

                if (enable_cg_keywords_)
                {
                    auto it1_ = HLSLKeywordsExtCg().find(spell_a);
                    if (it1_ != HLSLKeywordsExtCg().end())
                        return Make(it1_->second, spell_a);
                }

                return Make(Tokens::Ident, spell_a);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel