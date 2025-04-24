// Copyright (c) 2025 Case Technologies

#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#define PLUGIN_EXPORT __declspec(dllexport)
#else
#define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

namespace CE_Kernel
{
    namespace Aid
    {
        namespace Plugins
        {
            class IPlugin
            {
            public:
                virtual ~IPlugin() = default;
                virtual void Initialize() = 0;
                virtual void Execute() = 0;
                virtual void Shutdown() = 0;
            };

            class PluginManager
            {
            public:
                PluginManager();
                ~PluginManager();

                void LoadPlugin(const std::string& path_a);
                void UnloadPlugin(const std::string& path_a);
                void UnloadAll();

                const std::vector<std::unique_ptr<IPlugin>>& GetPlugins() const;

            private:
                struct PluginHandle
                {
                    void* library_handle_;
                    std::unique_ptr<IPlugin> plugin_;
                };

                std::vector<PluginHandle> plugins_;

                void* LoadPLibrary(const std::string& path_a);
                void  UnloadLibrary(void* handle_a);
                void* GetSymbolAddress(void* handle_a, const std::string& symbol_a);
            };
        } // namespace Plugins
    } // namespace Aid
} // namespace CE_Kernel