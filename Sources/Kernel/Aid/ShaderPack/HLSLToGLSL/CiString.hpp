// Copyright (c) 2025 Case Technologies

#pragma once
#include <cctype>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            template <typename T>
            struct CiCharTraits : public std::char_traits<T>
            {
                static bool Eq(T c1_a, T c2_a)
                {
                    return (std::toupper(c1_a) == std::toupper(c2_a));
                }

                static bool Ne(T c1_a, T c2_a)
                {
                    return (std::toupper(c1_a) != std::toupper(c2_a));
                }

                static bool Lt(T c1_a, T c2_a)
                {
                    return (std::toupper(c1_a) < std::toupper(c2_a));
                }

                static int Compare(const T* s1_a, const T* s2_a, size_t n_a)
                {
                    while (n_a-- != 0)
                    {
                        if (std::toupper(*s1_a) < std::toupper(*s2_a))
                            return -1;
                        if (std::toupper(*s1_a) > std::toupper(*s2_a))
                            return 1;
                        ++s1_a;
                        ++s2_a;
                    }
                    return 0;
                }

                static const T* Find(const T* s_a, int n_a, T a_a)
                {
                    const auto ua_ = std::toupper(a_a);
                    while (n_a-- > 0)
                    {
                        if (std::toupper(*s_a) == ua_)
                            return s_a;
                        s_a++;
                    }
                    return nullptr;
                }
            };

            using CiString = std::basic_string<char, CiCharTraits<char>>;

            inline CiString ToCiString(const std::string& s_a)
            {
                return CiString(s_a.begin(), s_a.end());
            }

            inline std::string ToString(const CiString& s_a)
            {
                return std::string(s_a.begin(), s_a.end());
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
