// Copyright (c) 2025 Case Technologies

#pragma once
#include "IndentHandler.hpp"

#include <list>
#include <ostream>
#include <stack>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class CodeWriter : public IndentHandler
            {
            public:
                struct Options
                {
                    Options() = default;

                    inline Options(bool enable_new_line_a, bool enable_indent_a)
                        : enable_new_line_ {enable_new_line_a},
                          enable_indent_ {enable_indent_a}
                    {}

                    bool enable_new_line_ = true;
                    bool enable_indent_ = true;
                };

                void OutputStream(std::ostream& stream_a);

                void PushOptions(const Options& options_a);
                void PopOptions();

                void BeginSeparation();
                void EndSeparation();

                void Separator();
                void BeginLine();
                void EndLine();

                void Write(const std::string& text_a);
                void WriteLine(const std::string& text_a);

                void BeginScope(bool compact_a = false,
                                bool end_with_semicolon_a = false,
                                bool use_braces_a = true);
                
                void EndScope();
                void ContinueScope();

                inline bool IsOpenLine() const
                {
                    return open_line_;
                }

                bool new_line_open_scope_ = false;

            private:
                struct SeparatedLine
                {
                    std::string indent_;
                    std::vector<std::string> parts_;

                    void Tab();
                    void Offsets(std::vector<std::size_t>& offsets_a) const;

                    SeparatedLine& operator<<(const std::string& text_a);
                };

                struct SeparatedLineQueue
                {
                    std::list<SeparatedLine> lines_;

                    SeparatedLine& Current();
                };

                struct ScopeState
                {
                    bool scope_can_continue_ = false;
                    bool scope_used_braces_ = false;
                    bool begin_line_queued_ = false;
                    bool end_line_queued_ = false;
                };

                struct ScopeOptions
                {
                    bool compact_;
                    bool end_with_semicolon_;
                    bool use_braces_;
                };

                Options CurrentOptions() const;

                void FlushSeparatedLines(SeparatedLineQueue& line_queue_a);

                inline std::ostream& Out()
                {
                    return (*stream_);
                }

            private:
                std::ostream* stream_ = nullptr;

                std::stack<Options> options_stack_;
                bool open_line_ = false;

                unsigned int line_separation_level_ = 0;
                SeparatedLineQueue queued_separated_lines_;

                ScopeState scope_state_;
                std::stack<ScopeOptions> scope_option_stack_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
