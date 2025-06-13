// Copyright (c) 2025 Case Technologies

#pragma once
#include "AssetsManager/AssetsManager.hpp"
#include "LogsManager/LogsManager.hpp"
#include "SettingsManager/SettingsManager.hpp"

#include <Aid/FPLParser/FPLParser.hpp>
#include <fstream>
#include <iostream>
#include <string>

namespace Editor
{
    using FPLParser = CE_Kernel::Aid::FPLParser::FPLParser;

    class PRManager
    {
    public:
        PRManager();

        bool Open(const std::string& path_to_project_a);

        // Getters
        std::string GetBeginLevel();
        std::string GetNameProject();
        std::string GetDescProject();
        int         GetVersionProject();
        std::string GetVersionProjectString();
        int         GetVersionEngine();
        std::string GetVersionEngineString();

    private:
        struct InfoProject
        {
            std::string name_;
            std::string desc_;
            int version_;
            std::string version_str_;
            int engine_v_;
            std::string engine_v_str_;
        };

    private:
        InfoProject pr_info_;
        FPLParser parser_;

        AssetsManager assets_manager_;
        LogsManager   logs_manager_;
        SettingsManager settings_manager_;
    };
}