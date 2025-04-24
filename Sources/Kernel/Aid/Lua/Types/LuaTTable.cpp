#include "LuaTTable.hpp"

#include "LuaTBoolean.hpp"
#include "LuaTNumber.hpp"
#include "LuaTString.hpp"

#include <iostream>
#include <sstream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Types
            {
                namespace Table
                {
                    bool operator<(const Key& lhs_a, const Key& rhs_a)
                    {
                        if (lhs_a.is_number_ and rhs_a.is_number_)
                        {
                            return lhs_a.int_val_ < rhs_a.int_val_;
                        }

                        if (!lhs_a.is_number_ and !rhs_a.is_number_)
                        {
                            return lhs_a.str_val_ < rhs_a.str_val_;
                        }

                        if (lhs_a.is_number_)
                        {
                            return true;
                        }

                        return false;
                    }

                    bool operator==(const Key& lhs_a, const Key& rhs_a)
                    {
                        if (lhs_a.is_number_ != rhs_a.is_number_)
                        {
                            return false;
                        }

                        if (lhs_a.is_number_)
                        {
                            return lhs_a.int_val_ == rhs_a.int_val_;
                        }

                        return lhs_a.str_val_ == rhs_a.str_val_;
                    }

                    std::ostream& operator<<(std::ostream& os_a,
                                             const Key& key_a)
                    {
                        if (key_a.is_number_)
                        {
                            os_a << std::to_string(key_a.int_val_);
                        }

                        else
                        {
                            os_a << key_a.str_val_;
                        }

                        return os_a;
                    }
                } // namespace Table

                namespace Table
                {
                    std::string Key::ToString() const
                    {
                        if (is_number_)
                        {
                            return std::to_string(int_val_);
                        }

                        else
                        {
                            return str_val_;
                        }
                    }

                    std::string Key::GetStringValue() const
                    {
                        return str_val_;
                    }

                    int Key::GetIntValue() const
                    {
                        return int_val_;
                    }

                    inline bool Key::IsNumber() const
                    {
                        return is_number_;
                    }
                } // namespace Table

                int LuaTTable::GetTypeId() const
                {
                    return LUA_TTABLE;
                }

                std::string LuaTTable::GetTypeName(LuaState& L_a) const
                {
                    return std::string(lua_typename(L_a, LUA_TTABLE));
                }

                void LuaTTable::PushValue(LuaState& L_a)
                {
                    lua_newtable(L_a);

                    for (const auto& pair : table_)
                    {
                        Table::Key& key_ = *((Table::Key*)&pair.first);
                        pair.second->PushValue(L_a);
                        if (key_.IsNumber())
                        {
                            lua_seti(L_a, -2, key_.GetIntValue());
                        }

                        else
                        {
                            lua_setfield(L_a,
                                         -2,
                                         key_.GetStringValue().c_str());
                        }
                    }
                }

                void LuaTTable::PopValue(LuaState& L_a, int idx_a)
                {
                    if (idx_a < 0)
                    {
                        idx_a = lua_gettop(L_a) + idx_a + 1;
                    }

                    if (idx_a <= 0)
                    {
                        throw std::invalid_argument("The stack position "
                                                    + std::to_string(idx_a)
                                                    + " is invalid.");
                    }

                    if (lua_istable(L_a, idx_a) == 1)
                    {
                        table_.clear();
                        lua_pushnil(L_a);
                        while (lua_next(L_a, idx_a) != 0)
                        {
                            std::shared_ptr<LuaType> field_;
                            switch (lua_type(L_a, -1))
                            {
                            case LUA_TSTRING: {
                                field_ = std::make_shared<LuaTString>("");
                                field_->PopValue(L_a, -1);
                                break;
                            }

                            case LUA_TTABLE: {
                                field_ = std::make_shared<LuaTTable>();
                                field_->PopValue(L_a, -1);
                                break;
                            }

                            case LUA_TNUMBER: {
                                field_ = std::make_shared<LuaTNumber>(0);
                                field_->PopValue(L_a, -1);
                                break;
                            }

                            case LUA_TBOOLEAN: {
                                field_ = std::make_shared<LuaTBoolean>(false);
                                field_->PopValue(L_a, -1);
                                break;
                            }

                            default: {
                                field_ = std::make_shared<LuaTString>(
                                        lua_typename(L_a, lua_type(L_a, -1)));
                                break;
                            }
                            }

                            if (lua_type(L_a, -2) == LUA_TSTRING)
                            {
                                SetValue(Table::Key(lua_tostring(L_a, -2)),
                                         field_);
                            }

                            else
                            {
                                SetValue(Table::Key(lua_tointeger(L_a, -2)),
                                         field_);
                            }

                            lua_pop(L_a, 1);
                        }
                    }

                    else
                    {
                        throw std::invalid_argument("The value at the index "
                                                    + std::to_string(idx_a)
                                                    + " is not a LUA_TTABLE");
                    }
                }

                std::string LuaTTable::ToString() const
                {
                    std::stringstream sso_;
                    if (is_array_)
                    {
                        sso_ << "[ ";
                    }

                    else
                    {
                        sso_ << "{ ";
                    }

                    bool add_comma_ = false;
                    for (const auto& pair : table_)
                    {
                        if (add_comma_)
                        {
                            sso_ << ", ";
                        }

                        else
                        {
                            add_comma_ = true;
                        }

                        if (!is_array_)
                        {
                            sso_ << "\"" << pair.first << "\" : ";
                        }

                        if (pair.second->GetTypeId() == LUA_TSTRING)
                        {
                            sso_ << "\"" << pair.second->ToString() << "\"";
                        }

                        else
                        {
                            sso_ << pair.second->ToString();
                        }
                    }

                    if (is_array_)
                    {
                        sso_ << " ]";
                    }

                    else
                    {
                        sso_ << " }";
                    }

                    return sso_.str();
                }

                std::map<Table::Key, std::shared_ptr<LuaType>>
                LuaTTable::GetValues() const
                {
                    return table_;
                }

                LuaType& LuaTTable::GetValue(Table::Key key_a)
                {
                    return *table_[key_a];
                }

                void LuaTTable::SetValue(Table::Key key_a,
                                         std::shared_ptr<LuaType> value_a)
                {
                    if (!key_a.IsNumber())
                    {
                        is_array_ = false;
                    }

                    table_[key_a] = std::move(value_a);
                }
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
