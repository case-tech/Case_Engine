// Copyright (c) 2025 Case Technologies

#pragma once
#include "PythonDefs.hpp"
#include "PythonObject.hpp"

#include <functional>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class Tuple : public Object
            {
            public:
                Tuple(PyObj tuple_a);
                Tuple(const Tuple& tuple_a);
                Tuple(std::vector<Object> tuple_a);
                Tuple(std::vector<std::reference_wrapper<Object>> tuple_a);
                Tuple(std::vector<PyObj> tuple_a);

                Tuple(Object&& data_a) noexcept;
                Tuple(Tuple&& tuple_a) noexcept;

                explicit Tuple(size_t element_count_a);

                size_t GetSize() const;

                Object operator[](unsigned int idx_a);
            };
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel