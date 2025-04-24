#include "LuaCompiler.hpp"

#include "../Types/LuaState.hpp"

#include <memory>
#include <stdexcept>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Registry
            {
                void inline _CheckErrorAndThrow(Types::LuaState& L_a,
                                                int error_a)
                {
                    if (error_a != LUA_OK)
                    {
                        switch (error_a)
                        {
                        case LUA_ERRMEM:
                            throw std::runtime_error("Out of memory");
                            break;
#if LUA_VERSION_NUM < 504
                        case LUA_ERRGCMM:
                            throw std::out_of_range("GC Error while loading");
                            break;
#endif
                        case LUA_ERRSYNTAX:
                            throw std::logic_error(lua_tostring(L_a, 1));
                            break;
                        default:
                            throw std::runtime_error("Unknown error code "
                                                     + std::to_string(error_a)
                                                     + " :"
                                                     + lua_tostring(L_a, 1));
                        }
                    }
                }

                std::unique_ptr<LuaCodeSnippet> LuaCompiler::CompileString(
                        std::string name_a,
                        std::string code_a)
                {
                    std::unique_ptr<LuaCodeSnippet> cb_ptr_ =
                            std::make_unique<LuaCodeSnippet>();

                    Types::LuaState L_;
                    int res_ = luaL_loadstring(L_.GetState(), code_a.c_str());
                    _CheckErrorAndThrow(L_, res_);

                    res_ = lua_dump(L_.GetState(),
                                    code_writer,
                                    (void*)cb_ptr_.get(),
                                    0);
                    _CheckErrorAndThrow(L_, res_);

                    cb_ptr_->SetName(name_a);
                    return cb_ptr_;
                }

                std::unique_ptr<LuaCodeSnippet> LuaCompiler::CompileFile(
                        std::string name_a,
                        std::string fname_a)
                {
                    std::unique_ptr<LuaCodeSnippet> cb_ptr_ =
                            std::make_unique<LuaCodeSnippet>();

                    Types::LuaState L_;
                    int res_ = luaL_loadfile(L_, fname_a.c_str());
                    _CheckErrorAndThrow(L_, res_);

                    res_ = lua_dump(L_, code_writer, (void*)cb_ptr_.get(), 0);
                    _CheckErrorAndThrow(L_, res_);

                    cb_ptr_->SetName(name_a);
                    return cb_ptr_;
                }
            } // namespace Registry
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
