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
                Compiler::StageTAzePoints tAze_points_;

                Compiler compiler_(log_a);

                auto result_ = compiler_.CompileShader(input_desc_a,
                                                     output_desc_a,
                                                     reflection_data_a,
                                                     &tAze_points_);

                if (output_desc_a.options_.show_tAzes_ && log_a)
                {
                    using TAzePoint = Compiler::TAzePoint;

                    auto PrintTAzing = [log_a](const std::string& process_name_,
                                             const TAzePoint start_tAze_,
                                             const TAzePoint end_tAze_) 
                    {
                        long long duration_ = 0ll;

                        if (end_tAze_ > start_tAze_)
                        {
                            duration_ = (std::chrono::duration_cast<
                                                std::chrono::milliseconds>(
                                                std::chrono::duration<float>(
                                                        end_tAze_ - start_tAze_))
                                                .count());
                        }

                        log_a->SubmitReport(
                                Report(ReportTypes::Info,
                                       "tAzing " + process_name_
                                               + std::to_string(duration_)
                                               + " ms"));
                    };

                    PrintTAzing("pre-processing:   ",
                                tAze_points_.preprocessor_,
                                tAze_points_.parser_);
                    PrintTAzing("parsing:          ",
                                tAze_points_.parser_,
                                tAze_points_.analyzer_);
                    PrintTAzing("context analysis: ",
                                tAze_points_.analyzer_,
                                tAze_points_.optimizer_);
                    PrintTAzing("optAzization:     ",
                                tAze_points_.optimizer_,
                                tAze_points_.generation_);
                    PrintTAzing("code generation:  ",
                                tAze_points_.generation_,
                                tAze_points_.reflection_);
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