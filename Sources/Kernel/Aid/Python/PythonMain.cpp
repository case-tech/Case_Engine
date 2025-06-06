#include "PythonMain.hpp"

#include "PythonBuilders.hpp"
#include "PythonCppCommon.hpp"
#include "PythonException.hpp"
#include "PythonList.hpp"
#include "PythonModule.hpp"

#include <cassert>
#include <cstdlib>
#include <stdlib.h>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            std::string MakeEnv(const std::string& name_a,
                                const std::string& value_a)
            {
                return name_a + "=" + value_a;
            }

            void SetEnv(const std::string& name_a, const std::string& value_a)
            {
                std::string e_ = MakeEnv(name_a, value_a);
                int res_ = putenv(&e_[0]);
                assert(res_ == 0);
                (void)res_;
            }

            constexpr size_t ObjSize()
            {
                return sizeof(PyObject);
            }

            Initializer Initializer::default_initializer_;

            Initializer& Initializer::GetDefaultInitializer()
            {
                return default_initializer_;
            }

            Initializer& Initializer::SetHome(const std::string& home_a)
            {
                this->envs_[PythonHome] = home_a;
                return *this;
            }

            std::string Initializer::GetHome() const
            {
                auto it_ = this->envs_.find(PythonHome);
                if (it_ != this->envs_.end())
                    return it_->second;
                return "";
            }

            Python::Python(const Initializer& initializer_a)
            {
                SetEnv(PythonHome, initializer_a.GetHome());

                Py_Initialize();
                CheckError();
            }

            Python::~Python()
            {
                if (Py_IsInitialized())
                {
                    CheckError();
                    Py_Finalize();
                }
            }

            Python& Python::GetInstance()
            {
                static Python py_;
                return py_;
            }

            void Python::SetArgs(std::vector<std::wstring> argv_a)
            {
                std::vector<wchar_t*> wargv_;
                for (std::wstring& str : argv_a)
                {
                    wargv_.push_back(&str[0]);
                }

                PySys_SetArgvEx(static_cast<int>(argv_a.size()),
                                wargv_.data(),
                                0);
                CheckError();
            }

            void Python::SetArgs(std::vector<std::string> argv_a)
            {
                std::vector<wchar_t*> wargv_;
                for (std::string& str_ : argv_a)
                {
                    std::wstring wstr_ = ConvString(str_);
                    wargv_.push_back(&wstr_[0]);
                }
                PySys_SetArgvEx(static_cast<int>(argv_a.size()),
                                wargv_.data(),
                                0);
                CheckError();
            }

            List Python::GetPath() const
            {
                return List(PySys_GetObject("path"));
            }

            void Python::SetPath(const List& list_a)
            {
                PySys_SetObject("path", PO(list_a));
            }

            void Python::SetPath(const std::string& path_a)
            {
                List l_;
                l_.PushItem(path_a);
                this->SetPath(l_);
            }

            void Python::AppendPath(const std::wstring& path_a)
            {
                PyObject* sys_path_ = PySys_GetObject("path");
                PyList_Append(sys_path_,
                              PyUnicode_FromWideChar(path_a.c_str(),
                                                     path_a.size()));
                CheckError();
            }

            void Python::AppendPath(const std::string& path_a)
            {
                this->AppendPath(ConvString(path_a));
            }

            std::string Python::GetHome() const
            {
                wchar_t* res_ = Py_GetPythonHome();

                if (res_)
                    return ConvString(res_);

                return "";
            }

            void Python::SetHome(const std::wstring& path_a) const
            {
                Py_SetPythonHome(path_a.c_str());
            }

            void Python::SetHome(const std::string& path_a) const
            {
                this->SetHome(ConvString(path_a));
            }

            Module Python::LoadModule(const std::string& path_a)
            {
                PyObject* pymod_ = PyImport_ImportModule(path_a.c_str());
                CheckError();

                return Module(pymod_);
            }

            Module Python::ReloadModule(Module& module_a)
            {
                PyObject* obj_ = PyImport_ReloadModule(PO(module_a));
                CheckError();
                return Module(obj_);
            }

            Module Python::RegisterModule(const std::string& name_a)
            {
                PyObject* mod_ = PyImport_AddModule(name_a.c_str());
                CheckError();

                return Module(mod_);
            }

            Module Python::GetModule(const std::string& name_a)
            {
                PyObject* pypath_ = PyUnicode_FromString(name_a.c_str());
                CheckError();

                PyObject* mod_ = PyImport_GetModule(pypath_);
                CheckError();

                Py_DECREF(pypath_);
                CheckError();

                return Module(mod_);
            }

            void Python::RedirectOutput(const Module& module_a,
                                        const std::string& stream_name_a)
            {
                assert("Printing module should define 'write' and 'flush' "
                       "functions to be able to print correctly. "
                       "Use 'registerPrintFunction' function on ModuleBuilder "
                       "to register the functions."
                       && module_a.Exists("write") && module_a.Exists("flush"));

                PySys_SetObject(stream_name_a.c_str(), PO(module_a));
                CheckError();
            }

            void Python::FlushStdout()
            {
                PyObject* pystdout_ = PySys_GetObject("stdout");
                PyObject* strflush_ = PyUnicode_FromString("flush");
                PyObject_CallMethodNoArgs(pystdout_, strflush_);
                CheckError();
            }

            void Python::FlushStderr()
            {
                PyObject* pystdout_ = PySys_GetObject("stderr");
                PyObject* strflush_ = PyUnicode_FromString("flush");
                PyObject_CallMethodNoArgs(pystdout_, strflush_);
                CheckError();
            }

            void Python::Flush()
            {
                this->FlushStdout();
                this->FlushStderr();
            }

            void Python::CheckErrors()
            {
                CheckError();
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel