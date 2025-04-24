// Copyright (c) 2025 Case Technologies)

#pragma once
#include "Lua.hpp"
#include "Types/LuaState.hpp"
#include "Types/LuaTUserData.hpp"

#include <memory>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            extern "C" {
            static int u_newindex(lua_State* L_a);
            static int u_index(lua_State* L_a);
            static int u_call(lua_State* L_a);
            }

            class LuaMetaObject : public Types::LuaTUserData
            {
                friend int u_newindex(lua_State* L_a);
                friend int u_index(lua_State* L_a);
                friend int u_call(lua_State* L_a);

            protected:
                void _StoreData();
                void _RetreiveData();

                virtual int _GetValue(Types::LuaState& L_a);
                virtual int _SetValue(Types::LuaState& L_a);

            public:
                LuaMetaObject();
                virtual std::shared_ptr<Types::LuaType> GetValue(int key_a);
                virtual std::shared_ptr<Types::LuaType> GetValue(
                        std::string& key_a);
                virtual void SetValue(int key_a,
                                      std::shared_ptr<Types::LuaType> val_a);
                virtual void SetValue(std::string& key_a,
                                      std::shared_ptr<Types::LuaType> val_a);
                virtual int Exercise(Types::LuaState& L_a);
            };
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
