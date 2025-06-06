// Copyright (c) 2025 Case Technologies

#pragma once
#include "../AST/Visitor/VisitorTracker.hpp"
#include "../CodeWriter.hpp"
#include "../Flags.hpp"
#include "../HTG.hpp"
#include "../Report/ReportHandler.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class Generator : protected VisitorTracker
            {
            public:
                Generator(Log* log_a);

                bool GenerateCode(Program& program_a,
                                  const ShaderInput& input_desc_a,
                                  const ShaderOutput& output_desc_a,
                                  Log* log_a = nullptr);

            protected:
                virtual void GenerateCodePrimary(
                        Program& program_a,
                        const ShaderInput& input_desc_a,
                        const ShaderOutput& output_desc_a) = 0;

                void Error(const std::string& msg_a,
                           const AST* ast_a = nullptr,
                           bool break_with_expection_a = true);
                
                void Warning(const std::string& msg_a, const AST* ast_a = nullptr);

                void BeginLn();
                void EndLn();

                void BeginSep();
                void EndSep();

                void Separator();

                void WriteScopeOpen(bool compact_a = false,
                                    bool end_with_semicolon_a = false,
                                    bool use_braces_a = true);
               
                void WriteScopeClose();
                void WriteScopeContinue();

                bool IsOpenLine() const;

                void Write(const std::string& text_a);
                void WriteLn(const std::string& text_a);

                void IncIndent();
                void DecIndent();

                void PushOptions(const CodeWriter::Options& options_a);
                void PopOptions();

                void PushWritePrefix(const std::string& text_a);
                void PopWritePrefix(const std::string& text_a = "");

                bool TopWritePrefix() const;

                void Blank();

                std::string TimePoint() const;

                inline Program* GetProgram() const
                {
                    return program_;
                }

                inline ShaderTarget GetShaderTarget() const
                {
                    return shader_target_;
                }

                bool WarnEnabled(unsigned int flags_a) const;

                bool IsVertexShader() const;
                bool IsTessControlShader() const;
                bool IsTessEvaluationShader() const;
                bool IsGeometryShader() const;
                bool IsFragmentShader() const;
                bool IsComputeShader() const;

            private:
                struct WritePrefix
                {
                    std::string text_;
                    bool written_;
                };

                void FlushWritePrefixes();

            private:
                CodeWriter writer_;
                ReportHandler report_handler_;

                Program* program_ = nullptr;

                ShaderTarget shader_target_ = ShaderTarget::VertexShader;
                Flags warnings_;

                bool allow_blanks_ = true;
                bool allow_line_separation_ = true;

                std::vector<WritePrefix> write_prefix_stack_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
