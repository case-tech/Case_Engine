#include "PythonFunction.hpp"

#include "PythonCppCommon.hpp"
#include "PythonTuple.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            Function::Function(PyObj function_a) : function_(function_a)
            {}

            Function::~Function()
            {
                Py_DECREF(this->function_);
            }

            Object Function::Call()
            {
                PyObject* result_ = PyObject_CallObject(PO(this->function_),
                                                       nullptr);
                CheckError();
                return Object(result_);
            }

            Object Function::Call(const Tuple& params_a)
            {
                PyObject* result_ = PyObject_CallObject(PO(this->function_),
                                                       PO(params_a.data_));
                CheckError();
                return Object(result_);
            }

            Object Function::Call(std::vector<Object> params_a)
            {
                Tuple tuple_(params_a);
                return this->Call(tuple_);
            }

            Object Function::CallV(std::vector<Object> params_a)
            {
                return this->Call(params_a);
            }

            Object Function::CallRef(
                    std::vector<std::reference_wrapper<Object>> params_a)
            {
                Tuple tuple_(params_a);
                return this->Call(tuple_);
            }

            std::string Function::GetDocString() const
            {
                Object obj_ = Object(
                        PyObject_GetAttrString(PO(this->function_), "__doc__"));
                return obj_.ToString();
            }

            std::string Function::GetName() const
            {
                Object obj_ = Object(
                        PyObject_GetAttrString(PO(this->function_), "__name__"));
                return obj_.ToString();
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel