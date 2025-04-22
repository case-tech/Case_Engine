// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Lua.hpp"
#include "LuaState.hpp"
#include "LuaType.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Types
            {
                class LuaTNil : public LuaType
                {
                public:
                    LuaTNil()
                        : LuaType()
                    {
                    }

                    ~LuaTNil()
                    {
                    }

                    int GetTypeId() const;
                    std::string GetTypeName(LuaState& L_a) const;
                    void PushValue(LuaState& L_a);
                    using LuaType::PopValue;
                    void PopValue(LuaState& L_a, int idx_a);
                    std::string ToString() const;
                };
            }
        }
    }
}