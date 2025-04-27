#include "LuaMetaObject.hpp"

#include "Types/LuaTBoolean.hpp"
#include "Types/LuaTNil.hpp"
#include "Types/LuaTNumber.hpp"
#include "Types/LuaTString.hpp"
#include "Types/LuaTTable.hpp"

#include <iostream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            LuaMetaObject::LuaMetaObject()
                : LuaTUserData(sizeof(LuaMetaObject*))
            {
                AddMetaFunction("__index", u_index);
                AddMetaFunction("__newindex", u_newindex);
                AddMetaFunction("__call", u_call);
            }

            void LuaMetaObject::_StoreData()
            {
                *((void**)userdata_) = (void*)this;
            }

            void LuaMetaObject::_RetreiveData()
            {}

            std::shared_ptr<Types::LuaType> LuaMetaObject::GetValue(int key_a)
            {
                (void)key_a;
                return std::make_shared<Types::LuaTNil>();
            }

            std::shared_ptr<Types::LuaType> LuaMetaObject::GetValue(
                    std::string& key_a)
            {
                (void)key_a;
                return std::make_shared<Types::LuaTNil>();
            }

            void LuaMetaObject::SetValue(int key_a,
                                         std::shared_ptr<LuaType> val_a)
            {
                (void)key_a;
                (void)val_a;
            }

            void LuaMetaObject::SetValue(std::string& key_a,
                                         std::shared_ptr<LuaType> val_a)
            {
                (void)key_a;
                (void)val_a;
            }

            int LuaMetaObject::_GetValue(Types::LuaState& L_a)
            {
                if (lua_type(L_a, 2) == LUA_TSTRING)
                {
                    std::string key_(lua_tostring(L_a, 2));
                    GetValue(key_)->PushValue(L_a);
                } else
                {
                    GetValue(static_cast<int>(lua_tointeger(L_a, 2)))->PushValue(L_a);
                }
                return 1;
            }

            int LuaMetaObject::_SetValue(Types::LuaState& L_a)
            {
                std::shared_ptr<Types::LuaType> val_ =
                        std::make_shared<Types::LuaTNil>();
                switch (lua_type(L_a, -1))
                {
                case LUA_TSTRING:
                    val_ = std::make_shared<Types::LuaTString>("");
                    val_->PopValue(L_a, -1);
                    break;
                case LUA_TTABLE:
                    val_ = std::make_shared<Types::LuaTTable>();
                    val_->PopValue(L_a, -1);
                    break;
                case LUA_TNUMBER:
                    val_ = std::make_shared<Types::LuaTNumber>(0);
                    val_->PopValue(L_a, -1);
                    break;
                case LUA_TBOOLEAN:
                    val_ = std::make_shared<Types::LuaTBoolean>(false);
                    val_->PopValue(L_a, -1);
                    break;
                default:
                    val_ = std::make_shared<Types::LuaTString>(
                            lua_typename(L_a, lua_type(L_a, -1)));
                    break;
                }

                if (lua_type(L_a, 2) == LUA_TSTRING)
                {
                    std::string key_(lua_tostring(L_a, 2));
                    SetValue(key_, val_);
                } else
                {
                    int key_ = static_cast<int>(lua_tointeger(L_a, 2));
                    SetValue(key_, val_);
                }

                return 0;
            }

            int LuaMetaObject::Exercise(Types::LuaState& L_a)
            {
                (void)L_a;
                return 0;
            }

            static int u_newindex(lua_State* L_a)
            {
                void* ud_ = lua_touserdata(L_a, 1);
                Types::LuaState L_(L_a, true);
                int res_ = (**((LuaMetaObject**)ud_))._SetValue(L_);
                return res_;
            }

            static int u_index(lua_State* L_a)
            {
                void* ud_ = lua_touserdata(L_a, 1);
                Types::LuaState L_(L_a, true);
                int res_ = (**((LuaMetaObject**)ud_))._GetValue(L_);
                return res_;
            }

            static int u_call(lua_State* L_a)
            {
                void* ud_ = lua_touserdata(L_a, 1);
                Types::LuaState L_(L_a, true);
                int res_ = (**((LuaMetaObject**)ud_)).Exercise(L_);
                return res_;
            }
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
