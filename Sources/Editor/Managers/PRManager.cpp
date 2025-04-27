#include "PRManager.hpp"

#include <sstream>

namespace Editor
{
    PRManager::PRManager() :
        assets_manager_(),
        logs_manager_(),
        settings_manager_()
    {

    }

    bool PRManager::Open(std::string& path_to_project_a)
    {
        std::fstream project_file_(path_to_project_a);
        if (!project_file_.is_open())
        {
            return false;
        }

        std::stringstream buffer_;
        buffer_ << project_file_.rdbuf();
        std::string file_content_ = buffer_.str();

        std::istringstream stream_(file_content_);
        auto pr_file_ = parser_.Parse(stream_);

        if (pr_file_.find("Info") != pr_file_.end())
        {
            auto& info_ = pr_file_["Info"];
            pr_info_.name_ = std::get<std::string>(
                    info_["name"].data_);

            pr_info_.desc_ = std::get<std::string>(
                    info_["description"].data_);

            pr_info_.version_ = static_cast<int>(std::get<double>(
                    info_["version"].data_));

            pr_info_.engine_v_ = static_cast<int>(std::get<double>(
                    info_["engine_v"].data_));
        }

        if (pr_file_.find("Paths") != pr_file_.end())
        {
            auto& paths_ = pr_file_["Paths"];
            assets_manager_.SetPath(
                    std::get<std::string>(paths_["assets"].data_));

            logs_manager_.SetPath(
                    std::get<std::string>(paths_["logs"].data_));

            settings_manager_.SetPath(
                    std::get<std::string>(paths_["settings"].data_));
        }

        return true;
    }

    std::string PRManager::GetBeginLevel()
    {
        return "";
    }
    
    std::string PRManager::GetNameProject()
    {
        return pr_info_.name_;
    }

    std::string PRManager::GetDescProject()
    {
        return pr_info_.desc_;
    }

    int PRManager::GetVersionProject()
    {
        return pr_info_.version_;
    }

    int PRManager::GetVersionEngine()
    {
        return pr_info_.engine_v_;
    }
}
