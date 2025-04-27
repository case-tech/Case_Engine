#include "CheckSystem.hpp"

namespace Editor
{
    bool CheckSystem::Checking()
    {
<<<<<<< HEAD
        INIParser parser_("sys.ini");
=======
        std::string exe_path_ = GetExecutablePath();
        std::string exe_dir_  = GetParentDirectory(exe_path_);
        std::string sys_ini_path_ = CombinePaths(exe_dir_, "sys.ini");

        INIParser parser_(sys_ini_path_);
>>>>>>> aa4b252 (Add open project)
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

<<<<<<< HEAD
            std::ofstream sys_file_("sys.ini");
=======
            std::ofstream sys_file_(sys_ini_path_);
>>>>>>> aa4b252 (Add open project)
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
<<<<<<< HEAD
            int mem_ = std::stoi(data_sys_.mem_info_);
            if (mem_ < 2048)
            {
=======
#if defined(__linux__)
            int mem_ = std::stoi(data_sys_.mem_info_);
            if (mem_ <= 2097152)
#elif defined(_WIN32) || defined(_WIN32)
            int mem_ = ExtractNumber(data_sys_.mem_info_);
            if (mem_ <= 2048)
#endif
            {
                CUtils::Notification::notify(CUtils::Notification::TYPE_ERROR,
                                             "Your device does not have enough RAM < 2048 MB");

>>>>>>> aa4b252 (Add open project)
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
