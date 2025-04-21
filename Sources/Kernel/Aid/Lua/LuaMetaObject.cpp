#include <iostream>

#include "LuaMetaObject.hpp"
#include "Types/LuaTNil.hpp"
#include "Types/LuaTTable.hpp"
#include "Types/LuaTString.hpp"
#include "Types/LuaTNumber.hpp"
#include "Types/LuaTBoolean.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            LuaMetaObject::LuaMetaObject() : LuaTUserData(sizeof(LuaMetaObject *))
            {
                AddMetaFunction("__index", u_index);
                AddMetaFunction("__newindex", u_newindex);
                AddMetaFunction("__call", u_call);
            }

            void LuaMetaObject::_StoreData()
            {
                *((void **) userdata) = (void *) this;
            }

            void LuaMetaObject::_RetreiveData()
            {
                
            }
    
            std::shared_ptr<LuaType> LuaMetaObject::GetValue(int key_a)
            {
                return std::make_shared<LuaTNil>();
            }

            std::shared_ptr<LuaType> LuaMetaObject::GetValue(std::string& key_a)
            {
                return std::make_shared<LuaTNil>();
            }

            void LuaMetaObject::SetValue(int key_a, std::shared_ptr<LuaType> val_a) 
            {

            }

            void LuaMetaObject::SetValue(std::string& key_a, std::shared_ptr<LuaType> val_a) 
            {

            }

            int LuaMetaObject::_GetValue(LuaState& L_a)
            {
                if (lua_type(L_a, 2) == LUA_TSTRING)
                {
                    std::string key_(lua_tostring(L_a, 2));
                    GetValue(key_)->PushValue(L_a);
                }

                else
                {
                    GetValue(lua_tointeger(L_a, 2))->PushValue(L_a);
                }

                return 1;
            }

            int LuaMetaObject::_SetValue(LuaState& L_a)
            {
                std::shared_ptr<LuaType> val_ = std::make_shared<LuaTNil>();
                switch (lua_type(L_a, -1))
                {
                case LUA_TSTRING:
                {
                    val_ = std::make_shared<LuaTString>("");
                    val_->PopValue(L_a, -1);
                    break;
                }
                case LUA_TTABLE:
                {
                    val_ = std::make_shared<LuaTTable>();
                    val_->PopValue(L_a, -1);
                    break;
                }
                case LUA_TNUMBER:
                {
                    val_ = std::make_shared<LuaTNumber>(0);
                    val_->PopValue(L_a, -1);
                    break;
                }
                case LUA_TBOOLEAN:
                {
                    val_ = std::make_shared<LuaTBoolean>(false);
                    val_->PopValue(L_a, -1);
                    break;
                }
                default: 
                {
                    val_ = std::make_shared<LuaTString>(lua_typename(L_a, lua_type(L_a, -1)));
                    break;
                }
                }

                if (lua_type(L_a, 2) == LUA_TSTRING)
                {
                    std::string key_(lua_tostring(L_a, 2));
                    SetValue(key_, val_);
                }

                else
                {
                    int key_ = lua_tointeger(L_a, 2);
                    SetValue(key_, val_);
                }

                return 0;
            }
        }

        int LuaMetaObject::Execute(LuaState& L_a)
        {
            return 0;
        }

        static int LuaCpp::u_newindex(lua_State* L_a)
        {
            void* ud_ = lua_touserdata(L_a, 1);
            LuaState L_(L_a, true);
            int res_ = (**((LuaMetaObject **)ud_))._SetValue(L_);

            return res_;
        }

        static int LuaCpp::u_index(lua_State* L_a)
        {
            void* ud_ = lua_touserdata(L_a, 1);
            LuaState L_(L_a, true);
            int res_ = (**((LuaMetaObject **) ud_))._GetValue(L_);

            return res_;
        }

        static int LuaCpp::u_call(lua_state* L_a)
        {
            void* ud_ = lua_touserdata(L_a, 1);
            LuaState L_(L_A, true);
            int res_ = (**((LuaMetaObject **) ud_)).Execute(L_);

            return res_;
        }
    }
}
