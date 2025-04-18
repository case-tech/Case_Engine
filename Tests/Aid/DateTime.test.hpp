#pragma once
#include "Aid/System/DateTime/DateTime.hpp"
#include <CUtils/UnitTests.hpp>
#include <regex>

using namespace CUtils;

UTEST(DateTimeTest, CurrentDateTimeFormat)
{
    CE_Kernel::Aid::System::DateTime dt_;
    std::string date_time_ = dt_.GetCurrentDateTime();
    
    std::regex dt_regex_(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})");
    ASSERT_TRUE(std::regex_match(date_time_, dt_regex_));
}

UTEST(DateTimeTest, TimeManipulation)
{
    CE_Kernel::Aid::System::DateTime dt_;
    time_t t_ = 1630454400;
    
    dt_.SetTime(t_);
    ASSERT_EQ(dt_.GetTime(), t_);
    
    std::string formatted_ = dt_.Format("%Y-%m-%d");
    ASSERT_STREQ(formatted_.c_str(), "2021-09-01");
}
