#include "Log.hpp"

#include "ConsoleManip.hpp"

#include <algorithm>
#include <iostream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            struct IndentReport
            {
                std::string indent_;
                Report report_;
            };

            using IndentReportList = std::vector<IndentReport>;

            static void PrintMultiLineString(const std::string& s_a,
                                             const std::string& indent_a)
            {
                auto text_start_pos_ = s_a.find(" : ");

                if (text_start_pos_ != std::string::npos)
                    text_start_pos_ += 3;
                else
                    text_start_pos_ = 0;

                std::string new_line_indent_(text_start_pos_, ' ');

                std::size_t start_ = 0;
                bool use_new_line_indent_ = false;

                while (start_ < s_a.size())
                {
                    std::cout << indent_a;

                    if (use_new_line_indent_)
                        std::cout << new_line_indent_;

                    auto end = s_a.find('\n', start_);

                    if (end != std::string::npos)
                    {
                        std::cout << s_a.substr(start_, end - start_);
                        start_ = end + 1;
                    } 
                    else
                    {
                        std::cout << s_a.substr(start_);
                        start_ = end;
                    }

                    std::cout << std::endl;

                    use_new_line_indent_ = true;
                }
            }

            using Colors = ConsoleManip::ColorFlags;

            static void PrintReport(const IndentReport& r_a, bool verbose_a)
            {
                if (verbose_a && !r_a.report_.Context().empty())
                    PrintMultiLineString(r_a.report_.Context(), r_a.indent_);

                auto type_ = r_a.report_.Type();
                const auto& msg_ = r_a.report_.Message();

                if (type_ == ReportTypes::Error)
                {
                    ConsoleManip::ScopedColor highlight_(Colors::Red
                                                        | Colors::Intens);
                    PrintMultiLineString(msg_, r_a.indent_);
                } 
                else if (type_ == ReportTypes::Warning)
                {
                    ConsoleManip::ScopedColor highlight_(Colors::Yellow);
                    PrintMultiLineString(msg_, r_a.indent_);
                } 
                else
                    PrintMultiLineString(msg_, r_a.indent_);

                if (!verbose_a)
                    return;

                if (r_a.report_.HasLine())
                {
                    const auto& line_ = r_a.report_.Line();
                    const auto& mark_ = r_a.report_.Marker();

                    {
                        ConsoleManip::ScopedColor highlight_(Colors::Green
                                                            | Colors::Blue);

                        std::cout << r_a.indent_;

                        std::size_t start_ = 0, end_ = 0;

                        while (end_ < mark_.size()
                               && (start_ = mark_.find_first_not_of(' ', end_))
                                          != std::string::npos)
                        {
                            std::cout << line_.substr(end_, start_ - end_);

                            {
                                ConsoleManip::ScopedColor highlight1_(
                                        Colors::Cyan);

                                end_ = mark_.find(' ', start_);

                                if (end_ == std::string::npos)
                                    end_ = std::min(line_.size(), mark_.size());

                                std::cout << line_.substr(start_, end_ - start_);
                            }
                        }

                        if (end_ < line_.size())
                            std::cout << line_.substr(end_);

                        std::cout << std::endl;
                    }

                    if (!mark_.empty())
                    {
                        ConsoleManip::ScopedColor highlight_(Colors::Cyan);
                        std::cout << r_a.indent_ << mark_ << std::endl;
                    }
                }

                for (const auto& hint_ : r_a.report_.GetHints())
                    std::cout << r_a.indent_ << hint_ << std::endl;
            }

            static void PrintAndClearReports(IndentReportList& reports_a,
                                             bool verbose_a,
                                             const std::string& headline_a = "")
            {
                if (!reports_a.empty())
                {
                    if (!headline_a.empty())
                    {
                        auto s_ = std::to_string(reports_a.size()) + " "
                                 + headline_a;
                        std::cout << s_ << std::endl;
                        std::cout << std::string(s_.size(), '-') << std::endl;
                    }

                    for (const auto& r_ : reports_a)
                        PrintReport(r_, verbose_a);

                    reports_a.clear();
                }
            }

            struct StdLog::OpaqueData
            {
                IndentReportList infos_;
                IndentReportList warnings_;
                IndentReportList errors_;
            };

            StdLog::StdLog() : data_ {new OpaqueData()}
            {}

            StdLog::~StdLog()
            {
                delete data_;
            }

            void StdLog::SubmitReport(const Report& report_)
            {
                switch (report_.Type())
                {
                case ReportTypes::Info:
                    data_->infos_.push_back({FullIndent(), report_});
                    break;
                case ReportTypes::Warning:
                    data_->warnings_.push_back({FullIndent(), report_});
                    break;
                case ReportTypes::Error:
                    data_->errors_.push_back({FullIndent(), report_});
                    break;
                }
            }

            void StdLog::PrintAll(bool verbose_a)
            {
                PrintAndClearReports(data_->infos_, verbose_a);
                PrintAndClearReports(data_->warnings_,
                                     verbose_a,
                                     (data_->warnings_.size() == 1
                                              ? "WARNING"
                                              : "WARNINGS"));
                PrintAndClearReports(data_->errors_,
                                     verbose_a,
                                     (data_->errors_.size() == 1 ? "ERROR"
                                                                 : "ERRORS"));
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
