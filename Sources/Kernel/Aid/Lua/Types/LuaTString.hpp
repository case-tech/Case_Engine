#include "../Lua.hpp"
#include "LuaState.hpp"
#include "LuaType.hpp"

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
                    {}

                    ~LuaTString()
                    {}
                    
                    int GetTypeId() const;
                    std::string GetTypeName(LuaState& l_a) const;
                    void PushValue(LuaState& l_a);

                    using LuaType::PopValue;
                    void PopValue(LuaState& l_a, int idx_a);

                    std::string ToString() const;
                    std::string GetValue() const;
                    void SetValue(std::string value_a);

                private:
                    std::string value_;
                };
            } // namespace Types
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel