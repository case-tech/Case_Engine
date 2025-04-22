#include "LuaCFunction.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Registry
            {
                void LuaCFunction::SetName(std::string name_a)
                {
                    name_ = std::move(name_a);
                }

                std::string LuaCFunction::GetName()
                {
                    return name_a;
                }

                void LuaCFunction::SetCFunction(lua_CFunction cfunction_a)
                {
                    cfunction_ = cfunction_a;
                }

                lua_CFunction LuaCFunction::GetCFunction()
                {
                    return cfunction_a;
                }
            }
        }
    }
}