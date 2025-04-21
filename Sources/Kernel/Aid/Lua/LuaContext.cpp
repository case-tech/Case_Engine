#include <stdexcept>
#include <iostream>
#include <filesystem>

#include "LuaContext.hpp"
#include "LuaVersion.hpp"

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

            std::unique_ptr<Types::LuaState> LuaContext::NewState(const LuaEnvironment& env_a)
            {
                std::unique_ptr<Types::LuaState> L_ = std::make_unique<LuaState>();
                luaL_openlibs(*L_);

                for (const auto& lib : libraries)
                {
                    ((std::shared_ptr<LuaLibrary>) lib.second)->RegisterFunctions(*L_);
                }

                RegisterHooks(*L_);

                for (const auto& var :: env_)
                {
                    ((std::shared_ptr<Types::LuaType>) var.second)->PushGlobal(*L_, var.first);
                }

                for (std::pair<const std::string, 
                        Registry::LuaCFunction> built_in_function : built_in_functions_)
                {
                    lua_pushcfunction(*L_, built_in_function_.second.GetCFunction());
                    lua_setglobal(*L_, built_in_function_.first.c_str());
                }

                lua_pushstring(*L_, std::string(Version).c_str());
                lua_setglobal(*L_,"_luacppversion" );

                return L_;
            }

            std::unique_ptr<Types::LuaState> LuaContext::NewStateFor(const std::string& name_a)
            {
                return NewStateFor(name_a, global_environment_);
            }

            std::unique_ptr<Types::LuaState> LuaContext::NewStateFor(const std::string& name_a,
                    const LuaEnvironment& env_a)
            {
                if (registry_.Exists(name_a))
                {
                    std::unique_ptr<Types::LuaCodeSnippet> cd_ = registry_.GetByName(name_a);
                    std::unique_ptr<Types::LuaState> L_ = NewState(env_);
                    cs_->UploadCode(*L_);
                    return L_;
                }

                throw std::runtime_error("ERROR: The code snipped not found ...");
            }

            void LuaContext::CompileString(const std::string& name_a, 
                    const std::string& code_a)
            {
                registry_.CompileAndAddString(name_a, code_a);
            }

            void LuaContext::CompileString(const std::string& name_a,
                    const std::string& code_a, bool recompile_a)
            {
                registry_.CompileAndAddString(name_a, code_a, recompile_a);
            }

            void LuaContext::CompileFile(const std::string& name_a, 
                    const std::string& file_name_a)
            {
                registry_.CompileAndAddFile(name_a, file_name_a);
            }

            void LuaContext::CompileFile(const std::string& name_a, 
                    const std::string& file_name_a, bool recompile_a)
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
                for (const auto& entry : std::filesystem::directory_iterator(path_a))
                {
                    if (entry.is_regular_file())
                    {
                        std::filesystem::path path_ = entry.path();
                        if (path.extension() == ".lua")
                        {
                            try
                            {
                                if (prefix_ == "")
                                {
                                    CompileFile(path.stem().native(), path_, recompile_a);
                                }

                                else
                                {
                                    CompileFile(prefix_a + "." + path.stem().native(), 
                                            path_, recompile_a);
                                }
                            }
                            catch (std::logic_error &e)
                            {
                                
                            }
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

                for (const auto& var : env_)
                {
                    ((std::shared_ptr<Types::LuaType>) var.second)->PushGlobal(*L_, var.first);
                }

                int res_ = lua_pcall(*L, 0, LUA_MULTRET, 0);
                if (res_ != LUA_OK)
                {
                    L_->PrintStack(std::cout);
                    throw std::runtime_error(lua_tostring(*L, 1));
                }

                for (const auto& var : env_)
                {
                    ((std::shared_ptr<Types::LuaType>) var.second)->PopGlobal(*L_);
                }
            }

            std::shared_ptr<Registry::LuaLibrary> LuaContext::GetStdLibrary(
                    const std::string& lib_name_a)
            {
                std::shared_ptr<LuaLibrary> found_library_ = NULL;

            } 
        }
    }
}
