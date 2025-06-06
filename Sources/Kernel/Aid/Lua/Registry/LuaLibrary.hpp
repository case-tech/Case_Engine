// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Lua.hpp"
#include "../Types/LuaState.hpp"
#include "LuaCFunction.hpp"

#include <map>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace LuaCpp
        {
            namespace Registry
            {
                class LuaLibrary
                {
                public:
                    explicit LuaLibrary(const std::string& name_a)
                        : name_(std::move(name_a)),
                          meta_table_name_(std::move(name_a))
                    {}

                    explicit LuaLibrary(const std::string& name_a,
                                        const std::string& meta_table_name_a)
                        : name_(std::move(name_a)),
                          meta_table_name_(std::move(meta_table_name_a))
                    {}

                    ~LuaLibrary()
                    {}
                    std::string GetName();
                    std::string GetMetaTableName();
                    void SetName(const std::string& name_a);

                    bool inline Exists_m(const std::string& name_a)
                    {
                        return !(methods_.find(name_a) == methods_.end());
                    }

                    bool inline Exists_f(const std::string& name_a)
                    {
                        return !(functions_.find(name_a) == functions_.end());
                    }

                    void AddCMetaMethod(const std::string& name_a,
                                        lua_CFunction cfunction_a);
                   
                    void AddCMetaMethod(const std::string& name_a,
                                        lua_CFunction cfunction_a,
                                        bool replace_a);
                    
                    void AddCMethod(const std::string& name_a,
                                    lua_CFunction cfunction_a);
                    
                    void AddCMethod(const std::string& name_a,
                                    lua_CFunction cfunction_a,
                                    bool replace_a);
                    
                    void AddCFunction(const std::string& name_a,
                                      lua_CFunction cfunction_a);
                    
                    void AddCFunction(const std::string& name_a,
                                      lua_CFunction cfunction_a,
                                      bool replace_a);

                    lua_CFunction GetLibMethod(const std::string& name_a);
                    lua_CFunction GetLibFunction(const std::string& name_a);

                    int RegisterFunctions(Types::LuaState& l_a);

                protected:
                    LuaLibrary()
                    {}

                private:
                    std::map<std::string, LuaCFunction> functions_;
                    std::map<std::string, LuaCFunction> methods_;
                    std::map<std::string, LuaCFunction> meta_methods_;
                    std::string name_;
                    std::string meta_table_name_;
                };
            } // namespace Registry
        } // namespace LuaCpp
    } // namespace Aid
} // namespace CE_Kernel