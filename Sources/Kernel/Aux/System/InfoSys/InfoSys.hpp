// Copyright (c) 2025 Case Technologies

#pragma once
#include <CUtils/SystemConfiguration.hpp>

namespace CE_Kernel
{
    namespace Aux
    {
        namespace System
        {
            class InfoSys
            {
            public:
                static std::string GetOSName();
                static std::string GetOSVersion();
                static std::string GetCPUInfo();
                static std::string GetMemoryInfo();
                static std::string GetDiskInfo();
            };
        } // namespace System
    } // namespace Aux
} // namespace CE_Kernel
