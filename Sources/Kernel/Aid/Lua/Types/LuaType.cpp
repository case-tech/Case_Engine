#include "LuaType.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace types
            {
                LuaType::LuaType()
                    : global_name_()
                {
                    global_ = false;
                }

                LuaType::~LuaType()
                {
                }

                void LuaType::PopValue(LuaState& L_a)
                {
                    PopValue(L_a, -1);
                }

                void LuaType::PushGlobal(LuaState& L_a, std::string global_name_a)
                {
                    PushValue(L);
                    global_name_ = std::move(global_name_a);
                    global_ = true;

                    lua_setglobal(L_a, global_name_a.c_str());
                }

                void LuaType::PopGlobal(LuaState& L_a)
                {
                    if (global_) 
                    {
                        lua_getglobal(L_a, global_name_.c_str());
                        PopValue(L_a);
                        lua_pop(L_a, 1);
                    }
                }

                void LuaType::PopGlobal(LuaState& L_a, std::string global_name_a)
                {
                    global_ = true;
                    global_name_ = std::move(global_name_a);
                    PopGlobal(L_a);
                }

                bool LuaType::IsGlobal() const
                {
                    return global_;
                }

                std::string LuaType::GetGlobalName() const
                {
                    return global_name_;
                }
            }
        }
    }
}