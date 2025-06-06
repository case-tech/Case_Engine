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
                std::unique_ptr<Types::LuaState> l_ =
                        std::make_unique<Types::LuaState>();
                luaL_openlibs(*l_);

                for (const auto& lib_ : libraries_)
                {
                    ((std::shared_ptr<Registry::LuaLibrary>)lib_.second)
                            ->RegisterFunctions(*l_);
                }

                RegisterHooks(*l_);

                for (const auto& var_ : env_a)
                {
                    ((std::shared_ptr<Types::LuaType>)var_.second)
                            ->PushGlobal(*l_, var_.first);
                }

                for (std::pair<const std::string,
                               LuaCpp::Registry::LuaCFunction> built_in_function_ :
                     built_in_functions_)
                {
                    lua_pushcfunction(*l_,
                                      built_in_function_.second.GetCFunction());
                    lua_setglobal(*l_, built_in_function_.first.c_str());
                }

                lua_pushstring(*l_, std::string(LuaCpp::Version).c_str());
                lua_setglobal(*l_, "_luacppversion");

                return l_;
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
                if (registry_.Exists(name_a))
                {
                    std::unique_ptr<Registry::LuaCodeSnippet> cs_ =
                            registry_.getByName(
                            name_a);
                    std::unique_ptr<Types::LuaState> l_ = NewState(env_a);
                    cs_->UploadCode(*l_);
                    return l_;
                }
                throw std::runtime_error(
                        "Error: The code snipped not found ...");
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
                                         const std::string& fname_a)
            {
                registry_.CompileAndAddFile(name_a, fname_a);
            }

            void LuaContext::CompileFile(const std::string& name_a,
                                         const std::string& fname_a,
                                         bool recompile_a)
            {
                registry_.CompileAndAddFile(name_a, fname_a, recompile_a);
            }

            void LuaContext::CompileFolder(const std::string& path_a)
            {
                CompileFolder(path_a, "", false);
            }

            void LuaContext::CompileFolder(const std::string& path,
                                           const std::string& prefix)
            {
                CompileFolder(path, prefix, false);
            }

            void LuaContext::CompileFolder(const std::string& path_a,
                                           const std::string& prefix_a,
                                           bool recompile_a)
            {
                for (const auto& entry_ :
                     std::filesystem::directory_iterator(path_a))
                {
                    if (!entry_.is_regular_file())
                        continue;

                    const auto fs_path_ = entry_.path();
                    if (fs_path_.extension() != ".lua")
                        continue;

                    std::string stem_ = fs_path_.stem().string();
                    std::string name_build_ = prefix_a.empty()
                                                     ? stem_
                                                     : prefix_a + "." + stem_;

                    try
                    {
                        CompileFile(name_build_, path_a, recompile_a);
                    } 
                    catch (const std::logic_error& e_)
                    {
                        (void)e_;
                    }
                }
            }

            void LuaContext::CompileStringAndRun(const std::string& code_a)
            {
                registry_.CompileAndAddString("default", code_a, true);
                Run("default");
            }

            void LuaContext::CompileFileAndRun(const std::string& code_a)
            {
                registry_.CompileAndAddFile("default", code_a, true);
                Run("default");
            }

            void LuaContext::Run(const std::string& name_a)
            {
                RunWithEnvironment(name_a, global_environment_);
            }

            void LuaContext::RunWithEnvironment(const std::string& name_a,
                                                const LuaEnvironment& env_a)
            {
                std::unique_ptr<Types::LuaState> l_ = NewStateFor(name_a);

                for (const auto& var_ : env_a)
                {
                    ((std::shared_ptr<Types::LuaType>)var_.second)
                            ->PushGlobal(*l_, var_.first);
                }

                int res_ = lua_pcall(*l_, 0, LUA_MULTRET, 0);
                if (res_ != LUA_OK)
                {
                    l_->PrintStack(std::cout);
                    throw std::runtime_error(lua_tostring(*l_, 1));
                }

                for (const auto& var_ : env_a)
                {
                    ((std::shared_ptr<Types::LuaType>)var_.second)
                            ->PopGlobal(*l_);
                }
            }

            std::shared_ptr<Registry::LuaLibrary> LuaContext::GetStdLibrary(
                    const std::string& lib_name_a)
            {
                std::shared_ptr<Registry::LuaLibrary> found_library_ = NULL;
                std::unique_ptr<LuaCpp::Types::LuaState> l_ = NewState(
                        global_environment_);
                lua_getglobal(*l_, lib_name_a.c_str());

                if (lua_istable(*l_, 1))
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

                    lua_pushnil(*l_);

                    while (lua_next(*l_, -2) != 0)
                    {
                        found_library_->AddCFunction(lua_tostring(*l_, -2),
                                                   lua_tocfunction(*l_, -1));

                        lua_pop(*l_, 1);
                    }

                    if (luaL_getmetatable(
                                *l_,
                                found_library_->GetMetaTableName().c_str()))
                    {
                        lua_pushnil(*l_);

                        while (lua_next(*l_, -2) != 0)
                        {
                            if (lua_iscfunction(*l_, -1))
                            {
                                found_library_->AddCMethod(lua_tostring(*l_, -2),
                                                         lua_tocfunction(*l_,
                                                                         -1));
                            }

                            lua_pop(*l_, 1);
                        }

                        lua_getfield(*l_, -1, "__index");
                        lua_pushnil(*l_);

                        while (lua_next(*l_, -2) != 0)
                        {
                            found_library_->AddCMethod(lua_tostring(*l_, -2),
                                                     lua_tocfunction(*l_, -1));

                            lua_pop(*l_, 1);
                        }
                    }
                }

                return found_library_;
            }

            std::shared_ptr<Registry::LuaCFunction> LuaContext::GetBuiltInFnc(
                    const std::string& fnc_name_a)
            {
                std::shared_ptr<Registry::LuaCFunction> built_in_fnc_ = NULL;
                std::unique_ptr<LuaCpp::Types::LuaState> l_ = NewState(
                        global_environment_);

                lua_getglobal(*l_, fnc_name_a.c_str());

                if (lua_iscfunction(*l_, 1))
                {
                    built_in_fnc_ = std::shared_ptr<Registry::LuaCFunction>(
                            new Registry::LuaCFunction(lua_tocfunction(*l_, 1)));
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
                std::unique_ptr<LuaCpp::Types::LuaState> l_ = NewState(
                        global_environment_);
                if (replace_a)
                {
                    built_in_functions_.erase(fnc_name_a);

                    if (Exists_buildInFnc(*l_, fnc_name_a))
                    {
                        lua_pushnil(*l_);
                        lua_setglobal(*l_, fnc_name_a.c_str());
                    }
                }

                if (!Exists_buildInFnc(*l_, fnc_name_a))
                {
                    std::unique_ptr<Registry::LuaCFunction> func_ =
                            std::make_unique<Registry::LuaCFunction>(cfunction_a);
                    func_->SetName(fnc_name_a);
                    built_in_functions_.insert(
                            std::make_pair(fnc_name_a, std::move(*func_)));
                }
            }

            bool LuaContext::Exists_buildInFnc(Types::LuaState& l_a,
                                               const std::string& fnc_name_a)
            {
                lua_getglobal(l_a, fnc_name_a.c_str());

                return !(built_in_functions_.find(fnc_name_a)
                         == built_in_functions_.end())
                       || (!lua_isnil(l_a, -1));
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

            void LuaContext::RegisterHooks(LuaCpp::Types::LuaState& l_a)
            {
                for (const auto& hook_ : hooks_)
                {
                    int mask_ = 0;
                    int count_ = std::get<1>(hook_);

                    if (std::get<0>(hook_) == "call")
                    {
                        mask_ = LUA_MASKCALL;
                    }

                    else if (std::get<0>(hook_) == "return")
                    {
                        mask_ = LUA_MASKRET;
                    }

                    else if (std::get<0>(hook_) == "line")
                    {
                        mask_ = LUA_MASKLINE;
                    }

                    else if (std::get<0>(hook_) == "count")
                    {
                        mask_ = LUA_MASKCOUNT;
                    }

                    lua_sethook(l_a, std::get<2>(hook_), mask_, count_);
                }
            }
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel