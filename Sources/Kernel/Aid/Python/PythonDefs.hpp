// Copyright (c) 2025 Case Technologies

#pragma once
#include <exception>
#include <limits>
#include <string>
#include <vector>

#define PYTHON_CPP_FLAG_OP(TARGET)                                                 \
    inline TARGET operator|(TARGET a_a, TARGET b_a)                                \
    {                                                                              \
        return static_cast<TARGET>(static_cast<unsigned int>(a_a)                  \
                                   | static_cast<unsigned int>(b_a));              \
    }                                                                              \
    inline TARGET operator&(TARGET a_a, TARGET b_a)                                \
    {                                                                              \
        return static_cast<TARGET>(static_cast<unsigned int>(a_a)                  \
                                   & static_cast<unsigned int>(b_a))  ;            \
    }                                                                              \
    inline TARGET operator|=(TARGET& a_a, TARGET b_a)                              \
    {                                                                              \
        reinterpret_cast<unsigned int&>(a_a) |= static_cast<unsigned int>(b_a);    \
        return a_a;                                                                \
    }                                                                              \
    inline TARGET operator&=(TARGET& a_a, TARGET b_a)                              \
    {                                                                              \
        reinterpret_cast<unsigned int&>(a_a) &= static_cast<unsigned int>(b_a);    \
        return a_a;                                                                \
    }                                                                              \
    inline TARGET operator~(TARGET a_a)                                            \
    {                                                                              \
        return static_cast<TARGET>(~static_cast<unsigned int>(a_a));               \
    }                                                                              \
    inline bool operator!(TARGET a_a)                                              \
    {                                                                              \
        return !static_cast<bool>(a_a);                                            \
    }

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            using PyObj = void*;

            class Module;
            class Class;

            enum class FunctionFlag
            {
                VarArgs = 0x01,
                Keywords = 0x02,
                NoArgs = 0x04,
                OneArg = 0x08,
                ClassMethod = 0x10,
                StaticMethod = 0x20,
                Coexist = 0x40,
                Fastcall = 0x80
            };

            PYTHON_CPP_FLAG_OP(FunctionFlag);

            constexpr FunctionFlag default_function_flag_ = FunctionFlag::VarArgs;

            struct PythonFunctionDefContainer
            {};

            struct PythonClassDefContainer
            {};

            constexpr size_t ObjSize();

            namespace Types
            {
                typedef void* (*FncPtr)(void* py_self_a, void* py_args_a);
                typedef int (*InitFncPtr)(void* self_a, void* args_a, void* kwargs_a);
                typedef void* (*NewFncPtr)(void* type_a,
                                           void* args_a,
                                           void* kwargs_a);
                
                typedef void* (*AllocFncPtr)(void* type_a, int64_t size_a);
                typedef void (*DeallocFncPtr)(void* self_a);
                typedef void* (*ReprFncPtr)(void* self_a);
                typedef void* (*SetAttrFunc)(void* self_a, void* args_a);

                struct FunctionDefinition
                {
                    std::string name_;
                    FncPtr function_;
                    std::string doc_string_;
                    FunctionFlag flags_;

                    FunctionDefinition(
                            const std::string& name_a,
                            FncPtr function_a,
                            FunctionFlag flags_a = FunctionFlag::VarArgs,
                            const std::string& doc_string_a = "")
                        : name_(name_a), function_(function_a), doc_string_(doc_string_a),
                          flags_(flags_a)
                    {}
                };

                struct MemberDefinition
                {
                    enum class Type
                    {
                        Short,
                        Int,
                        Long,
                        Float,
                        Double,
                        String,
                        Object,
                        ObjectEx,
                        Char,
                        Byte,
                        Ubyte,
                        Uint,
                        Ushort,
                        Ulong,
                        Bool,
                        Longlong,
                        Ulonglong,
                        Pysizet
                    };

                    std::string name_;
                    Type type_;
                    int64_t offset_;
                    bool readonly_ = false;
                    std::string doc_string_ = "";
                };

                struct ClassDefinition
                {
                    std::string name_;
                    std::string doc_string_;
                    std::vector<FunctionDefinition> function_definitions_;
                    std::vector<MemberDefinition> member_definitions_;
                    size_t type_size_ = std::numeric_limits<size_t>::max();
                    InitFncPtr init_function_ = nullptr;
                    NewFncPtr new_function_ = nullptr;
                    AllocFncPtr alloc_function_ = nullptr;
                    DeallocFncPtr dealloc_function_ = nullptr;
                    ReprFncPtr repr_function_ = nullptr;
                    SetAttrFunc set_attr_function_ = nullptr;

                    ClassDefinition(const std::string& name_a) : name_(name_a)
                    {}

                    template <typename T>
                    ClassDefinition& SetType()
                    {
                        this->type_size_ = sizeof(T);
                        return *this;
                    }

                    ClassDefinition& SetName(const std::string& name_a)
                    {
                        this->name_ = name_a;
                        return *this;
                    }
                    
                    ClassDefinition& SetDocString(const std::string& doc_string_a)
                    {
                        this->doc_string_ = doc_string_a;
                        return *this;
                    }

                    ClassDefinition& addFunction(FunctionDefinition def)
                    {
                        this->function_definitions_.push_back(def);
                        return *this;
                    }

                    ClassDefinition& SetInitFunction(InitFncPtr def_a)
                    {
                        this->init_function_ = def_a;
                        return *this;
                    }

                    ClassDefinition& UseDefaultInitFunction()
                    {
                        return this->SetInitFunction(nullptr);
                    }

                    ClassDefinition& SetNewFunction(NewFncPtr def_a)
                    {
                        this->new_function_ = def_a;
                        return *this;
                    }

                    ClassDefinition& UseDefaultNewFunction()
                    {
                        return this->SetNewFunction(nullptr);
                    }

                    ClassDefinition& SetDeleteFunction(DeallocFncPtr def_a)
                    {
                        this->dealloc_function_ = def_a;
                        return *this;
                    }

                    ClassDefinition& SetDefaultDeleteFunction()
                    {
                        return this->SetDeleteFunction(nullptr);
                    }

                    ClassDefinition& SetReprFunction(ReprFncPtr def_a)
                    {
                        this->repr_function_ = def_a;
                        return *this;
                    }

                    ClassDefinition& SetSetAttrFunction(SetAttrFunc def_a)
                    {
                        this->set_attr_function_ = def_a;
                        return *this;
                    }

                    ClassDefinition& AddMemberDefinition(MemberDefinition def_a)
                    {
                        this->member_definitions_.push_back(def_a);
                        return *this;
                    }
                };
            } // namespace Types
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel