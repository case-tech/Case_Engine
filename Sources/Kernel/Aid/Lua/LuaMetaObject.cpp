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
                AddMetaFunction("__index", UIndex);
                AddMetaFunction("__newindex", UNewIndex);
                AddMetaFunction("__call", UCall);
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

            void LuaMetaObject::SetValue(int key_a, std::shared_ptr<LuaType> val_a)
            {
                (void)key_a, val_a;
            }

            void LuaMetaObject::SetValue(std::string& key_a,
                                         std::shared_ptr<LuaType> val_a)
            {
                (void)key_a, val_a;
            }

            int LuaMetaObject::_GetValue(Types::LuaState& l_a)
            {
                if (lua_type(l_a, 2) == LUA_TSTRING)
                {
                    std::string key_(lua_tostring(l_a, 2));
                    GetValue(key_)->PushValue(l_a);
                } 
                else
                {
                    GetValue((int)lua_tointeger(l_a, 2))->PushValue(l_a);
                }
                return 1;
            }

            int LuaMetaObject::_SetValue(Types::LuaState& l_a)
            {
                std::shared_ptr<LuaType> val_ =
                        std::make_shared<Types::LuaTNil>();
                switch (lua_type(l_a, -1))
                {
                case LUA_TSTRING: {
                    val_ = std::make_shared<Types::LuaTString>("");
                    val_->PopValue(l_a, -1);
                    break;
                }
                case LUA_TTABLE: {
                    val_ = std::make_shared<Types::LuaTTable>();
                    val_->PopValue(l_a, -1);
                    break;
                }
                case LUA_TNUMBER: {
                    val_ = std::make_shared<Types::LuaTNumber>(0);
                    val_->PopValue(l_a, -1);
                    break;
                }
                case LUA_TBOOLEAN: {
                    val_ = std::make_shared<Types::LuaTBoolean>(false);
                    val_->PopValue(l_a, -1);
                    break;
                }
                default: {
                    val_ = std::make_shared<Types::LuaTString>(
                            lua_typename(l_a, lua_type(l_a, -1)));
                    break;
                }
                }
                if (lua_type(l_a, 2) == LUA_TSTRING)
                {
                    std::string key_(lua_tostring(l_a, 2));
                    SetValue(key_, val_);
                } 
                else
                {
                    int key_ = (int)lua_tointeger(l_a, 2);
                    SetValue(key_, val_);
                }

                return 0;
            }

            int LuaMetaObject::Execute(Types::LuaState& l_a)
            {
                (void)l_a;
                return 0;
            }

            static int LuaCpp::UNewIndex(lua_State* l_a)
            {
                void* ud_ = lua_touserdata(l_a, 1);
                Types::LuaState l_(l_a, true);
                int res_ = (**((LuaMetaObject**)ud_))._SetValue(l_);

                return res_;
            }

            static int LuaCpp::UIndex(lua_State* l_a)
            {
                void* ud_ = lua_touserdata(l_a, 1);
                Types::LuaState l_(l_a, true);
                int res_ = (**((LuaMetaObject**)ud_))._GetValue(l_);

                return res_;
            }

            static int LuaCpp::UCall(lua_State* l_a)
            {
                void* ud_ = lua_touserdata(l_a, 1);
                Types::LuaState l_(l_a, true);
                int res_ = (**((LuaMetaObject**)ud_)).Execute(l_);

                return res_;
            }
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel