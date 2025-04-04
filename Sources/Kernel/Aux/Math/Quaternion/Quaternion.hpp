// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Vector/Vector3.hpp"
#include <ostream>

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Math
        {
            class Quaternion
            {
            public:
                Quaternion();
                Quaternion(float w_a, float x_a, float y_a, float z_a);
                Quaternion(const float s_a, const Vector3& v_a);

                friend float operator==(const Quaternion& q1_a, const Quaternion& q2_a);
                friend float operator!=(const Quaternion& q1_a, const Quaternion& q2_a);
                friend Quaternion operator*(const Quaternion& lhs_a, const Quaternion& rhs_a);
                friend Vector3 operator*(const Quaternion& q_a, const Vector3& v_a);
                friend std::ostream& operator<<(std::ostream& out_a, const Quaternion& q_a);

                float SqrMagnitude() const;
                float Magnitude() const;
                Quaternion Normalized() const;
                void Normalize();
                void RotateToWards(const Quaternion& target_a, float max_radians_delta_a);

                static Quaternion AngleAxis(const float angle_a, const Vector3& axis_a);
                static Quaternion Euler(const Vector3& euler_angles_a);
                static Quaternion FromToRotation(const Vector3& from_direction_a, const Vector3& to_direction_a);
                static Quaternion LookRotation(const Vector3& forward_a);
                static Quaternion LookRotation(const Vector3& forward_a, const Vector3& upwards_a);

                Quaternion Conjugate() const;
                Quaternion Inverse() const;

            public:
                float w_;
                float x_;
                float y_;
                float z_;

            public:
                static const Quaternion IDENTITY;
            };

            float Dot(const Quaternion& lhs_a, const Quaternion& rhs_a);
            float Angle(const Quaternion& from_a, const Quaternion& to_a);
        } // namespace Math
    } // namespace Aux
} // namespace CE_Kernel
