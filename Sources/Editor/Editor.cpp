#include "Editor.hpp"

namespace Editor
{
    Editor::Editor()
    {
        if (check_sys_.Checking())
        {
            std::cout << "SUCCESS";
        } else
        {
            CUtils::Notification::notify(
                    CUtils::Notification::TYPE_ERROR,
                    "Your device does not have enough RAM < 2048 MB");
        }
    }

    Editor::~Editor()
    {}

    bool Editor::Run()
    {
        return true;
    }
} // namespace Editor

int main()
{
    try
    {
        Editor::Editor editor_;
        editor_.Run();
    } catch (...)
    {
        CUtils::Notification::notify(CUtils::Notification::TYPE_ERROR,
                                     "This is a critical error! \
                The engine is unlikely to be able to work on your device yet :(");
    }

    return 0;
}