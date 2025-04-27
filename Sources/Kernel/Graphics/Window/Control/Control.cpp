#include "Control.hpp"

namespace CE_Kernel
{
    namespace Graphics
    {
        namespace Window
        {
            Control::Control(
                CE_Kernel::Graphics::Window::Window& window_a)
            {
#if defined(_WIN32) || defined(_WIN64)
                hwnd_ = window_a.GetHWND();
#elif defined(__linux__)
                display_ = window_a.GetXDisplay();
                xwindow_ = window_a.GetXWindow();
#endif
                key_pressed_.clear();
                mouse_x_ = 0;
                mouse_y_ = 0;
                mouse_buttons_ = 0;
            }

            void Control::Update()
            {
#if defined(_WIN32) || defined(_WIN64)
                MSG msg;
                while (PeekMessage(&msg, hwnd_, 0, 0, PM_REMOVE)) 
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                key_pressed_.clear();
                for (int vkey = 0; vkey < 256; ++vkey) 
                {
                    if (GetAsyncKeyState(vkey) & 0x8000) 
                    {
                        char keyName[256];
                        if (GetKeyNameTextA(MapVirtualKeyA(vkey, MAPVK_VK_TO_CHAR) << 16, keyName, sizeof(keyName)))
                            key_pressed_ = keyName;
                    }
                }

                POINT cursorPos;
                GetCursorPos(&cursorPos);
                ScreenToClient(hwnd_, &cursorPos);
                mouse_x_ = cursorPos.x;
                mouse_y_ = cursorPos.y;

                mouse_buttons_ = 0;
                if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
                    mouse_buttons_ |= 1 << 0;
                if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
                    mouse_buttons_ |= 1 << 1;
                if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
                    mouse_buttons_ |= 1 << 2;

#elif defined(__linux__)
                XEvent event_;
                while (XPending(display_)) {
                    XNextEvent(display_, &event_);
                    switch (event_.type) {
                    case KeyPress: {
                        char buffer_[32];
                        KeySym keysym_;
                        XComposeStatus compose_;
                        XLookupString(&event_.xkey, buffer_, sizeof(buffer_), &keysym_, &compose_);

                        const char* key_name_ = XKeysymToString(keysym_);
                        if (key_name_)
                            key_pressed_ = key_name_;
                        break;
                    }
                    case MotionNotify:
                        mouse_x_ = event_.xmotion.x;
                        mouse_y_ = event_.xmotion.y;
                        break;
                    case ButtonPress:
                        mouse_buttons_ |= (1 << (event_.xbutton.button - 1));
                        break;
                    case ButtonRelease:
                        mouse_buttons_ &= ~(1 << (event_.xbutton.button - 1));
                        break;
                    }
                }
#endif
            }

            std::string Control::GetKeyPressed() const
            {
                return key_pressed_;
            }

            void Control::GetMousePosition(int& x_a, int& y_a) const
            {
                x_a = mouse_x_;
                y_a = mouse_y_;
            }

            bool Control::IsMouseButtonPressed(
                const std::string& button_a) const
            {
#if defined(_WIN32) || defined(_WIN64)
                if (button_a == "Left")
                    return (mouse_buttons_ & (1 << 0));
                if (button_a == "Middle")
                    return (mouse_buttons_ & (1 << 1));
                if (button_a == "Right")
                    return (mouse_buttons_ & (1 << 2));
#elif defined(__linux__)
                if (button_a == "Left")
                    return (mouse_buttons_ & (1 << 0));
                if (button_a == "Middle")
                    return (mouse_buttons_ & (1 << 1));
                if (button_a == "Right")
                    return (mouse_buttons_ & (1 << 2));
#endif
                return false;
            }
        } // namespace Window
    } // namespace Graphics
} // namespace CE_Kernel