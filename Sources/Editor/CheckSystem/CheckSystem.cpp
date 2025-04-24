#include "CheckSystem.hpp"

namespace Editor
{
    bool CheckSystem::Checking()
    {
        INIParser parser_("sys.ini");
        if (parser_.Load())
        {
            {
               data_sys_.os_name_ = parser_.GetValue("SysInfo", "OSName"   );
               data_sys_.os_name_ = parser_.GetValue("SysInfo", "OSVersion");
               data_sys_.os_name_ = parser_.GetValue("SysInfo", "CPUInfo"  );
               data_sys_.os_name_ = parser_.GetValue("SysInfo", "MemInfo"  );
               data_sys_.os_name_ = parser_.GetValue("SysInfo", "DiskInfo" );
            }
        }
        else
        {
            {
               data_sys_.os_name_    = InfoSys::GetOSName();
               data_sys_.os_version_ = InfoSys::GetOSVersion();
               data_sys_.cpu_info_   = InfoSys::GetCPUInfo();
               data_sys_.mem_info_   = InfoSys::GetMemoryInfo();
               data_sys_.disk_info_  = InfoSys::GetDiskInfo();
            }            

            std::ofstream sys_file_("sys.ini");
            sys_file_ << "[SysInfo]"                             << std::endl;
            sys_file_ << "OSName ="    << data_sys_.os_name_     << std::endl;
            sys_file_ << "OSVersion =" <<  data_sys_.os_version_ << std::endl;
            sys_file_ << "CPUInfo ="   << data_sys_.cpu_info_    << std::endl;
            sys_file_ << "MemInfo ="   << data_sys_.mem_info_    << std::endl;
            sys_file_ << "DiskInfo ="  << data_sys_.disk_info_   << std::endl;
            sys_file_.close();
        }

        try
        {
            int mem_ = std::stoi(data_sys_.mem_info_);
            if (mem_ < 2048)
            {
                return false;
            }
        }
        catch (...)
        {
            return false;
        }

        return true;
    }
}
