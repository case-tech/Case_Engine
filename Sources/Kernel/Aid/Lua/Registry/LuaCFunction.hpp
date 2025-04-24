// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Lua.hpp"

#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Registry
            {
                class LuaCFunction
                {
                public:
                    LuaCFunction() {};
                    explicit LuaCFunction(lua_CFunction cfunction_a)
                        : cfunction_(cfunction_a) {};
                    ~LuaCFunction() {};

                    void SetName(std::string name_a);
                    std::string GetName();

                    void SetCFunction(lua_CFunction cfunction_a);
                    lua_CFunction GetCFunction();

                private:
                    std::string name_;
                    lua_CFunction cfunction_;
                };
            } // namespace Registry
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
