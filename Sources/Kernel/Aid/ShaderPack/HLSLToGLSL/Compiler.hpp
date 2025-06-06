// Copyright (c) 2025 Case Technologies

#pragma once
#include "HTG.hpp"

#include <array>
#include <chrono>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class Compiler
            {
            public:
                using Time = std::chrono::system_clock;
                using TimePoint =
                        std::chrono::time_point<std::chrono::system_clock>;

                struct StageTimePoints
                {
                    TimePoint preprocessor_;
                    TimePoint parser_;
                    TimePoint analyzer_;
                    TimePoint optimizer_;
                    TimePoint generation_;
                    TimePoint reflection_;
                };

                Compiler(Log* log_a = nullptr);

                bool CompileShader(
                        const ShaderInput& input_desc_a,
                        const ShaderOutput& output_desc_a,
                        Reflection::ReflectionData* reflection_data_a = nullptr,
                        StageTimePoints* stage_time_points_a = nullptr);

            private:
                bool ReturnWithError(const std::string& msg_a);
                void Warning(const std::string& msg_a);

                void ValidateArguments(const ShaderInput& input_desc_a,
                                       const ShaderOutput& output_desc_a);

                bool CompileShaderPrimary(
                        const ShaderInput& input_desc_a,
                        const ShaderOutput& output_desc_a,
                        Reflection::ReflectionData* reflection_data_a);

            private:
                Log* log_ = nullptr;
                StageTimePoints time_points_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
