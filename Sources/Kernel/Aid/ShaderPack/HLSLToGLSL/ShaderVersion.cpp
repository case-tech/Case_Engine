#include "ShaderVersion.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            ShaderVersion::ShaderVersion(int major_a, int minor_a)
                : major_ {major_a}, minor_ {minor_a}
            {}

            std::string ShaderVersion::ToString() const
            {
                return (std::to_string(major_) + "." + std::to_string(minor_));
            }

            bool operator==(const ShaderVersion& lhs_a, const ShaderVersion& rhs_a)
            {
                return (lhs_a.Major() == rhs_a.Major()
                        && lhs_a.Minor() == rhs_a.Minor());
            }

            bool operator!=(const ShaderVersion& lhs_a, const ShaderVersion& rhs_a)
            {
                return !(lhs_a == rhs_a);
            }

            bool operator<(const ShaderVersion& lhs_a, const ShaderVersion& rhs_a)
            {
                if (lhs_a.Major() == rhs_a.Major())
                    return (lhs_a.Minor() < rhs_a.Minor());
                return (lhs_a.Major() < rhs_a.Major());
            }

            bool operator<=(const ShaderVersion& lhs_a, const ShaderVersion& rhs_a)
            {
                return (lhs_a == rhs_a || lhs_a < rhs_a);
            }

            bool operator>(const ShaderVersion& lhs_a, const ShaderVersion& rhs_a)
            {
                return (rhs_a < lhs_a);
            }

            bool operator>=(const ShaderVersion& lhs_a, const ShaderVersion& rhs_a)
            {
                return (lhs_a == rhs_a || rhs_a < lhs_a);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel