// Copyright (c) 2025 Case Technologies

#pragma once
#include "PythonDefs.hpp"
#include "PythonObject.hpp"

#include <iterator>
#include <string>
#include <unordered_map>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class Dict : public Object
            {
            public:
                class Iterator
                {
                    friend class Dict;

                public:
                    using IteratorCategory = std::forward_iterator_tag;
                    using ValueType = Object;
                    using DifferenceType = int;
                    using Pointer = Object*;
                    using Reference = Object&;

                public:
                    Iterator(const Dict& dict_a);
                    Iterator(const Iterator& it_a);
                    ~Iterator();
                    Iterator& operator++();
                    Iterator& operator++(int);
                    bool operator==(const Iterator& it_a);
                    bool operator!=(const Iterator& it_a);
                    Object& operator*();
                    Object* operator->();

                    std::string GetKey() const;

                private:
                    Iterator(const Dict& dict_a, int ret_a);
                    static Iterator End(const Dict& dict_a);

                    void Next();

                    const Dict& dict_;
                    int ret_ = 1;
                    size_t pos_;
                    PyObj* key_ = nullptr;
                    Object val_;
                };

            public:
                Dict();
                Dict(PyObj data_a);
                Dict(const Dict& data_a);

                Dict(Object&& data_a) noexcept;
                Dict(Dict&& data_a) noexcept;

                explicit Dict(
                        const std::unordered_map<std::string, Object>& data_a);

                Dict& operator=(Object&& data_a) noexcept;
                Dict& operator=(Dict&& data_a) noexcept;

                void Append(const std::string& key_a, Object& data_a);
                void Append(const std::string& key_a, const Object& data_a);
                void Append(const std::string& key_a, Object&& data_a);
                void Append(const std::string& key_a, Module& module_a);

                bool Contains(const Object& key_a);
                Object operator[](const Object& key_a);
                const Object operator[](const Object& key_a) const;
                Object At(const Object& key_a) const;

                size_t GetSize() const;

                Iterator Begin();
                Iterator End();

                Iterator Begin() const;
                Iterator End() const;
            };
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel