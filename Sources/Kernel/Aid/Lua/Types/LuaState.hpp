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
                    explicit LuaState(lua_State* L_a)
                        : L_(L_a), shared_(false)
                    {
                    }

                    explicit LuaState(lua_State* L_a, bool shared_a)
                        : L_(L_a), shared_(shared_a)
                    {
                    }

                    ~LuaState();

                    operator lua_State*();
                    lua_State* GetState();
                    void PrintStack(std::ostream& out_a);

                private:
                    lua_State* L_;
                    bool shared_;
                };
            }
        }
	}
}