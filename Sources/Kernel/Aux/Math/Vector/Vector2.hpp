// Copyright (c) 2025 Case Technologies

#pragma once
#include <ostream>

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Math
        {
            class Vector2
            {
            public:
                Vector2();
                Vector2(float x_a, float y_a);

                friend float operator==(const Vector2& v1_a, const Vector2& v2_a);
                friend float operator!=(const Vector2& v1_a, const Vector2& v2_a);
                friend Vector2 operator+(const Vector2& v1_a, const Vector2& v2_a);
                friend Vector2 operator-(const Vector2& v1_a, const Vector2& v2_a);
                friend Vector2 operator-(const Vector2& v_a);
                friend Vector2 operator*(const Vector2& v_a, const float s_a);
                friend Vector2 operator*(const float s_a, const Vector2& v_a);
                friend Vector2 operator/(const Vector2& v_a, const float s_a);
                friend std::ostream& operator<<(std::ostream& out_a, const Vector2& v_a);

                float SqrMagnitude() const;
                float Magnitude() const;
                Vector2 Normalized() const;
                void Normalize();

            public:
                float x_;
                float y_;
            };

            Vector2 Scale(const Vector2& v1_a, const Vector2& v2_a);
            float Dot(const Vector2& lhs_a, const Vector2& rhs_a);
            float Angle(const Vector2& from_a, const Vector2& to_a);
            float SqrDistance(const Vector2& p1_a, const Vector2& p2_a);
            float Distance(const Vector2& p1_a, const Vector2& p2_a);
        } // namespace Math
    } // namespace Aux
} // namespace CE_Kernel
