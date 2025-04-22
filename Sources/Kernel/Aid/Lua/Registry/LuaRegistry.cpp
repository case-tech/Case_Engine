#include "LuaRegistry.hpp"
#include "LuaCompiler.hpp"

#include <memory>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Registry
            {
                void LuaRegistry::CompileAndAddString(const std::string& name_a, const std::string& code_a)
                {
                    CompileAndAddString(name_a, code_a, false);
                }

                void LuaRegistry::CompileAndAddString(const std::string& name_a, const std::string& code_a, bool recompile_a)
                {
                    if (!Exists(name_a) or recompile_a) 
                    {
                        LuaCompiler cmp_;
                        std::unique_ptr<LuaCodeSnippet> snp_ = cmp_.CompileString(name_a, code_a);

                        registry_[name_a] = std::move(*snp_);
                    }
                }

                void LuaRegistry::CompileAndAddFile(const std::string& name_a, const std::string& fname_a)
                {
                    CompileAndAddFile(name_a, fname_a, false);
                }

                void LuaRegistry::CompileAndAddFile(const std::string& name_a, const std::string& fname_a, bool recompile_a)
                {
                    if (!Exists(name_a) or recompile_a) 
                    {
                        LuaCompiler cmp_;
                        std::unique_ptr<LuaCodeSnippet> snp_ = cmp_.CompileFile(name_a, fname_a);

                        registry_[name_a] = std::move(*snp_);
                    }
                }

                std::unique_ptr<LuaCodeSnippet> LuaRegistry::GetByName(const std::string& name_a)
                {
                    return std::make_unique<LuaCodeSnippet>(registry_[name_a]);
                }
            }
        }
    }
}