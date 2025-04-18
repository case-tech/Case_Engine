#pragma once
#include "Aid/System/InfoSys/InfoSys.hpp"
#include <CUtils/UnitTests.hpp>
#include <regex>

using namespace CUtils;

UTEST(InfoSysTest, OSInfoFormat)
{
    std::string os_name_ = CE_Kernel::Aid::System::InfoSys::GetOSName();
    ASSERT_FALSE(os_name_.empty());
    
    std::regex os_regex_("Windows|Linux|macOS|Darwin");
    ASSERT_TRUE(std::regex_search(os_name_, os_regex_));
}
