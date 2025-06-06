// Copyright (c) 2025 Case Technologies

#pragma once
#include "../AST/SourceArea.hpp"
#include "../Log.hpp"
#include "../Report.hpp"

#include <functional>
#include <set>
#include <stack>
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class SourceCode;

            using OnReportProc =
                    std::function<void(const std::string& msg_, const AST* ast_)>;

            class ReportHandler
            {
            public:
                ReportHandler(Log* log_a);

                void Warning(bool break_with_expection_a,
                             const std::string& msg_a,
                             SourceCode* source_code_a = nullptr,
                             const SourceArea& area_a = SourceArea::ignore);

                void SubmitReport(
                        bool break_with_expection_a,
                        const ReportTypes type_a,
                        const std::string& type_name_a,
                        const std::string& msg_a,
                        SourceCode* source_code_a = nullptr,
                        const SourceArea& area_a = SourceArea::ignore,
                        const std::vector<SourceArea>& secondary_areas_a = {});

                inline bool HasErrors() const
                {
                    return has_errors_;
                }

                void PushContextDesc(const std::string& context_desc_a);
                void PopContextDesc();

                static void HintForNextReport(const std::string& hint_a);

            private:
                Report MakeReport(
                        const ReportTypes type_a,
                        const std::string& msg_a,
                        SourceCode* source_code_a,
                        const SourceArea& area_a,
                        const std::vector<SourceArea>& secondary_areas_a);

            private:
                Log* log_ = nullptr;
                bool has_errors_ = false;

                std::stack<std::string> context_desc_stack_;
                std::set<SourcePosition> errorPositions_;
                std::set<SourcePosition> warning_positions_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel