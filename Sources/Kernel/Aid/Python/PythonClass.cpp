#include "PythonClass.hpp"

#include "PythonCppCommon.hpp"
#include "PythonFunction.hpp"
#include "PythonInstance.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            Class::Class(PyObj cls_a) : cls_(cls_a)
            {}

            Instance Class::CreateInstance(std::vector<Object> params_a) const
            {
                return Instance(*this, params_a);
            }

            bool Class::Contains(const std::string& name_a) const
            {
                bool ret_ = PyObject_HasAttrString(PO(this->cls_), name_a.c_str());
                CheckError();
                return ret_;
            }

            Function Class::GetFunction(const std::string& name_a) const
            {
                PyObject* function_ = PyObject_GetAttrString(PO(this->cls_),
                                                            name_a.c_str());
                CheckError();

                return Function(function_);
            }

            std::string Class::GetDocstring() const
            {
                Object obj_ = Object(
                        PyObject_GetAttrString(PO(this->cls_), "__doc__"));
                return obj_.ToString();
            }

            std::string Class::GetName() const
            {
                Object obj_ = Object(
                        PyObject_GetAttrString(PO(this->cls_), "__name__"));
                return obj_.ToString();
            }

            PyObj Class::GetType() const
            {
                PyObj ret_ = PyObject_Type(PO(this->cls_));
                CheckError();
                return ret_;
            }

            bool Class::IsHeapType() const
            {
                PyTypeObject* type_ = reinterpret_cast<PyTypeObject*>(this->cls_);
                return type_->tp_flags & Py_TPFLAGS_HEAPTYPE;
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel