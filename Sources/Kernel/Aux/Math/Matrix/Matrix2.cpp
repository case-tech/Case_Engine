#include "Matrix2.hpp"

#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Math
        {
            const Matrix2 Matrix2::IDENTITY = Matrix2();
            const Matrix2 Matrix2::ZERO = Matrix2({0.0f, 0.0f, 0.0f, 0.0f});

            Matrix2::Matrix2()
                : m_{1.0f, 0.0f, 0.0f, 1.0f} {}

            Matrix2::Matrix2(const float arr_a[4])
            {
                std::memcpy(m_, arr_a, 4 * sizeof(float));
            }

            Matrix2::Matrix2(float entry00_a, float entry01_a, float entry10_a, float entry11_a)
                : m_{entry00_a, entry01_a, entry10_a, entry11_a} {}

            float Matrix2::operator[](std::size_t index_a) const
            {
                assert(index_a < 4);
                return m_[index_a];
            }

            bool operator==(const Matrix2& A_a, const Matrix2& B_a)
            {
                const float epsilon_ = 1e-6;
                for (std::size_t i = 0; i < 4; ++i) 
                {
                    if (std::abs(A_a[i] - B_a[i]) > epsilon_)
                        return false;
                }

                return true;
            }

            bool operator!=(const Matrix2& A_a, const Matrix2& B_a)
            {
                return !(A_a == B_a);
            }

            Matrix2 operator+(const Matrix2& A_a, const Matrix2& B_a)
            {
                return Matrix2(
                    A_a[0] + B_a[0],
                    A_a[1] + B_a[1],
                    A_a[2] + B_a[2],
                    A_a[3] + B_a[3]);
            }

            Matrix2 operator-(const Matrix2& lhs_a, const Matrix2& rhs_a)
            {
                return Matrix2(
                    lhs_a[0] - rhs_a[0],
                    lhs_a[1] - rhs_a[1],
                    lhs_a[2] - rhs_a[2],
                    lhs_a[3] - rhs_a[3]);
            }

            Matrix2 operator-(const Matrix2& A_a)
            {
                return Matrix2(
                    -A_a[0],
                    -A_a[1],
                    -A_a[2],
                    -A_a[3]);
            }

            Matrix2 operator*(const Matrix2& A_a, const float s_a)
            {
                return Matrix2(
                    A_a[0] * s_a,
                    A_a[1] * s_a,
                    A_a[2] * s_a,
                    A_a[3] * s_a);
            }

            Matrix2 operator*(const float s_a, const Matrix2& A_a)
            {
                return A_a * s_a;
            }

            Vector2 operator*(const Matrix2& lhs_a, const Vector2& rhs_a)
            {
                return Vector2(
                    lhs_a[0] * rhs_a.x_ + lhs_a[1] * rhs_a.y_,
                    lhs_a[2] * rhs_a.x_ + lhs_a[3] * rhs_a.y_);
            }

            Vector2 operator*(const Vector2& lhs_a, const Matrix2& rhs_a)
            {
                return Vector2(
                    lhs_a.x_ * rhs_a[0] + lhs_a.y_ * rhs_a[2],
                    lhs_a.x_ * rhs_a[1] + lhs_a.y_ * rhs_a[3]);
            }

            Matrix2 operator*(const Matrix2& lhs_a, const Matrix2& rhs_a)
            {
                return Matrix2(
                    lhs_a[0] * rhs_a[0] + lhs_a[1] * rhs_a[2],
                    lhs_a[0] * rhs_a[1] + lhs_a[1] * rhs_a[3],

                    lhs_a[2] * rhs_a[0] + lhs_a[3] * rhs_a[2],
                    lhs_a[2] * rhs_a[1] + lhs_a[3] * rhs_a[3]);
            }

            std::ostream& operator<<(std::ostream& out_a, const Matrix2& A_a)
            {
                std::string string_matrix_[4];
                std::size_t column_lengths_[2] = {0, 0};

                for (std::size_t i = 0; i < 2; ++i)
                {
                    for (std::size_t j = 0; j < 2; ++j) 
                    {
                        const std::string str_ = std::to_string(A_a[2 * i + j]);

                        const std::size_t len_ = str_.length();
                        if (len_ > column_lengths_[j])
                            column_lengths_[j] = len_;

                        string_matrix_[2 * i + j] = str_;
                    }
                }

                const std::size_t total_length_ = column_lengths_[0] + column_lengths_[1] + 1;

                out_a << "┌─";
                for (std::size_t i = 0; i < total_length_; ++i)
                    out_a << " ";
                out_a << "─┐" << std::endl;

                for (std::size_t i = 0; i < 2; ++i) 
                {
                    out_a << "│";

                    for (std::size_t j = 0; j < 2; ++j) 
                    {
                        const std::string str_ = string_matrix_[2 * i + j];
                        const std::size_t len_ = str_.length();

                        for (std::size_t k = 0; k < column_lengths_[j] - len_; ++k) 
                        {
                            out_a << " ";
                        }

                        out_a << " " << str_;
                    }

                    out_a << " │" << std::endl;
                }

                out_a << "└─";
                for (std::size_t i = 0; i < total_length_; ++i)
                    out_a << " ";
                out_a << "─┘";

                return out_a;
            }

            Matrix2 Matrix2::Transposed() const
            {
                return Matrix2(
                    m_[0],
                    m_[2],
                    m_[1],
                    m_[3]);
            }

            void Matrix2::Transpose()
            {
                std::swap(m_[1], m_[2]);
            }

            float Matrix2::Determinant() const
            {
                return m_[0] * m_[3] - m_[1] * m_[2];
            }

            Matrix2 Matrix2::Inverse() const
            {
                const float det_ = Determinant();
                assert(det_ != 0.0f);

                const float inv_det_ = 1.0f / det_;
                return Matrix2(
                    m_[3] * inv_det_,
                   -m_[1] * inv_det_,
                   -m_[2] * inv_det_,
                    m_[0] * inv_det_);
            }

            Matrix2 Matrix2::Scaling(const Vector2& scale_factors_a)
            {
                return Matrix2(
                    scale_factors_a.x_,
                    0.0f,
                    0.0f,
                    scale_factors_a.y_);
            }

            Matrix2 Matrix2::Scaling(const float factor_a)
            {
                return Matrix2(
                    factor_a,
                    0.0f,
                    0.0f,
                    factor_a);
            }

            Matrix2 Matrix2::AngleRotation(const float angle_a)
            {
                const float cos_theta_ = std::cos(angle_a);
                const float sin_theta_ = std::sin(angle_a);
                return Matrix2(
                    cos_theta_,
                    -sin_theta_,
                    sin_theta_,
                    cos_theta_);
            }

            Matrix2 Matrix2::FromToRotation(const Vector2& from_direction_a, const Vector2& to_direction_a)
            {
                assert(from_direction_a.SqrMagnitude() > 0.0f && to_direction_a.SqrMagnitude() > 0.0f);

                const float theta_ = Angle(from_direction_a, to_direction_a);

                return AngleRotation(theta_);
            }
        } // namespace Math
    } // namespace Aux
} // namespace CE_Kernel
