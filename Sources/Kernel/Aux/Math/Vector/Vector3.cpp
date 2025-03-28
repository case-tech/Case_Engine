#include "Vector3.hpp"
#include "Vector4.hpp"

#include <cassert>
#include <cmath>

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Math
        {
            Vector3::Vector3()
                : x_(0.0f), y_(0.0f), z_(0.0f) {}

            Vector3::Vector3(float x_a, float y_a, float z_a)
                : x_(x_a), y_(y_a), z_(z_a) {}

            Vector3::Vector3(const Vector4& v_a)
                : x_(v_a.x_), y_(v_a.y_), z_(v_a.z_) {}

            float operator==(const Vector3& v1_a, const Vector3& v2_a)
            {
                const float epsilon_ = 1e-6;
                return std::abs(v1_a.x_ - v2_a.x_) < epsilon_ && std::abs(v1_a.y_ - v2_a.y_) < epsilon_
                       && std::abs(v1_a.z_ - v2_a.z_) < epsilon_;
            }

            float operator!=(const Vector3& v1_a, const Vector3& v2_a)
            {
                return !(v1_a == v2_a);
            }

            Vector3 operator+(const Vector3& v1_a, const Vector3& v2_a)
            {
                return Vector3(v1_a.x_ + v2_a.x_, v1_a.y_ + v2_a.y_, v1_a.z_ + v2_a.z_);
            }

            Vector3& operator+=(Vector3& v1_a, const Vector3& v2_a)
            {
                v1_a.x_ += v2_a.x_;
                v1_a.y_ += v2_a.y_;
                v1_a.z_ += v2_a.z_;

                return v1_a;
            }

            Vector3 operator-(const Vector3& v1_a, const Vector3& v2_a)
            {
                return Vector3(v1_a.x_ - v2_a.x_, v1_a.y_ - v2_a.y_, v1_a.z_ - v2_a.z_);
            }

            Vector3& operator-=(Vector3& v1_a, const Vector3& v2_a)
            {
                v1_a.x_ -= v2_a.x_;
                v1_a.y_ -= v2_a.y_;
                v1_a.z_ -= v2_a.z_;

                return v1_a;
            }

            Vector3 operator-(const Vector3& v_a)
            {
                return Vector3(-v_a.x_, -v_a.y_, -v_a.z_);
            }

            Vector3 operator*(const Vector3& v_a, const float s_a)
            {
                return Vector3(v_a.x_ * s_a, v_a.y_ * s_a, v_a.z_ * s_a);
            }

            Vector3& operator*=(Vector3& v_a, const float s_a)
            {
                v_a.x_ *= s_a;
                v_a.y_ *= s_a;
                v_a.z_ *= s_a;

                return v_a;
            }

            Vector3 operator*(const float s_a, const Vector3& v_a)
            {
                return v_a * s_a;
            }

            Vector3 operator/(const Vector3& v_a, const float s_a)
            {
                assert(s_a != 0.0f);
                return v_a * (1.0f / s_a);
            }

            Vector3& operator/=(Vector3& v_a, const float s_a)
            {
                assert(s_a != 0.0f);
                v_a.x_ /= s_a;
                v_a.y_ /= s_a;
                v_a.z_ /= s_a;

                return v_a;
            }

            std::ostream& operator<<(std::ostream& out_a, const Vector3& v_a)
            {
                out_a << "(" << v_a.x_ << ", " << v_a.y_ << ", " << v_a.z_ << ")";
                return out_a;
            }

            float Vector3::SqrMagnitude() const
            {
                return Dot(*this, *this);
            }

            float Vector3::Magnitude() const
            {
                return sqrt(SqrMagnitude());
            }

            Vector3 Vector3::Normalized() const
            {
                assert(SqrMagnitude() != 0.0f);
                const float inv_length_ = 1.0f / Magnitude();
                return *this * inv_length_;
            }

            void Vector3::Normalize()
            {
                assert(SqrMagnitude() != 0.0f);
                const float inv_length_ = 1.0f / Magnitude();

                x_ *= inv_length_;
                y_ *= inv_length_;
                z_ *= inv_length_;
            }

            Vector3 Scale(const Vector3& v1_a, const Vector3& v2_a)
            {
                return Vector3(v1_a.x_ * v2_a.x_, v1_a.y_ * v2_a.y_, v1_a.z_ * v2_a.z_);
            }

            float Dot(const Vector3& lhs_a, const Vector3& rhs_a)
            {
                return lhs_a.x_ * rhs_a.x_ + lhs_a.y_ * rhs_a.y_ + lhs_a.z_ * rhs_a.z_;
            }

            Vector3 Cross(const Vector3& lhs_a, const Vector3& rhs_a)
            {
                return Vector3(
                    lhs_a.y_ * rhs_a.z_ - lhs_a.z_ * rhs_a.y_,
                    lhs_a.z_ * rhs_a.x_ - lhs_a.x_ * rhs_a.z_,
                    lhs_a.x_ * rhs_a.y_ - lhs_a.y_ * rhs_a.x_);
            }

            Vector3 Lerp(const Vector3& from_a, const Vector3& to_a, float factor_a)
            {
                return from_a * (1.0f - factor_a) + to_a * factor_a;
            }

            float Angle(const Vector3& from_a, const Vector3& to_a)
            {
                const float cos_theta_ = Dot(from_a, to_a) / sqrt(from_a.SqrMagnitude() * to_a.SqrMagnitude());
                return std::acos(std::fmin(1.0f, cos_theta_));
            }

            float SqrDistance(const Vector3& p1_a, const Vector3& p2_a)
            {
                return (p1_a - p2_a).SqrMagnitude();
            }

            float Distance(const Vector3& p1_a, const Vector3& p2_a)
            {
                return (p1_a - p2_a).Magnitude();
            }
        } // namespace Math
    } // namespace Aux
} // namespace CE_Kernel
