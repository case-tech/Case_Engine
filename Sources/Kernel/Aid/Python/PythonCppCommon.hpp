// Copyright (c) 2025 Case Technologies

#pragma once
#include "PythonDefs.hpp"

#ifdef _DEBUG
#define RESTORE_DEBUG
#undef _DEBUG
#endif
#include <Python.h>
#include <structmember.h>
#ifdef RESTORE_DEBUG
#undef RESTORE_DEBUG
#define _DEBUG
#endif

#include <codecvt>
#include <locale>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            inline std::string ConvString(const std::wstring& wstr_a)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv_;
                return conv_.to_bytes(wstr_a);
            }

            inline std::wstring ConvString(const std::string& wstr_a)
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv_;
                return conv_.from_bytes(wstr_a);
            }

            inline PyObject* PO(PyObj o_a)
            {
                return static_cast<PyObject*>(o_a);
            }

            inline PyTypeObject* PT(PyObj o_a)
            {
                return static_cast<PyTypeObject*>(o_a);
            }

            inline std::string py_str(PyObject* obj_a)
            {
                PyObject* enc_ = PyUnicode_AsEncodedString(obj_a,
                                                          "utf-8",
                                                          "strict");
                const char* bytes_ = PyBytes_AS_STRING(enc_);

                Py_XDECREF(enc_);

                return std::string(bytes_);
            }

            inline std::string PyObjToString(PyObj obj_a)
            {
                PyObject* str_ = PyObject_Str(PO(obj_a));
                std::string res_ = py_str(str_);
                Py_XDECREF(str_);
                return res_;
            }

            inline std::string PyObjRepr(PyObj obj_a)
            {
                PyObject* str_ = PyObject_Repr(PO(obj_a));
                std::string res_ = py_str(str_);
                Py_XDECREF(str_);
                return res_;
            }

            void CheckError();

            inline PyObject* DeepCopy(PyObject* obj_a)
            {
                if (!obj_a)
                    return nullptr;

                assert(Py_REFCNT(obj_a) >= 1);
                PyGILState_STATE state_ = PyGILState_Ensure();

                PyObject* pypath_ = PyUnicode_FromString("copy");
                PyObject* pymod_ = PyImport_Import(pypath_);
                CheckError();

                Py_DECREF(pypath_);

                CheckError();

                PyObject* function_ = PyObject_GetAttrString(pymod_, "deepcopy");
                CheckError();

                PyObject* args_ = PyTuple_Pack(1, obj_a);
                CheckError();

                PyObject* result_ = PyObject_CallObject(function_, args_);
                CheckError();

                PyGILState_Release(state_);

                return result_;
            }

            inline const char* ToUtf8(const std::string& str_a)
            {
                PyObject* obj_ = PyUnicode_FromString(str_a.c_str());
                const char* chars_ = PyUnicode_AsUTF8(obj_);
                return chars_;
            }

            inline int TranslateType(Types::MemberDefinition::Type type_a)
            {
                switch (type_a)
                {
                case Types::MemberDefinition::Type::Short:
                    return T_SHORT;
                case Types::MemberDefinition::Type::Int:
                    return T_INT;
                case Types::MemberDefinition::Type::Long:
                    return T_LONG;
                case Types::MemberDefinition::Type::Float:
                    return T_FLOAT;
                case Types::MemberDefinition::Type::Double:
                    return T_DOUBLE;
                case Types::MemberDefinition::Type::String:
                    return T_STRING;
                case Types::MemberDefinition::Type::Object:
                    return T_OBJECT;
                case Types::MemberDefinition::Type::ObjectEx:
                    return T_OBJECT_EX;
                case Types::MemberDefinition::Type::Char:
                    return T_CHAR;
                case Types::MemberDefinition::Type::Byte:
                    return T_BYTE;
                case Types::MemberDefinition::Type::Ubyte:
                    return T_UBYTE;
                case Types::MemberDefinition::Type::Uint:
                    return T_UINT;
                case Types::MemberDefinition::Type::Ushort:
                    return T_USHORT;
                case Types::MemberDefinition::Type::Ulong:
                    return T_LONG;
                case Types::MemberDefinition::Type::Bool:
                    return T_BOOL;
                case Types::MemberDefinition::Type::Longlong:
                    return T_LONGLONG;
                case Types::MemberDefinition::Type::Ulonglong:
                    return T_ULONGLONG;
                case Types::MemberDefinition::Type::Pysizet:
                    return T_PYSSIZET;
                }

                return 0;
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel