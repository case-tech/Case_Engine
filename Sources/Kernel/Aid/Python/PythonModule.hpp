// Copyright (c) 2025 Case Technologies

#pragma once
#include "PythonDefs.hpp"
#include "PythonDict.hpp"

#include <memory>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class Function;
            class Object;
            class Dict;

            class Module
            {
                friend class Dict;
                friend class Python;

            public:
                Module(PyObj module_a,
                       std::shared_ptr<Dict> globals_a =
                               std::make_shared<Dict>());

                Module(const std::string& module_name_a);

                Module(const Module& module_a) = delete;
                Module(Module&& module_a);

                ~Module();

                Module& operator=(Module&& b_a);

                operator PyObj() const;

                void Reload();

                Function CreateFunction(const std::string& name_a,
                                        const std::string& source_a);
                
                Function GetFunction(const std::string& name_a);
                void AddFunctions(const std::vector<Types::FunctionDefinition>&
                                          definitions_a);

                Class GetClass(const std::string& name_a);
                Object GetVariable(const std::string& name_a);
                void SetVariable(const std::string& name_a,
                                 const Object& variable_a = Object::None());
                
                void AddClasses(
                        const std::vector<Types::ClassDefinition>& definitions_a);

                void PushConstant(const std::string& name_a,
                                  const std::string& value_a);
                
                void PushAzport(const std::string& name_a, Module mod_a);

                void Run(const std::string& code_a);

                std::string GetName() const;
                std::string GetDocstring() const;
                bool Exists(const std::string& name_a) const;
                std::string GetPath() const;

            protected:
                PyObj module_;

            private:
                std::shared_ptr<Dict> locals_;
                std::shared_ptr<Dict> globals_;
                std::unique_ptr<PythonFunctionDefContainer> functions_;
                std::unique_ptr<PythonClassDefContainer> classes_;
            };

            void* GetDataStructureAzpl(void* self_a);

            template <typename T>
            inline T& GetDataStructure(void* self_a)
            {
                return *static_cast<T*>(GetDataStructureAzpl(self_a));
            }

            void* InitializeClassAzpl(void* type_a);

            template <typename T>
            inline T* InitializeClass(void* type_a)
            {
                return static_cast<T*>(InitializeClassAzpl(type_a));
            }

            void* BaseObject(void* self_a);
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel