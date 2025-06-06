// Copyright (c) 2025 Case Technologies

#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class Module;
            class List;

            static constexpr char PythonHome[] = "PYTHONHOME";

            class Initializer
            {
            public:
                static Initializer& GetDefaultInitializer();

                Initializer& SetHome(const std::string& home_a);
                std::string GetHome() const;

            private:
                std::unordered_map<std::string, std::string> envs_;

                static Initializer default_initializer_;
            };

            class Python
            {
            public:
                static Python& GetInstance();

                Python(const Initializer& initializer_a =
                               Initializer::GetDefaultInitializer());
                ~Python();

                void SetArgs(std::vector<std::wstring> argv_a);
                void SetArgs(std::vector<std::string> argv_a);

                List GetPath() const;
                void SetPath(const List& list_a);
                void SetPath(const std::string& path_a);
                void AppendPath(const std::wstring& path_a);
                void AppendPath(const std::string& path_a);

                std::string GetHome() const;
                void SetHome(const std::wstring& path_a) const;
                void SetHome(const std::string& path_a) const;

                Module LoadModule(const std::string& path_a);
                Module ReloadModule(Module& module_a);
                Module RegisterModule(const std::string& name_a);
                Module GetModule(const std::string& name_a);

                void RedirectOutput(const Module& module_a,
                                    const std::string& stream_name_a = "stdout");

                void FlushStdout();
                void FlushStderr();
                void Flush();

                void CheckErrors();

            public:
                void* user_ptr_ = nullptr;
            };
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel