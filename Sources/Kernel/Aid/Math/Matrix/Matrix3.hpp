// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Vector/Vector3.hpp"
#include <ostream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace Math
        {
            class Matrix3
            {
            public:
                Matrix3();
                Matrix3(const float arr_a[9]);
                Matrix3(float entry00_a, float entry01_a, float entry02_a, 
                        float entry10_a, float entry11_a, float entry12_a, 
                        float entry20_a, float entry21_a, float entry22_a);

                Matrix3(const Matrix3& other_a) = default;

                float operator[](std::size_t index_a) const;
                friend bool operator==(const Matrix3& A_a, const Matrix3& B_a);
                friend bool operator!=(const Matrix3& A_a, const Matrix3& B_a);
                friend Matrix3 operator+(const Matrix3& A_a, const Matrix3& B_a);
                friend Matrix3 operator-(const Matrix3& lhs_a, const Matrix3& rhs_a);
                friend Matrix3 operator-(const Matrix3& A_a);
                friend Matrix3 operator*(const Matrix3& A_a, const float s_a);
                friend Matrix3 operator*(const float s_a, const Matrix3& A_a);
                friend Vector3 operator*(const Matrix3& lhs_a, const Vector3& rhs_a);
                friend Vector3 operator*(const Vector3& lhs_a, const Matrix3& rhs_a);
                friend Matrix3 operator*(const Matrix3& lhs_a, const Matrix3& rhs_a);
                friend std::ostream& operator<<(std::ostream& out_a, const Matrix3& A_a);

                Matrix3 Transposed() const;
                void Transpose();
                float Determinant() const;
                Matrix3 Inverse() const;
                static Matrix3 AngleAxis(const float angle_a, const Vector3& axis_a);
                static Matrix3 Euler(const Vector3& euler_angles_a);
                static Matrix3 FromToRotation(const Vector3& from_direction_a, const Vector3& to_direction_a);
                static Matrix3 LookRotation(const Vector3& forward_a, const Vector3& upwards_a);

            public:
                static const Matrix3 IDENTITY;
                static const Matrix3 ZERO;

            private:
                float m_[9];
            };
        } // namespace Math
    } // namespace Aid
} // namespace CE_Kernel
