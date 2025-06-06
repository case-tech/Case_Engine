#include "PythonException.hpp"

#include "PythonCppCommon.hpp"
#include "PythonFunction.hpp"
#include "PythonList.hpp"
#include "PythonMain.hpp"
#include "PythonModule.hpp"
#include "PythonTuple.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            inline std::string FormatError2(PyObject* type_a,
                                            PyObject* values_a,
                                            PyObject* trace_a)
            {
                PyObject* traceback_str_ = PyUnicode_FromString("traceback");
                PyObject* traceback = PyImport_Import(traceback_str_);
                Py_DECREF(traceback_str_);

                PyObject* format_exception_ =
                        PyObject_GetAttrString(traceback, "format_exception");
                PyObject* res_ = PyObject_CallFunctionObjArgs(format_exception_,
                                                             type_a,
                                                             values_a,
                                                             trace_a,
                                                             NULL);
                std::string formatted_ = PyObjToString(res_);

                return formatted_;
            }

            inline std::string GetErrorString(PyObject* type_a, PyObject* values_a)
            {
                std::string result_;
                result_ += PyObjToString(type_a) + ": ";
                result_ += PyObjToString(values_a);
                return result_;
            }

            inline std::string FormatError(PyObject* type_a,
                                           PyObject* values_a,
                                           PyObject* trace_a)
            {
                if (!trace_a)
                    return GetErrorString(type_a, values_a);

                static bool guard_ = false;
                if (guard_)
                {
                    guard_ = false;
                    return GetErrorString(type_a, values_a);
                }

                guard_ = true;

                Python& py_ = Python::GetInstance();
                Module traceback_ = py_.LoadModule("traceback");
                Function format_exception_ = traceback_.GetFunction(
                        "format_exception");
                Tuple args_(std::vector<PyObj> {type_a, values_a, trace_a});
                List list_ = format_exception_.Call(args_);

                std::string result_;
                for (Object d : list_)
                {
                    result_ += d.ToString() + "\n";
                }

                guard_ = false;
                return result_;
            }

            void checkError()
            {
                if (PyErr_Occurred())
                {
                    PyObject* type_;
                    PyObject* values_;
                    PyObject* trace_;

                    PyErr_Fetch(&type_, &values_, &trace_);
                    PyErr_NormalizeException(&type_, &values_, &trace_);

                    throw PythonErrorException(type_, values_, trace_);
                }
            }

            PythonErrorException::PythonErrorException(PyObj type_a,
                                                       PyObj values_a,
                                                       PyObj trace_a)
                : PythonException(FormatError(PO(type_a), PO(values_a), PO(trace_a))),
                  type_(type_a), values_(values_a), trace_(trace_a)
            {}

            std::string PythonErrorException::GetExceptionType() const
            {
                return PyObjToString(this->type_);
            }

            std::string PythonErrorException::GetExceptionValue() const
            {
                return PyObjToString(this->values_);
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel