#include "LuaTUserData.hpp"

#include <iostream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Types
            {
                int LuaTUserData::GetTypeId() const
                {
                    return LUA_TUSERDATA;
                }

                std::string LuaTUserData::GetTypeName(LuaState& L_a) const
                {
                    return std::string(lua_typename(L_a, LUA_TUSERDATA));
                }

                void LuaTUserData::PushValue(LuaState& L_a)
                {
                    userdata_ = lua_newuserdata(L_a, size_);
                    if (metatable_.size() > 0)
                    {
                        lua_newtable(L_a);
                        for (const auto& pairs : metatable_)
                        {
                            lua_pushcfunction(L_a, pairs.second);
                            lua_setfield(L_a, -2, pairs.first.c_str());
                        }

                        lua_setmetatable(L_a, -2);
                    }

                    _StoreData();
                }

                void LuaTUserData::PopValue(LuaState& L_a, int idx_a)
                {
                    if (lua_type(L_a, idx_a) == LUA_TUSERDATA)
                    {
                        void* userdatal_ = lua_touserdata(L_a, idx_a);
                        if (userdatal_ == userdata_)
                        {
                            _RetreiveData();
                        }

                        else
                        {
                            throw std::domain_error("The value on the stack " + std::to_string(idx_a) + " has different pointer to  the userdata buffer.");
                        }
                    }

                    else
                    {
                        throw std::invalid_argument(
                                "The value at the stack position "
                                + std::to_string(idx_a)
                                + " is not LUA_TNUMBER");
                    }
                }

                std::string LuaTUserData::ToString() const
                {
                    return "userdata";
                }

                void LuaTUserData::_StoreData()
                {}

                void LuaTUserData::_RetreiveData()
                {}

                void* LuaTUserData::GetRawUserData() const
                {
                    return userdata_;
                }

                void LuaTUserData::AddMetaFunction(std::string fname_a,
                                                   lua_CFunction fn_a)
                {
                    metatable_[fname_a] = fn_a;
                }
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
