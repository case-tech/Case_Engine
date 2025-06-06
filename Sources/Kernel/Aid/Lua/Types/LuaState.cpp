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
                    l_ = luaL_newstate();
                }

                LuaState::~LuaState()
                {
                    if (!shared_)
                    {
                        lua_close(l_);
                    }
                }

                LuaState::operator lua_State*()
                {
                    return GetState();
                }

                lua_State* LuaState::GetState()
                {
                    return l_;
                }

                void LuaState::PrintStack(std::ostream& out_a)
                {
                    int top_ = lua_gettop(l_);
                    out_a << "Total on stack " << top_ << "\n";
                    for (int i_ = 1; i_ <= top_; i_++)
                    {
                        out_a << "[" << i_ << "] -> ("
                            << lua_typename(l_, lua_type(l_, i_)) << ") ";
                        
                        if (lua_type(l_, i_) == LUA_TSTRING)
                        {
                            out_a << lua_tostring(l_, i_);
                        }
                        out_a << "\n";
                    }
                }
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel