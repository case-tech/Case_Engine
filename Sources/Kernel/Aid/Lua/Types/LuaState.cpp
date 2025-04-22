#include "LuaState.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Types
            {
                LuaState::LuaState()
                {
                    shared_ = false;
                    L_ = luaL_newstate();
                }

                LuaState::~LuaState()
                {
                    if (!shared_) 
                    {
                        lua_close(L_);
                    }
                }

                LuaState::operator lua_State*()
                {
                    return GetState();
                }

                lua_State* LuaState::getState()
                {
                    return L_;
                }

                void LuaState::PrintStack(std::ostream& out_a)
                {
                    int top_ = lua_gettop(L);
                    out_a << "Total on stack " << top_ << "\n";
                    for (int i = 1; i <= top_; i++) 
                    {
                        out_a << "[" << i << "] -> (" << lua_typename(L, lua_type(L, i)) << ") ";
                        if (lua_type(L_, i) == LUA_TSTRING) 
                        {
                            out_a << lua_tostring(L_, i);
                        }

                        out_a << "\n";
                    }
                }
            }
        }
    }
}