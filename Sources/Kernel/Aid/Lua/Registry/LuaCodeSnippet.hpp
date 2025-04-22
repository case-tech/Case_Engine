// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Engine/LuaState.hpp"
#include "../Lua.hpp"

#include <string>
#include <vector>

extern "C" 
{
    int code_writer(lua_State* L_a, const void* p_a, size_t size_a, void* u_a);
    const char* code_reader(lua_State* L_a, void* data_a, size_t* size_a);
}

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Registry
            {
                class LuaCodeSnippet
                {
                public:
                    LuaCodeSnippet();
                    ~LuaCodeSnippet()
                    {
                    }

                    int WriteCode(unsigned char* buff_a, size_t size_a);
                    void UploadCode(types::LuaState& L_a);
                    const char* GetBuffer();
                    int GetSize();
                    std::string GetName();
                    void SetName(std::string name_a);

                private:
                    std::string name_;
                    std::vector<unsigned char> code_;

                };
            }
        }
    }
}