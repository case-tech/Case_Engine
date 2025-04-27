#include "LuaContext.hpp"

#include "LuaVersion.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            std::unique_ptr<Types::LuaState> LuaContext::NewState()
            {
                return NewState(global_environment_);
            }

            std::unique_ptr<Types::LuaState> LuaContext::NewState(
                    const LuaEnvironment& env_a)
            {
                std::unique_ptr<Types::LuaState> L_ =
                        std::make_unique<Types::LuaState>();
                luaL_openlibs(*L_);

                for (const auto& lib : libraries_)
                {
                    ((std::shared_ptr<Registry::LuaLibrary>)lib.second)
                            ->RegisterFunctions(*L_);
                }

                RegisterHooks(*L_);

                for (const auto& var : env_a)
                {
                    ((std::shared_ptr<Types::LuaType>)var.second)
                            ->PushGlobal(*L_, var.first);
                }

                for (std::pair<const std::string, Registry::LuaCFunction>
                             built_in_function : built_in_functions_)
                {
                    lua_pushcfunction(*L_,
                                      built_in_function.second.GetCFunction());
                    lua_setglobal(*L_, built_in_function.first.c_str());
                }

                lua_pushstring(*L_, std::string(version_).c_str());
                lua_setglobal(*L_, "_luacppversion");

                return L_;
            }

            std::unique_ptr<Types::LuaState> LuaContext::NewStateFor(
                    const std::string& name_a)
            {
                return NewStateFor(name_a, global_environment_);
            }

            std::unique_ptr<Types::LuaState> LuaContext::NewStateFor(
                    const std::string& name_a,
                    const LuaEnvironment& env_a)
            {
                (void)env_a;
                if (registry_.Exists(name_a))
                {
                    std::unique_ptr<Registry::LuaCodeSnippet> cd_ =
                            registry_.GetByName(name_a);
                    std::unique_ptr<Types::LuaState> L_ = NewState(env_a);
                    cd_->UploadCode(*L_);
                    return L_;
                }

                throw std::runtime_error(
                        "ERROR: The code snipped not found ...");
            }

            void LuaContext::CompileString(const std::string& name_a,
                                           const std::string& code_a)
            {
                registry_.CompileAndAddString(name_a, code_a);
            }

            void LuaContext::CompileString(const std::string& name_a,
                                           const std::string& code_a,
                                           bool recompile_a)
            {
                registry_.CompileAndAddString(name_a, code_a, recompile_a);
            }

            void LuaContext::CompileFile(const std::string& name_a,
                                         const std::string& file_name_a)
            {
                registry_.CompileAndAddFile(name_a, file_name_a);
            }

            void LuaContext::CompileFile(const std::string& name_a,
                                         const std::string& file_name_a,
                                         bool recompile_a)
            {
                registry_.CompileAndAddFile(name_a, file_name_a, recompile_a);
            }

            void LuaContext::CompileFolder(const std::string& path_a)
            {
                CompileFolder(path_a, ", false");
            }

            void LuaContext::CompileFolder(const std::string& path_a,
                                           const std::string& prefix_a)
            {
                CompileFolder(path_a, prefix_a, false);
            }

            void LuaContext::CompileFolder(const std::string& path_a,
                                           const std::string& prefix_a,
                                           bool recompile_a)
            {
                for (const auto& entry :
                     std::filesystem::directory_iterator(path_a))
                {
                    if (entry.is_regular_file())
                    {
                        std::filesystem::path path_ = entry.path();
                        if (path_.extension() == ".lua")
                        {
                            try
                            {
                                if (prefix_a == "")
                                {
<<<<<<< HEAD
                                    CompileFile(path_.stem().native(),
                                                path_,
=======
                                    CompileFile(path_.stem().string(),
                                                path_.string(),
>>>>>>> aa4b252 (Add open project)
                                                recompile_a);
                                }

                                else
                                {
                                    CompileFile(prefix_a + "."
<<<<<<< HEAD
                                                        + path_.stem().native(),
                                                path_,
                                                recompile_a);
                                }
                            } catch (std::logic_error& e)
=======
                                                        + path_.stem().string(),
                                                path_.string(),
                                                recompile_a);
                                }
                            } catch (...)
>>>>>>> aa4b252 (Add open project)
                            {}
                        }
                    }
                }
            }

            void LuaContext::CompileStringAndRun(const std::string& code_a)
            {
                registry_.CompileAndAddString("default", code_a, true);
                Run("default");
            }

            void LuaContext::Run(const std::string& name_a)
            {
                RunWithEnvironment(name_a, global_environment_);
            }

            void LuaContext::RunWithEnvironment(const std::string& name_a,
                                                const LuaEnvironment& env_a)
            {
                std::unique_ptr<Types::LuaState> L_ = NewStateFor(name_a);

                for (const auto& var : env_a)
                {
                    ((std::shared_ptr<Types::LuaType>)var.second)
                            ->PushGlobal(*L_, var.first);
                }

                int res_ = lua_pcall(*L_, 0, LUA_MULTRET, 0);
                if (res_ != LUA_OK)
                {
                    L_->PrintStack(std::cout);
                    throw std::runtime_error(lua_tostring(*L_, 1));
                }

                for (const auto& var : env_a)
                {
                    ((std::shared_ptr<Types::LuaType>)var.second)
                            ->PopGlobal(*L_);
                }
            }

            std::shared_ptr<Registry::LuaLibrary> LuaContext::GetStdLibrary(
                    const std::string& lib_name_a)
            {
                std::shared_ptr<Registry::LuaLibrary> found_library_ = NULL;
                std::unique_ptr<Types::LuaState> L_ = NewState(
                        global_environment_);
                lua_getglobal(*L_, lib_name_a.c_str());

                if (lua_istable(*L_, 1))
                {
                    if (lib_name_a == "io")
                    {
                        found_library_ = std::make_shared<Registry::LuaLibrary>(
                                lib_name_a,
                                LUA_FILEHANDLE);
                    }

                    else
                    {
                        found_library_ = std::make_shared<Registry::LuaLibrary>(
                                lib_name_a);
                    }

                    lua_pushnil(*L_);

                    while (lua_next(*L_, -2) != 0)
                    {
                        found_library_->AddCFunction(lua_tostring(*L_, -2),
                                                     lua_tocfunction(*L_, -1));

                        lua_pop(*L_, 1);
                    }

                    if (luaL_getmetatable(
                                *L_,
                                found_library_->GetMetaTableName().c_str()))
                    {
                        lua_pushnil(*L_);

                        while (lua_next(*L_, -2) != 0)
                        {
                            if (lua_iscfunction(*L_, -1))
                            {
                                found_library_->AddCMethod(lua_tostring(*L_,
                                                                        -2),
                                                           lua_tocfunction(*L_,
                                                                           -1));
                            }

                            lua_pop(*L_, 1);
                        }

                        lua_getfield(*L_, -1, "__index");
                        lua_pushnil(*L_);

                        while (lua_next(*L_, -2) != 0)
                        {
                            found_library_->AddCMethod(lua_tostring(*L_, -2),
                                                       lua_tocfunction(*L_,
                                                                       -1));

                            lua_pop(*L_, 1);
                        }
                    }
                }

                return found_library_;
            }

            std::shared_ptr<Registry::LuaCFunction> LuaContext::GetBuiltInFnc(
                    const std::string& fnc_name_a)
            {
                std::shared_ptr<Registry::LuaCFunction> built_in_fnc_ = NULL;
                std::unique_ptr<Types::LuaState> L_ = NewState(
                        global_environment_);

                lua_getglobal(*L_, fnc_name_a.c_str());

                if (lua_iscfunction(*L_, 1))
                {
                    built_in_fnc_ = std::shared_ptr<Registry::LuaCFunction>(
                            new Registry::LuaCFunction(
                                    lua_tocfunction(*L_, 1)));
                }

                return built_in_fnc_;
            }

            void LuaContext::SetBuiltInFnc(const std::string& fnc_name_a,
                                           lua_CFunction cfunction_a)
            {
                SetBuiltInFnc(fnc_name_a, cfunction_a, false);
            }

            void LuaContext::SetBuiltInFnc(const std::string& fnc_name_a,
                                           lua_CFunction cfunction_a,
                                           bool replace_a)
            {
                std::unique_ptr<Types::LuaState> L_ = NewState(
                        global_environment_);

                if (replace_a)
                {
                    built_in_functions_.erase(fnc_name_a);

                    if (Exists_buildInFnc(*L_, fnc_name_a))
                    {
                        lua_pushnil(*L_);
                        lua_setglobal(*L_, fnc_name_a.c_str());
                    }
                }

                if (!Exists_buildInFnc(*L_, fnc_name_a))
                {
                    std::unique_ptr<Registry::LuaCFunction> func_ =
                            std::make_unique<Registry::LuaCFunction>(
                                    cfunction_a);
                    func_->SetName(fnc_name_a);
                    built_in_functions_.insert(
                            std::make_pair(fnc_name_a, std::move(*func_)));
                }
            }

            bool LuaContext::Exists_buildInFnc(Types::LuaState& L_a,
                                               const std::string& fnc_name_a)
            {
                lua_getglobal(L_a, fnc_name_a.c_str());

                return !(built_in_functions_.find(fnc_name_a)
                         == built_in_functions_.end())
                       || (!lua_isnil(L_a, -1));
            }

            void LuaContext::AddLibrary(
                    std::shared_ptr<Registry::LuaLibrary>& library_a)
            {
                libraries_[library_a->GetName()] = std::move(library_a);
            }

            void LuaContext::AddGlobalVariable(
                    const std::string& name_a,
                    std::shared_ptr<Types::LuaType> var_a)
            {
                global_environment_[name_a] = std::move(var_a);
            }

            std::shared_ptr<Types::LuaType>& LuaContext::GetGlobalVariable(
                    const std::string& name_a)
            {
                return global_environment_[name_a];
            }

            void LuaContext::AddHook(lua_Hook hook_func_a,
                                     const std::string& hook_type_a,
                                     const int count_a)
            {
                hooks_.push_back(
                        std::tuple<std::string, int, lua_Hook>(hook_type_a,
                                                               count_a,
                                                               hook_func_a));
            }

            void LuaContext::RegisterHooks(Types::LuaState& L_a)
            {
                for (const auto& hook : hooks_)
                {
<<<<<<< HEAD
                    int mask_;
=======
                    int mask_ = 0;
>>>>>>> aa4b252 (Add open project)
                    int count_ = std::get<1>(hook);

                    if (std::get<0>(hook) == "call")
                    {
                        mask_ = LUA_MASKCALL;
                    }

                    else if (std::get<0>(hook) == "return")
                    {
                        mask_ = LUA_MASKRET;
                    }

                    else if (std::get<0>(hook) == "line")
                    {
                        mask_ = LUA_MASKLINE;
                    }

                    else if (std::get<0>(hook) == "count")
                    {
                        mask_ = LUA_MASKCOUNT;
                    }

                    lua_sethook(L_a, std::get<2>(hook), mask_, count_);
                }
            }
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
