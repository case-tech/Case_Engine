// Copyright (c) 2025 Case Technologies

#pragma once
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class Identifier
            {
            public:
                Identifier() = default;
                Identifier(const Identifier&) = default;

                Identifier& operator=(const Identifier& rhs_a);
                Identifier& operator=(const std::string& s_a);
                Identifier& AppendPrefix(const std::string& prefix_a);
                Identifier& RemovePrefix(const std::string& prefix_a);

                const std::string& Final() const;

                inline bool Empty() const
                {
                    return Final().empty();
                }

                inline operator const std::string&() const
                {
                    return Final();
                }

                inline const std::string& Original() const
                {
                    return original_;
                }

                inline bool IsRenamed() const
                {
                    return !renamed_.empty();
                }

            private:
                bool original_set_ = false;
                std::string original_;

                bool renamed_set_ = false;
                std::string renamed_;

                int counter_ = 0;
            };

            inline bool operator==(const Identifier& lhs_a, const Identifier& rhs_a)
            {
                return (std::string(lhs_a) == std::string(rhs_a));
            }

            inline bool operator==(const std::string& lhs_a,
                                   const Identifier& rhs_a)
            {
                return (lhs_a == std::string(rhs_a));
            }

            inline bool operator==(const Identifier& lhs_a,
                                   const std::string& rhs_a)
            {
                return (std::string(lhs_a) == rhs_a);
            }

            inline bool operator!=(const Identifier& lhs_a, const Identifier& rhs_a)
            {
                return (std::string(lhs_a) != std::string(rhs_a));
            }

            inline bool operator!=(const std::string& lhs_a,
                                   const Identifier& rhs_a)
            {
                return (lhs_a != std::string(rhs_a));
            }

            inline bool operator!=(const Identifier& lhs_a,
                                   const std::string& rhs_a)
            {
                return (std::string(lhs_a) != rhs_a);
            }

            inline std::string operator+(const Identifier& lhs_a,
                                         const Identifier& rhs_a)
            {
                return (std::string(lhs_a) + std::string(rhs_a));
            }

            inline std::string operator+(const std::string& lhs_a,
                                         const Identifier& rhs_a)
            {
                return (lhs_a + std::string(rhs_a));
            }

            inline std::string operator+(const Identifier& lhs_a,
                                         const std::string& rhs_a)
            {
                return (std::string(lhs_a) + rhs_a);
            }

            inline std::string operator+(char lhs_a, const Identifier& rhs_a)
            {
                return (lhs_a + std::string(rhs_a));
            }

            inline std::string operator+(const Identifier& lhs_a, char rhs_a)
            {
                return (std::string(lhs_a) + rhs_a);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
