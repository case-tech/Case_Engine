// Copyright (c) 2025 Case Technologies

#pragma once
#include "Matrix3.hpp"
#include <ostream>

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Math
        {
            class Vector3;
            class Vector4;
            class Quaternion;

            class Matrix4
            {
            public:
                Matrix4();
                Matrix4(const float arr_a[16]);
                Matrix4(float entry00_a, float entry01_a, float entry02_a, float entry03_a, 
                        float entry10_a, float entry11_a, float entry12_a, float entry13_a, 
                        float entry20_a, float entry21_a, float entry22_a, float entry23_a, 
                        float entry30_a, float entry31_a, float entry32_a, float entry33_a);

                Matrix4(const Matrix3& A_a);
                Matrix4(const Quaternion& q_a);
                Matrix4(const Matrix4& other_a) = default;

                float operator[](std::size_t index_a) const;
                friend bool operator==(const Matrix4& A_a, const Matrix4& B_a);
                friend bool operator!=(const Matrix4& A_a, const Matrix4& B_a);
                friend Matrix4 operator+(const Matrix4& A_a, const Matrix4& B_a);
                friend Matrix4 operator-(const Matrix4& lhs_a, const Matrix4& rhs_a);
                friend Matrix4 operator-(const Matrix4& A_a);
                friend Matrix4 operator*(const Matrix4& A_a, const float s_a);
                friend Matrix4 operator*(const float s_a, const Matrix4& A_a);
                friend Vector4 operator*(const Matrix4& lhs_a, const Vector4& rhs_a);
                friend Vector4 operator*(const Vector4& lhs_a, const Matrix4& rhs_a);
                friend Matrix4 operator*(const Matrix4& lhs_a, const Matrix4& rhs_a);
                friend std::ostream& operator<<(std::ostream& out_a, const Matrix4& A_a);

                Matrix4 Transposed() const;
                void Transpose();
                float Determinant() const;
                Matrix4 Inverse() const;

                static Matrix4 Scaling(const Vector3& scale_factors_a);
                static Matrix4 Scaling(const float factor_a);
                static Matrix4 Translation(const Vector3& translation_a);
                static Matrix4 AngleAxis(const float angle_a, const Vector3& axis_a);
                static Matrix4 Euler(const Vector3& euler_angles_a);
                static Matrix4 FromToRotation(const Vector3& from_direction_a, const Vector3& to_direction_a);
                static Matrix4 LookRotation(const Vector3& forward_a, const Vector3& upwards_a);
                static Matrix4 LookRotation(const Vector3& target_a, const Vector3& eye_a, const Vector3& upwards_a);

            public:
                static const Matrix4 IDENTITY;
                static const Matrix4 ZERO;

            private:
                float m_[16];
            };
        } // namespace Math
    } // namespace Aux
} // namespace CE_Kernel
