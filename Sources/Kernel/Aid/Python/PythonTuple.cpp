#include "PythonTuple.hpp"

#include "PythonCppCommon.hpp"
#include "PythonException.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class TupleSetter : public Setter
            {
            public:
                TupleSetter(Object& container_a, const unsigned int key_a)
                    : container_(container_a), key_(key_a)
                {}

                void Set(const Object& data_a) override
                {
                    PyTuple_SET_ITEM(PO(this->container_), this->key_, PO(data_a));
                    Py_INCREF(PO(data_a));
                    CheckError();
                }

            private:
                Object& container_;
                const unsigned int key_;
            };

            Tuple::Tuple(PyObj tuple_a) : Object(tuple_a)
            {}

            Tuple::Tuple(const Tuple& tuple_a) : Object(DeepCopy(PO(tuple_a.data_)))
            {}

            Tuple::Tuple(std::vector<Object> tuple_a) : Tuple(tuple_a.size())
            {
                for (unsigned int i_ = 0; i_ < tuple_a.size(); i_++)
                {
                    PyTuple_SET_ITEM(PO(this->data_), i_, PO(tuple_a[i_]));
                    CheckError();
                }
            }

            Tuple::Tuple(std::vector<std::reference_wrapper<Object>> tuple_a)
                : Tuple(tuple_a.size())
            {
                for (unsigned int i_ = 0; i_ < tuple_a.size(); i_++)
                {
                    Object& pydata_ = tuple_a[i_].get();
                    PyTuple_SET_ITEM(PO(this->data_), i_, PO(pydata_));
                    CheckError();
                }
            }

            Tuple::Tuple(std::vector<PyObj> tuple_a) : Tuple(tuple_a.size())
            {
                for (unsigned int i_ = 0; i_ < tuple_a.size(); i_++)
                {
                    PyTuple_SET_ITEM(PO(this->data_), i_, PO(tuple_a[i_]));
                    CheckError();
                }
            }

            Tuple::Tuple(Object&& data_a) noexcept
            {
                this->data_ = std::move(data_a.data_);
                data_a.data_ = nullptr;
            }

            Tuple::Tuple(Tuple&& data_a) noexcept
            {
                this->data_ = std::move(data_a.data_);
                data_a.data_ = nullptr;
            }

            Tuple::Tuple(size_t element_count_a)
                : Object(PyTuple_New(element_count_a))
            {
                CheckError();
            }

            size_t Tuple::GetSize() const
            {
                size_t ret_ = PyTuple_Size(PO(this->data_));
                CheckError();
                return ret_;
            }

            Object Tuple::operator[](unsigned int idx_a)
            {
                if (idx_a >= PyTuple_Size(PO(this->data_)))
                    throw PythonOutOfBoundsException("Tuple out of bounds");

                PyObject* pydata_ = PyTuple_GET_ITEM(PO(this->data_), idx_a);
                Object ret_(pydata_, new TupleSetter(*this, idx_a));
                CheckError();
                return ret_;
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel