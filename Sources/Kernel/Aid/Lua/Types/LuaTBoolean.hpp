// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Lua.hpp"
#include "LuaState.hpp"
#include "LuaType.hpp"
<<<<<<< HEAD
=======
#include <string>
>>>>>>> aa4b252 (Add open project)

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
                    std::string GetTypeName(LuaState& L_a) const;
                    void PushValue(LuaState& L_a);
                    using LuaType::PopValue;
                    void PopValue(LuaState& L_a, int idx_a);
                    std::string ToString() const;
                    bool GetValue() const;
                    void SetValue(bool value_a);

                private:
                    bool value_;
                };
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
