#include "InfoSys.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace System
        {
            std::string InfoSys::GetOSName()
            {
                return CUtils::SystemConfiguration::GetOSName();
            }

            std::string InfoSys::GetOSVersion()
            {
                return CUtils::SystemConfiguration::GetOSVersion();
            }

            std::string InfoSys::GetCPUInfo()
            {
                return CUtils::SystemConfiguration::GetCPUInfo();
            }

            std::string InfoSys::GetMemoryInfo()
            {
                return CUtils::SystemConfiguration::GetMemoryInfo();
            }

            std::string InfoSys::GetDiskInfo()
            {
                return CUtils::SystemConfiguration::GetDiskInfo();
            }
        }
    }
}
