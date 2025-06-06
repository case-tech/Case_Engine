#include "PythonBuilders.hpp"

#include "PythonCppCommon.hpp"
#include "PythonModule.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            void* defaultFlushFunc(void*, void*)
            {
                return Object::None();
            }

            ModuleBuilder::ModuleBuilder(
                    std::initializer_list<Types::FunctionDefinition> defs_a)
                : function_definitions_(defs_a)
            {}

            ModuleBuilder& ModuleBuilder::AddFunction(
                    const Types::FunctionDefinition& def_a)
            {
                this->function_definitions_.push_back(def_a);
                return *this;
            }

            void ModuleBuilder::RegisterPrintFunction(
                    FunctionDefinition::FncPtr print_func_a,
                    FunctionDefinition::FncPtr flush_func_a)
            {
                this->AddFunction(
                        {"write", print_func_a, default_function_flag_, ""});
                
                this->AddFunction({"flush",
                                   flush_func_a ? flush_func_a : &defaultFlushFunc,
                                   default_function_flag_,
                                   ""});
            }

            void ModuleBuilder::Build(Module& module_a) const
            {
                module_a.AddFunctions(this->function_definitions_);
            }

            TypeBuilder::TypeBuilder(
                    std::initializer_list<Types::ClassDefinition> defs_a)
                : class_definitions_(defs_a)
            {}

            TypeBuilder& TypeBuilder::AddMethod()
            {
                return *this;
            }

            void TypeBuilder::BuildInto(Module& module_a)
            {
                module_a.AddClasses(this->class_definitions_);
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel