// Copyright (c) 2025 Case Technologies

#pragma once
#include "AST/Visitor/Visitor.hpp"

#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class Variant
            {
            public:
                using BoolType = bool;
                using IntType = long long;
                using RealType = double;

                enum class Types
                {
                    Undefined,
                    Bool,
                    Int,
                    Real,
                    Array,
                };

                Variant() = default;
                Variant(const Variant&) = default;
                Variant(Variant&&) = default;
                Variant(BoolType value_a);
                Variant(IntType value_a);
                Variant(RealType value_a);
                Variant(const std::vector<Variant>& sub_values_a);
                Variant(std::vector<Variant>&& sub_values_a);

                Variant& operator=(const Variant&) = default;
                Variant& operator=(Variant&&) = default;

                Variant& operator+=(const Variant& rhs_a);
                Variant& operator-=(const Variant& rhs_a);
                Variant& operator*=(const Variant& rhs_a);
                Variant& operator/=(const Variant& rhs_a);
                Variant& operator%=(const Variant& rhs_a);
                Variant& operator|=(const Variant& rhs_a);
                Variant& operator&=(const Variant& rhs_a);
                Variant& operator^=(const Variant& rhs_a);
                Variant& operator<<=(const Variant& rhs_a);
                Variant& operator>>=(const Variant& rhs_a);
                Variant& operator++();
                Variant& operator--();

                Variant operator-();
                Variant operator~();
                Variant operator!();

                BoolType ToBool() const;
                IntType ToInt() const;
                RealType ToReal() const;

                int CompareWith(const Variant& rhs_a) const;
                Variant ArraySub(std::size_t idx_a) const;

                inline bool IsValid() const
                {
                    return (Type() != Types::Undefined);
                }

                inline bool IsBool() const
                {
                    return (Type() == Types::Bool);
                }

                inline bool IsInt() const
                {
                    return (Type() == Types::Int);
                }

                inline bool IsReal() const
                {
                    return (Type() == Types::Real);
                }

                inline bool IsArray() const
                {
                    return (Type() == Types::Array);
                }

                inline BoolType Bool() const
                {
                    return bool_;
                }

                inline IntType Int() const
                {
                    return int_;
                }

                inline RealType Real() const
                {
                    return real_;
                }

                inline const std::vector<Variant>& Array() const
                {
                    return array_;
                }

                inline Types Type() const
                {
                    return type_;
                }

                inline operator bool() const
                {
                    return IsValid();
                }

                static Variant ParseFrom(const std::string& s_a);
                static Types FindCommonType(const Variant& lhs_a,
                                            const Variant& rhs_a);
                std::string ToString() const;

                bool IsRepresentableAsString() const;

            private:
                Types type_ = Types::Undefined;
                BoolType bool_ = false;
                IntType int_ = 0;
                RealType real_ = 0.0;
                std::vector<Variant> array_;
            };

            Variant operator==(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator!=(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator<(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator<=(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator>(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator>=(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator+(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator-(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator*(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator/(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator%(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator|(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator&(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator^(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator<<(const Variant& lhs_a, const Variant& rhs_a);
            Variant operator>>(const Variant& lhs_a, const Variant& rhs_a);
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
