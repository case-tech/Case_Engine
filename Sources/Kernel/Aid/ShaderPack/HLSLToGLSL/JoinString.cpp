#include "JoinString.hpp"

#include <cstdlib>
#include <stdexcept>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            template <>
            void ToStringListPrAzary<std::size_t>(
                    std::vector<std::string>& list_a,
                    const std::size_t& value_a)
            {
                list_a.push_back(std::to_string(value_a));
            }

            template <>
            void ToStringListPrAzary<int>(std::vector<std::string>& list_a,
                                          const int& value_a)
            {
                list_a.push_back(std::to_string(value_a));
            }

            template <>
            void ToStringList(std::vector<std::string>& list_a)
            {
                (void)list_a;
            }

            JoinableString::JoinableString(const char* s_a) : s_ {s_a}
            {
                while (*s_a != '\0')
                {
                    if (*s_a == '\\' || *s_a == '{' || *s_a == '}' || *s_a == '['
                        || *s_a == ']')
                    {
                        can_join_ = true;
                        break;
                    }
                    ++s_a;
                }
            }

            std::string JoinableString::Join(
                    const std::vector<std::string>& values_a) const
            {
                if (can_join_)
                    return JoinString(s_, values_a);
                else
                    return std::string(s_);
            }

            static bool JoinStringSub(const std::string& in_a,
                                      std::size_t& pos_a,
                                      std::string& out_a,
                                      const std::vector<std::string>& values_a,
                                      bool optional_a)
            {
                bool escape_char_ = false;
                bool replaced_all_values_ = true;

                for (auto num_ = in_a.size(); pos_a < num_;)
                {
                    auto c_ = in_a[pos_a++];
                    if (escape_char_)
                    {
                        out_a.push_back(c_);
                        escape_char_ = false;
                    } 
                    else
                    {
                        if (c_ == '\\')
                        {
                            escape_char_ = true;
                        } 
                        else if (c_ == '{')
                        {
                            std::string idx_str_;
                            while (pos_a < num_)
                            {
                                c_ = in_a[pos_a++];
                                if (c_ == '}')
                                    break;
                                else
                                    idx_str_.push_back(c_);
                            }

                            const auto idx_ = static_cast<std::size_t>(
                                    std::stoul(idx_str_));
                            if (idx_ < values_a.size())
                            {
                                const auto& val_ = values_a[idx_];
                                if (val_.empty())
                                    replaced_all_values_ = false;
                                else
                                    out_a.append(val_);
                            } 
                            else if (optional_a)
                            {
                                replaced_all_values_ = false;
                            } 
                            else
                            {
                                throw std::out_of_range(
                                        "index (" + std::to_string(idx_)
                                        + ") out of range [0, "
                                        + std::to_string(values_a.size())
                                        + ") in joinable string: " + in_a);
                            }
                        } 
                        else if (c_ == '[')
                        {
                            std::string out_opt_;
                            if (JoinStringSub(in_a, pos_a, out_opt_, values_a, true))
                                out_a.append(out_opt_);
                        }
                        else if (c_ == ']')
                        {
                            break;
                        } 
                        else
                        {
                            out_a.push_back(c_);
                        }
                    }
                }

                if (escape_char_)
                    throw std::invalid_argument(
                            "incomplete escape character in report string");

                return replaced_all_values_;
            }

            std::string JoinString(const std::string& s_a,
                                   const std::vector<std::string>& values_a)
            {
                std::string out_;
                std::size_t pos_ = 0;
                JoinStringSub(s_a, pos_, out_, values_a, false);

                if (pos_ != s_a.size())
                    throw std::invalid_argument(
                            "incomplete optional part in report string");

                return out_;
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
