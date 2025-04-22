#include "LuaTNumber.hpp"

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

                std::string LuaTNumber::GetTypeName(LuaState& L_a) const
                {
                    return std::string(lua_typename(L_a, LUA_TNUMBER));
                }

                void LuaTNumber::PushValue(LuaState& L_a)
                {
                    lua_pushnumber(L_a, value_);
                }

                void LuaTNumber::PopValue(LuaState& L_a, int idx_a)
                {
                    if (lua_type(L_a, idx_a) == LUA_TNUMBER) 
                    {
                        value_ = lua_tonumber(L_a, idx_a);
                    } 
                    
                    else 
                    {
                        throw std::invalid_argument("The value at the stack position " + std::to_string(idx_a) + " is not LUA_TNUMBER");
                    }
                }

                std::string LuaTNumber::ToString() const
                {
                    return std::to_string(value_);
                }

                double LuaTNumber::getValue() const
                {
                    return value_;
                }

                void LuaTNumber::setValue(double value_a)
                {
                    value_ = value_a;
                }
            }
        }
    }
}