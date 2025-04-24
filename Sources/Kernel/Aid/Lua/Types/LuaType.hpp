// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Lua.hpp"
#include "LuaState.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Types
            {
                class LuaType
                {
                public:
                    LuaType();
                    virtual ~LuaType() = 0;

                    virtual int GetTypeId() const = 0;
                    virtual std::string GetTypeName(LuaState& L_a) const = 0;
                    virtual void PushValue(LuaState& L_a) = 0;
                    virtual void PopValue(LuaState& L_a, int idx_a) = 0;
                    virtual std::string ToString() const = 0;
                    void PopValue(LuaState& L_a);
                    void PushGlobal(LuaState& L_a, std::string global_name_a);
                    void PopGlobal(LuaState& L_a);
                    void PopGlobal(LuaState& L_a, std::string global_name_a);

                    bool IsGlobal() const;
                    std::string GetGlobalName() const;

                private:
                    bool global_;
                    std::string global_name_;
                };
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
