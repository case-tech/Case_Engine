// Copyright(c) 2025 Case Technologies

#pragma once
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#if defined(WM_X11)
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>

#elif defined(WM_WAYLAND)
#include <wayland-client.h>
#endif

#if defined(__APPLE__)
#import <Cocoa/Cocoa.h>
#endif

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace Window
        {
            enum class Flags
            {
                NONE,
                BORDERLESS,
                OVERLAPPED,
#if defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
                FULLSCREEN,
                DISABLED
#endif
            };

            class Window
            {
            public:
                Window(const std::string& title_a,
                       uint32_t pos_x_a,
                       uint32_t pos_y_a,
                       uint32_t width_a,
                       uint32_t height_a,
                       const std::string& path_to_icon_a = " ",
                       Flags flags_a = Flags::NONE);

                ~Window();

                void Quit();
                bool Process();

                // Getters
                std::string GetTitle();
                uint32_t GetPosX();
                uint32_t GetPosY();
                uint32_t GetWidth();
                uint32_t GetHeight();

#if defined(WM_X11)
                Display* GetXDisplay();
                ::Window GetXWindow();
#endif

            private:
                struct WinInfo
                {
                    std::string title_;
                    std::string path_to_icon_;

                    uint32_t pos_x_;
                    uint32_t pos_y_;
                    uint32_t width_;
                    uint32_t height_;

                    Flags flags_;
                };

            private:
                WinInfo win_info_;
                bool should_quit_ = false;

#if defined(WM_X11)
                Display* display_;
                ::Window window_;
                Pixmap icon_pixmap_;
                Pixmap icon_mask_;
#endif
            };
        } // namespace Window
    }     // namespace Graphics
} // namespace CE_Kernel
