#include "LuaType.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Types
            {
                LuaType::LuaType() : global_name_()
                {
                    global_ = false;
                }

                LuaType::~LuaType()
                {}

                void LuaType::PopValue(LuaState& l_a)
                {
                    PopValue(l_a, -1);
                }

                void LuaType::PushGlobal(LuaState& l_a, std::string global_name_a)
                {
                    PushValue(l_a);
                    global_name_ = std::move(global_name_a);
                    global_ = true;

                    lua_setglobal(l_a, global_name_.c_str());
                }

                void LuaType::PopGlobal(LuaState& l_a)
                {
                    if (global_)
                    {
                        lua_getglobal(l_a, global_name_.c_str());
                        PopValue(l_a);
                        lua_pop(l_a, 1);
                    }
                }

                void LuaType::PopGlobal(LuaState& l_a, std::string global_name_a)
                {
                    global_ = true;
                    global_name_ = std::move(global_name_a);
                    PopGlobal(l_a);
                }

                bool LuaType::isGlobal() const
                {
                    return global_;
                }

                std::string LuaType::GetGlobalName() const
                {
                    return global_name_;
                }
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel