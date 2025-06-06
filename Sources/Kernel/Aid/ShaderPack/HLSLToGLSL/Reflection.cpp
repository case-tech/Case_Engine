#include "Reflection.hpp"

#include "AST/ASTEnums.hpp"
#include "ReflectionPrinter.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            HTG_EXPORT std::string ToString(const Reflection::Filter t_a)
            {
                return FilterToString(t_a);
            }

            HTG_EXPORT std::string ToString(
                    const Reflection::TextureAddressMode t_a)
            {
                return TexAddressModeToString(t_a);
            }

            HTG_EXPORT std::string ToString(const Reflection::ComparisonFunc t_a)
            {
                return CompareFuncToString(t_a);
            }

            HTG_EXPORT std::string ToString(const Reflection::ResourceType t_a)
            {
                return ResourceTypeToString(t_a);
            }

            HTG_EXPORT void PrintReflection(
                    std::ostream& stream_a,
                    const Reflection::ReflectionData& reflection_data_a,
                    bool referenced_only_a)
            {
                ReflectionPrinter printer_ {stream_a};
                printer_.PrintReflection(reflection_data_a, referenced_only_a);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel