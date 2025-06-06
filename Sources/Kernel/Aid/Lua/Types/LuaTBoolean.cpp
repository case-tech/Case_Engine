#include "LuaTBoolean.hpp"

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
                int LuaTBoolean::GetTypeId() const
                {
                    return LUA_TBOOLEAN;
                }

                std::string LuaTBoolean::GetTypeName(LuaState& l_a) const
                {
                    return std::string(lua_typename(l_a, LUA_TBOOLEAN));
                }

                void LuaTBoolean::PushValue(LuaState& l_a)
                {
                    lua_pushboolean(l_a, value_);
                }

                void LuaTBoolean::PopValue(LuaState& l_a, int idx_a)
                {
                    if (lua_type(l_a, idx_a) == LUA_TBOOLEAN)
                    {
                        value_ = lua_toboolean(l_a, idx_a);
                    } 
                    else
                    {
                        throw std::invalid_argument(
                                "The value at the stack position "
                                + std::to_string(idx_a) + " is not LUA_TBOOLEAN");
                    }
                }

                std::string LuaTBoolean::ToString() const
                {
                    if (value_)
                    {
                        return "true";
                    }
                    else
                    {
                        return "false";
                    }
                }

                bool LuaTBoolean::GetValue() const
                {
                    return value_;
                }

                void LuaTBoolean::SetValue(bool value_a)
                {
                    value_ = value_a;
                }
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
