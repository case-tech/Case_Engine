// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Lua.hpp"
#include "LuaCodeSnippet.hpp"

#include <map>
#include <memory>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Registry
            {
                class LuaRegistry
                {
                public:
                    LuaRegistry()
                        : registry() {};
                    ~LuaRegistry() {};

                    void CompileAndAddString(const std::string& name_a, const std::string& code_a);
                    void CompileAndAddString(const std::string& name_a, const std::string& code_a, bool recompile_a);
                    void CompileAndAddFile(const std::string& name_a, const std::string& fname_a);
                    void CompileAndAddFile(const std::string& name_a, const std::string& fname_a, bool recompile_a);

                    bool inline Exists(const std::string& name_a)
                    {
                        return !(registry_.find(name_a) == registry_.end());
                    }

                    std::unique_ptr<LuaCodeSnippet> GetByName(const std::string& name_a);

                private:
                    std::map<std::string, LuaCodeSnippet> registry_;
                };
            }
        }
    }
}