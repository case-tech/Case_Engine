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
                    virtual std::string GetTypeName(LuaState& l_a) const = 0;
                    virtual void PushValue(LuaState& l_a) = 0;
                    virtual void PopValue(LuaState& l_a, int idx_a) = 0;
                    virtual std::string ToString() const = 0;

                    void PopValue(LuaState& l_a);
                    void PushGlobal(LuaState& l_a, std::string global_name_a);
                    void PopGlobal(LuaState& l_a);
                    void PopGlobal(LuaState& l_a, std::string global_name_a);
                    bool isGlobal() const;

                    std::string GetGlobalName() const;

                private:
                    bool global_;
                    std::string global_name_;
                };
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel