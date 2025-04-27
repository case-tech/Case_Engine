// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Lua.hpp"
#include "LuaState.hpp"
#include "LuaType.hpp"

<<<<<<< HEAD
=======
#include <string>
>>>>>>> aa4b252 (Add open project)
#include <ostream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Types
            {
                class LuaTString : public LuaType
                {
                public:
                    explicit LuaTString(std::string value_a)
                        : LuaType(), value_(std::move(value_a))
                    {
                    }

                    ~LuaTString()
                    {
                    }

                    int GetTypeId() const;
                    std::string GetTypeName(LuaState& L_a) const;
                    void PushValue(LuaState& L_a);
                    using LuaType::PopValue;
                    void PopValue(LuaState& L_a, int idx_a);
                    std::string ToString() const;
                    std::string GetValue() const;
                    void SetValue(std::string value_a);

                private:
                    std::string value_;

                };
            }
        }
    }
}