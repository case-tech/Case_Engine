// Copyright (c) 2025 Case Technologies

#pragma once
#include "CheckSystem/CheckSystem.hpp"

namespace Editor
{
    class Editor
    {
    public:
        Editor();
        ~Editor();

        bool Run();

    private:
        CheckSystem check_sys_;
    };
} // namespace Editor
