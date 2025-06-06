#include "Identifier.hpp"
#include "../Helper.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            Identifier& Identifier::operator=(const Identifier& rhs_a)
            {
                *this = rhs_a.Final();
                return *this;
            }

            Identifier& Identifier::operator=(const std::string& s_a)
            {
                if (!original_set_)
                {
                    original_set_ = true;
                    original_ = s_a;
                } 
                else
                {
                    renamed_set_ = true;
                    renamed_ = s_a;
                }
                return *this;
            }

            Identifier& Identifier::AppendPrefix(const std::string& prefix_a)
            {
                if (!prefix_a.empty()
                    && Final().compare(0, prefix_a.size(), prefix_a) == 0)
                {
                    RemovePrefix(prefix_a);

                    ++counter_;
                    return (*this = prefix_a + std::to_string(counter_)
                                    + Final());
                } 
                else
                {
                    counter_ = 0;
                    return (*this = prefix_a + Final());
                }
            }

            Identifier& Identifier::RemovePrefix(const std::string& prefix_a)
            {
                if (!prefix_a.empty()
                    && Final().compare(0, prefix_a.size(), prefix_a) == 0)
                {
                    auto prefix_len_ = prefix_a.size();
                    if (counter_ > 0)
                        prefix_len_ += NumDigits(counter_);
                    *this = Final().substr(prefix_len_);
                }
                return *this;
            }

            const std::string& Identifier::Final() const
            {
                return (renamed_set_ ? renamed_ : original_);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
