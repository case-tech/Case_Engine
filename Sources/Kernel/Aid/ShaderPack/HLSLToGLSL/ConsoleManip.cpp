#include "ConsoleManip.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            namespace ConsoleManip
            {
                static bool g_enabled_ = true;

                void HTG_EXPORT Enable(bool enable_a)
                {
                    g_enabled_ = enable_a;
                }

                bool HTG_EXPORT IsEnabled()
                {
                    return g_enabled_;
                }
            } // namespace ConsoleManip
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
