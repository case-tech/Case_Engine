// Copyright (c) 2025 Case Technologies

#pragma once
#include "PythonDefs.hpp"
#include "PythonObject.hpp"

#include <iterator>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class List : public Object
            {
            public:
                class Iterator
                {
                    friend class List;

                public:
                    using iterator_category_ = std::forward_iterator_tag;
                    using value_type_ = Object;
                    using difference_type_ = int;
                    using pointer_ = Object*;
                    using reference_ = Object&;

                public:
                    Iterator(List& dict_a);
                    Iterator(const Iterator& it_a);
                    Iterator& operator++();
                    Iterator operator++(int);
                    bool operator==(const Iterator& it_a);
                    bool operator!=(const Iterator& it_a);
                    Object operator*();
                    Object* operator->();

                private:
                    static Iterator End(List& dict_a);
                    void Cache();

                    List& list_;
                    size_t idx_;
                    Object val_;
                };

            public:
                List();
                explicit List(PyObj data_a);
                List(std::vector<Object> data_a);

                List(const List& data_a);

                List(Object&& data_a) noexcept;
                List(List&& data_a) noexcept;

                List& operator=(Object&& data_a) noexcept;
                List& operator=(List&& data_a) noexcept;

                size_t GetSize() const;

                const Object operator[](unsigned int idx_a) const;
                Object operator[](unsigned int idx_a);

                void PushItem(Object item_a);

                Iterator begin();
                Iterator end();
            };
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel