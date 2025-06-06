// Copyright (c) 2025 Case Technologies

#pragma once
#include "PythonDefs.hpp"
#include "PythonObject.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class Type : public Object
            {
            public:
                static constexpr char TypeNone[] = "NoneType";
                static constexpr char TypeString[] = "str";
                static constexpr char TypeInt[] = "int";
                static constexpr char TypeFloat[] = "float";
                static constexpr char TypeList[] = "list";
                static constexpr char TypeDict[] = "dict";
                static constexpr char TypeFunction[] = "function";
                static constexpr char TypeType[] = "type";
                static constexpr char TypeModule[] = "module";

            public:
                explicit Type(const PyObj& data_a);

                std::string ToString() const;

                bool IsNone() const;
                bool IsString() const;
                bool IsInt() const;
                bool IsFloat() const;
                bool IsList() const;
                bool IsDict() const;
                bool IsFunction() const;
                bool IsType() const;
                bool IsModule() const;
                bool operator==(const Type& b_a) const;
            };
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel