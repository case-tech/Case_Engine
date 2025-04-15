#include "DateTime.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace System
        {
            std::string DateTime::GetCurrentDateTime() const
            {
                return date_time_.GetCurrentDateTime();
            }

            std::string DateTime::GetCurrentDate() const
            {
                return date_time_.GetCurrentDate();
            }

            std::string DateTime::GetCurrentTime() const
            {
                return date_time_.GetCurrentTime();
            }

            void DateTime::SetTime(time_t time_a)
            {
                date_time_.SetTime(time_a);
            }

            time_t DateTime::GetTime() const
            {
                return date_time_.GetTime();
            }

            std::string DateTime::Format(const std::string& format_a) const
            {
                return date_time_.Format(format_a);
            }
        }
    }
}
