// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Lua.hpp"
#include "LuaState.hpp"
#include "LuaType.hpp"

#include <map>
#include <memory>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Types
            {
                namespace Table
                {
                    class Key
                    {
                    public:
                        explicit Key(int value_a)
                            : is_number_(true), str_val_(), int_val_(value_a)
                        {}

                        explicit Key(std::string value_a)
                            : is_number_(false), str_val_(std::move(value_a)),
                              int_val_(0)
                        {}

                        explicit Key(const char* value_a)
                            : is_number_(false), str_val_(std::string(value_a)),
                              int_val_(0)
                        {}

                        bool IsNumber() const;

                        std::string GetStringValue() const;
                        int GetIntValue() const;

                        std::string ToString() const;

                        friend bool operator<(const Key& lhs_a, const Key& rhs_a);
                        friend bool operator==(const Key& lhs_a, const Key& rhs_a);
                        friend std::ostream& operator<<(std::ostream& os_a,
                                                        const Key& key_a);

                    private:
                        bool is_number_;
                        std::string str_val_;
                        int int_val_;
                    };
                } // namespace Table

                class LuaTTable : public LuaType
                {
                public:
                    explicit LuaTTable() : LuaType(), is_array_(true), table_()
                    {}

                    ~LuaTTable()
                    {}
                    
                    int GetTypeId() const;
                    std::string GetTypeName(LuaState& l_a) const;
                    void PushValue(LuaState& l_a);

                    using LuaType::PopValue;
                    void PopValue(LuaState& l_a, int idx_a);
                    std::string ToString() const;
                    std::map<Table::Key, std::shared_ptr<LuaType>> GetValues()
                            const;
                    LuaType& GetValue(Table::Key key_a);
                    void SetValue(Table::Key key_a,
                                  std::shared_ptr<LuaType> value_a);

                private:
                    bool is_array_;
                    std::map<Table::Key, std::shared_ptr<LuaType>> table_;
                };
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
