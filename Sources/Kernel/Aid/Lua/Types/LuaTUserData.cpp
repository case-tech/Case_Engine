#include "LuaTUserData.hpp"

#include <iostream>
#include <string>
#include <vector>

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

                std::string LuaTUserData::GetTypeName(LuaState& l_a) const
                {
                    return std::string(lua_typename(l_a, LUA_TUSERDATA));
                }

                void LuaTUserData::PushValue(LuaState& l_a)
                {
                    userdata_ = lua_newuserdata(l_a, size_);
                    if (metatable_.size() > 0)
                    {
                        lua_newtable(l_a);
                        for (const auto& pairs_ : metatable_)
                        {
                            lua_pushcfunction(l_a, pairs_.second);
                            lua_setfield(l_a, -2, pairs_.first.c_str());
                        }
                        lua_setmetatable(l_a, -2);
                    }
                    _StoreData();
                }

                void LuaTUserData::PopValue(LuaState& l_a, int idx_a)
                {
                    if (lua_type(l_a, idx_a) == LUA_TUSERDATA)
                    {
                        void* user_data_ = lua_touserdata(l_a, idx_a);
                        if (userdata_ == user_data_)
                        {
                            _RetreiveData();
                        } 
                        else
                        {
                            throw std::domain_error("The value on the stack "+
                               std::to_string(idx_a)+" has different pointer to  the userdata buffer.");
                        }
                    } 
                    else
                    {
                        throw std::invalid_argument(
                                "The value at the stack position "
                                + std::to_string(idx_a) + " is not LUA_TNUMBER");
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
