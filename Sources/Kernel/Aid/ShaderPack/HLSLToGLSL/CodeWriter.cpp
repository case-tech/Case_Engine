#include "CodeWriter.hpp"
#include "Report/ReportIdents.hpp"

#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            void CodeWriter::OutputStream(std::ostream& stream_a)
            {
                stream_ = &stream_a;
                if (!stream_->good())
                    throw std::runtime_error(R_InvalidOutputStream);
            }

            void CodeWriter::PushOptions(const Options& options_a)
            {
                options_stack_.push(options_a);
            }

            void CodeWriter::PopOptions()
            {
                if (!options_stack_.empty())
                    options_stack_.pop();
            }

            void CodeWriter::BeginSeparation()
            {
                if (line_separation_level_ > 0)
                    FlushSeparatedLines(queued_separated_lines_);
                ++line_separation_level_;
            }

            void CodeWriter::EndSeparation()
            {
                if (line_separation_level_ > 0)
                {
                    FlushSeparatedLines(queued_separated_lines_);
                    --line_separation_level_;
                }
            }

            void CodeWriter::BeginLine()
            {
                if (scope_state_.end_line_queued_)
                    EndLine();

                if (!open_line_)
                {
                    open_line_ = true;
                    scope_state_.begin_line_queued_ = false;

                    if (line_separation_level_ > 0)
                    {
                        auto& lines_ = queued_separated_lines_.lines_;
                        lines_.resize(lines_.size() + 1);
                    }

                    if (CurrentOptions().enable_indent_)
                    {
                        if (line_separation_level_ > 0)
                            queued_separated_lines_.Current().indent_ =
                                    FullIndent();
                        else
                            Out() << FullIndent();
                    }
                }
            }

            void CodeWriter::EndLine()
            {
                if (open_line_ && CurrentOptions().enable_new_line_)
                {
                    open_line_ = false;
                    scope_state_.end_line_queued_ = false;

                    if (line_separation_level_ == 0)
                        Out() << '\n';
                }
            }

            void CodeWriter::Write(const std::string& text_a)
            {
                if (scope_state_.begin_line_queued_)
                    BeginLine();

                if (line_separation_level_ > 0)
                {
                    queued_separated_lines_.Current() << text_a;
                } else
                {
                    Out() << text_a;
                }
            }

            void CodeWriter::WriteLine(const std::string& text_a)
            {
                BeginLine();
                Write(text_a);
                EndLine();
            }

            void CodeWriter::BeginScope(bool compact_a,
                                        bool end_with_semicolon_a,
                                        bool use_braces_a)
            {
                if (compact_a)
                {
                    if (use_braces_a)
                        Write(" { ");
                    else
                        Write(" ");
                } 
                else if (new_line_open_scope_)
                {
                    if (IsOpenLine())
                    {
                        EndLine();
                        if (use_braces_a)
                            WriteLine("{");
                        IncIndent();
                        BeginLine();
                    } 
                    else
                    {
                        if (use_braces_a)
                            WriteLine("{");
                        IncIndent();
                    }
                } 
                else
                {
                    if (IsOpenLine())
                    {
                        if (use_braces_a)
                            Write(" {");
                        EndLine();
                        IncIndent();
                        BeginLine();
                    } 
                    else
                    {
                        if (use_braces_a)
                            WriteLine("{");
                        IncIndent();
                    }
                }

                scope_option_stack_.push({compact_a, end_with_semicolon_a, use_braces_a});
            }

            void CodeWriter::EndScope()
            {
                auto opt_ = scope_option_stack_.top();
                scope_option_stack_.pop();

                if (opt_.compact_)
                {
                    if (opt_.use_braces_)
                        Write(" }");
                    if (opt_.end_with_semicolon_)
                        Write(";");
                } 
                else if (new_line_open_scope_)
                {
                    if (IsOpenLine())
                        EndLine();

                    DecIndent();

                    if (opt_.use_braces_ || opt_.end_with_semicolon_)
                    {
                        BeginLine();
                        {
                            if (opt_.use_braces_)
                                Write("}");
                            if (opt_.end_with_semicolon_)
                                Write(";");
                        }
                        EndLine();
                    }
                } 
                else
                {
                    if (IsOpenLine())
                        EndLine();

                    DecIndent();
                    scope_state_.begin_line_queued_ = true;

                    if (opt_.use_braces_)
                        Write("}");

                    if (opt_.end_with_semicolon_)
                    {
                        Write(";");
                        EndLine();
                    }
                    else
                    {
                        scope_state_.scope_can_continue_ = true;
                        scope_state_.end_line_queued_ = true;
                    }

                    scope_state_.scope_used_braces_ = opt_.use_braces_;
                }
            }

            void CodeWriter::ContinueScope()
            {
                if (scope_state_.scope_can_continue_)
                {
                    scope_state_.scope_can_continue_ = false;
                    scope_state_.end_line_queued_ = false;
                
                    if (scope_state_.scope_used_braces_)
                        Write(" ");
                } 
                else
                    BeginLine();
            }

            void CodeWriter::Separator()
            {
                if (line_separation_level_ > 0)
                {
                    Write("");
                    queued_separated_lines_.Current().Tab();
                }
            }

            CodeWriter::Options CodeWriter::CurrentOptions() const
            {
                return (!options_stack_.empty() ? options_stack_.top()
                                               : Options());
            }

            void CodeWriter::FlushSeparatedLines(SeparatedLineQueue& line_queue_a)
            {
                std::vector<std::size_t> offsets_;
                for (const auto& line_ : line_queue_a.lines_)
                    line_.Offsets(offsets_);

                for (const auto& line_ : line_queue_a.lines_)
                {
                    Out() << line_.indent_;

                    for (std::size_t i_ = 0; i_ < line_.parts_.size(); ++i_)
                    {
                        const auto& s_ = line_.parts_[i_];
                        Out() << s_;

                        if (i_ + 1 < line_.parts_.size())
                        {
                            static const std::size_t tab_limit_ = 50;
                            auto len_ = (offsets_[i_ + 1] - offsets_[i_] - s_.size());
                            if (len_ > 0 && len_ <= tab_limit_)
                                Out() << std::string(len_, ' ');
                        }
                    }

                    if (!line_.parts_.empty())
                        Out() << '\n';
                }

                line_queue_a.lines_.clear();
            }

            void CodeWriter::SeparatedLine::Tab()
            {
                parts_.resize(parts_.size() + 1);
            }

            void CodeWriter::SeparatedLine::Offsets(
                    std::vector<std::size_t>& offsets_a) const
            {
                offsets_a.resize(std::max(offsets_a.size(), parts_.size()));

                std::size_t shift_ = 0, i_ = 0;

                for (std::size_t pos_ = 0; i_ < parts_.size(); ++i_)
                {
                    shift_ = pos_ - offsets_a[i_];
                    offsets_a[i_] = pos_;

                    if (i_ + 1 < parts_.size())
                    {
                        pos_ = std::max(pos_ + parts_[i_].size(), offsets_a[i_ + 1]);
                    }
                }

                for (; i_ < offsets_a.size(); ++i_)
                    offsets_a[i_] += shift_;
            }

            CodeWriter::SeparatedLine& CodeWriter::SeparatedLine::operator<<(
                    const std::string& text_a)
            {
                if (parts_.empty())
                    Tab();
               
                parts_.back() += text_a;
                return *this;
            }

            CodeWriter::SeparatedLine& CodeWriter::SeparatedLineQueue::Current()
            {
                if (lines_.empty())
                    lines_.resize(lines_.size() + 1);
              
                return lines_.back();
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
