// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Lua.hpp"

#include <ostream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Types
            {
                class LuaState
                {
                public:
                    explicit LuaState();
                    explicit LuaState(lua_State* l_a) : l_(l_a), shared_(false)
                    {}
                    
                    explicit LuaState(lua_State* l_a, bool shared_a)
                        : l_(l_a), shared_(shared_a)
                    {}

                    ~LuaState();

                    operator lua_State*();
                    lua_State* GetState();
                    void PrintStack(std::ostream& out_a);

                private:
                    lua_State* l_;
                    bool shared_;
                };
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel