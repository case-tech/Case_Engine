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

    bool PRManager::Open(const std::string& path_to_project_a)
    {
        try 
        {
            std::fstream project_file_(path_to_project_a);
            if (!project_file_.is_open())
            {
                std::cerr << "Cannot open project file: " << path_to_project_a << std::endl;
                return false;
            }

            std::stringstream buffer_;
            buffer_ << project_file_.rdbuf();
            std::string file_content_ = buffer_.str();
            project_file_.close();

            std::istringstream stream_(file_content_);
            auto pr_file_ = parser_.Parse(stream_);

            if (pr_file_.find("Info") == pr_file_.end()) 
            {
                throw std::runtime_error("Missing 'Info' section");
            }
            
            auto& info_ = pr_file_["Info"];
            if (info_.find("name") == info_.end()) 
            {
                throw std::runtime_error("Missing 'name' in Info section");
            }
            if (info_.find("description") == info_.end()) 
            {
                throw std::runtime_error("Missing 'description' in Info section");
            }
            if (info_.find("version") == info_.end()) 
            {
                throw std::runtime_error("Missing 'version' in Info section");
            }
            if (info_.find("engine_v") == info_.end()) 
            {
                throw std::runtime_error("Missing 'engine_v' in Info section");
            }
            
            if (!std::holds_alternative<std::string>(info_["name"].data_)) 
            {
                throw std::runtime_error("'name' must be a string");
            }
            if (!std::holds_alternative<std::string>(info_["description"].data_)) 
            {
                throw std::runtime_error("'description' must be a string");
            }

            pr_info_.name_ = std::get<std::string>(info_["name"].data_);
            pr_info_.desc_ = std::get<std::string>(info_["description"].data_);
            
            if (std::holds_alternative<std::string>(info_["version"].data_)) 
            {
                pr_info_.version_str_ = std::get<std::string>(info_["version"].data_);
                try 
                {
                    pr_info_.version_ = static_cast<int>(std::stod(pr_info_.version_str_));
                } 
                catch (...) 
                {
                    pr_info_.version_ = 1;
                }
            }
            else if (std::holds_alternative<double>(info_["version"].data_)) 
            {
                double version_double = std::get<double>(info_["version"].data_);
                pr_info_.version_ = static_cast<int>(version_double);
                pr_info_.version_str_ = std::to_string(version_double);
            }
            else 
            {
                throw std::runtime_error("'version' must be a string or number");
            }

            if (std::holds_alternative<std::string>(info_["engine_v"].data_)) 
            {
                pr_info_.engine_v_str_ = std::get<std::string>(info_["engine_v"].data_);
                try 
                {
                    pr_info_.engine_v_ = static_cast<int>(std::stod(pr_info_.engine_v_str_));
                } 
                catch (...) 
                {
                    pr_info_.engine_v_ = 1;
                }
            }
            else if (std::holds_alternative<double>(info_["engine_v"].data_)) 
            {
                double engine_v_double = std::get<double>(info_["engine_v"].data_);
                pr_info_.engine_v_ = static_cast<int>(engine_v_double);
                pr_info_.engine_v_str_ = std::to_string(engine_v_double);
            }
            else 
            {
                throw std::runtime_error("'engine_v' must be a string or number");
            }

            if (pr_file_.find("Paths") == pr_file_.end()) 
            {
                throw std::runtime_error("Missing 'Paths' section");
            }
            
            auto& paths_ = pr_file_["Paths"];
            const std::vector<std::string> required_paths = {"assets", "logs", "settings"};
            for (const auto& path : required_paths) 
            {
                if (paths_.find(path) == paths_.end()) 
                {
                    throw std::runtime_error("Missing '" + path + "' in Paths section");
                }
                if (!std::holds_alternative<std::string>(paths_[path].data_)) 
                {
                    throw std::runtime_error("'" + path + "' must be a string");
                }
            }

            assets_manager_.SetPath(std::get<std::string>(paths_["assets"].data_));
            logs_manager_.SetPath(std::get<std::string>(paths_["logs"].data_));
            settings_manager_.SetPath(std::get<std::string>(paths_["settings"].data_));

            return true;
        }
        catch (const std::exception& e) 
        {
            std::cerr << "Project loading error: " << e.what() << std::endl;
            return false;
        }
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

    std::string PRManager::GetVersionProjectString()
    {
        return pr_info_.version_str_;
    }

    int PRManager::GetVersionEngine()
    {
        return pr_info_.engine_v_;
    }

    std::string PRManager::GetVersionEngineString()
    {
        return pr_info_.engine_v_str_;
    }
}