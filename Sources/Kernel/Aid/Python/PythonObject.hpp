// Copyright (c) 2025 Case Technologies

#pragma once
#include "PythonDefs.hpp"

#include <memory>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class Object;

            size_t GetReferenceCount(PyObj obj_a);
            void DecrementReferenceCount(PyObj obj_a);
            void IncrementReferenceCount(PyObj obj_a);

            class Setter
            {
            public:
                virtual ~Setter() = default;

                virtual void Set(const Object& data_a) = 0;

                virtual void Set(const std::string& data_a);
                virtual void Set(int data_a);
                virtual void Set(unsigned int data_a);
                virtual void Set(long data_a);
                virtual void Set(unsigned long data_a);
                virtual void Set(float data_a);
                virtual void Set(double data_a);
            };

            class Synchronizer
            {
            public:
                virtual ~Synchronizer() = default;

                virtual void Synchronize(PyObj& obj_a) = 0;
            };

            class Object
            {
                friend class Dict;
                friend class Tuple;
                friend class List;
                friend class Function;
                friend class Instance;
                friend class Module;
                friend class RefData;
                friend class DefaultDataSetter;

            public:
                Object();
                Object(const Object& data_a);
                Object(Object&& data_a) noexcept;

                explicit Object(PyObj data_a);
                Object(PyObj data_a,
                       Setter* setter_a,
                       Synchronizer* synchronizer_a = nullptr);

                Object(const std::string& data_a);
                Object(const char* data_a);
                Object(int data_a);
                Object(unsigned int data_a);
                Object(long data_a);
                Object(unsigned long data_a);
                Object(float data_a);
                Object(double data_a);

                virtual Object& operator=(const Object& data_a);
                virtual Object& operator=(const std::string& data_a);
                virtual Object& operator=(const char* data_a);
                virtual Object& operator=(int data_a);
                virtual Object& operator=(unsigned int data_a);
                virtual Object& operator=(long data_a);
                virtual Object& operator=(unsigned long data_a);
                virtual Object& operator=(float data_a);
                virtual Object& operator=(double data_a);

                virtual ~Object();

                PyObj GetType() const;

                [[nodiscard]] operator PyObj() const;

                std::string ToString() const;
                std::string Repr() const;
                long ToLong() const;
                double ToDouble() const;
                bool ToBool() const;
                bool IsNone() const;
                bool IsNull() const;

                unsigned int GetReferenceCount() const;

                static Object None();

            protected:
                mutable PyObj data_;
                std::unique_ptr<Setter> setter_ = nullptr;
                std::unique_ptr<Synchronizer> synchronizer_ = nullptr;
            };
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel