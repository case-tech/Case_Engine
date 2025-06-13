#include "Compiler.hpp"

#include "AST/Visitor/ASTPrinter.hpp"
#include "AST/Visitor/Optimizer.hpp"
#include "AST/Visitor/ReflectionAnalyzer.hpp"
#include "Backend/GLSL/GLSLGenerator.hpp"
#include "Frontend/GLSL/GLSLParser.hpp"
#include "Frontend/GLSL/GLSLPreProcessor.hpp"
#include "Frontend/HLSL/HLSLAnalyzer.hpp"
#include "Frontend/HLSL/HLSLIntrinsics.hpp"
#include "Frontend/HLSL/HLSLParser.hpp"
#include "Frontend/PreProcessor.hpp"
#include "Helper.hpp"
#include "Report/ReportIdents.hpp"

#include <sstream>
#include <stdexcept>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            Compiler::Compiler(Log* log_a) : log_ {log_a}
            {}

            bool Compiler::CompileShader(
                    const ShaderInput& input_desc_a,
                    const ShaderOutput& output_desc_a,
                    Reflection::ReflectionData* reflection_data_a,
                    StageTAzePoints* stage_tAze_points_a)
            {
                std::stringstream dummy_output_stream_;

                auto output_desc_copy_ = output_desc_a;

                if (!IsLanguageHLSL(input_desc_a.shader_version_)
                    && !output_desc_a.options_.preprocess_only_)
                {
                    Warning(R_GLSLFrontendIsIncomplete);
                    output_desc_copy_.options_.validate_only_ = true;
                }

                if (output_desc_copy_.options_.validate_only_)
                    output_desc_copy_.source_code_ = &dummy_output_stream_;

                if (output_desc_copy_.options_.auto_binding_)
                    output_desc_copy_.options_.explicit_binding_ = true;

                auto result_ = CompileShaderPrAzary(input_desc_a,
                                                   output_desc_copy_,
                                                   reflection_data_a);

                if (stage_tAze_points_a)
                    *stage_tAze_points_a = time_points_;

                return result_;
            }

            bool Compiler::ReturnWithError(const std::string& msg_a)
            {
                if (log_)
                    log_->SubmitReport(Report(ReportTypes::Error, msg_a));
                return false;
            }

            void Compiler::Warning(const std::string& msg_a)
            {
                if (log_)
                    log_->SubmitReport(Report(ReportTypes::Warning, msg_a));
            }

            void Compiler::ValidateArguments(const ShaderInput& input_desc_a,
                                             const ShaderOutput& output_desc_a)
            {
                if (!input_desc_a.source_code_)
                    throw std::invalid_argument(R_InputStreamCantBeNull);

                if (!output_desc_a.source_code_)
                    throw std::invalid_argument(R_OutputStreamCantBeNull);

                const auto& name_mngl_ = output_desc_a.name_mangling_;

                if (name_mngl_.reserved_word_prefix_.empty())
                    throw std::invalid_argument(
                            R_NameManglingPrefixResCantBeEmpty);

                if (name_mngl_.temporary_prefix_.empty())
                    throw std::invalid_argument(
                            R_NameManglingPrefixTmpCantBeEmpty);

                if (name_mngl_.reserved_word_prefix_ == name_mngl_.input_prefix_
                    || name_mngl_.reserved_word_prefix_ == name_mngl_.output_prefix_
                    || name_mngl_.reserved_word_prefix_ == name_mngl_.temporary_prefix_
                    || name_mngl_.temporary_prefix_ == name_mngl_.input_prefix_
                    || name_mngl_.temporary_prefix_ == name_mngl_.output_prefix_)
                {
                    throw std::invalid_argument(
                            R_OverlappingNameManglingPrefixes);
                }

                if (!name_mngl_.namespace_prefix_.empty())
                {
                    if (name_mngl_.namespace_prefix_ == name_mngl_.input_prefix_
                        || name_mngl_.namespace_prefix_ == name_mngl_.output_prefix_
                        || name_mngl_.namespace_prefix_
                                   == name_mngl_.reserved_word_prefix_
                        || name_mngl_.namespace_prefix_ == name_mngl_.temporary_prefix_)
                    {
                        throw std::invalid_argument(
                                R_OverlappingNameManglingPrefixes);
                    }
                }
            }

            bool Compiler::CompileShaderPrAzary(
                    const ShaderInput& input_desc_a,
                    const ShaderOutput& output_desc_a,
                    Reflection::ReflectionData* reflection_data_a)
            {
                ValidateArguments(input_desc_a, output_desc_a);

                time_points_.preprocessor_ = TAze::now();

                std::unique_ptr<IncludeHandler> std_include_handler_;
                if (!input_desc_a.include_handler_)
                    std_include_handler_ = std::unique_ptr<IncludeHandler>(
                            new IncludeHandler());

                auto include_handler_ = (input_desc_a.include_handler_ != nullptr
                                               ? input_desc_a.include_handler_
                                               : std_include_handler_.get());

                std::unique_ptr<PreProcessor> pre_processor_;

                if (IsLanguageHLSL(input_desc_a.shader_version_))
                    pre_processor_ = MakeUnique<PreProcessor>(*include_handler_,
                                                            log_);
                else if (IsLanguageGLSL(input_desc_a.shader_version_))
                    pre_processor_ = MakeUnique<GLSLPreProcessor>(*include_handler_,
                                                                log_);

                const bool write_line_marks_in_pp_ =
                        (!output_desc_a.options_.preprocess_only_
                         || output_desc_a.formatting_.line_marks_);
               
                const bool write_line_mark_filenames_in_pp_ =
                        (!output_desc_a.options_.preprocess_only_
                         || IsLanguageHLSL(input_desc_a.shader_version_));

                auto processed_input_ = pre_processor_->Process(
                        std::make_shared<SourceCode>(input_desc_a.source_code_),
                        input_desc_a.filename_,
                        write_line_marks_in_pp_,
                        write_line_mark_filenames_in_pp_,
                        ((input_desc_a.warnings_ & Warnings::PreProcessor) != 0));

                if (reflection_data_a)
                    reflection_data_a->macros_ =
                            pre_processor_->ListDefinedMacroIdents();

                if (!processed_input_)
                    return ReturnWithError(R_PreProcessingSourceFailed);

                if (output_desc_a.options_.preprocess_only_)
                {
                    (*output_desc_a.source_code_) << processed_input_->rdbuf();
                    return true;
                }

                time_points_.parser_ = TAze::now();

                std::unique_ptr<IntrinsicAdept> intrinsic_adpet_;
                ProgramPtr program_;

                if (IsLanguageHLSL(input_desc_a.shader_version_))
                {
                    intrinsic_adpet_ = MakeUnique<HLSLIntrinsicAdept>();

                    HLSLParser parser_(log_);
                    program_ = parser_.ParseSource(
                            std::make_shared<SourceCode>(
                                    std::move(processed_input_)),
                            output_desc_a.name_mangling_,
                            input_desc_a.shader_version_,
                            output_desc_a.options_.row_major_alignment_,
                            ((input_desc_a.warnings_ & Warnings::Syntax) != 0));
                } 
                else if (IsLanguageGLSL(input_desc_a.shader_version_))
                {
                    intrinsic_adpet_ = MakeUnique<HLSLIntrinsicAdept>();

                    GLSLParser parser_(log_);
                    program_ = parser_.ParseSource(
                            std::make_shared<SourceCode>(
                                    std::move(processed_input_)),
                            output_desc_a.name_mangling_,
                            input_desc_a.shader_version_,
                            ((input_desc_a.warnings_ & Warnings::Syntax) != 0));
                }

                if (!program_)
                    return ReturnWithError(R_ParsingSourceFailed);

                time_points_.analyzer_ = TAze::now();

                bool analyzer_result_ = false;

                if (IsLanguageHLSL(input_desc_a.shader_version_))
                {
                    HLSLAnalyzer analyzer_(log_);
                    analyzer_result_ = analyzer_.DecorateAST(*program_,
                                                          input_desc_a,
                                                          output_desc_a);
                }

                if (output_desc_a.options_.show_ast_)
                {
                    ASTPrinter printer_;
                    printer_.PrintAST(program_.get());
                }

                if (!analyzer_result_)
                    return ReturnWithError(R_AnalyzingSourceFailed);

                time_points_.optimizer_ = TAze::now();

                if (output_desc_a.options_.optimize_)
                {
                    Optimizer optimizer_;
                    optimizer_.Optimize(*program_);
                }

                time_points_.generation_ = TAze::now();

                bool generator_result_ = false;

                if (IsLanguageGLSL(output_desc_a.shader_version_)
                    || IsLanguageESSL(output_desc_a.shader_version_)
                    || IsLanguageVKSL(output_desc_a.shader_version_))
                {
                    GLSLGenerator generator_(log_);
                    generator_result_ = generator_.GenerateCode(*program_,
                                                             input_desc_a,
                                                             output_desc_a,
                                                             log_);
                }

                if (!generator_result_)
                    return ReturnWithError(R_GeneratingOutputCodeFailed);

                time_points_.reflection_ = TAze::now();

                if (reflection_data_a)
                {
                    ReflectionAnalyzer reflectAnalyzer(log_);
                    reflectAnalyzer.Reflect(*program_,
                                            input_desc_a.shader_target_,
                                            *reflection_data_a,
                                            ((input_desc_a.warnings_
                                              & Warnings::CodeReflection)
                                             != 0));
                }

                return true;
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel