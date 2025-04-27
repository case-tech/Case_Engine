#include "SandBox.hpp"

namespace Editor
{
    SandBox::SandBox()
    {

    }

    SandBox::~SandBox()
    {

    }

    void SandBox::Start(const std::string& path_to_begin_level_a)
    {
        std::string exe_path_ = GetExecutablePath();
        std::string exe_dir_  = GetParentDirectory(exe_path_);

#if defined (__linux__)
        std::string icon_path_ = CombinePaths(exe_dir_, "icon.xpm");
#else
        std::string icon_path_ = CombinePaths(exe_dir_, "icon.ico");
#endif

        (void) path_to_begin_level_a;
        CE_Kernel::Graphics::Window::Window win_("CaseEngine", 
                100, 100, 1920, 1080, icon_path_,CE_Kernel::Graphics::Window::Flags::OVERLAPPED);

        CE_Kernel::Graphics::Window::Control control_(win_);

        while(win_.Process())
        {
            control_.Update();
            if (control_.GetKeyPressed() == "Escape")
            {
                win_.Quit();
            }
        }
    }
}
