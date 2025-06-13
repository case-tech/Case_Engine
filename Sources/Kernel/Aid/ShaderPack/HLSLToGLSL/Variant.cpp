#include "Variant.hpp"

#include "Helper.hpp"

#include <sstream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            Variant::Variant(BoolType value_a)
                : type_ {Types::Bool}, bool_ {value_a}
            {}

            Variant::Variant(IntType value_a) : type_ {Types::Int}, int_ {value_a}
            {}

            Variant::Variant(RealType value_a)
                : type_ {Types::Real}, real_ {value_a}
            {}

            Variant::Variant(const std::vector<Variant>& sub_values_a)
                : type_ {Types::Array}
            {
                array_ = sub_values_a;
            }

            Variant::Variant(std::vector<Variant>&& sub_values_a)
                : type_ {Types::Array}
            {
                array_ = std::move(sub_values_a);
            }

            Variant& Variant::operator+=(const Variant& rhs_a)
            {
                *this = (*this + rhs_a);
                return *this;
            }

            Variant& Variant::operator-=(const Variant& rhs_a)
            {
                *this = (*this - rhs_a);
                return *this;
            }

            Variant& Variant::operator*=(const Variant& rhs_a)
            {
                *this = (*this * rhs_a);
                return *this;
            }

            Variant& Variant::operator/=(const Variant& rhs_a)
            {
                *this = (*this / rhs_a);
                return *this;
            }

            Variant& Variant::operator%=(const Variant& rhs_a)
            {
                *this = (*this % rhs_a);
                return *this;
            }

            Variant& Variant::operator|=(const Variant& rhs_a)
            {
                *this = (*this | rhs_a);
                return *this;
            }

            Variant& Variant::operator&=(const Variant& rhs_a)
            {
                *this = (*this & rhs_a);
                return *this;
            }

            Variant& Variant::operator^=(const Variant& rhs_a)
            {
                *this = (*this ^ rhs_a);
                return *this;
            }

            Variant& Variant::operator<<=(const Variant& rhs_a)
            {
                *this = (*this << rhs_a);
                return *this;
            }

            Variant& Variant::operator>>=(const Variant& rhs_a)
            {
                *this = (*this >> rhs_a);
                return *this;
            }

#undef AzPLEMENT_VARIANT_OP
#undef AzPLEMENT_VARIANT_BITWISE_OP

            Variant& Variant::operator++()
            {
                switch (type_)
                {
                case Types::Int:
                    ++int_;
                    break;
                case Types::Real:
                    ++real_;
                    break;
                default:
                    break;
                }
                return *this;
            }

            Variant& Variant::operator--()
            {
                switch (type_)
                {
                case Types::Int:
                    --int_;
                    break;
                case Types::Real:
                    --real_;
                    break;
                default:
                    break;
                }
                return *this;
            }

            Variant Variant::operator-()
            {
                Variant result_ = *this;

                switch (type_)
                {
                case Types::Int:
                    result_.int_ = -int_;
                    break;
                case Types::Real:
                    result_.real_ = -real_;
                    break;
                default:
                    break;
                }

                return result_;
            }

            Variant Variant::operator~()
            {
                Variant result_ = *this;

                switch (type_)
                {
                case Types::Int:
                    result_.int_ = ~int_;
                    break;
                default:
                    break;
                }

                return result_;
            }

            Variant Variant::operator!()
            {
                Variant result_ = *this;

                switch (type_)
                {
                case Types::Bool:
                    result_.bool_ = !bool_;
                    break;
                case Types::Int:
                    result_.int_ = !int_;
                    break;
                case Types::Real:
                    result_.real_ = !real_;
                    break;
                default:
                    break;
                }

                return result_;
            }

            Variant::BoolType Variant::ToBool() const
            {
                switch (type_)
                {
                case Types::Int:
                    return (int_ != 0);
                case Types::Real:
                    return (real_ != 0.0);
                default:
                    return bool_;
                }
            }

            Variant::IntType Variant::ToInt() const
            {
                switch (type_)
                {
                case Types::Bool:
                    return (bool_ ? 1 : 0);
                case Types::Real:
                    return static_cast<IntType>(real_);
                default:
                    return int_;
                }
            }

            Variant::RealType Variant::ToReal() const
            {
                switch (type_)
                {
                case Types::Bool:
                    return (bool_ ? 1.0 : 0.0);
                case Types::Int:
                    return static_cast<RealType>(int_);
                default:
                    return real_;
                }
            }

            int Variant::CompareWith(const Variant& rhs_a) const
            {
                auto cmp_ = rhs_a;

                switch (type_)
                {
                case Types::Bool: {
                    cmp_ = cmp_.ToBool();
                    if (Bool() && !cmp_.Bool())
                        return 1;
                    if (!Bool() && cmp_.Bool())
                        return -1;
                }
                break;

                case Types::Int: {
                    cmp_ = cmp_.ToInt();
                    if (Int() < cmp_.Int())
                        return -1;
                    if (Int() > cmp_.Int())
                        return 1;
                }
                break;

                case Types::Real: {
                    cmp_ = cmp_.ToReal();
                    if (Real() < cmp_.Real())
                        return -1;
                    if (Real() > cmp_.Real())
                        return 1;
                }
                break;

                default:
                    break;
                }

                return 0;
            }

            Variant Variant::ArraySub(std::size_t idx_a) const
            {
                if (Type() == Types::Array)
                {
                    if (idx_a < array_.size())
                        return array_[idx_a];
                }
                return {};
            }

            Variant Variant::ParseFrom(const std::string& s_a)
            {
                if (s_a == "true")
                    return Variant {true};
                else if (s_a == "false")
                    return Variant {false};
                else if (s_a.find_first_of(".eE") != std::string::npos)
                    return Variant {FromStringOrDefault<double>(s_a)};
                else
                    return Variant {FromStringOrDefault<long long>(s_a)};
            }

            Variant::Types Variant::FindCommonType(const Variant& lhs_a,
                                                   const Variant& rhs_a)
            {
                auto lhs_type_ = lhs_a.Type();
                auto rhs_type_ = rhs_a.Type();

                if (lhs_type_ == Types::Real || rhs_type_ == Types::Real)
                    return Types::Real;
                if (lhs_type_ == Types::Int || rhs_type_ == Types::Int)
                    return Types::Int;
                if (lhs_type_ == Types::Bool || rhs_type_ == Types::Bool)
                    return Types::Bool;

                return lhs_type_;
            }

            static void CropStringRight(std::string& s_a, std::size_t pos_a)
            {
                if (pos_a < s_a.size())
                    s_a.erase(pos_a, std::string::npos);
            }

            static std::string RealToString(Variant::RealType v_a)
            {
                auto s_ = std::to_string(v_a);
                auto pos_fract_ = s_.find('.');
                if (pos_fract_ != std::string::npos)
                {
                    auto pos_ = s_.find_last_not_of('0');
                    if (pos_ != std::string::npos)
                    {
                        if (pos_ == pos_fract_)
                            CropStringRight(s_, pos_ + 2);
                        else
                            CropStringRight(s_, pos_ + 1);
                    }
                }
                return s_;
            }

            std::string Variant::ToString() const
            {
                switch (Type())
                {
                case Types::Bool:
                    return (Bool() ? "true" : "false");
                case Types::Int:
                    return std::to_string(Int());
                case Types::Real:
                    return RealToString(Real());
                default:
                    return "";
                }
            }

            bool Variant::IsRepresentableAsString() const
            {
                auto t_ = Type();
                return (t_ == Types::Bool || t_ == Types::Int
                        || t_ == Types::Real);
            }

            Variant operator==(const Variant& lhs_a, const Variant& rhs_a)
            {
                return (lhs_a.CompareWith(rhs_a) == 0);
            }

            Variant operator!=(const Variant& lhs_a, const Variant& rhs_a)
            {
                return (lhs_a.CompareWith(rhs_a) != 0);
            }

            Variant operator<(const Variant& lhs_a, const Variant& rhs_a)
            {
                return (lhs_a.CompareWith(rhs_a) < 0);
            }

            Variant operator<=(const Variant& lhs_a, const Variant& rhs_a)
            {
                return (lhs_a.CompareWith(rhs_a) <= 0);
            }

            Variant operator>(const Variant& lhs_a, const Variant& rhs_a)
            {
                return (lhs_a.CompareWith(rhs_a) > 0);
            }

            Variant operator>=(const Variant& lhs_a, const Variant& rhs_a)
            {
                return (lhs_a.CompareWith(rhs_a) >= 0);
            }

