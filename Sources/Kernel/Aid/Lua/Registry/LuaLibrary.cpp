#include "LuaLibrary.hpp"

#include <iostream>
#include <memory>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Registry
            {
                std::string LuaLibrary::GetName()
                {
                    return name_;
                }

                std::string LuaLibrary::GetMetaTableName()
                {
                    return meta_table_name_;
                }

                void LuaLibrary::SetName(const std::string& name_a)
                {
                    name_ = std::move(name_a);
                }

                void LuaLibrary::AddCMetaMethod(const std::string& name_a,
                                                lua_CFunction cfunction_a)
                {
                    AddCMetaMethod(name_a, cfunction_a, false);
                }

                void LuaLibrary::AddCMetaMethod(const std::string& name_a,
                                                lua_CFunction cfunction_a,
                                                bool replace_a)
                {
                    if (replace_a)
                    {
                        meta_methods_.erase(name_a);
                    }

                    if (!Exists_m(name_a))
                    {
                        std::unique_ptr<LuaCFunction> func_ =
                                std::make_unique<LuaCFunction>(cfunction_a);
                        func_->SetName(name_a);
                        meta_methods_.insert(
                                std::make_pair(name_a, std::move(*func_)));
                    }
                }

                void LuaLibrary::AddCMethod(const std::string& name_a,
                                            lua_CFunction cfunction_a)
                {
                    AddCMethod(name_a, cfunction_a, false);
                }

                void LuaLibrary::AddCMethod(const std::string& name_a,
                                            lua_CFunction cfunction_a,
                                            bool replace_a)
                {
                    if (replace_a)
                    {
                        methods_.erase(name_a);
                    }

                    if (!Exists_m(name_a))
                    {
                        std::unique_ptr<LuaCFunction> func_ =
                                std::make_unique<LuaCFunction>(cfunction_a);
                        func_->SetName(name_a);
                        methods_.insert(
                                std::make_pair(name_a, std::move(*func_)));
                    }
                }

                void LuaLibrary::AddCFunction(const std::string& name_a,
                                              lua_CFunction cfunction_a)
                {
                    AddCFunction(name_a, cfunction_a, false);
                }

                void LuaLibrary::AddCFunction(const std::string& name_a,
                                              lua_CFunction cfunction_a,
                                              bool replace_a)
                {
                    if (replace_a)
                    {
                        functions_.erase(name_a);
                    }

                    if (!Exists_f(name_a))
                    {
                        std::unique_ptr<LuaCFunction> func_ =
                                std::make_unique<LuaCFunction>(cfunction_a);
                        func_->SetName(name_a);
                        functions_.insert(
                                std::make_pair(name_a, std::move(*func_)));
                    }
                }

                lua_CFunction LuaLibrary::GetLibMethod(
                        const std::string& name_a)
                {
                    return methods_.at(name_a).GetCFunction();
                }

                lua_CFunction LuaLibrary::GetLibFunction(
                        const std::string& name_a)
                {
                    return functions_.at(name_a).GetCFunction();
                }

                int LuaLibrary::RegisterFunctions(Types::LuaState& L_a)
                {
                    lua_pushnil(L_a);
                    lua_setglobal(L_a, name_.c_str());

                    luaL_newmetatable(L_a, meta_table_name_.c_str());

                    std::vector<luaL_Reg> array_meta_meth_(meta_methods_.size()
                                                           + 1);
                    int count_ = 0;

                    for (auto& x : meta_methods_)
                    {
                        LuaCFunction& lcf_ = x.second;
                        array_meta_meth_[count_].name = x.first.c_str();
                        array_meta_meth_[count_].func = lcf_.GetCFunction();
                        count_++;
                    }

                    array_meta_meth_[count_].name = NULL;
                    array_meta_meth_[count_].func = NULL;

                    std::vector<luaL_Reg> arraylib_f_(meta_methods_.size() + 1);
                    count_ = 0;

                    for (auto& x : functions_)
                    {
                        LuaCFunction& lcf_ = x.second;
                        arraylib_f_[count_].name = x.first.c_str();
                        arraylib_f_[count_].func = lcf_.GetCFunction();
                        count_++;
                    }

                    arraylib_f_[count_].name = NULL;
                    arraylib_f_[count_].func = NULL;

                    std::vector<luaL_Reg> arraylib_m_(meta_methods_.size() + 1);
                    count_ = 0;

                    for (auto& x : methods_)
                    {
                        LuaCFunction& lcf_ = x.second;
                        arraylib_m_[count_].name = x.first.c_str();
                        arraylib_m_[count_].func = lcf_.GetCFunction();
                        count_++;
                    }

                    arraylib_m_[count_].name = NULL;
                    arraylib_m_[count_].func = NULL;

                    luaL_setfuncs(L_a, array_meta_meth_.data(), 0);
<<<<<<< HEAD
                    lua_createtable(L_a, 0, arraylib_m_.size() - 1);
=======
                    lua_createtable(L_a, 0, static_cast<int>(arraylib_m_.size()) - 1);
>>>>>>> aa4b252 (Add open project)
                    luaL_setfuncs(L_a, arraylib_m_.data(), 0);

                    lua_setfield(L_a, -2, "__index");
                    lua_pop(L_a, 1);

<<<<<<< HEAD
                    lua_createtable(L_a, 0, arraylib_f_.size() - 1);
=======
                    lua_createtable(L_a, 0, static_cast<int>(arraylib_f_.size()) - 1);
>>>>>>> aa4b252 (Add open project)
                    luaL_setfuncs(L_a, arraylib_f_.data(), 0);
                    lua_setglobal(L_a, name_.c_str());

                    return 0;
                }
            } // namespace Registry
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
