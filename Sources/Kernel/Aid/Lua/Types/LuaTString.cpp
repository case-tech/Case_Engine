// Copyright (c) 2025 Case Technologies

#include "LuaTString.hpp"

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

                std::string LuaTString::GetTypeName(LuaState& L_a) const
                {
                    return std::string(lua_typename(L_a, LUA_TSTRING));
                }

                void LuaTString::PushValue(LuaState& L_a)
                {
                    lua_pushstring(L_a, value_.c_str());
                }

                void LuaTString::PopValue(LuaState& L_a, int idx_a)
                {
                    if (lua_type(L_a, idx_a) == LUA_TSTRING)
                    {
                        value_ = std::move(
                                std::string(lua_tostring(L_a, idx_a)));
                    }

                    else
                    {
                        throw std::invalid_argument(
                                "The value at the stack position "
                                + std::to_string(idx_a)
                                + " is not LUA_TSTRING");
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
