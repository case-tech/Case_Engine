#include "LuaTNumber.hpp"

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
                int LuaTNumber::GetTypeId() const
                {
                    return LUA_TNUMBER;
                }

                std::string LuaTNumber::GetTypeName(LuaState& l_a) const
                {
                    return std::string(lua_typename(l_a, LUA_TNUMBER));
                }

                void LuaTNumber::PushValue(LuaState& l_a)
                {
                    lua_pushnumber(l_a, value_);
                }

                void LuaTNumber::PopValue(LuaState& l_a, int idx_a)
                {
                    if (lua_type(l_a, idx_a) == LUA_TNUMBER)
                    {
                        value_ = lua_tonumber(l_a, idx_a);
                    } 
                    else
                    {
                        throw std::invalid_argument(
                                "The value at the stack position "
                                + std::to_string(idx_a) + " is not LUA_TNUMBER");
                    }
                }

                std::string LuaTNumber::ToString() const
                {
                    return std::to_string(value_);
                }

                double LuaTNumber::GetValue() const
                {
                    return value_;
                }

                void LuaTNumber::SetValue(double value_a)
                {
                    value_ = value_a;
                }
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel