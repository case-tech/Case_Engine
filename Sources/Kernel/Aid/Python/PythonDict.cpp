#include "PythonDict.hpp"

#include "PythonCppCommon.hpp"
#include "PythonException.hpp"
#include "PythonModule.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            Dict::Iterator::Iterator(const Dict& dict_a, int ret_a)
                : dict_(dict_a), pos_(0), ret_(ret_a)
            {}

            Dict::Iterator::Iterator(const Dict& dict_a) : dict_(dict_a), pos_(0)
            {
                this->Next();
            }

            Dict::Iterator::Iterator(const Iterator& it_a)
                : dict_(it_a.dict_), ret_(it_a.ret_), pos_(it_a.pos_), key_(it_a.key_),
                  val_(it_a.val_)
            {}

            Dict::Iterator::~Iterator()
            {}

            Dict::Iterator Dict::Iterator::End(const Dict& dict_a)
            {
                return Dict::Iterator(dict_a, 0);
            }

            void Dict::Iterator::Next()
            {
                if (PyDict_Size(PO(dict_.data_)) > 0)
                {
                    this->ret_ = PyDict_Next(
                            PO(dict_.data_),
                            reinterpret_cast<Py_ssize_t*>(&this->pos_),
                            reinterpret_cast<PyObject**>(&this->key_),
                            reinterpret_cast<PyObject**>(&this->val_.data_));
                    Py_INCREF(this->val_.data_);
                } 
                else
                    this->ret_ = 0;

                CheckError();
            }

            Dict::Iterator& Dict::Iterator::operator++()
            {
                this->Next();
                return *this;
            }

            Dict::Iterator& Dict::Iterator::operator++(int)
            {
                Dict::Iterator& it_ = *this;
                this->operator++();
                return it_;
            }

            bool Dict::Iterator::operator==(const Iterator& it_a)
            {
                return this->dict_ == it_a.dict_ && this->ret_ == it_a.ret_;
            }

            bool Dict::Iterator::operator!=(const Iterator& it_a)
            {
                return !(*this == it_a);
            }

            Object& Dict::Iterator::operator*()
            {
                return this->val_;
            }

            Object* Dict::Iterator::operator->()
            {
                return &this->val_;
            }

            std::string Dict::Iterator::GetKey() const
            {
                return PyObjToString(this->key_);
            }

            class DictSetter : public Setter
            {
            public:
                DictSetter(Object& container_a, const Object& key_a)
                    : container_(container_a), key_(key_a)
                {}

                void Set(const Object& data_a) override
                {
                    PyDict_SetItem(PO(this->container_),
                                   PO(this->key_),
                                   PO(data_a));
                    CheckError();
                }

            private:
                Object& container_;
                const Object& key_;
            };

            Dict::Dict() : Object(PyDict_New())
            {
                CheckError();
            }

            Dict::Dict(PyObj data_a) : Object(data_a)
            {}

            Dict::Dict(const Dict& data_a) : Object(DeepCopy(PO(data_a.data_)))
            {
                if (this->data_)
                    Py_INCREF(this->data_);
            }

            Dict::Dict(Object&& data_a) noexcept
            {
                this->data_ = std::move(data_a.data_);
                data_a.data_ = nullptr;
            }

            Dict::Dict(Dict&& data_a) noexcept
            {
                this->data_ = std::move(data_a.data_);
                data_a.data_ = nullptr;
            }

            Dict::Dict(const std::unordered_map<std::string, Object>& data_a)
                : Dict()
            {
                for (const auto& pair_ : data_a)
                {
                    (*this)[pair_.first] = pair_.second;
                }
            }

            Dict& Dict::operator=(Object&& data_a) noexcept
            {
                this->data_ = std::move(data_a.data_);
                data_a.data_ = nullptr;
                return *this;
            }

            Dict& Dict::operator=(Dict&& data_a) noexcept
            {
                this->data_ = std::move(data_a.data_);
                data_a.data_ = nullptr;
                return *this;
            }

            void Dict::Append(const std::string& key_a, Object& data_a)
            {
                PyMapping_SetItemString(PO(this->data_),
                                        key_a.c_str(),
                                        PO(data_a.data_));
                CheckError();
            }

            void Dict::Append(const std::string& key_a, const Object& data_a)
            {
                PyMapping_SetItemString(PO(this->data_),
                                        key_a.c_str(),
                                        PO(data_a.data_));
                CheckError();
            }

            void Dict::Append(const std::string& key_a, Object&& data_a)
            {
                PyMapping_SetItemString(PO(this->data_),
                                        key_a.c_str(),
                                        PO(data_a.data_));
                CheckError();
            }

            void Dict::Append(const std::string& key_a, Module& module_a)
            {
                PyMapping_SetItemString(PO(this->data_),
                                        key_a.c_str(),
                                        PO(module_a.module_));
                CheckError();
            }

            bool Dict::Contains(const Object& key_a)
            {
                bool ret_ = PyDict_Contains(PO(this->data_), PO(key_a.data_));
                CheckError();
                return ret_;
            }

            Object Dict::operator[](const Object& key_a)
            {
                if (!PyDict_Contains(PO(this->data_), PO(key_a)))
                    PyDict_SetItem(PO(this->data_), PO(key_a), PO(Object::None()));
                CheckError();
                PyObject* pydata_ = PyDict_GetItem(PO(this->data_), PO(key_a));
                CheckError();

                Object ret_(pydata_, new DictSetter(*this, key_a));
                CheckError();
                return ret_;
            }

            const Object Dict::operator[](const Object& key_a) const
            {
                if (!PyDict_Contains(PO(this->data_), PO(key_a)))
                    throw PythonOutOfBoundsException(
                            std::string("Dictionary doesn't contain element ")
                            + key_a.ToString());
                CheckError();
                PyObject* pydata_ = PyDict_GetItem(PO(this->data_), PO(key_a));
                CheckError();

                Object ret_(pydata_);
                CheckError();
                return ret_;
            }

            Object Dict::At(const Object& key_a) const
            {
                if (!PyDict_Contains(PO(this->data_), PO(key_a)))
                    throw PythonOutOfBoundsException("Key \"" + key_a.ToString()
                                                     + "\" doesn't exist");
                Object ret_(PyDict_GetItem(PO(this->data_), PO(key_a)));
                CheckError();
                return ret_;
            }

            size_t Dict::GetSize() const
            {
                size_t ret_ = PyDict_Size(PO(this->data_));
                CheckError();
                return ret_;
            }

            Dict::Iterator Dict::Begin()
            {
                return Iterator(*this);
            }

            Dict::Iterator Dict::End()
            {
                return Iterator::End(*this);
            }

            Dict::Iterator Dict::Begin() const
            {
                return Iterator(*this);
            }

            Dict::Iterator Dict::End() const
            {
                return Iterator::End(*this);
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel