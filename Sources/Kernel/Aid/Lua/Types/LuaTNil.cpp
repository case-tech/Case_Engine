#include "LuaTNil.hpp"

#include <iostream>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Types
            {
                int LuaTNil::GetTypeId() const
                {
                    return LUA_TNIL;
                }

                std::string LuaTNil::GetTypeName(LuaState& l_a) const
                {
                    return std::string(lua_typename(l_a, LUA_TNIL));
                }

                void LuaTNil::PushValue(LuaState& l_a)
                {
                    lua_pushnil(l_a);
                }

                void LuaTNil::PopValue(LuaState& l_a, int idx_a)
                {
                    if (!lua_type(l_a, idx_a) == LUA_TNIL)
                    {
                        throw std::invalid_argument(
                                "The value at the stack position "
                                + std::to_string(idx_a) + " is not LUA_TNUMBER");
                    }
                }

                std::string LuaTNil::ToString() const
                {
                    return "nil";
                }
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
