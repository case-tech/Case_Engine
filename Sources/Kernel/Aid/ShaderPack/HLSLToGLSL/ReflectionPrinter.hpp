// Copyright (c) 2025 Case Technologies

#pragma once
#include "IndentHandler.hpp"
#include "Reflection.hpp"

#include <ostream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class ReflectionPrinter
            {
            public:
                ReflectionPrinter(std::ostream& output_a);

                void PrintReflection(
                        const Reflection::ReflectionData& reflection_data_a,
                        bool referenced_only_a = false);

            private:
                std::ostream& IndentOut();

                void PrintReflectionObjects(
                        const std::vector<std::string>& idents_a,
                        const char* title_a);
               
                void PrintFields(const std::vector<Reflection::Field>& objects_a,
                                 bool referenced_only_a);
                
                void PrintReflectionObjects(
                        const std::vector<Reflection::Record>& objects_a,
                        const char* title_a,
                        bool referenced_only_a);
                
                void PrintReflectionObjects(
                        const std::vector<Reflection::Attribute>& objects_a,
                        const char* title_a,
                        bool referenced_only_a);
                
                void PrintReflectionObjects(
                        const std::vector<Reflection::Resource>& objects_a,
                        const char* title_a,
                        bool referenced_only_a);
                
                void PrintReflectionObjects(
                        const std::vector<Reflection::ConstantBuffer>& objects_a,
                        const char* title_a,
                        bool referenced_only_a);
                
                void PrintReflectionObjects(
                        const std::vector<Reflection::SamplerState>& objects_a,
                        const char* title_a,
                        bool referenced_only_a);
                
                void PrintReflectionObjects(
                        const std::vector<Reflection::StaticSamplerState>&
                                sampler_states_a,
                        const char* title_a);
                
                void PrintReflectionAttribute(
                        const Reflection::NumThreads& num_threads_a,
                        const char* title_a);

            private:
                std::ostream& output_;
                IndentHandler indent_handler_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel