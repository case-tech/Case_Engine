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
            class Instance;
            class Function;

            class Class
            {
                friend class Instance;
                friend class Module;

            public:
                Class(PyObj cls_a);

                Instance CreateInstance(std::vector<Object> params_a = {}) const;

                bool Contains(const std::string& name_a) const;

                Function GetFunction(const std::string& name_a) const;

                std::string GetDocstring() const;
                std::string GetName() const;

                PyObj GetType() const;
                bool IsHeapType() const;

            protected:
                PyObj cls_;
            };
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel