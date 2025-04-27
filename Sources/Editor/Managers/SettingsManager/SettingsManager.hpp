// Copyright (c) 2025 Case Technologies

#pragma once
#include <iostream>
#include <string>

namespace Editor
{
    class SettingsManager
    {
    public:
        SettingsManager();
        ~SettingsManager();

        void SetPath(std::string& new_path_a);
    };
}
