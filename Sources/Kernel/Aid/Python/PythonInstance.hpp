// Copyright (c) 2025 Case Technologies

#pragma once
#include "PythonDefs.hpp"
#include "PythonObject.hpp"

#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class Class;

            class Instance : public Object
            {
                friend class Class;

            public:
                Instance() = default;
                Instance(const Instance& inst_a);
                Instance(Instance&& inst_a);
                Instance(Object&& data_a);
                Instance(const Object& data_a);

                Instance& operator=(Instance&& inst_a);

                Object Call(const std::string& func_a) const;
                Object Call(const std::string& func_a, Object params_a) const;
                Object Call(const std::string& func_a,
                            const std::vector<Object>& params_a) const;
                
                Object Call(const std::string& func_a,
                            const std::initializer_list<Object>& params_a) const;

                Object GetVariable(const std::string& name_a);
                void SetVariable(const std::string& name_a, Object variable_a);

                bool Contains(const std::string& name_a) const;

            protected:
                Instance(const Class& cls_a, std::vector<Object> params_a = {});

                static Object CreateNew(const Class& cls_a,
                                        const std::vector<Object>& params_a);
            };
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel