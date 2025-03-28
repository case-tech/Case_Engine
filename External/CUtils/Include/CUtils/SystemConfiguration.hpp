// The MIT License (MIT)
// Copyright � 2024 Case Technologies

#pragma once
#include "Common.hpp"

namespace CUtils
{
    class SystemConfiguration 
    {
    public:
        static std::string GetOSName();
        static std::string GetOSVersion();
        static std::string GetCPUInfo();
        static std::string GetMemoryInfo();
        static std::string GetDiskInfo();

    private:
        #if defined(_WIN32) || defined(_WIN64)
            static std::string execCommand(const std::string& command);
        #else
            static std::string execCommand(const char* command);
        #endif
    };
}
