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
#if defined(__linux__)
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
#if defined(__linux__)
                XEvent event_;
                while (XPending(display_)) 
                {
                    XNextEvent(display_, &event_);
                    switch (event_.type) 
                    {
                    case KeyPress: 
                        {
                        char buffer_[32];
                        KeySym keysym_;
                        XComposeStatus compose_;
                        XLookupString(&event_.xkey, buffer_, 
                                sizeof(buffer_), &keysym_, &compose_);

                        const char* key_name_ = XKeysymToString(
                                                        keysym_);
                        if (key_name_)
                            key_pressed_ = key_name_;
                        break;
                    }
                    case MotionNotify:
                        mouse_x_ = event_.xmotion.x;
                        mouse_y_ = event_.xmotion.y;
                        break;
                    case ButtonPress:
                        mouse_buttons_ |= (1 << (
                                    event_.xbutton.button - 1));
                        break;
                    case ButtonRelease:
                        mouse_buttons_ &= ~(1 << (
                                    event_.xbutton.button - 1));
                        break;
                    }
                }
#endif
            }

            std::string Control::GetKeyPressed() const
            {
#if defined(__linux__)
                return key_pressed_;
#else
                return "";
#endif
            }

            void Control::GetMousePosition(int& x_a, int& y_a) const
            {
#if defined(__linux__)
                x_a = mouse_x_;
                y_a = mouse_y_;
#endif
            }

            bool Control::IsMouseButtonPressed(
                    const std::string& button_a) const
            {
#if defined(__linux__)
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
    }     // namespace Graphics
} // namespace CE_Kernel
