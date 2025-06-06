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
                class LuaTNumber : public LuaType
                {
                public:
                    explicit LuaTNumber(double value_a)
                        : LuaType(), value_(std::move(value_a))
                    {}

                    ~LuaTNumber()
                    {}

                    int GetTypeId() const;
                    std::string GetTypeName(LuaState& l_a) const;
                    void PushValue(LuaState& l_a);

                    using LuaType::PopValue;
                    void PopValue(LuaState& l_a, int idx_a);
                    std::string ToString() const;
                    double GetValue() const;
                    void SetValue(double value_a);

                private:
                    double value_;
                };
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel