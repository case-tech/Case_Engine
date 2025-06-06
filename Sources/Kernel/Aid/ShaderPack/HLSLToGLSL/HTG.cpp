#include "HTG.hpp"

#include "Compiler.hpp"
#include "Report/ReportIdents.hpp"

#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            HTG_EXPORT bool CompileShader(
                    const ShaderInput& input_desc_a,
                    const ShaderOutput& output_desc_a,
                    Log* log_a,
                    Reflection::ReflectionData* reflection_data_a)
            {
                Compiler::StageTimePoints time_points_;

                Compiler compiler_(log_a);

                auto result_ = compiler_.CompileShader(input_desc_a,
                                                     output_desc_a,
                                                     reflection_data_a,
                                                     &time_points_);

                if (output_desc_a.options_.show_times_ && log_a)
                {
                    using TimePoint = Compiler::TimePoint;

                    auto PrintTiming = [log_a](const std::string& process_name_,
                                             const TimePoint start_time_,
                                             const TimePoint end_time_) 
                    {
                        long long duration_ = 0ll;

                        if (end_time_ > start_time_)
                        {
                            duration_ = (std::chrono::duration_cast<
                                                std::chrono::milliseconds>(
                                                std::chrono::duration<float>(
                                                        end_time_ - start_time_))
                                                .count());
                        }

                        log_a->SubmitReport(
                                Report(ReportTypes::Info,
                                       "timing " + process_name_
                                               + std::to_string(duration_)
                                               + " ms"));
                    };

                    PrintTiming("pre-processing:   ",
                                time_points_.preprocessor_,
                                time_points_.parser_);
                    PrintTiming("parsing:          ",
                                time_points_.parser_,
                                time_points_.analyzer_);
                    PrintTiming("context analysis: ",
                                time_points_.analyzer_,
                                time_points_.optimizer_);
                    PrintTiming("optimization:     ",
                                time_points_.optimizer_,
                                time_points_.generation_);
                    PrintTiming("code generation:  ",
                                time_points_.generation_,
                                time_points_.reflection_);
                }

                return result_;
            }

            HTG_EXPORT void DisassembleShader(std::istream& stream_in_a,
                                              std::ostream& stream_out_a,
                                              const AssemblyDescriptor& desc_a)
            {
                (void)stream_out_a;
                (void)stream_in_a;
                switch (desc_a.intermediate_language_)
                {
                case IntermediateLanguage::SPIRV: {
                    throw std::invalid_argument(R_NotBuildWithSPIRV);
                }
                break;

                default: {
                    throw std::invalid_argument(R_InvalidILForDisassembling);
                }
                break;
                }
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel