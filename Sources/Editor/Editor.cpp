#include "Editor.hpp"

namespace Editor
{
    Editor::Editor(const std::string& path_to_project_a)
    {
        //if (check_sys_.Checking())
        //{
            if (pr_manager_.Open(path_to_project_a))
            {
                sand_box_.Start(" ");
            }
        //}
    }

    Editor::~Editor()
    {}

    bool Editor::Run()
    {
        return true;
    }
} // namespace Editor

int main(int argc, char* argv[])
{
	(void)argc;
    try
    {
        Editor::Editor editor_(argv[1]);
        editor_.Run();
    } 
    catch (...)
    {
        CUtils::Notification::notify(CUtils::Notification::TYPE_ERROR,
                                     "This is a critical error! \
                The engine is unlikely to be able to work on your device yet :(");
    }

    return 0;
}
