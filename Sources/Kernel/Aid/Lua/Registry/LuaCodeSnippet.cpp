#include "LuaCodeSnippet.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Registry
            {
                LuaCodeSnippet::LuaCodeSnippet() : code_()
                {
                    code_.clear();
                }

                int LuaCodeSnippet::WriteCode(unsigned char* buff_a, size_t size_a)
                {
                    unsigned char* end_ = (unsigned char*)buff_a + size_a;
                    try
                    {
                        code_.insert(code_.end(), buff_a, end_);
                        return 0;
                    } 
                    catch (...)
                    {
                        return 1;
                    }
                }

                int LuaCodeSnippet::GetSize()
                {
                    return int(code_.size());
                }

                void LuaCodeSnippet::SetName(std::string name_a)
                {
                    name_ = std::move(name_a);
                }

                std::string LuaCodeSnippet::GetName()
                {
                    return name_;
                }

                const char* LuaCodeSnippet::GetBuffer()
                {
                    return (const char*)&code_[0];
                }

                void LuaCodeSnippet::UploadCode(Types::LuaState& l_a)
                {
                    lua_load(l_a,
                             CodeReader,
                             this,
                             (const char*)name_.c_str(),
                             NULL);
                }

                int CodeWriter(lua_State* l_a,
                                const void* p_a,
                                size_t size_a,
                                void* u_a)
                {
                    (void)l_a;
                    return ((LuaCodeSnippet*)u_a)
                            ->WriteCode((unsigned char*)p_a, size_a);
                }

                const char* CodeReader(lua_State* l_a, void* data_a, size_t* size_a)
                {
                    (void)l_a;
                    *size_a = ((LuaCodeSnippet*)data_a)->GetSize();
                    return ((LuaCodeSnippet*)data_a)->GetBuffer();
                }
            } // namespace Registry
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel
