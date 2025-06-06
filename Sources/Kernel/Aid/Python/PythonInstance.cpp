#include "PythonInstance.hpp"

#include "PythonClass.hpp"
#include "PythonCppCommon.hpp"
#include "PythonTuple.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class InstanceSetter : public Setter
            {
            public:
                InstanceSetter(Instance& instance_a, 
                        const std::string& var_name_a)
                    : instance_(instance_a), var_name_(var_name_a)
                {}

                void Set(const Object& data_a) override
                {
                    this->instance_.SetVariable(this->var_name_, data_a);
                }

            private:
                Instance& instance_;
                std::string var_name_;
            };

            Object Instance::CreateNew(const Class& cls_a,
                                       const std::vector<Object>& params_a)
            {
                Tuple tuple_(params_a);
                return Object(PyObject_CallObject(PO(cls_a.cls_), PO(tuple_)));
            }

            Instance::Instance(const Instance& inst_a)
                : Object(DeepCopy(PO(inst_a)))
            {}

            Instance::Instance(Instance&& inst_a) : Object(inst_a.data_)
            {
                inst_a.data_ = nullptr;
            }

            Instance::Instance(Object&& data_a) : Object(std::move(data_a))
            {
                data_a.data_ = nullptr;
            }

            Instance::Instance(const Object& data_a) : Object(PO(data_a))
            {}

            Instance::Instance(const Class& cls_a, std::vector<Object> params_a)
                : Object(CreateNew(cls_a, params_a))
            {
                CheckError();
            }

            Instance& Instance::operator=(Instance&& inst_a)
            {
                this->data_ = std::move(inst_a.data_);
                inst_a.data_ = nullptr;
                return *this;
            }

            Object Instance::Call(const std::string& func_a) const
            {
                PyObject* result_ = PyObject_CallMethod(PO(this->data_),
                                                       func_a.c_str(),
                                                       NULL);
                CheckError();
                return Object(result_);
            }

            Object Instance::Call(const std::string& func_a, Object params_a) const
            {
                PyObject* keywords_ = PyDict_New();
                PyObject* method_ = PyObject_GetAttrString(PO(this->data_),
                                                          func_a.c_str());
                PyObject* result_ = PyObject_Call(method_,
                                                 PO(params_a.data_),
                                                 keywords_);
                CheckError();
                return Object(result_);
            }

            Object Instance::Call(const std::string& func_a,
                                  const std::vector<Object>& params_a) const
            {
                Tuple tuple_(params_a);
                PyObject* keywords_ = PyDict_New();
                CheckError();

                PyObject* method_ = PyObject_GetAttrString(PO(this->data_),
                                                          func_a.c_str());
                CheckError();

                PyObject* result_ = PyObject_Call(method_,
                                                 PO(tuple_.data_),
                                                 keywords_);
                CheckError();

                return Object(result_);
            }

            Object Instance::Call(
                    const std::string& func_a,
                    const std::initializer_list<Object>& params_a) const
            {
                Tuple tuple_(params_a);
                PyObject* keywords_ = PyDict_New();
                CheckError();

                PyObject* method_ = PyObject_GetAttrString(PO(this->data_),
                                                          func_a.c_str());
                CheckError();

                PyObject* result_ = PyObject_Call(method_,
                                                 PO(tuple_.data_),
                                                 keywords_);
                CheckError();

                return Object(result_);
            }

            bool Instance::Contains(const std::string& name_a) const
            {
                bool ret_ = PyObject_HasAttrString(PO(this->data_), name_a.c_str());
                CheckError();
                return ret_;
            }

            Object Instance::GetVariable(const std::string& name_a)
            {
                PyObject* var_ = PyObject_GetAttrString(PO(this->data_),
                                                       name_a.c_str());
                CheckError();
                return Object(var_, new InstanceSetter(*this, name_a));
            }

            void Instance::SetVariable(const std::string& name_a, Object variable_a)
            {
                PyObject_SetAttrString(PO(this->data_),
                                       name_a.c_str(),
                                       PO(variable_a));
                CheckError();
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel