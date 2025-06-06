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
                class LuaTBoolean : public LuaType
                {
                public:
                    explicit LuaTBoolean(bool value_a)
                        : LuaType(), value_(std::move(value_a))
                    {}

                    ~LuaTBoolean()
                    {}

                    int GetTypeId() const;
                    std::string GetTypeName(LuaState& l_a) const;
                    void PushValue(LuaState& l_a);

                    using LuaType::PopValue;
                    void PopValue(LuaState& l_a, int idx_a);
                    std::string ToString() const;
                    bool GetValue() const;
                    void SetValue(bool value_a);

                private:
                    bool value_;
                };
            } // namespace Engine
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel