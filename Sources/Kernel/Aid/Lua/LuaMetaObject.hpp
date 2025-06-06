// Copyright (c) 2025 Case Technologies

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

            extern "C" 
            {
                static int UNewIndex(lua_State* l_a);
                static int UIndex(lua_State* l_a);
                static int UCall(lua_State* l_a);
            }

            class LuaMetaObject : public Types::LuaTUserData
            {
                friend int UNewIndex(lua_State* l_a);
                friend int UIndex(lua_State* l_a);
                friend int UCall(lua_State* l_a);

            public:
                LuaMetaObject();

                virtual std::shared_ptr<LuaType> GetValue(int key_a);
                virtual std::shared_ptr<LuaType> GetValue(std::string& key_a);
                virtual void SetValue(int key_a, std::shared_ptr<LuaType> val_a);
                virtual void SetValue(std::string& key_a,
                                      std::shared_ptr<LuaType> val_a);
                
                virtual int Execute(Types::LuaState& l_a);

            protected:
                void _StoreData();
                void _RetreiveData();
                virtual int _GetValue(Types::LuaState& l_a);
                virtual int _SetValue(Types::LuaState& l_a);
            };
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
