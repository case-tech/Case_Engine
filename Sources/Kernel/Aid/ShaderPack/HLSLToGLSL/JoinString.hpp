// Copyright (c) 2025 Case Technologies

#pragma once
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            std::string JoinString(const std::string& s_a,
                                   const std::vector<std::string>& values_a);

            template <typename T>
            void ToStringListPrimary(std::vector<std::string>& list_a,
                                     const T& value_a)
            {
                list_a.push_back(value_a);
            }

            template <>
            void ToStringListPrimary<std::size_t>(
                    std::vector<std::string>& list_a,
                    const std::size_t& value_a);

            template <>
            void ToStringListPrimary<int>(std::vector<std::string>& list_a,
                                          const int& value_a);

            template <typename... Args>
            void ToStringList(std::vector<std::string>& list_a, Args&&... args_a);

            template <typename Arg0, typename... ArgsN>
            void ToStringListSecondary(std::vector<std::string>& list_a,
                                       Arg0&& arg0_a,
                                       ArgsN&&... args_n_a)
            {
                ToStringListPrimary(list_a, std::forward<Arg0>(arg0_a));
                ToStringList(list_a, std::forward<ArgsN>(args_n_a)...);
            }

            template <typename... Args>
            void ToStringList(std::vector<std::string>& list_a, Args&&... args_a)
            {
                ToStringListSecondary(list_a, std::forward<Args>(args_a)...);
            }

            template <>
            void ToStringList(std::vector<std::string>& list_a);

            template <typename Arg0>
            void ToStringListSecondary(std::vector<std::string>& list_a,
                                       Arg0&& arg0_a)
            {
                ToStringListPrimary(list_a, std::forward<Arg0>(arg0_a));
            }

            class JoinableString
            {
            public:
                JoinableString() = default;
                JoinableString(const JoinableString&) = default;
                JoinableString& operator=(const JoinableString&) = default;

                JoinableString(const char* s_a);

                std::string Join(
                        const std::vector<std::string>& values_a = {}) const;

                template <typename... Args>
                std::string operator()(Args&&... args_a) const
                {
                    std::vector<std::string> values_;
                    ToStringList(values_, std::forward<Args>(args_a)...);
                    return Join(values_);
                }

                inline operator std::string() const
                {
                    return Join({});
                }

            private:
                const char* s_ = nullptr;
                bool can_join_ = false;
            };

            inline std::string operator+(const JoinableString& lhs_a,
                                         const std::string& rhs_a)
            {
                return lhs_a.Join() + rhs_a;
            }

            inline std::string operator+(const std::string& lhs_a,
                                         const JoinableString& rhs_a)
            {
                return lhs_a + rhs_a.Join();
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
