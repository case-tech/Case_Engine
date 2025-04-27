// Copyright (c) 2025 Case Technologies

#pragma once
#include "CheckSystem/CheckSystem.hpp"
<<<<<<< HEAD
=======
#include "Managers/PRManager.hpp"
#include "SandBox/SandBox.hpp"

#include <iostream>
#include <string>
>>>>>>> aa4b252 (Add open project)

namespace Editor
{
    class Editor
    {
    public:
<<<<<<< HEAD
        Editor();
=======
        Editor(std::string& path_to_project_a);
>>>>>>> aa4b252 (Add open project)
        ~Editor();

        bool Run();

    private:
        CheckSystem check_sys_;
<<<<<<< HEAD
=======
        PRManager pr_manager_;
        SandBox sandbox_;

        std::string path_to_project_;
>>>>>>> aa4b252 (Add open project)
    };
} // namespace Editor
