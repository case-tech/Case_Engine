// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Vector/Vector2.hpp"
#include <ostream>

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Math
        {
            class Matrix2
            {
            public:
                Matrix2();
                Matrix2(const float arr_[4]);
                Matrix2(float entry00_a, float entry01_a, float entry10_a, float entry11_a);
                Matrix2(const Matrix2& other_a) = default;

                float operator[](std::size_t index_a) const;
                friend bool operator==(const Matrix2& A_a, const Matrix2& B_a);
                friend bool operator!=(const Matrix2& A_a, const Matrix2& B_a);
                friend Matrix2 operator+(const Matrix2& A_a, const Matrix2& B_a);
                friend Matrix2 operator-(const Matrix2& lhs_a, const Matrix2& rhs_a);
                friend Matrix2 operator-(const Matrix2& A_a);
                friend Matrix2 operator*(const Matrix2& A_a, const float s_a);
                friend Matrix2 operator*(const float s_a, const Matrix2& A_a);
                friend Vector2 operator*(const Matrix2& lhs_a, const Vector2& rhs_a);
                friend Vector2 operator*(const Vector2& lhs_a, const Matrix2& rhs_a);
                friend Matrix2 operator*(const Matrix2& lhs_a, const Matrix2& rhs_a);
                friend std::ostream& operator<<(std::ostream& out_a, const Matrix2& A_a);

                Matrix2 Transposed() const;
                void Transpose();
                float Determinant() const;
                Matrix2 Inverse() const;

                static Matrix2 Scaling(const Vector2& scale_factors_a);
                static Matrix2 Scaling(const float factor_a);
                static Matrix2 AngleRotation(const float angle_a);
                static Matrix2 FromToRotation(const Vector2& from_direction_a, const Vector2& to_direction_a);

            public:
                static const Matrix2 IDENTITY;
                static const Matrix2 ZERO;

            private:
                float m_[4];
            };
        } // namespace Math
    } // namespace Aux
} // namespace CE_Kernel
