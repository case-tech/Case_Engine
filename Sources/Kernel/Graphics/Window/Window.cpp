#include "Window.hpp"

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace Window
        {
            Window::Window(const std::string& title_a,
                           uint32_t pos_x_a,
                           uint32_t pos_y_a,
                           uint32_t width_a,
                           uint32_t height_a,
                           const std::string& path_to_icon_a,
                           Flags flags_a)
                : win_info_ {
                    title_a, 
                    path_to_icon_a, 
                    pos_x_a, 
                    pos_y_a, 
                    width_a, 
                    height_a, 
                    flags_a }
            {
#if defined(WM_X11)
                display_ = XOpenDisplay(nullptr);
                if (!display_) 
                {
                    throw std::runtime_error("Failed to open X11 display");
                }

                int screen_ = DefaultScreen(display_);
                window_ = XCreateSimpleWindow(display_, 
                            RootWindow(display_, screen_), 
                            win_info_.pos_x_, win_info_.pos_y_, 
                            win_info_.width_, win_info_.height_, 1, 
                            BlackPixel(display_, screen_), 
                            WhitePixel(display_, screen_));

                if ((win_info_.flags_ == Flags::OVERLAPPED && 
                     win_info_.flags_ != Flags::OVERLAPPED) || 
                    (win_info_.flags_ == Flags::BORDERLESS && 
                     win_info_.flags_ != Flags::BORDERLESS)) 
                {
                    throw std::runtime_error("Mutually exclusive flags detected");
                }

                if (win_info_.flags_ == Flags::OVERLAPPED) 
                {
                    XWindowAttributes screen_attr_;
                    XGetWindowAttributes(display_, 
                            RootWindow(display_, screen_), &screen_attr_);

                    XMoveResizeWindow(display_, window_, 0, 0, 
                            screen_attr_.width, screen_attr_.height);
                }

                else if (win_info_.flags_ == Flags::BORDERLESS) 
                {
                    XSetWindowAttributes attr_;
                    attr_.override_redirect = True;
                    XChangeWindowAttributes(display_, window_, 
                            CWOverrideRedirect, &attr_);
                }

                XClassHint* class_hint_ = XAllocClassHint();
                std::string class_name_ = win_info_.title_ + "Class";
                if (class_hint_) 
                {
                    class_hint_->res_name = win_info_.title_.data();
                    class_hint_->res_class = class_name_.data();

                    XSetClassHint(display_, window_, class_hint_);
                    XFree(class_hint_);
                }

                if (!win_info_.path_to_icon_.empty() && 
                     win_info_.path_to_icon_ != " ") 
                {
                    int status_ = XpmReadFileToPixmap(display_, window_, 
                            win_info_.path_to_icon_.c_str(), &icon_pixmap_, 
                            &icon_mask_, nullptr);

                    if (status_ == XpmSuccess) 
                    {
                        XWMHints* hints_ = XAllocWMHints();
                        if (hints_) 
                        {
                            hints_->flags = IconPixmapHint | IconMaskHint;
                            hints_->icon_pixmap = icon_pixmap_;
                            hints_->icon_mask = icon_mask_;
                            XSetWMHints(display_, window_, hints_);
                            XFree(hints_);
                        }
                    }
                }

                XSelectInput(display_, window_, ExposureMask | KeyPressMask);
                XStoreName(display_, window_, win_info_.title_.c_str());
                XMapWindow(display_, window_);
#endif
            }

            bool Window::Process()
            {
#if defined(WM_X11)
                if (should_quit_)
                    return false;

                XEvent event_;
                while (XPending(display_)) 
                {
                    XNextEvent(display_, &event_);

                    switch (event_.type) 
                    {
                    case KeyPress:
                        should_quit_ = true;
                        break;
                    }
                }

                return !should_quit_;
#endif
            }

            void Window::Quit()
            {
#if defined(WM_X11)
                if (icon_pixmap_)
                    XFreePixmap(display_, icon_pixmap_);
                if (icon_mask_)
                    XFreePixmap(display_, icon_mask_);
                if (window_)
                    XDestroyWindow(display_, window_);
                if (display_)
                    XCloseDisplay(display_);

                display_ = nullptr;
                window_ = 0;
                should_quit_ = true;
#endif
            }

            Window::~Window()
            {
                Quit();
            }

            std::string Window::GetTitle()
            {
                return win_info_.title_;
            }

            uint32_t Window::GetPosX()
            {
                return win_info_.pos_x_;
            }

            uint32_t Window::GetPosY()
            {
                return win_info_.pos_y_;
            }

            uint32_t Window::GetWidth()
            {
                return win_info_.width_;
            }

            uint32_t Window::GetHeight()
            {
                return win_info_.height_;
            }

#if defined(WM_X11)
            Display* Window::GetXDisplay()
            {
                return display_;
            }

            ::Window Window::GetXWindow()
            {
                return window_;
            }
#endif
        } // namespace Window
    }     // namespace Graphics
} // namespace CE_Kernel
