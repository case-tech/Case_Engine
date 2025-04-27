// Copyright (c) 2025 Case Technologies

#pragma once
#include <Graphics/Window/Window.hpp>
#include <Graphics/Window/Control/Control.hpp>
#include <Aid/TypeToType/TypeToType.hpp>

#include <iostream>
#include <string>

using namespace CE_Kernel::Aid::TypeToType;

namespace Editor
{
    class SandBox
    {
    public:
        SandBox();
        ~SandBox();

        void Start(const std::string& path_to_begin_level_a);

    private:
        
    };
}
