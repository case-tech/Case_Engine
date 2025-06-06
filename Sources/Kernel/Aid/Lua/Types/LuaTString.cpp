#include "LuaTString.hpp"

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
                int LuaTString::GetTypeId() const
                {
                    return LUA_TSTRING;
                }

                std::string LuaTString::GetTypeName(LuaState& l_a) const
                {
                    return std::string(lua_typename(l_a, LUA_TSTRING));
                }

                void LuaTString::PushValue(LuaState& l_a)
                {
                    lua_pushstring(l_a, value_.c_str());
                }

                void LuaTString::PopValue(LuaState& l_a, int idx_a)
                {
                    if (lua_type(l_a, idx_a) == LUA_TSTRING)
                    {
                        value_ = std::move(std::string(lua_tostring(l_a, idx_a)));
                    } 
                    else
                    {
                        throw std::invalid_argument(
                                "The value at the stack position "
                                + std::to_string(idx_a) + " is not LUA_TSTRING");
                    }
                }

                std::string LuaTString::ToString() const
                {
                    return value_;
                }

                std::string LuaTString::GetValue() const
                {
                    return value_;
                }

                void LuaTString::SetValue(std::string value_a)
                {
                    value_ = std::move(value_a);
                }
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel