#include "Plugins.hpp"

#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace CE_Kernel
{
    namespace Aid
    {
        namespace Plugins
        {
            PluginManager::PluginManager() = default;

            PluginManager::~PluginManager()
            {
                UnloadAll();
            }

            void PluginManager::LoadPlugin(const std::string& path_a)
            {
                void* handle_ = LoadPLibrary(path_a);
                if (!handle_) 
                {
                    throw std::runtime_error("Failed to load library: " + path_a);
                }

                using CreatePluginFunc = IPlugin* (*)();
                auto create_plugin_ = reinterpret_cast<CreatePluginFunc>(
                    GetSymbolAddress(handle_, "CreatePlugin"));

                if (!create_plugin_) 
                {
                    UnloadLibrary(handle_);
                    throw std::runtime_error("Failed to find CreatePlugin symbol in: " + path_a);
                }

                std::unique_ptr<IPlugin> plugin_(create_plugin_());
                if (!plugin_) 
                {
                    UnloadLibrary(handle_);
                    throw std::runtime_error("Failed to create plugin from: " + path_a);
                }

                try 
                {
                    plugin_->Initialize();
                } 
                
                catch (...) 
                {
                    UnloadLibrary(handle_);
                    throw;
                }

                plugins_.push_back({handle_, std::move(plugin_)});
            }

            void PluginManager::UnloadPlugin(const std::string& path_a)
            {
                auto it_ = std::find_if(plugins_.begin(),
                                       plugins_.end(),
                                       [&path_a](const PluginHandle& ph_) {
                                           std::string module_path_;

#if defined(_WIN32) || defined(_WIN64)
                                           CHAR buf_[MAX_PATH];
                                           DWORD len_ = ::GetModuleFileNameA(
                                                   static_cast<HMODULE>(
                                                           ph_.library_handle_),
                                                   buf_,
                                                   sizeof(buf_));
                                           if (len_ == 0)
                                           {
                                               return false;
                                           }
                                           module_path_.assign(buf_, len_);
#else
            Dl_info info_;
            if (dladdr(ph_.library_handle_, &info_) && info_.dli_fname) {
                module_path_ = info_.dli_fname;
            } 
            else 
            {
                return false;
            }
#endif
                                           return module_path_ == path_a;
                                       });

                if (it_ != plugins_.end())
                {
                    it_->plugin_->Shutdown();
                    UnloadLibrary(it_->library_handle_);
                    plugins_.erase(it_);
                }
            }


            void PluginManager::UnloadAll()
            {
                for (auto& ph_ : plugins_) 
                {
                    ph_.plugin_->Shutdown();
                    UnloadLibrary(ph_.library_handle_);
                }

                plugins_.clear();
            }

            const std::vector<std::unique_ptr<IPlugin>>& PluginManager::GetPlugins() const
            {
                return reinterpret_cast<const std::vector<std::unique_ptr<IPlugin>>&>(plugins_);
            }

            void* PluginManager::LoadPLibrary(const std::string& path_a)
            {
#if defined(_WIN32) || defined(_WIN64)
                return LoadLibraryA(path_a.c_str());
#else
                return dlopen(path_a.c_str(), RTLD_LAZY);
#endif
            }

            void PluginManager::UnloadLibrary(void* handle_a)
            {
                if (!handle_a)
                    return;
#if defined(_WIN32) || defined(_WIN64)
                FreeLibrary((HMODULE)handle_a);
#else
                dlclose(handle_a);
#endif
            }

            void* PluginManager::GetSymbolAddress(void* handle_a, const std::string& symbol_a)
            {
#if defined(_WIN32) || defined(_WIN64)
                return (void*)GetProcAddress((HMODULE)handle_a, symbol_a.c_str());
#else
                return dlsym(handle_a, symbol_a.c_str());
#endif
            }
        } // namespace Plugins
    } // namespace Aid
} // namespace CE_Kernel