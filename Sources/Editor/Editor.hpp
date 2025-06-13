// Copyright (c) 2025 Case Technologies

#pragma once
#include "CheckSystem/CheckSystem.hpp"
#include "Managers/PRManager.hpp"
#include "SandBox/SandBox.hpp"

namespace Editor
{
    class Editor
    {
    public:
        Editor(const std::string& path_to_project_a);
        ~Editor();

        bool Run();

    private:
        CheckSystem check_sys_;
        PRManager pr_manager_;
        SandBox sand_box_;
    };
} // namespace Editor
