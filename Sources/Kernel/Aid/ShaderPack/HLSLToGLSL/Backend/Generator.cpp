#include "Generator.hpp"

#include "../AST/AST.hpp"
#include "../Report/ReportIdents.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            Generator::Generator(Log* log_a) : report_handler_ {log_a}
            {}

            bool Generator::GenerateCode(Program& program_a,
                                         const ShaderInput& input_desc_a,
                                         const ShaderOutput& output_desc_a,
                                         Log* log_a)
            {
                writer_.SetIndent(output_desc_a.formatting_.indent_);

                shader_target_ = input_desc_a.shader_target_;
                warnings_ = input_desc_a.warnings_;
                allow_blanks_ = output_desc_a.formatting_.blanks_;
                allow_line_separation_ = output_desc_a.formatting_.line_separation_;
                writer_.new_line_open_scope_ =
                        output_desc_a.formatting_.new_line_open_scope_;
                
                program_ = &program_a;

                try
                {
                    writer_.OutputStream(*output_desc_a.source_code_);
                    GenerateCodePrimary(program_a, input_desc_a, output_desc_a);
                } 
                catch (const Report& err_)
                {
                    if (log_a)
                        log_a->SubmitReport(err_);
                    return false;
                }

                return (!report_handler_.HasErrors());
            }

            void Generator::Error(const std::string& msg_a,
                                  const AST* ast_a,
                                  bool break_with_expection_a)
            {
                report_handler_.SubmitReport(break_with_expection_a,
                                            ReportTypes::Error,
                                            R_CodeGenerationError,
                                            msg_a,
                                            program_->source_code_.get(),
                                            (ast_a ? ast_a->area_
                                                 : SourceArea::ignore));
            }

            void Generator::Warning(const std::string& msg_a, const AST* ast_a)
            {
                report_handler_.Warning(false,
                                       msg_a,
                                       program_->source_code_.get(),
                                       (ast_a ? ast_a->area_ : SourceArea::ignore));
            }

            void Generator::BeginLn()
            {
                writer_.BeginLine();
            }

            void Generator::EndLn()
            {
                writer_.EndLine();
            }

            void Generator::BeginSep()
            {
                if (allow_line_separation_)
                    writer_.BeginSeparation();
            }

            void Generator::EndSep()
            {
                if (allow_line_separation_)
                    writer_.EndSeparation();
            }

            void Generator::Separator()
            {
                writer_.Separator();
            }

            void Generator::WriteScopeOpen(bool compact_a,
                                           bool end_with_semicolon_a,
                                           bool use_braces_a)
            {
                writer_.BeginScope(compact_a, end_with_semicolon_a, use_braces_a);
            }

            void Generator::WriteScopeClose()
            {
                writer_.EndScope();
            }

            void Generator::WriteScopeContinue()
            {
                writer_.ContinueScope();
            }

            bool Generator::IsOpenLine() const
            {
                return writer_.IsOpenLine();
            }

            void Generator::Write(const std::string& text_a)
            {
                FlushWritePrefixes();
                writer_.Write(text_a);
            }

            void Generator::WriteLn(const std::string& text_a)
            {
                FlushWritePrefixes();
                writer_.WriteLine(text_a);
            }

            void Generator::IncIndent()
            {
                writer_.IncIndent();
            }

            void Generator::DecIndent()
            {
                writer_.DecIndent();
            }

            void Generator::PushOptions(const CodeWriter::Options& options_a)
            {
                writer_.PushOptions(options_a);
            }

            void Generator::PopOptions()
            {
                writer_.PopOptions();
            }

            void Generator::PushWritePrefix(const std::string& text_a)
            {
                write_prefix_stack_.push_back({text_a, false});
            }

            void Generator::PopWritePrefix(const std::string& text_a)
            {
                if (write_prefix_stack_.empty())
                    throw std::underflow_error(R_WritePrefixStackUnderflow);
                else
                {
                    if (!text_a.empty() && TopWritePrefix())
                        Write(text_a);
                    write_prefix_stack_.pop_back();
                }
            }

            bool Generator::TopWritePrefix() const
            {
                return (write_prefix_stack_.empty()
                                ? false
                                : write_prefix_stack_.back().written_);
            }

            void Generator::FlushWritePrefixes()
            {
                for (auto& prefix_ : write_prefix_stack_)
                {
                    if (!prefix_.written_)
                    {
                        writer_.Write(prefix_.text_);
                        prefix_.written_ = true;
                    }
                }
            }

            void Generator::Blank()
            {
                if (allow_blanks_)
                    WriteLn("");
            }

            std::string Generator::TimePoint() const
            {
                auto current_time_ = std::chrono::system_clock::now();
                auto date_ = std::chrono::system_clock::to_time_t(current_time_);

                std::stringstream s_;
                s_ << std::put_time(std::localtime(&date_), "%d/%m/%Y %H:%M:%S");

                return s_.str();
            }

            bool Generator::WarnEnabled(unsigned int flags_a) const
            {
                return warnings_(flags_a);
            }

            bool Generator::IsVertexShader() const
            {
                return (shader_target_ == ShaderTarget::VertexShader);
            }

            bool Generator::IsTessControlShader() const
            {
                return (shader_target_
                        == ShaderTarget::TessellationControlShader);
            }

            bool Generator::IsTessEvaluationShader() const
            {
                return (shader_target_
                        == ShaderTarget::TessellationEvaluationShader);
            }

            bool Generator::IsGeometryShader() const
            {
                return (shader_target_ == ShaderTarget::GeometryShader);
            }

            bool Generator::IsFragmentShader() const
            {
                return (shader_target_ == ShaderTarget::FragmentShader);
            }

            bool Generator::IsComputeShader() const
            {
                return (shader_target_ == ShaderTarget::ComputeShader);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
