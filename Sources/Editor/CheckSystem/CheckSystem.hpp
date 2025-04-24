// Copyright (c) 2025 Case Technologies

#pragma once
#include <CUtils/Notification.hpp>
#include <Aid/System/InfoSys/InfoSys.hpp>
#include <Aid/INIParser/INIParser.hpp>

#include <iostream>
#include <string>
#include <fstream>

using namespace CE_Kernel::Aid::System;
using namespace CE_Kernel::Aid::INIParser;

namespace Editor
{
    class CheckSystem
    {
    public:
        CheckSystem() = default;
        ~CheckSystem() = default;
        
        bool Checking();

    private:
        struct DataSys
        {
            std::string os_name_;
            std::string os_version_;
            std::string cpu_info_;
            std::string mem_info_;
            std::string disk_info_;
        };

     private:
        DataSys data_sys_;
    };
}
