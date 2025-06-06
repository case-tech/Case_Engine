// Copyright (c) 2025 Case Technologies

#pragma once
#include "Registry/LuaLibrary.hpp"
#include "Registry/LuaRegistry.hpp"
#include "Types/LuaState.hpp"
#include "Types/LuaType.hpp"

#include <memory>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            typedef std::map<std::string, std::shared_ptr<Types::LuaType>>
                    LuaEnvironment;

            class LuaContext
            {
            public:
                LuaContext()
                    : registry_(), libraries_(), global_environment_(),
                      built_in_functions_() {};
                ~LuaContext() {};

                std::unique_ptr<Types::LuaState> NewState();
                std::unique_ptr<Types::LuaState> NewState(
                        const LuaEnvironment& env_a);
                
                std::unique_ptr<Types::LuaState> NewStateFor(
                        const std::string& name_a);
                
                std::unique_ptr<Types::LuaState> NewStateFor(
                        const std::string& name_a,
                        const LuaEnvironment& env_a);

                void CompileString(const std::string& name_a,
                                   const std::string& code_a);
                
                void CompileString(const std::string& name_a,
                                   const std::string& code_a,
                                   bool recompile_a);
                
                void CompileFile(const std::string& name_a,
                                 const std::string& fname_a);
                
                void CompileFile(const std::string& name_a,
                                 const std::string& fname_a,
                                 bool recompile_a);
                
                void CompileFolder(const std::string& path_a);
                void CompileFolder(const std::string& path_a,
                                   const std::string& prefix_a);
                
                void CompileFolder(const std::string& path_a,
                                   const std::string& prefix_a,
                                   bool recompile_a);
                
                void CompileStringAndRun(const std::string& code_a);
                void CompileFileAndRun(const std::string& code_a);
                void Run(const std::string& name_a);
                void RunWithEnvironment(const std::string& name_a,
                                        const LuaEnvironment& env_a);
                
                std::shared_ptr<Registry::LuaLibrary> GetStdLibrary(
                        const std::string& lib_name_a);
                
                std::shared_ptr<Registry::LuaCFunction> GetBuiltInFnc(
                        const std::string& fnc_name_a);
                
                void SetBuiltInFnc(const std::string& fnc_name_a,
                                   lua_CFunction cfunction_a);
                
                void SetBuiltInFnc(const std::string& fnc_name_a,
                                   lua_CFunction cfunction_a,
                                   bool replace_a);
                
                bool Exists_buildInFnc(Types::LuaState& l_a,
                                       const std::string& fnc_name_a);
                
                void AddLibrary(std::shared_ptr<Registry::LuaLibrary>& library_a);
                void AddGlobalVariable(const std::string& name_a,
                                       std::shared_ptr<Types::LuaType> var_a);
                
                std::shared_ptr<Types::LuaType>& GetGlobalVariable(
                        const std::string& name_a);

                void AddHook(lua_Hook hook_func_a,
                             const std::string& hook_type_a,
                             const int count_a = 0);

                void RegisterHooks(LuaCpp::Types::LuaState& l_a);

            private:
                std::string name_;
                Registry::LuaRegistry registry_;
                std::map<std::string, std::shared_ptr<Registry::LuaLibrary>>
                        libraries_;
                
                std::vector<std::tuple<std::string, int, lua_Hook>> hooks_;
                LuaEnvironment global_environment_;
                std::map<std::string, LuaCpp::Registry::LuaCFunction>
                        built_in_functions_;
            };
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
