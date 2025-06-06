#include "ReportHandler.hpp"

#include "../Helper.hpp"
#include "../SourceCode.hpp"
#include "ReportIdents.hpp"

#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            thread_local static std::vector<std::string> g_hint_queue_;

            ReportHandler::ReportHandler(Log* log_a) : log_ {log_a}
            {}

            void ReportHandler::Warning(bool break_with_expection_a,
                                        const std::string& msg_a,
                                        SourceCode* source_code_a,
                                        const SourceArea& area_a)
            {
                SubmitReport(break_with_expection_a,
                             ReportTypes::Warning,
                             R_Warning,
                             msg_a,
                             source_code_a,
                             area_a);
            }

            void ReportHandler::SubmitReport(
                    bool break_with_expection_a,
                    const ReportTypes type_a,
                    const std::string& type_name_a,
                    const std::string& msg_a,
                    SourceCode* source_code_a,
                    const SourceArea& area_a,
                    const std::vector<SourceArea>& secondary_areas_a)
            {
                if (type_a == ReportTypes::Error)
                    has_errors_ = true;

                if (!break_with_expection_a && area_a.Pos().IsValid())
                {
                    switch (type_a)
                    {
                    case ReportTypes::Warning: {
                        if (warning_positions_.find(area_a.Pos())
                            == warning_positions_.end())
                            warning_positions_.insert(area_a.Pos());
                        else
                            return;
                    }
                    break;

                    case ReportTypes::Error: {
                        if (errorPositions_.find(area_a.Pos())
                            == errorPositions_.end())
                            errorPositions_.insert(area_a.Pos());
                        else
                            return;
                    }
                    break;

                    default:
                        break;
                    }
                }

                auto output_msg_ = type_name_a;

                if (area_a.Pos().IsValid())
                {
                    output_msg_ += " (";
                    output_msg_ += area_a.Pos().ToString();
                    output_msg_ += ") ";
                } 
                else
                    output_msg_ += " ";

                output_msg_ += ": ";
                output_msg_ += msg_a;

                auto report_ = MakeReport(type_a,
                                         output_msg_,
                                         source_code_a,
                                         area_a,
                                         secondary_areas_a);

                report_.TakeHints(std::move(g_hint_queue_));

                if (break_with_expection_a)
                    throw report_;
                else if (log_)
                    log_->SubmitReport(report_);
            }

            void ReportHandler::PushContextDesc(const std::string& context_desc_a)
            {
                context_desc_stack_.push(context_desc_a);
            }

            void ReportHandler::PopContextDesc()
            {
                context_desc_stack_.pop();
            }

            void ReportHandler::HintForNextReport(const std::string& hint_a)
            {
                g_hint_queue_.push_back(hint_a);
            }

            Report ReportHandler::MakeReport(
                    const ReportTypes type_a,
                    const std::string& msg_a,
                    SourceCode* source_code_a,
                    const SourceArea& area_a,
                    const std::vector<SourceArea>& secondary_areas_a)
            {
                std::string context_desc_;
                if (!context_desc_stack_.empty())
                {
                    context_desc_ += R_In + " '";
                    context_desc_ += context_desc_stack_.top();
                    context_desc_ += "':";
                }

                if (source_code_a != nullptr && area_a.Length() > 0)
                {
                    std::string line_, marker_;
                    source_code_a->FetchLineMarker(area_a, line_, marker_);

                    for (const auto& next_area_ : secondary_areas_a)
                    {
                        if (next_area_.Pos().GetOrigin() == area_a.Pos().GetOrigin()
                            && next_area_.Pos().Row() == area_a.Pos().Row())
                        {
                            std::string next_line_, next_marker_;
                            if (source_code_a->FetchLineMarker(next_area_,
                                                            next_line_,
                                                            next_marker_))
                            {
                                MergeString(marker_, next_marker_, '^', ' ');
                            }
                        }
                    }

                    if (!line_.empty())
                        return Report {type_a, msg_a, line_, marker_, context_desc_};
                    else
                        return Report {type_a, msg_a, context_desc_};
                } else
                    return Report {type_a, msg_a, context_desc_};
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel