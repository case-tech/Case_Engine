// Copyright (c) 2025 Case Technologies

#pragma once
#include "PythonDefs.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            class PythonException : public std::exception
            {
            public:
                PythonException(const std::string& msg_a) : message_(msg_a)
                {}

                const char* what() const noexcept override
                {
                    return this->message_.c_str();
                }

            private:
                std::string message_;
            };

            class PythonErrorException : public PythonException
            {
            public:
                PythonErrorException(PyObj type_a, PyObj values_a, PyObj trace_a);

                std::string GetExceptionType() const;
                std::string GetExceptionValue() const;

            private:
                PyObj type_;
                PyObj values_;
                PyObj trace_;
            };

            class PythonGeneralException : public PythonException
            {
            public:
                PythonGeneralException(const std::string& msg_a)
                    : PythonException(msg_a)
                {}
            };

            class PythonOutOfBoundsException : public PythonException
            {
            public:
                PythonOutOfBoundsException(const std::string& msg_a)
                    : PythonException(msg_a)
                {}
            };
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel