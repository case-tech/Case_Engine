#include "../Kernel/Graphics/Window/Window.hpp"

int main() {
    using namespace CE_Kernel::Graphics::Window;

    try {
        // Создание окна с параметрами
        Window window(
            "My App",       // заголовок
            100, 100,       // позиция X/Y
            800, 600,       // ширина/высота
            "icon.xpm",      // путь к иконке
            Flags::OVERLAPPED
        );

        // Главный цикл приложения
        while (window.Process()) {
            // Здесь ваша основная логика рендеринга
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
