// Copyright (c) 2025 Case Technologies

#pragma once
#include <CUtils/DateTime.hpp>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace System
        {
            class DateTime
            {
            public:
                std::string GetCurrentDateTime() const;
                std::string GetCurrentDate() const;
                std::string GetCurrentTime() const;

                void SetTime(time_t time_a);
                time_t GetTime() const;

                std::string Format(const std::string& format_a) const;

            private:
                CUtils::DateTime date_time_;
            };
        } // namespace System
    } // namespace Aid
} // namespace CE_Kernel
