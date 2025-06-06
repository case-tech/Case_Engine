// Copyright (c) 2025 Case Technologies

#pragma once
#include "PythonDefs.hpp"

#include <initializer_list>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class Module;

            struct FunctionDefinition
            {
                typedef void* (*FncPtr)(void* py_self_a, void* py_args_a);

                std::string name_;
                FncPtr function_;
                std::string doc_string_;

                FunctionDefinition(const std::string& name_a,
                                   FncPtr function_a,
                                   const std::string& doc_string_a = "")
                    : name_(name_a), function_(function_a), 
                        doc_string_(doc_string_a)
                {}
            };

            class ModuleBuilder
            {
                friend class Module;

            public:
                ModuleBuilder(std::initializer_list<Types::FunctionDefinition>
                                      defs_a = {});

                ModuleBuilder& AddFunction(
                        const Types::FunctionDefinition& def_a);

                void RegisterPrintFunction(
                        FunctionDefinition::FncPtr print_func_a,
                        FunctionDefinition::FncPtr flush_func_a = nullptr);

                void Build(Module& module_a) const;

            protected:
                std::vector<Types::FunctionDefinition> function_definitions_;
            };

            struct ClassDefinition
            {
                std::string name_;
                std::string doc_string_;

                ClassDefinition(const std::string& name_a) : name_(name_a)
                {}
            };

            class TypeBuilder
            {
                friend class Module;

            public:
                TypeBuilder(std::initializer_list<Types::ClassDefinition> defs_a =
                                    {});

                TypeBuilder& AddMethod();

                void BuildInto(Module& module_a);

            protected:
                std::vector<Types::ClassDefinition> class_definitions_;
            };
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel