#include "Editor.hpp"

namespace Editor
<<<<<<< HEAD
{
    Editor::Editor()
    {
       if (check_sys_.Checking())
       {
           std::cout << "SUCCESS";
       }
       else
       {
            CUtils::Notification::notify(CUtils::Notification::TYPE_ERROR,
                    "Your device does not have enough RAM < 2048 MB");
       }
    }

    Editor::~Editor()
    {

    }

    bool Editor::Run()
    {
      return true;
    }
}

int main()
{
    try
    {
        Editor::Editor editor_;
        editor_.Run();
    } 
    catch(...)
    {
        CUtils::Notification::notify(CUtils::Notification::TYPE_ERROR, 
                "This is a critical error! \
                The engine is unlikely to be able to work on your device yet :(");
=======
{
    Editor::Editor(std::string& path_to_project_a) : 
        path_to_project_(path_to_project_a) {}

    Editor::~Editor() {}

    bool Editor::Run()
    {
        if (check_sys_.Checking()) 
        {
            if (pr_manager_.Open(path_to_project_)) 
            {
                sandbox_.Start(" ");
            }
        }

        return true;
    }
} // namespace Editor

int main(int argc, char* argv[])
{
    if (argc > 1) 
    {
        std::string arg_ = argv[1];
        Editor::Editor editor_(arg_);
        editor_.Run();
    }
    else 
    {
        CUtils::Notification::notify(CUtils::Notification::TYPE_ERROR,
                "Specify the correct path to the project file");
>>>>>>> aa4b252 (Add open project)
    }

    return 0;
}
