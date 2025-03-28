// Copyright (c) 2025 Case Technologies

#pragma once
#include <ostream>

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Math
        {
            class Vector4;

            class Vector3
            {
            public:
                Vector3();
                Vector3(float x_a, float y_a, float z_a);
                Vector3(const Vector4& v_a);

                friend float operator==(const Vector3& v1_a, const Vector3& v2_a);
                friend float operator!=(const Vector3& v1_a, const Vector3& v2_a);
                friend Vector3 operator+(const Vector3& v1_a, const Vector3& v2_A);
                friend Vector3& operator+=(Vector3& v1_a, const Vector3& v2_a);
                friend Vector3 operator-(const Vector3& v1_a, const Vector3& v2_a);
                friend Vector3& operator-=(Vector3& v1_a, const Vector3& v2_a);
                friend Vector3 operator-(const Vector3& v_a);
                friend Vector3 operator*(const Vector3& v_a, const float s_a);
                friend Vector3& operator*=(Vector3& v_a, const float s_a);
                friend Vector3 operator*(const float s_a, const Vector3& v_a);
                friend Vector3 operator/(const Vector3& v_a, const float s_a);
                friend Vector3& operator/=(Vector3& v_a, const float s_a);
                friend std::ostream& operator<<(std::ostream& out_a, const Vector3& v_a);

                float SqrMagnitude() const;
                float Magnitude() const;
                Vector3 Normalized() const;
                void Normalize();

            public:
                float x_;
                float y_;
                float z_;
            };

            Vector3 Scale(const Vector3& v1_a, const Vector3& v2_a);
            float Dot(const Vector3& lhs_a, const Vector3& rhs_a);
            Vector3 Cross(const Vector3& lhs_a, const Vector3& rhs_a);
            Vector3 Lerp(const Vector3& from_a, const Vector3& to_a, float factor_a);
            float Angle(const Vector3& from_a, const Vector3& to_a);
            float SqrDistance(const Vector3& p1_a, const Vector3& p2_a);
            float Distance(const Vector3& p1_a, const Vector3& p2_a);
        } // namespace Math
    } // namespace Aux
} // namespace CE_Kernel
