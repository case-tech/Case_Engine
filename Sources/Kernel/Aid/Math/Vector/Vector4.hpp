// Copyright (c) 2025 Case Technologies

#pragma once
#include "Vector3.hpp"
#include <ostream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace Math
        {
            class Vector4
            {
            public:
                Vector4();
                Vector4(float x_a, float y_a, float z_a, float w_a);
                Vector4(const Vector3& v_a);
                Vector4(const Vector3& v_a, float w_a);

                friend float operator==(const Vector4& v1_a, const Vector4& v2_a);
                friend float operator!=(const Vector4& v1_a, const Vector4& v2_a);
                friend Vector4 operator+(const Vector4& v1_a, const Vector4& v2_a);
                friend Vector4 operator-(const Vector4& v1_a, const Vector4& v2_a);
                friend Vector4 operator-(const Vector4& v_a);
                friend Vector4 operator*(const Vector4& v_a, const float s_a);
                friend Vector4 operator*(const float s_a, const Vector4& v_a);
                friend Vector4 operator/(const Vector4& v_a, const float s_a);
                friend std::ostream& operator<<(std::ostream& out_a, const Vector4& v_a);

                float SqrMagnitude() const;
                float Magnitude() const;
                Vector4 Normalized() const;
                void Normalize();

            public:
                float x_;
                float y_;
                float z_;
                float w_;
            };

            Vector4 Scale(const Vector4& v1_a, const Vector4& v2_a);
            float Dot(const Vector4& lhs_a, const Vector4& rhs_a);
            float SqrDistance(const Vector4& p1_a, const Vector4& p2_a);
            float Distance(const Vector4& p1_a, const Vector4& p2_a);
        } // namespace Math
    } // namespace Aid
} // namespace CE_Kernel