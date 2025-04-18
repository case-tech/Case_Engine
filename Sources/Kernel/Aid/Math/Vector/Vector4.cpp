#include "Vector4.hpp"

#include <cassert>
#include <cmath>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace Math
        {
            Vector4::Vector4()
                : x_(0.0f), y_(0.0f), z_(0.0f), w_(0.0f) {}

            Vector4::Vector4(float x_a, float y_a, float z_a, float w_a)
                : x_(x_a), y_(y_a), z_(z_a), w_(w_a) {}

            Vector4::Vector4(const Vector3& v_a)
                : x_(v_a.x_), y_(v_a.y_), z_(v_a.z_), w_(0.0f) {}

            Vector4::Vector4(const Vector3& v_a, float w_a)
                : x_(v_a.x_), y_(v_a.y_), z_(v_a.z_), w_(w_a) {}

            float operator==(const Vector4& v1_a, const Vector4& v2_a)
            {
                const float epsilon_ = 1e-6f;
                return std::abs(v1_a.x_ - v2_a.x_) < epsilon_ && 
                       std::abs(v1_a.y_ - v2_a.y_) < epsilon_ && 
                       std::abs(v1_a.z_ - v2_a.z_) < epsilon_ && 
                       std::abs(v1_a.w_ - v2_a.w_) < epsilon_;
            }

            float operator!=(const Vector4& v1_a, const Vector4& v2_a)
            {
                return !(v1_a == v2_a);
            }

            Vector4 operator+(const Vector4& v1_a, const Vector4& v2_a)
            {
                return Vector4(v1_a.x_ + v2_a.x_, v1_a.y_ + v2_a.y_, v1_a.z_ + v2_a.z_, v1_a.w_ + v2_a.w_);
            }

            Vector4 operator-(const Vector4& v1_a, const Vector4& v2_a)
            {
                return Vector4(v1_a.x_ - v2_a.x_, v1_a.y_ - v2_a.y_, v1_a.z_ - v2_a.z_, v1_a.w_ - v2_a.w_);
            }

            Vector4 operator-(const Vector4& v_a)
            {
                return Vector4(-v_a.x_, -v_a.y_, -v_a.z_, -v_a.w_);
            }

            Vector4 operator*(const Vector4& v_a, const float s_a)
            {
                return Vector4(v_a.x_ * s_a, v_a.y_ * s_a, v_a.z_ * s_a, v_a.w_ * s_a);
            }

            Vector4 operator*(const float s_a, const Vector4& v_a)
            {
                return v_a * s_a;
            }

            Vector4 operator/(const Vector4& v_a, const float s_a)
            {
                assert(s_a != 0.0f);
                return v_a * (1.0f / s_a);
            }

            std::ostream& operator<<(std::ostream& out_a, const Vector4& v_a)
            {
                out_a << "(" << v_a.x_ << ", " << v_a.y_ << ", " << v_a.z_ << ", " << v_a.w_ << ")";
                return out_a;
            }

            float Vector4::SqrMagnitude() const
            {
                return Dot(*this, *this);
            }

            float Vector4::Magnitude() const
            {
                return sqrt(SqrMagnitude());
            }

            Vector4 Vector4::Normalized() const
            {
                assert(SqrMagnitude() != 0.0f);
                const float inv_length_ = 1.0f / Magnitude();
                return (*this) * inv_length_;
            }

            void Vector4::Normalize()
            {
                assert(SqrMagnitude() != 0.0f);
                const float inv_length_ = 1.0f / Magnitude();

                x_ *= inv_length_;
                y_ *= inv_length_;
                z_ *= inv_length_;
                w_ *= inv_length_;
            }

            Vector4 Scale(const Vector4& v1_a, const Vector4& v2_a)
            {
                return Vector4(v1_a.x_ * v2_a.x_, v1_a.y_ * v2_a.y_, v1_a.z_ * v2_a.z_, v1_a.w_ * v2_a.w_);
            }

            float Dot(const Vector4& lhs_a, const Vector4& rhs_a)
            {
                return lhs_a.x_ * rhs_a.x_ + lhs_a.y_ * rhs_a.y_ + lhs_a.z_ * rhs_a.z_ + lhs_a.w_ * rhs_a.w_;
            }

            float SqrDistance(const Vector4& p1_a, const Vector4& p2_a)
            {
                return (p1_a - p2_a).SqrMagnitude();
            }

            float Distance(const Vector4& p1_a, const Vector4& p2_a)
            {
                return (p1_a - p2_a).Magnitude();
            }
        } // namespace Math
    } // namespace Aid
} // namespace CE_Kernel
