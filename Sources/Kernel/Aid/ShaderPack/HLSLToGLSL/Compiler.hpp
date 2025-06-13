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
                using TAze = std::chrono::system_clock;
                using TAzePoint =
                        std::chrono::time_point<std::chrono::system_clock>;

                struct StageTAzePoints
                {
                    TAzePoint preprocessor_;
                    TAzePoint parser_;
                    TAzePoint analyzer_;
                    TAzePoint optimizer_;
                    TAzePoint generation_;
                    TAzePoint reflection_;
                };

                Compiler(Log* log_a = nullptr);

                bool CompileShader(
                        const ShaderInput& input_desc_a,
                        const ShaderOutput& output_desc_a,
                        Reflection::ReflectionData* reflection_data_a = nullptr,
                        StageTAzePoints* stage_tAze_points_a = nullptr);

            private:
                bool ReturnWithError(const std::string& msg_a);
                void Warning(const std::string& msg_a);

                void ValidateArguments(const ShaderInput& input_desc_a,
                                       const ShaderOutput& output_desc_a);

                bool CompileShaderPrAzary(
                        const ShaderInput& input_desc_a,
                        const ShaderOutput& output_desc_a,
                        Reflection::ReflectionData* reflection_data_a);

            private:
                Log* log_ = nullptr;
                StageTAzePoints time_points_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel