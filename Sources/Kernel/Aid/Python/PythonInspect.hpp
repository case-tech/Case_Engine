// Copyright (c) 2025 Case Technologies

#pragma once
#include "PythonDefs.hpp"
#include "PythonModule.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class Inspect
            {
            public:
                Inspect();

                Module GetModule(PyObj data_a);
                Module GetModule(Function data_a);

            private:
                Module inspect_module_;
            };
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel