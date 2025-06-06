// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Lua.hpp"
#include "../Types/LuaState.hpp"

#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Registry
            {
                extern "C" 
                {
                int CodeWriter(lua_State* l_a,
                               const void* p_a,
                               size_t size_a,
                               void* u_a);
                
                const char* CodeReader(lua_State* l_a,
                                       void* data_a,
                                       size_t* size_a);
                }

                class LuaCodeSnippet
                {
                public:
                    LuaCodeSnippet();
                    ~LuaCodeSnippet()
                    {}

                    int WriteCode(unsigned char* buff_a, size_t size_a);
                    void UploadCode(Types::LuaState& l_a);
                    const char* GetBuffer();
                    int GetSize();

                    std::string GetName();
                    void SetName(std::string name_a);

                private:
                    std::string name_;
                    std::vector<unsigned char> code_;
                };
            } // namespace Registry
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