#define AzPLEMENT_VARIANT_OP(OP)                                               \
    switch (Variant::FindCommonType(lhs_a, rhs_a))                                 \
    {                                                                          \
    case Variant::Types::Int:                                                  \
        return (lhs_a.ToInt() OP rhs_a.ToInt());                                   \
    case Variant::Types::Real:                                                 \
        return (lhs_a.ToReal() OP rhs_a.ToReal());                                 \
    default:                                                                   \
        return lhs_a;                                                            \
    }

#define AzPLEMENT_VARIANT_INT_OP(OP)                                           \
    switch (Variant::FindCommonType(lhs_a, rhs_a))                                 \
    {                                                                          \
    case Variant::Types::Int:                                                  \
        return (lhs_a.ToInt() OP rhs_a.ToInt());                                   \
    default:                                                                   \
        return lhs_a;                                                            \
    }

            Variant operator+(const Variant& lhs_a, const Variant& rhs_a)
            {
                AzPLEMENT_VARIANT_OP(+);
            }

            Variant operator-(const Variant& lhs_a, const Variant& rhs_a)
            {
                AzPLEMENT_VARIANT_OP(-);
            }

            Variant operator*(const Variant& lhs_a, const Variant& rhs_a)
            {
                AzPLEMENT_VARIANT_OP(*);
            }

            Variant operator/(const Variant& lhs_a, const Variant& rhs_a)
            {
                AzPLEMENT_VARIANT_OP(/);
            }

            Variant operator%(const Variant& lhs_a, const Variant& rhs_a)
            {
                AzPLEMENT_VARIANT_INT_OP(%);
            }

            Variant operator|(const Variant& lhs_a, const Variant& rhs_a)
            {
                AzPLEMENT_VARIANT_INT_OP(|);
            }

            Variant operator&(const Variant& lhs_a, const Variant& rhs_a)
            {
                AzPLEMENT_VARIANT_INT_OP(&);
            }

            Variant operator^(const Variant& lhs_a, const Variant& rhs_a)
            {
                AzPLEMENT_VARIANT_INT_OP(^);
            }

            Variant operator<<(const Variant& lhs_a, const Variant& rhs_a)
            {
                AzPLEMENT_VARIANT_INT_OP(<<);
            }

            Variant operator>>(const Variant& lhs_a, const Variant& rhs_a)
            {
                AzPLEMENT_VARIANT_INT_OP(>>);
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel