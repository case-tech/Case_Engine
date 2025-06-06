#include "PythonObject.hpp"

#include "PythonCppCommon.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            size_t GetReferenceCount(PyObj obj_a)
            {
                return Py_REFCNT(obj_a);
            }

            void DecrementReferenceCount(PyObj obj_a)
            {
                Py_DECREF(obj_a);
            }

            void IncrementReferenceCount(PyObj obj_a)
            {
                Py_INCREF(obj_a);
            }

            void Setter::Set(const std::string& data_a)
            {
                PyObject* obj_ = PyUnicode_FromString(data_a.c_str());
                CheckError();
                this->Set(Object(obj_));
            }

            void Setter::Set(int data_a)
            {
                PyObject* obj_ = PyLong_FromLong(data_a);
                CheckError();
                this->Set(Object(obj_));
            }

            void Setter::Set(unsigned int data_a)
            {
                PyObject* obj_ = PyLong_FromUnsignedLong(data_a);
                CheckError();
                this->Set(Object(obj_));
            }

            void Setter::Set(long data_a)
            {
                PyObject* obj_ = PyLong_FromLong(data_a);
                CheckError();
                this->Set(Object(obj_));
            }

            void Setter::Set(unsigned long data_a)
            {
                PyObject* obj_ = PyLong_FromUnsignedLong(data_a);
                CheckError();
                this->Set(Object(obj_));
            }

            void Setter::Set(float data_a)
            {
                PyObject* obj_ = PyFloat_FromDouble(data_a);
                CheckError();
                this->Set(Object(obj_));
            }

            void Setter::Set(double data_a)
            {
                PyObject* obj_ = PyFloat_FromDouble(data_a);
                CheckError();
                this->Set(Object(obj_));
            }

            class DefaultDataSetter : public Setter
            {
            public:
                DefaultDataSetter(Object& data_a) : data_(data_a)
                {}

                virtual void Set(const Object& data_a) override
                {
                    if (this->data_)
                        Py_DECREF(PO(this->data_));
                    this->data_.data_ = data_a.data_;
                }

            private:
                Object& data_;
            };

            class DefaultSynchronizer : public Synchronizer
            {
            public:
                virtual void Synchronize(PyObj&) override
                {}
            };

            Object::Object()
                : data_(nullptr), setter_(new DefaultDataSetter(*this)),
                  synchronizer_(new DefaultSynchronizer())
            {}

            Object::Object(const Object& data_a)
                : data_(DeepCopy(PO(data_a.data_))),
                  setter_(new DefaultDataSetter(*this)),
                  synchronizer_(new DefaultSynchronizer())
            {
                assert(!this->data_ || Py_REFCNT(this->data_) >= 1);
            }

            Object::Object(Object&& data_a) noexcept
                : data_(std::move(data_a.data_)), setter_(std::move(data_a.setter_)),
                  synchronizer_(std::move(data_a.synchronizer_))
            {
                data_a.data_ = nullptr;
            }

            Object::Object(PyObj data_a)
                : data_(data_a), setter_(new DefaultDataSetter(*this)),
                  synchronizer_(new DefaultSynchronizer())
            {
                if (data_a)
                    Py_INCREF(data_a);
            }

            Object::Object(PyObj data_a,
                           Setter* setter_a,
                           Synchronizer* synchronizer_a)
                : data_(data_a), setter_(setter_a),
                  synchronizer_(synchronizer_a ? synchronizer_a
                                            : new DefaultSynchronizer())
            {
                if (data_a)
                    Py_INCREF(data_a);
            }

            Object::Object(const std::string& data)
                : data_(PyUnicode_FromString(data.c_str())),
                  setter_(new DefaultDataSetter(*this)),
                  synchronizer_(new DefaultSynchronizer())
            {
                assert(Py_REFCNT(this->data_) >= 1);
                CheckError();
            }

            Object::Object(const char* data) : Object(std::string(data))
            {}

            Object::Object(int data)
                : data_(PyLong_FromLong(data)),
                  setter_(new DefaultDataSetter(*this)),
                  synchronizer_(new DefaultSynchronizer())
            {
                assert(Py_REFCNT(this->data_) >= 1);
                CheckError();
            }

            Object::Object(unsigned int data)
                : data_(PyLong_FromUnsignedLong(data)),
                  setter_(new DefaultDataSetter(*this)),
                  synchronizer_(new DefaultSynchronizer())
            {
                assert(Py_REFCNT(this->data_) >= 1);
                CheckError();
            }

            Object::Object(long data)
                : data_(PyLong_FromLong(data)),
                  setter_(new DefaultDataSetter(*this)),
                  synchronizer_(new DefaultSynchronizer())
            {
                assert(Py_REFCNT(this->data_) >= 1);
                CheckError();
            }

            Object::Object(unsigned long data)
                : data_(PyLong_FromUnsignedLong(data)),
                  setter_(new DefaultDataSetter(*this)),
                  synchronizer_(new DefaultSynchronizer())
            {
                assert(Py_REFCNT(this->data_) >= 1);
                CheckError();
            }

            Object::Object(float data)
                : data_(PyFloat_FromDouble(data)),
                  setter_(new DefaultDataSetter(*this)),
                  synchronizer_(new DefaultSynchronizer())
            {
                assert(Py_REFCNT(this->data_) >= 1);
                CheckError();
            }

            Object::Object(double data)
                : data_(PyFloat_FromDouble(data)),
                  setter_(new DefaultDataSetter(*this)),
                  synchronizer_(new DefaultSynchronizer())
            {
                assert(Py_REFCNT(this->data_) >= 1);
                CheckError();
            }

            Object& Object::operator=(const Object& data)
            {
                if (data.data_)
                    Py_INCREF(data.data_);
                this->setter_->Set(data);
                return *this;
            }

            Object& Object::operator=(const std::string& data)
            {
                this->setter_->Set(data);
                return *this;
            }

            Object& Object::operator=(const char* data)
            {
                return (*this = std::string(data));
            }

            Object& Object::operator=(int data)
            {
                this->setter_->Set(data);
                return *this;
            }

            Object& Object::operator=(unsigned int data)
            {
                this->setter_->Set(data);
                return *this;
            }

            Object& Object::operator=(long data)
            {
                this->setter_->Set(data);
                return *this;
            }

            Object& Object::operator=(unsigned long data)
            {
                this->setter_->Set(data);
                return *this;
            }

            Object& Object::operator=(float data)
            {
                this->setter_->Set(data);
                return *this;
            }

            Object& Object::operator=(double data)
            {
                this->setter_->Set(data);
                return *this;
            }

            Object::~Object()
            {
                if (this->data_ && !this->IsNone())
                    Py_DECREF(this->data_);
            }

            PyObj Object::GetType() const
            {
                PyObj ret_ = PyObject_Type(PO(this->data_));
                CheckError();
                return ret_;
            }

            Object::operator PyObj() const
            {
                if (this->data_)
                    Py_INCREF(this->data_);
                return this->data_;
            }

            std::string Object::ToString() const
            {
                this->synchronizer_->Synchronize(this->data_);
                std::string s_ = PyObjToString(this->data_);
                CheckError();
                return s_;
            }

            long Object::ToLong() const
            {
                this->synchronizer_->Synchronize(this->data_);
                long ret_ = PyLong_AsLong(PO(this->data_));
                CheckError();
                return ret_;
            }

            double Object::ToDouble() const
            {
                this->synchronizer_->Synchronize(this->data_);
                double ret_ = PyFloat_AsDouble(PO(this->data_));
                CheckError();
                return ret_;
            }

            bool Object::ToBool() const
            {
                this->synchronizer_->Synchronize(this->data_);
                bool ret_ = this->data_ && this->ToLong();
                CheckError();
                return ret_;
            }

            std::string Object::Repr() const
            {
                this->synchronizer_->Synchronize(this->data_);
                return PyObjRepr(this->data_);
            }

            bool Object::IsNone() const
            {
                this->synchronizer_->Synchronize(this->data_);
                return this->data_ == Py_None;
            }

            bool Object::IsNull() const
            {
                return !this->data_;
            }

            unsigned int Object::GetReferenceCount() const
            {
                return static_cast<unsigned int>(Py_REFCNT(this->data_));
            }

            Object Object::None()
            {
                Object d_;
                d_.data_ = Py_None;
                return d_;
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel