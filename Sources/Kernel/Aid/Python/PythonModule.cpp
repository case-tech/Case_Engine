#include "PythonModule.hpp"

#include "PythonClass.hpp"
#include "PythonCppCommon.hpp"
#include "PythonDict.hpp"
#include "PythonException.hpp"
#include "PythonFunction.hpp"

#include <list>
#include <sstream>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            std::unordered_map<FunctionFlag, int> function_flag_map_ {
                    {FunctionFlag::ClassMethod, METH_CLASS},
                    {FunctionFlag::Coexist, METH_COEXIST},
                    {FunctionFlag::Fastcall, METH_FASTCALL},
                    {FunctionFlag::Keywords, METH_KEYWORDS},
                    {FunctionFlag::NoArgs, METH_NOARGS},
                    {FunctionFlag::OneArg, METH_O},
                    {FunctionFlag::StaticMethod, METH_STATIC},
                    {FunctionFlag::VarArgs, METH_VARARGS}};

            int TranslateFunctionFlag(FunctionFlag flag_a)
            {
                int res_ = 0x00;
                for (auto pair : function_flag_map_)
                {
                    if (!!(flag_a & pair.first))
                    {
                        res_ |= pair.second;
                    }
                }

                return res_;
            }

            struct PythonFunctionDefContainerImpl
                : public PythonFunctionDefContainer
            {
                std::vector<PyMethodDef> defs_;
            };

            struct PythonClassDefContainerImpl : public PythonClassDefContainer
            {
                struct Definition
                {
                    PyTypeObject object_;
                    std::vector<PyMethodDef> methods_;
                    std::vector<PyMemberDef> members_;
                };
                std::list<Definition> defs_;

                Definition& Create()
                {
                    return defs_.emplace_back();
                }
            };

            class ModuleSetter : public Setter
            {
            public:
                explicit ModuleSetter(Module& module_a,
                                      const std::string& var_name_a)
                    : module_(module_a), var_name_(var_name_a)
                {}

                virtual void Set(const Object& data_a) override
                {
                    this->module_.SetVariable(this->var_name_, data_a);
                }

            private:
                Module& module_;
                std::string var_name_;
            };

            class ModuleSynchronizer : public Synchronizer
            {
            public:
                explicit ModuleSynchronizer(Module& module_a,
                                            const std::string& var_name_a)
                    : module_(module_a), var_name_(var_name_a)
                {}

                virtual void Synchronize(PyObj& data_a) override
                {
                    PyObj newobj_ =
                            PyObject_GetAttrString(PO(this->module_),
                                                   this->var_name_.c_str());
                    if (newobj_ != data_a)
                    {
                        Py_DECREF(data_a);
                        data_a = newobj_;
                    }
                }

            private:
                Module& module_;
                std::string var_name_;
            };

            Module::Module(PyObj module_a, std::shared_ptr<Dict> globals_a)
                : module_(module_a),
                  locals_(module_a ? std::make_shared<Dict>(
                                          PyModule_GetDict(PO(module_a)))
                                : nullptr),
                  globals_(globals_a),
                  functions_(std::make_unique<PythonFunctionDefContainerImpl>()),
                  classes_(std::make_unique<PythonClassDefContainerImpl>())
            {
                if (this->module_)
                    Py_INCREF(this->module_);
            }

            Module::Module(const std::string& module_name_a)
                : module_(PyModule_New(module_name_a.c_str())),
                  locals_(std::make_shared<Dict>(PyModule_GetDict(PO(module_)))),
                  globals_(std::make_shared<Dict>()),
                  functions_(std::make_unique<PythonFunctionDefContainerImpl>()),
                  classes_(std::make_unique<PythonClassDefContainerImpl>())
            {}

            Module::Module(Module&& module_a)
                : module_(std::move(module_a.module_)),
                  locals_(std::move(module_a.locals_)),
                  globals_(std::move(module_a.globals_)),
                  functions_(std::move(module_a.functions_)),
                  classes_(std::move(module_a.classes_))
            {
                module_a.module_ = nullptr;
            }

            Module::~Module()
            {
                if (this->module_)
                {
                    assert(Py_REFCNT(this->module_) > 0);
                    Py_DECREF(this->module_);
                }
            }

            Module& Module::operator=(Module&& b_a)
            {
                this->module_ = std::move(b_a.module_);
                this->locals_ = std::move(b_a.locals_);
                this->globals_ = std::move(b_a.globals_);
                this->functions_ = std::move(b_a.functions_);
                this->classes_ = std::move(b_a.classes_);
                b_a.module_ = nullptr;
                return *this;
            }

            Module::operator PyObj() const
            {
                if (this->module_)
                    Py_INCREF(this->module_);
                return this->module_;
            }

            void Module::Reload()
            {
                this->module_ = PyImport_ReloadModule(PO(this->module_));
            }

            Function Module::CreateFunction(const std::string& name_a,
                                            const std::string& source_a)
            {
                PyRun_String(source_a.c_str(),
                             Py_file_input,
                             PO(this->globals_->data_),
                             PO(this->locals_->data_));
                CheckError();

                return this->GetFunction(name_a);
            }

            Function Module::GetFunction(const std::string& name_a)
            {
                PyObject* function_ = PyObject_GetAttrString(PO(this->module_),
                                                            name_a.c_str());
                CheckError();

                return Function(function_);
            }

            void Module::AddFunctions(
                    const std::vector<Types::FunctionDefinition>& definitions_a)
            {
                PythonFunctionDefContainerImpl& def_cnt_ =
                        static_cast<PythonFunctionDefContainerImpl&>(
                                *this->functions_);

                for (const Types::FunctionDefinition& def : definitions_a)
                {
                    def_cnt_.defs_.push_back(
                            {def.name_.c_str(),
                             reinterpret_cast<PyCFunction>(def.function_),
                             METH_VARARGS,
                             def.doc_string_.c_str()});
                }

                def_cnt_.defs_.push_back({NULL, NULL, NULL, NULL});

                PyModule_AddFunctions(PO(this->module_), &def_cnt_.defs_[0]);
                CheckError();
            }

            Class Module::GetClass(const std::string& name_a)
            {
                PyObject* cls_ = PyObject_GetAttrString(PO(this->module_),
                                                       name_a.c_str());
                CheckError();

                return Class(cls_);
            }

            Object Module::GetVariable(const std::string& name_a)
            {
                PyObject* var_ = PyObject_GetAttrString(PO(this->module_),
                                                       name_a.c_str());
                CheckError();
                return Object(var_,
                              new ModuleSetter(*this, name_a),
                              new ModuleSynchronizer(*this, name_a));
            }

            void Module::SetVariable(const std::string& name_a,
                                     const Object& variable_a)
            {
                PyObject_SetAttrString(PO(this->module_),
                                       name_a.c_str(),
                                       PO(variable_a));
                CheckError();
            }

            struct CustomObject
            {
                PyObject_HEAD;
                PyObject* dict_;
            };

            PyObject* CustomReduce(PyObject* self_a, PyObject*)
            {
                PyObject* type_ = PyObject_Type(self_a);

                PyObject* tuple_ = PyTuple_New(2);
                PyTuple_SetItem(tuple_, 0, type_);

                PyObject* tuple_args_ = PyTuple_New(0);
                PyTuple_SetItem(tuple_, 1, tuple_args_);

                return tuple_;
            }

            static int CustomSetattr(PyObject* self_a,
                                     PyObject* attr_a,
                                     PyObject* value_a)
            {
                if (PyObject_HasAttrString(self_a, "__setattr"))
                {
                    std::string attrname_ = py_str(attr_a);
                    PyObject* args_ = PyTuple_New(2);
                    PyTuple_SetItem(args_, 0, attr_a);
                    PyTuple_SetItem(args_, 1, value_a);

                    PyObject* notif_function_ =
                            PyObject_GetAttrString(self_a, "__setattr");
                    PyObject_CallObject(notif_function_, args_);
                    Py_DECREF(args_);
                }

                return PyObject_GenericSetAttr(self_a, attr_a, value_a);
            }

            void Module::AddClasses(
                    const std::vector<Types::ClassDefinition>& definitions_a)
            {
                PythonClassDefContainerImpl& def_cnt_ =
                        static_cast<PythonClassDefContainerImpl&>(
                                *this->classes_);

                for (const Types::ClassDefinition& def_ : definitions_a)
                {
                    assert(def_.type_size_ != static_cast<size_t>(-1));

                    const char* full_name_ = ToUtf8(this->GetName() + "."
                                                  + def_.name_);
                    const char* desc_ = ToUtf8(def_.doc_string_);

                    PythonClassDefContainerImpl::Definition& obj_def_ =
                            def_cnt_.Create();

                    for (const Types::FunctionDefinition& functiondef :
                         def_.function_definitions_)
                    {
                        int flags_ = TranslateFunctionFlag(functiondef.flags_);
                        obj_def_.methods_.push_back(
                                {functiondef.name_.c_str(),
                                 reinterpret_cast<PyCFunction>(
                                         functiondef.function_),
                                 flags_,
                                 functiondef.doc_string_.c_str()});
                    }

                    if (def_.set_attr_function_)
                        obj_def_.methods_.push_back({"__setattr",
                                                  reinterpret_cast<PyCFunction>(
                                                          def_.set_attr_function_),
                                                  METH_VARARGS,
                                                  NULL});
                    obj_def_.methods_.push_back(
                            {"__reduce__",
                             reinterpret_cast<PyCFunction>(CustomReduce),
                             METH_VARARGS,
                             NULL});
                    obj_def_.methods_.push_back({NULL, NULL, NULL, NULL});

                    for (const Types::MemberDefinition& memberdef :
                         def_.member_definitions_)
                    {
                        obj_def_.members_.push_back(
                                {memberdef.name_.c_str(),
                                 TranslateType(memberdef.type_),
                                 memberdef.offset_
                                         + static_cast<int64_t>(
                                                 sizeof(CustomObject)),
                                 memberdef.readonly_ ? READONLY : 0,
                                 memberdef.doc_string_.c_str()});
                    }

                    obj_def_.members_.push_back(
                            {"__dict__",
                             TranslateType(
                                     Types::MemberDefinition::Type::Object),
                             offsetof(CustomObject, dict_),
                             0,
                             NULL});

                    obj_def_.members_.push_back({NULL});

                    newfunc newFunction = PyType_GenericNew;
                    if (def_.new_function_)
                        newFunction = reinterpret_cast<newfunc>(
                                def_.new_function_);

                    allocfunc allocFunction = PyType_GenericAlloc;
                    if (def_.alloc_function_)
                        allocFunction = reinterpret_cast<allocfunc>(
                                def_.alloc_function_);

                    obj_def_.object_ = {
                            PyObject_HEAD_INIT(NULL)
                            full_name_,
                            static_cast<int64_t>(sizeof(CustomObject)
                                                 + def_.type_size_),
                            0,
                            reinterpret_cast<destructor>(def_.dealloc_function_),
                            0,
                            nullptr,
                            0,
                            0,
                            reinterpret_cast<reprfunc>(def_.repr_function_),
                            0,
                            0,
                            0,
                            nullptr,
                            0,
                            nullptr,
                            PyObject_GenericGetAttr,
                            reinterpret_cast<setattrofunc>(CustomSetattr),
                            0,
                            Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
                            desc_,
                            0,
                            0,
                            0,
                            0,
                            0,
                            0,
                            &obj_def_.methods_[0],
                            obj_def_.members_.empty()
                                    ? nullptr
                                    : &obj_def_.members_[0],
                            0,
                            0,
                            0, // This is for type object itself
                            0,
                            0,
                            offsetof(CustomObject, dict_),
                            reinterpret_cast<initproc>(def_.init_function_),
                            allocFunction,
                            newFunction,
                    };

                    PyTypeObject* obj = &obj_def_.object_;

                    if (PyType_Ready(obj) < 0)
                        throw PythonException(std::string("Type \"") + full_name_
                                              + "\" could not be finalized");

                    Py_INCREF(obj);

                    int result = PyModule_AddObject(PO(this->module_),
                                                    def_.name_.c_str(),
                                                    reinterpret_cast<PyObject*>(
                                                            obj));
                    CheckError();

                    if (result < 0)
                    {
                        Py_DECREF(obj);
                        Py_DECREF(this->module_);
                    }
                }
            }

            void Module::PushConstant(const std::string& name_a,
                                      const std::string& value_a)
            {
                PyModule_AddStringConstant(PO(this->module_),
                                           name_a.c_str(),
                                           value_a.c_str());
                CheckError();
            }

            void Module::PushImport(const std::string& name_a, Module mod_a)
            {
                this->globals_->Append(name_a, mod_a);
            }

            void Module::Run(const std::string& code_a)
            {
                PyRun_String(code_a.c_str(),
                             Py_single_input,
                             PO(this->globals_->data_),
                             PO(this->locals_->data_));
                CheckError();
            }

            std::string Module::GetName() const
            {
                const char* name_ = PyModule_GetName(PO(this->module_));
                CheckError();
                return std::string(name_);
            }

            std::string Module::GetDocstring() const
            {
                Object obj_ = Object(
                        PyObject_GetAttrString(PO(this->module_), "__doc__"));
                return obj_.ToString();
            }

            bool Module::Exists(const std::string& name_a) const
            {
                bool ret_ = PyObject_HasAttrString(PO(this->module_),
                                                  name_a.c_str());
                CheckError();
                return ret_;
            }

            std::string Module::GetPath() const
            {
                Object filename_(PyModule_GetFilenameObject(PO(this->module_)));
                CheckError();
                return filename_.ToString();
            }

            void* GetDataStructureImpl(void* self_a)
            {
                return reinterpret_cast<CustomObject*>(
                        reinterpret_cast<unsigned char*>(self_a)
                        + sizeof(CustomObject));
            }

            void* InitializeClassImpl(void* type_a)
            {
                PyTypeObject* py_type_ = reinterpret_cast<PyTypeObject*>(type_a);
                void* obj_ = py_type_->tp_alloc(py_type_, 0);
                return GetDataStructureImpl(obj_);
            }

            void* baseObject(void* self_a)
            {
                return reinterpret_cast<CustomObject*>(
                        reinterpret_cast<unsigned char*>(self_a)
                        - sizeof(CustomObject));
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel