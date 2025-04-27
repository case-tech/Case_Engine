// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Window.hpp"

#if defined(__linux__)
#include <X11/keysym.h>
#endif

#include <string>

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace Window
        {
            class Control
            {
            public:
                Control(CE_Kernel::Graphics::Window::Window& window_a);

                void Update();
                std::string GetKeyPressed() const;
                void GetMousePosition(int& x_a, int& y_a) const;
                bool IsMouseButtonPressed(
                        const std::string& button_a) const;

            private:
#if defined(_WIN32) || defined(_WIN64)
                HWND hwnd_;
#elif defined(__linux__)
                Display* display_;
                ::Window xwindow_;
#endif
                std::string key_pressed_;
                int mouse_x_ = 0;
                int mouse_y_ = 0;
                unsigned int mouse_buttons_ = 0;
            };
        } // namespace Window
    }     // namespace Graphics
} // namespace CE_Kernel
