// Copyright (c) 2025 Case Technologies

#pragma once
#include <stdexcept>
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            enum class ReportTypes
            {
                Info,
                Warning,
                Error
            };

            class Report : public std::exception
            {
            public:
                Report(const Report&) = default;
                Report& operator=(const Report&) = default;

                inline Report(const ReportTypes type_a,
                              const std::string& message_a,
                              const std::string& context_a = "")
                    : type_ {type_a}, context_ {context_a}, message_ {message_a}
                {}

                inline Report(const ReportTypes type_a,
                              const std::string& message_a,
                              const std::string& line_a,
                              const std::string& marker_a,
                              const std::string& context_a = "")
                    : type_ {type_a}, context_ {context_a}, message_ {message_a},
                      line_ {line_a}, marker_ {marker_a}
                {
                    while (!line_.empty()
                           && (line_.back() == '\n' || line_.back() == '\r'))
                        line_.pop_back();
                }

                inline const char* what() const throw() override
                {
                    return message_.c_str();
                }

                inline void TakeHints(std::vector<std::string>&& hints_a)
                {
                    hints_ = std::move(hints_a);
                }

                inline ReportTypes Type() const
                {
                    return type_;
                }

                inline const std::string& Context() const
                {
                    return context_;
                }

                inline const std::string& Message() const
                {
                    return message_;
                }

                inline const std::string& Line() const
                {
                    return line_;
                }

                inline const std::string& Marker() const
                {
                    return marker_;
                }

                inline const std::vector<std::string>& GetHints() const
                {
                    return hints_;
                }

                inline bool HasLine() const
                {
                    return (!line_.empty());
                }

            private:
                ReportTypes type_ = ReportTypes::Info;
                std::string context_;
                std::string message_;
                std::string line_;
                std::string marker_;
                std::vector<std::string> hints_;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel