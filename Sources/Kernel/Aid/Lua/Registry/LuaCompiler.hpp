// Copyright (c) 2025 Case Technologies

#pragma once
#include "LuaCodeSnippet.hpp"

#include <memory>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Registry
            {
                class LuaCompiler
                {
                public:
                    explicit LuaCompiler()
                    {}
                    ~LuaCompiler()
                    {}

                    std::unique_ptr<LuaCodeSnippet> CompileString(
                            std::string name_a,
                            std::string code_a);

                    std::unique_ptr<LuaCodeSnippet> CompileFile(
                            std::string name_a,
                            std::string fname_a);
                };
            } // namespace Registry
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
