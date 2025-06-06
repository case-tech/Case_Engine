#include "PythonList.hpp"

#include "PythonCppCommon.hpp"
#include "PythonException.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            List::Iterator::Iterator(List& list_a) : list_(list_a), idx_(0)
            {
                this->Cache();
            }

            List::Iterator::Iterator(const Iterator& it_a)
                : list_(it_a.list_), idx_(it_a.idx_), val_(it_a.val_)
            {}

            List::Iterator List::Iterator::End(List& list_a)
            {
                List::Iterator it_(list_a);
                it_.idx_ = list_a.GetSize();
                return it_;
            }

            void List::Iterator::Cache()
            {
                if (PyList_Size(PO(this->list_.data_)) > 0)
                    this->val_ = Object(
                            PyList_GetItem(PO(this->list_.data_), this->idx_));

                CheckError();
            }

            List::Iterator& List::Iterator::operator++()
            {
                this->idx_++;
                return *this;
            }

            List::Iterator List::Iterator::operator++(int)
            {
                List::Iterator it_ = *this;
                this->operator++();
                return it_;
            }

            bool List::Iterator::operator==(const Iterator& it_a)
            {
                return this->list_ == it_a.list_ && this->idx_ == it_a.idx_;
            }

            bool List::Iterator::operator!=(const Iterator& it_a)
            {
                return !(*this == it_a);
            }

            Object List::Iterator::operator*()
            {
                this->Cache();
                return this->val_;
            }

            Object* List::Iterator::operator->()
            {
                this->Cache();
                return &this->val_;
            }

            class ListSetter : public Setter
            {
            public:
                ListSetter(Object& container_a, unsigned int key_a)
                    : container_(container_a), key_(key_a)
                {}

                void Set(const Object& data_a) override
                {
                    PyList_SetItem(PO(this->container_), this->key_, PO(data_a));
                    CheckError();
                }

            private:
                Object& container_;
                unsigned int key_;
            };

            List::List() : Object(PyList_New(0))
            {
                CheckError();
            }

            List::List(PyObj data_a) : Object(data_a)
            {}

            List::List(std::vector<Object> data_a)
                : Object(PyList_New(data_a.size()))
            {
                CheckError();
                unsigned int i_ = 0;
                for (Object d_ : data_a)
                {
                    PyList_SetItem(PO(this->data_), i_++, PO(d_));
                    CheckError();
                }
            }

            List::List(const List& data_a) : Object(DeepCopy(PO(data_a.data_)))
            {}

            List::List(Object&& data_a) noexcept
            {
                this->data_ = std::move(data_a.data_);
                data_a.data_ = nullptr;
            }

            List::List(List&& data_a) noexcept
            {
                this->data_ = std::move(data_a.data_);
                data_a.data_ = nullptr;
            }

            List& List::operator=(Object&& data_a) noexcept
            {
                this->data_ = std::move(data_a.data_);
                data_a.data_ = nullptr;
                return *this;
            }

            List& List::operator=(List&& data_a) noexcept
            {
                this->data_ = std::move(data_a.data_);
                data_a.data_ = nullptr;
                return *this;
            }

            size_t List::GetSize() const
            {
                size_t ret_ = PyList_Size(PO(this->data_));
                CheckError();
                return ret_;
            }

            const Object List::operator[](unsigned int idx_a) const
            {
                if (idx_a >= this->GetSize())
                    throw PythonOutOfBoundsException(
                            std::string("List out of bounds, requested index ")
                            + std::to_string(idx_a) + ", size "
                            + std::to_string(this->GetSize()));
                CheckError();

                Object ret_ = Object(PyList_GetItem(PO(this->data_), idx_a));
                CheckError();
                return ret_;
            }

            Object List::operator[](unsigned int idx_a)
            {
                if (idx_a >= this->GetSize())
                    throw PythonOutOfBoundsException(
                            std::string("List out of bounds, requested index ")
                            + std::to_string(idx_a) + ", size "
                            + std::to_string(this->GetSize()));
                CheckError();

                Object ret_ = Object(PyList_GetItem(PO(this->data_), idx_a),
                                    new ListSetter(*this, idx_a));
                CheckError();
                return ret_;
            }

            void List::PushItem(Object item_a)
            {
                PyList_Append(PO(this->data_), PO(item_a.data_));
                CheckError();
            }

            List::Iterator List::begin()
            {
                return Iterator(*this);
            }

            List::Iterator List::end()
            {
                return Iterator::End(*this);
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel