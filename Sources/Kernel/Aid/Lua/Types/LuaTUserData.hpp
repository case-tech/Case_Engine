// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Lua.hpp"
#include "LuaState.hpp"
#include "LuaType.hpp"

#include <map>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Types
            {
                class LuaTUserData : public LuaType
                {
                public:
                    explicit LuaTUserData(size_t size_a)
                        : LuaType(), userdata_(NULL), size_(size_a),
                          metatable_()
                    {}

                    ~LuaTUserData()
                    {}

                    int GetTypeId() const;
                    std::string GetTypeName(LuaState& L_a) const;
                    void PushValue(LuaState& L_a);
                    using LuaType::PopValue;
                    void PopValue(LuaState& L_a, int idx_a);
                    virtual std::string ToString() const;
                    void* GetRawUserData() const;
                    void AddMetaFunction(std::string fname_a,
                                         lua_CFunction fn_a);

                protected:
                    void* userdata_;
                    size_t size_;
                    std::map<std::string, lua_CFunction> metatable_;
                    virtual void _StoreData();
                    virtual void _RetreiveData();
                };
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
