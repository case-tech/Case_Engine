// Copyright (c) 2025 Case Technologies

#pragma warning(disable : 4099)
#pragma once
#include <CUtils/Notification.hpp>
#include <Aid/System/InfoSys/InfoSys.hpp>
#include <Aid/INIParser/INIParser.hpp>
#include <Aid/TypeToType/TypeToType.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace CE_Kernel::Aid::System;
using namespace CE_Kernel::Aid::INIParser;
using namespace CE_Kernel::Aid::TypeToType;

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
