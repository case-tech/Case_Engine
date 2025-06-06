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
            class Tuple;

            template <typename T>
            class Ref : public T
            {
            public:
                Ref(const Ref<T>& r_a) = delete;
            };

            template <typename T,
                      std::enable_if_t<
                              std::is_class<typename std::remove_reference<
                                      T>::type_>::value_,
                              bool> = true,
                      std::enable_if_t<
                              std::is_base_of<Object,
                                              typename std::remove_reference<
                                                      T>::type_>::value_,
                              bool> = true>
            PyObj Makeref(T&& t_a)
            {
                return static_cast<PyObj>(t_a);
            };

            template <typename T,
                      std::enable_if_t<
                              std::is_class<typename std::remove_reference<
                                      T>::type_>::value_,
                              bool> = true,
                      std::enable_if_t<
                              !std::is_base_of<Object,
                                               typename std::remove_reference<
                                                       T>::type_>::value_,
                              bool> = true>
            PyObj Makeref(T&& t_a)
            {
                return static_cast<PyObj>(t_a);
            };

            template <typename T,
                      std::enable_if_t<
                              !std::is_class<typename std::remove_reference<
                                      T>::type_>::value_,
                              bool> = true>
            PyObj Makeref(T&& t_a)
            {
                return static_cast<PyObj>(Object(t_a));
            };

            class Function
            {
                friend class Module;
                friend class Inspect;

            public:
                Function(PyObj function_a);
                ~Function();

                Object Call();
                Object Call(const Tuple& params_a);
                Object Call(std::vector<Object> params_a);
                Object CallV(std::vector<Object> params_a);
                Object CallRef(
                        std::vector<std::reference_wrapper<Object>> params_a);

                std::string GetDocString() const;
                std::string GetName() const;

                template <typename... T>
                Object operator()(T&&... params_a)
                {
                    Tuple tuple_(std::vector<PyObj> {MakeRef(params_a)...});
                    return this->Call(tuple_);
                }

            protected:
                PyObj function_;
            };
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel