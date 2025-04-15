#include "../Kernel/Graphics/Window/Control/Control.hpp"
#include "../Kernel/Graphics/Window/Window.hpp"

#include <iostream>

int main()
{
    try {
        CE_Kernel::Graphics::Window::Window win("Title", 0, 0, 800, 600);
        CE_Kernel::Graphics::Window::Control control(win);

        while (win.Process()) {
            control.Update(); // Обновляем состояние Control

            // Проверяем клавиши (учитывайте регистр: "Q" vs "q")
            std::string key = control.GetKeyPressed();
            if (!key.empty()) {
                std::cout << "Pressed: " << key << std::endl;
                if (key == "Q" || key == "q")
                    win.Quit();
            }

            // Проверяем мышь
            int x, y;
            control.GetMousePosition(x, y);
            if (control.IsMouseButtonPressed("Left")) {
                std::cout << "Left click at: " << x << ", " << y << std::endl;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
