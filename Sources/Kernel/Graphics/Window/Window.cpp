#include "Window.hpp"

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace Window
        {
#if defined(_WIN32) || defined(_WIN64)
            std::wstring Window::StringToWstring(const std::string& str_a, UINT code_page_a)
            {
                if (str_a.empty())
                    return L"";

                int size_needed_ = MultiByteToWideChar(
                    code_page_a,
                    0,
                    str_a.c_str(),
                    (int)str_a.size(),
                    nullptr,
                    0);

                std::wstring result_(size_needed_, 0);
                MultiByteToWideChar(
                    code_page_a,
                    0,
                    str_a.c_str(),
                    (int)str_a.size(),
                    result_.data(),
                    size_needed_);

                return result_;
            }

            HICON Window::LoadIconFromFile(const wchar_t* path_a)
            {
                return (HICON)LoadImageW(
                    NULL,
                    path_a,
                    IMAGE_ICON,
                    0,
                    0,
                    LR_LOADFROMFILE | LR_DEFAULTSIZE);
            }
#endif

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
                    flags_a  }
            {
#if defined(_WIN32) || defined(_WIN64)   
                std::string stemp_ = win_info_.title_ + "Class";
                std::wstring wstemp_ = 
                    std::wstring(stemp_.begin(), stemp_.end());
                LPCWSTR sw_ = wstemp_.c_str();

                int size_ = MultiByteToWideChar(CP_UTF8, 0,
                    win_info_.path_to_icon_.c_str(), -1, nullptr, 0);
                wchar_t* wstr_ = new wchar_t[size_];
                MultiByteToWideChar(CP_UTF8, 0, 
                    win_info_.path_to_icon_.c_str(), -1, wstr_, size_);

                const wchar_t* icon_path_ = wstr_;

                HICON h_icon_ = LoadIconFromFile(icon_path_);
                WNDCLASSW wc_;
                wc_.style = CS_HREDRAW | CS_VREDRAW;
                wc_.cbClsExtra = 0;
                wc_.cbWndExtra = 0;
                wc_.lpszClassName = sw_;
                wc_.hInstance = GetModuleHandle(nullptr);
                wc_.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
                wc_.lpszMenuName = NULL;
                wc_.lpfnWndProc = &WndProc;
                wc_.hCursor = LoadCursor(NULL, IDC_ARROW);
                wc_.hIcon = h_icon_;

                std::wstring title_wide_ = 
                    StringToWstring(win_info_.title_, CP_ACP);

               uint32_t window_style_ = WS_VISIBLE;

                switch (win_info_.flags_) 
                {
                case Flags::BORDERLESS:
                    window_style_ |= WS_POPUP;
                    break;
                case Flags::OVERLAPPED:
                    window_style_ |= WS_OVERLAPPEDWINDOW;
                    break;
                case Flags::FULLSCREEN:
                    window_style_ |= WS_POPUP;
                    break;
                case Flags::DISABLED:
                    window_style_ |= WS_DISABLED;
                    break;
                default:
                    window_style_ |= WS_OVERLAPPEDWINDOW;
                    break;
                }

                RegisterClassW(&wc_);
                hwnd_ = CreateWindowExW(
                    0,
                    wc_.lpszClassName,
                    title_wide_.c_str(),
                    window_style_,
                    win_info_.pos_x_,
                    win_info_.pos_y_,
                    win_info_.width_,
                    win_info_.height_,
                    NULL,
                    NULL,
                    GetModuleHandle(nullptr),
                    NULL);

                SetWindowLongPtrW(hwnd_, GWLP_USERDATA, (LONG_PTR)this);

                if (win_info_.flags_ == Flags::OVERLAPPED) 
                {
                    ShowWindow(hwnd_, SW_MAXIMIZE); 
                } 
                else if (win_info_.flags_ == Flags::FULLSCREEN) 
                {
                    RECT desktopRect;
                    GetWindowRect(GetDesktopWindow(), &desktopRect);
                    SetWindowPos(hwnd_, NULL, 
                        desktopRect.left, desktopRect.top, 
                        desktopRect.right - desktopRect.left, 
                        desktopRect.bottom - desktopRect.top, SWP_NOZORDER);
                }
                else 
                {
                    ShowWindow(hwnd_, SW_SHOW);
                }

                UpdateWindow(hwnd_);
                delete[] wstr_;

#elif defined(__linux__)
                display_ = XOpenDisplay(nullptr);
                if (!display_) 
                {
                    throw std::runtime_error(
                            "Failed to open X11 display");
                }

                int screen_ = DefaultScreen(display_);
                window_ = XCreateSimpleWindow(
                        display_,
                        RootWindow(display_, screen_),
                        win_info_.pos_x_,
                        win_info_.pos_y_,
                        win_info_.width_,
                        win_info_.height_,
                        1,
                        BlackPixel(display_, screen_),
                        WhitePixel(display_, screen_));

                if ((win_info_.flags_ == Flags::OVERLAPPED  && 
                     win_info_.flags_ != Flags::OVERLAPPED) || 
                    (win_info_.flags_ == Flags::BORDERLESS  && 
                     win_info_.flags_ != Flags::BORDERLESS)) 
                {
                    throw std::runtime_error(
                            "Mutually exclusive flags detected");
                }

                if (win_info_.flags_ == Flags::OVERLAPPED) 
                {
                    XWindowAttributes screen_attr_;
                    XGetWindowAttributes(
                            display_,
                            RootWindow(display_, screen_),
                            &screen_attr_);

                    XMoveResizeWindow(
                            display_, 
                            window_, 
                            0, 0, 
                            screen_attr_.width, 
                            screen_attr_.height);
                }

                else if (win_info_.flags_ == Flags::BORDERLESS) 
                {
                    XSetWindowAttributes attr_;
                    attr_.override_redirect = True;
                    XChangeWindowAttributes(
                            display_, window_, 
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
                    int status_ = XpmReadFileToPixmap(
                            display_, window_, 
                            win_info_.path_to_icon_.c_str(), 
                            &icon_pixmap_, &icon_mask_, nullptr);

                    if (status_ == XpmSuccess) 
                    {
                        XWMHints* hints_ = XAllocWMHints();
                        if (hints_) 
                        {
                            hints_->flags = IconPixmapHint | 
                                            IconMaskHint;
                            hints_->icon_pixmap = icon_pixmap_;
                            hints_->icon_mask = icon_mask_;
                            XSetWMHints(display_, window_, hints_);
                            XFree(hints_);
                        }
                    }
                }

                XSelectInput(display_, window_, 
                        ExposureMask    | KeyPressMask      | 
                        KeyReleaseMask  | PointerMotionMask | 
                        ButtonPressMask | ButtonReleaseMask);

                XStoreName(display_, window_, 
                        win_info_.title_.c_str());
                XMapWindow(display_, window_);
#endif
            }

            bool Window::Process()
            {
#if defined(_WIN32) || defined(_WIN64)
                while (GetMessage(&msg_, NULL, 0, 0)) 
                {
                    TranslateMessage(&msg_);
                    DispatchMessage(&msg_);
                }

#elif defined(__linux__)
                if (should_quit_)
                    return false;
#endif
                return !should_quit_;
            }

            void Window::Quit()
            {
#if defined(__linux__)
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
#endif
                should_quit_ = true;
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

#if defined(_WIN32) || defined(_WIN64)
            HWND Window::GetHWND()
            {
                return hwnd_;
            }

           LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
           {
                switch (msg) 
                {
                case WM_DESTROY:
                    PostQuitMessage(0);
                    exit(-1);
                    break;
                }

                return DefWindowProcW(hwnd, msg, wParam, lParam);
           }

#elif defined(__linux__)
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
