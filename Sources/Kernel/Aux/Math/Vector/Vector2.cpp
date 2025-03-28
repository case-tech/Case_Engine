#include "Vector2.hpp"

#include <cassert>
#include <cmath>

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Math
        {
            Vector2::Vector2()
                : x_(0.0f), y_(0.0f) {}

            Vector2::Vector2(float x_a, float y_a)
                : x_(x_a), y_(y_a) {}

            float operator==(const Vector2& v1_a, const Vector2& v2_a)
            {
                const float epsilon_ = 1e-6;
                return std::abs(v1_a.x_ - v2_a.x_) < epsilon_ && std::abs(v1_a.y_ - v2_a.y_) < epsilon_;
            }

            float operator!=(const Vector2& v1_a, const Vector2& v2_a)
            {
                return !(v1_a == v2_a);
            }

            Vector2 operator+(const Vector2& v1_a, const Vector2& v2_a)
            {
                return Vector2(v1_a.x_ + v2_a.x_, v1_a.y_ + v2_a.y_);
            }

            Vector2 operator-(const Vector2& v1_a, const Vector2& v2_a)
            {
                return Vector2(v1_a.x_ - v2_a.x_, v1_a.y_ - v2_a.y_);
            }

            Vector2 operator-(const Vector2& v_a)
            {
                return Vector2(-v_a.x_, -v_a.y_);
            }

            Vector2 operator*(const Vector2& v_a, const float s_a)
            {
                return Vector2(v_a.x_ * s_a, v_a.y_ * s_a);
            }

            Vector2 operator*(const float s_a, const Vector2& v_a)
            {
                return v_a * s_a;
            }

            Vector2 operator/(const Vector2& v_a, const float s_a)
            {
                assert(s_a != 0.0f);
                return v_a * (1.0f / s_a);
            }

            std::ostream& operator<<(std::ostream& out_a, const Vector2& v_a)
            {
                out_a << "(" << v_a.x_ << ", " << v_a.y_ << ")";
                return out_a;
            }

            float Vector2::SqrMagnitude() const
            {
                return Dot(*this, *this);
            }

            float Vector2::Magnitude() const
            {
                return sqrt(SqrMagnitude());
            }

            Vector2 Vector2::Normalized() const
            {
                assert(SqrMagnitude() != 0.0f);
                const float inv_length_ = 1.0f / Magnitude();
                return *this * inv_length_;
            }

            void Vector2::Normalize()
            {
                assert(SqrMagnitude() != 0.0f);
                const float inv_length_ = 1.0f / Magnitude();

                x_ *= inv_length_;
                y_ *= inv_length_;
            }

            Vector2 Scale(const Vector2& v1_a, const Vector2& v2_a)
            {
                return Vector2(v1_a.x_ * v2_a.x_, v1_a.y_ * v2_a.y_);
            }

            float Dot(const Vector2& lhs_a, const Vector2& rhs_a)
            {
                return lhs_a.x_ * rhs_a.x_ + lhs_a.y_ * rhs_a.y_;
            }

            float Angle(const Vector2& from_a, const Vector2& to_a)
            {
                const float cos_theta_ = Dot(from_a, to_a) / sqrt(from_a.SqrMagnitude() * to_a.SqrMagnitude());
                return std::acos(std::fmin(1.0f, cos_theta_));
            }

            float SqrDistance(const Vector2& p1_a, const Vector2& p2_a)
            {
                return (p1_a - p2_a).SqrMagnitude();
            }

            float Distance(const Vector2& p1_a, const Vector2& p2_a)
            {
                return (p1_a - p2_a).Magnitude();
            }
        } // namespace Math
    } // namespace Aux
} // namespace CE_Kernel
