#include "PythonType.hpp"

#include "PythonCppCommon.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            Type::Type(const PyObj& data_a) : Object(PyObject_Type(PO(data_a)))
            {
                CheckError();
            }

            std::string Type::ToString() const
            {
                return PT(this->data_)->tp_name;
            }

            bool Type::IsNone() const
            {
                return this->ToString() == TypeNone;
            }

            bool Type::IsString() const
            {
                return this->ToString() == TypeString;
            }

            bool Type::IsInt() const
            {
                return this->ToString() == TypeInt;
            }

            bool Type::IsFloat() const
            {
                return this->ToString() == TypeFloat;
            }

            bool Type::IsList() const
            {
                return this->ToString() == TypeList;
            }

            bool Type::IsDict() const
            {
                return this->ToString() == TypeDict;
            }

            bool Type::IsFunction() const
            {
                return this->ToString() == TypeFunction;
            }

            bool Type::IsType() const
            {
                return this->ToString() == TypeType;
            }

            bool Type::IsModule() const
            {
                return this->ToString() == TypeModule;
            }

            bool Type::operator==(const Type& b_a) const
            {
                return this->ToString() == b_a.ToString();
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel