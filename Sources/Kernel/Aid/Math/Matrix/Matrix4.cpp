#include "Matrix4.hpp"

#include "../Quaternion/Quaternion.hpp"
#include "../Vector/Vector3.hpp"
#include "../Vector/Vector4.hpp"

#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace Math
        {
            const Matrix4 Matrix4::IDENTITY = Matrix4();
            const Matrix4 Matrix4::ZERO = Matrix4({0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                                   0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
                                                   0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});

            Matrix4::Matrix4()
                : m_{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
                     1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
                     1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f} {}

            Matrix4::Matrix4(const float arr_a[16])
            {
                std::memcpy(m_, arr_a, 16 * sizeof(float));
            }

            Matrix4::Matrix4(float entry00_a, float entry01_a, float entry02_a, float entry03_a, 
                             float entry10_a, float entry11_a, float entry12_a, float entry13_a, 
                             float entry20_a, float entry21_a, float entry22_a, float entry23_a, 
                             float entry30_a, float entry31_a, float entry32_a, float entry33_a)
                              : m_{entry00_a, entry01_a, entry02_a, entry03_a, entry10_a, entry11_a,
                                   entry12_a, entry13_a, entry20_a, entry21_a, entry22_a, entry23_a, 
                                   entry30_a, entry31_a, entry32_a, entry33_a} {}

            Matrix4::Matrix4(const Matrix3& a_a)
                : m_{a_a[0], a_a[1], a_a[2], 0.0f, 
                     a_a[3], a_a[4], a_a[5], 0.0f, 
                     a_a[6], a_a[7], a_a[8], 0.0f, 
                     0.0f, 0.0f, 0.0f, 1.0f} {}

            Matrix4::Matrix4(const Quaternion& q_a)
                : m_{1.0f - 2.0f * q_a.y_ * q_a.y_ - 2.0f * q_a.z_ * q_a.z_,
                     2.0f * q_a.x_ * q_a.y_ - 2.0f * q_a.w_ * q_a.z_,
                     2.0f * q_a.x_ * q_a.z_ + 2.0f * q_a.w_ * q_a.y_,
                     0.0f,
                     2.0f * q_a.x_ * q_a.y_ + 2.0f * q_a.w_ * q_a.z_,
                     1.0f - 2.0f * q_a.x_ * q_a.x_ - 2.0f * q_a.z_ * q_a.z_,
                     2.0f * q_a.y_ * q_a.z_ - 2.0f * q_a.w_ * q_a.x_,
                     0.0f,
                     2.0f * q_a.x_ * q_a.z_ - 2.0f * q_a.w_ * q_a.y_,
                     2.0f * q_a.y_ * q_a.z_ + 2.0f * q_a.w_ * q_a.x_,
                     1.0f - 2.0f * q_a.x_ * q_a.x_ - 2.0f * q_a.y_ * q_a.y_,
                     1.0f} {}

            float Matrix4::operator[](std::size_t index_a) const
            {
                assert(index_a < 16);
                return m_[index_a];
            }

            bool operator==(const Matrix4& a_a, const Matrix4& b_a)
            {
                const float epsilon_ = 1e-6f;
                for (std::size_t i_ = 0; i_ < 16; ++i_) 
                {
                    if (std::abs(a_a[i_] - b_a[i_]) > epsilon_)
                        return false;
                }

                return true;
            }

            bool operator!=(const Matrix4& a_a, const Matrix4& b_a)
            {
                return !(a_a == b_a);
            }

            Matrix4 operator+(const Matrix4& a_a, const Matrix4& b_a)
            {
                return Matrix4(
                    a_a[0] + b_a[0],
                    a_a[1] + b_a[1],
                    a_a[2] + b_a[2],
                    a_a[3] + b_a[3],
                    a_a[4] + b_a[4],
                    a_a[5] + b_a[5],
                    a_a[6] + b_a[6],
                    a_a[7] + b_a[7],
                    a_a[8] + b_a[8],
                    a_a[9] + b_a[9],
                    a_a[10] + b_a[10],
                    a_a[11] + b_a[11],
                    a_a[12] + b_a[12],
                    a_a[13] + b_a[13],
                    a_a[14] + b_a[14],
                    a_a[15] + b_a[15]);
            }

            Matrix4 operator-(const Matrix4& lhs_a, const Matrix4& rhs_a)
            {
                return Matrix4(
                    lhs_a[0] - rhs_a[0],
                    lhs_a[1] - rhs_a[1],
                    lhs_a[2] - rhs_a[2],
                    lhs_a[3] - rhs_a[3],
                    lhs_a[4] - rhs_a[4],
                    lhs_a[5] - rhs_a[5],
                    lhs_a[6] - rhs_a[6],
                    lhs_a[7] - rhs_a[7],
                    lhs_a[8] - rhs_a[8],
                    lhs_a[9] - rhs_a[9],
                    lhs_a[10] - rhs_a[10],
                    lhs_a[11] - rhs_a[11],
                    lhs_a[12] - rhs_a[12],
                    lhs_a[13] - rhs_a[13],
                    lhs_a[14] - rhs_a[14],
                    lhs_a[15] - rhs_a[15]);
            }

            Matrix4 operator-(const Matrix4& a_a)
            {
                return Matrix4(
                    -a_a[0],
                    -a_a[1],
                    -a_a[2],
                    -a_a[3],
                    -a_a[4],
                    -a_a[5],
                    -a_a[6],
                    -a_a[7],
                    -a_a[8],
                    -a_a[9],
                    -a_a[10],
                    -a_a[11],
                    -a_a[12],
                    -a_a[13],
                    -a_a[14],
                    -a_a[15]);
            }

            Matrix4 operator*(const Matrix4& a_a, const float s_a)
            {
                return Matrix4(
                    a_a[0] * s_a,
                    a_a[1] * s_a,
                    a_a[2] * s_a,
                    a_a[3] * s_a,
                    a_a[4] * s_a,
                    a_a[5] * s_a,
                    a_a[6] * s_a,
                    a_a[7] * s_a,
                    a_a[8] * s_a,
                    a_a[9] * s_a,
                    a_a[10] * s_a,
                    a_a[11] * s_a,
                    a_a[12] * s_a,
                    a_a[13] * s_a,
                    a_a[14] * s_a,
                    a_a[15] * s_a);
            }

            Matrix4 operator*(const float s_a, const Matrix4& a_a)
            {
                return a_a * s_a;
            }

            Vector4 operator*(const Matrix4& lhs_a, const Vector4& rhs_a)
            {
                return Vector4(
                    lhs_a[0] * rhs_a.x_ + lhs_a[1] * rhs_a.y_ + lhs_a[2] * rhs_a.z_ + lhs_a[3] * rhs_a.w_,
                    lhs_a[4] * rhs_a.x_ + lhs_a[5] * rhs_a.y_ + lhs_a[6] * rhs_a.z_ + lhs_a[7] * rhs_a.w_,
                    lhs_a[8] * rhs_a.x_ + lhs_a[9] * rhs_a.y_ + lhs_a[10] * rhs_a.z_ + lhs_a[11] * rhs_a.w_,
                    lhs_a[12] * rhs_a.x_ + lhs_a[13] * rhs_a.y_ + lhs_a[14] * rhs_a.z_ + lhs_a[15] * rhs_a.w_);
            }

            Vector4 operator*(const Vector4& lhs_a, const Matrix4& rhs_a)
            {
                return Vector4(
                    lhs_a.x_ * rhs_a[0] + lhs_a.y_ * rhs_a[4] + lhs_a.z_ * rhs_a[8] + lhs_a.w_ * rhs_a[12],
                    lhs_a.x_ * rhs_a[1] + lhs_a.y_ * rhs_a[5] + lhs_a.z_ * rhs_a[9] + lhs_a.w_ * rhs_a[13],
                    lhs_a.x_ * rhs_a[2] + lhs_a.y_ * rhs_a[6] + lhs_a.z_ * rhs_a[10] + lhs_a.w_ * rhs_a[14],
                    lhs_a.x_ * rhs_a[3] + lhs_a.y_ * rhs_a[7] + lhs_a.z_ * rhs_a[11] + lhs_a.w_ * rhs_a[15]);
            }

            Matrix4 operator*(const Matrix4& lhs_a, const Matrix4& rhs_a)
            {
                return Matrix4(
                    lhs_a[0] * rhs_a[0] + lhs_a[1] * rhs_a[4] + lhs_a[2] * rhs_a[8] + lhs_a[3] * rhs_a[12],
                    lhs_a[0] * rhs_a[1] + lhs_a[1] * rhs_a[5] + lhs_a[2] * rhs_a[9] + lhs_a[3] * rhs_a[13],
                    lhs_a[0] * rhs_a[2] + lhs_a[1] * rhs_a[6] + lhs_a[2] * rhs_a[10] + lhs_a[3] * rhs_a[14],
                    lhs_a[0] * rhs_a[3] + lhs_a[1] * rhs_a[7] + lhs_a[2] * rhs_a[11] + lhs_a[3] * rhs_a[15],

                    lhs_a[4] * rhs_a[0] + lhs_a[5] * rhs_a[4] + lhs_a[6] * rhs_a[8] + lhs_a[7] * rhs_a[12],
                    lhs_a[4] * rhs_a[1] + lhs_a[5] * rhs_a[5] + lhs_a[6] * rhs_a[9] + lhs_a[7] * rhs_a[13],
                    lhs_a[4] * rhs_a[2] + lhs_a[5] * rhs_a[6] + lhs_a[6] * rhs_a[10] + lhs_a[7] * rhs_a[14],
                    lhs_a[4] * rhs_a[3] + lhs_a[5] * rhs_a[7] + lhs_a[6] * rhs_a[11] + lhs_a[7] * rhs_a[15],

                    lhs_a[8] * rhs_a[0] + lhs_a[9] * rhs_a[4] + lhs_a[10] * rhs_a[8] + lhs_a[11] * rhs_a[12],
                    lhs_a[8] * rhs_a[1] + lhs_a[9] * rhs_a[5] + lhs_a[10] * rhs_a[9] + lhs_a[11] * rhs_a[13],
                    lhs_a[8] * rhs_a[2] + lhs_a[9] * rhs_a[6] + lhs_a[10] * rhs_a[10] + lhs_a[11] * rhs_a[14],
                    lhs_a[8] * rhs_a[3] + lhs_a[9] * rhs_a[7] + lhs_a[10] * rhs_a[11] + lhs_a[11] * rhs_a[15],

                    lhs_a[12] * rhs_a[0] + lhs_a[13] * rhs_a[4] + lhs_a[14] * rhs_a[8] + lhs_a[15] * rhs_a[12],
                    lhs_a[12] * rhs_a[1] + lhs_a[13] * rhs_a[5] + lhs_a[14] * rhs_a[9] + lhs_a[15] * rhs_a[13],
                    lhs_a[12] * rhs_a[2] + lhs_a[13] * rhs_a[6] + lhs_a[14] * rhs_a[10] + lhs_a[15] * rhs_a[14],
                    lhs_a[12] * rhs_a[3] + lhs_a[13] * rhs_a[7] + lhs_a[14] * rhs_a[11] + lhs_a[15] * rhs_a[15]);
            }

            std::ostream& operator<<(std::ostream& out_a, const Matrix4& a_a)
            {
                std::string string_matrix_[16];
                std::size_t column_lengths_[4] = {0, 0, 0, 0};

                for (std::size_t i_ = 0; i_ < 4; ++i_) 
                {
                    for (std::size_t j_ = 0; j_ < 4; ++j_) 
                    {
                        const std::string str_ = std::to_string(a_a[4 * i_ + j_]);

                        const std::size_t len_ = str_.length();
                        if (len_ > column_lengths_[j_])
                            column_lengths_[j_] = len_;

                        string_matrix_[4 * i_ + j_] = str_;
                    }
                }

                const std::size_t total_length_ = column_lengths_[0] + column_lengths_[1] + 
                                                  column_lengths_[2] + column_lengths_[3] + 3;

                out_a << "┌─";
                for (std::size_t i_ = 0; i_ < total_length_; ++i_)
                    out_a << " ";
                out_a << "─┐" << std::endl;

                for (std::size_t i_ = 0; i_ < 4; ++i_) 
                {
                    out_a << "│";

                    for (std::size_t j_ = 0; j_ < 4; ++j_) 
                    {
                        const std::string str_ = string_matrix_[4 * i_ + j_];
                        const std::size_t len_ = str_.length();

                        for (std::size_t k_ = 0; k_ < column_lengths_[j_] - len_; ++k_)
                        {
                            out_a << " ";
                        }

                        out_a << " " << str_;
                    }

                    out_a << " │" << std::endl;
                }

                out_a << "└─";
                for (std::size_t i_ = 0; i_ < total_length_; ++i_)
                    out_a << " ";
                out_a << "─┘";

                return out_a;
            }

            Matrix4 Matrix4::Transposed() const
            {
                return Matrix4(
                    m_[0],
                    m_[4],
                    m_[8],
                    m_[12],
                    m_[1],
                    m_[5],
                    m_[9],
                    m_[13],
                    m_[2],
                    m_[6],
                    m_[10],
                    m_[14],
                    m_[3],
                    m_[7],
                    m_[11],
                    m_[15]);
            }

            void Matrix4::Transpose()
            {
                std::swap(m_[1], m_[4]);
                std::swap(m_[2], m_[8]);
                std::swap(m_[3], m_[12]);
                std::swap(m_[6], m_[9]);
                std::swap(m_[7], m_[13]);
                std::swap(m_[11], m_[14]);
            }

            float Matrix4::Determinant() const
            {
                const float det1_ = m_[10] * (m_[15] * m_[5]  - m_[7]  * m_[13]) + m_[11] * (m_[13] * m_[6]  - m_[5] * m_[14]) + m_[9] * (m_[14] * m_[7]  - m_[6]  * m_[15]);
                const float det2_ = m_[1]  * (m_[10] * m_[15] - m_[11] * m_[14]) + m_[2]  * (m_[11] * m_[13] - m_[9] * m_[15]) + m_[3] * (m_[9]  * m_[14] - m_[10] * m_[13]);
                const float det3_ = m_[1]  * (m_[6]  * m_[15] - m_[7]  * m_[14]) + m_[2]  * (m_[7]  * m_[13] - m_[5] * m_[15]) + m_[3] * (m_[5]  * m_[14] - m_[6]  * m_[13]);
                const float det4_ = m_[1]  * (m_[6]  * m_[11] - m_[7]  * m_[10]) + m_[2]  * (m_[7]  * m_[9]  - m_[5] * m_[11]) + m_[3] * (m_[5]  * m_[10] - m_[6]  * m_[9]);

                return (m_[0] * det1_ - m_[4] * det2_ + m_[8] * det3_ - m_[12] * det4_);
            }

            Matrix4 Matrix4::Inverse() const
            {
                float inv_[16];

                inv_[0]  =  m_[5] * m_[10] * m_[15] - m_[5] * m_[11] * m_[14] - m_[9] * m_[6] * m_[15] + m_[9] * m_[7] * m_[14] + m_[13] * m_[6] * m_[11] - m_[13] * m_[7] * m_[10];
                inv_[4]  = -m_[4] * m_[10] * m_[15] + m_[4] * m_[11] * m_[14] + m_[8] * m_[6] * m_[15] - m_[8] * m_[7] * m_[14] - m_[12] * m_[6] * m_[11] + m_[12] * m_[7] * m_[10];
                inv_[8]  =  m_[4] * m_[9]  * m_[15] - m_[4] * m_[11] * m_[13] - m_[8] * m_[5] * m_[15] + m_[8] * m_[7] * m_[13] + m_[12] * m_[5] * m_[11] - m_[12] * m_[7] * m_[9];
                inv_[12] = -m_[4] * m_[9]  * m_[14] + m_[4] * m_[10] * m_[13] + m_[8] * m_[5] * m_[14] - m_[8] * m_[6] * m_[13] - m_[12] * m_[5] * m_[10] + m_[12] * m_[6] * m_[9];
                inv_[1]  = -m_[1] * m_[10] * m_[15] + m_[1] * m_[11] * m_[14] + m_[9] * m_[2] * m_[15] - m_[9] * m_[3] * m_[14] - m_[13] * m_[2] * m_[11] + m_[13] * m_[3] * m_[10];
                inv_[5]  =  m_[0] * m_[10] * m_[15] - m_[0] * m_[11] * m_[14] - m_[8] * m_[2] * m_[15] + m_[8] * m_[3] * m_[14] + m_[12] * m_[2] * m_[11] - m_[12] * m_[3] * m_[10];
                inv_[9]  = -m_[0] * m_[9]  * m_[15] + m_[0] * m_[11] * m_[13] + m_[8] * m_[1] * m_[15] - m_[8] * m_[3] * m_[13] - m_[12] * m_[1] * m_[11] + m_[12] * m_[3] * m_[9];
                inv_[13] =  m_[0] * m_[9]  * m_[14] - m_[0] * m_[10] * m_[13] - m_[8] * m_[1] * m_[14] + m_[8] * m_[2] * m_[13] + m_[12] * m_[1] * m_[10] - m_[12] * m_[2] * m_[9];
                inv_[2]  =  m_[1] * m_[6]  * m_[15] - m_[1] * m_[7]  * m_[14] - m_[5] * m_[2] * m_[15] + m_[5] * m_[3] * m_[14] + m_[13] * m_[2] * m_[7]  - m_[13] * m_[3] * m_[6];
                inv_[6]  = -m_[0] * m_[6]  * m_[15] + m_[0] * m_[7]  * m_[14] + m_[4] * m_[2] * m_[15] - m_[4] * m_[3] * m_[14] - m_[12] * m_[2] * m_[7]  + m_[12] * m_[3] * m_[6];
                inv_[10] =  m_[0] * m_[5]  * m_[15] - m_[0] * m_[7]  * m_[13] - m_[4] * m_[1] * m_[15] + m_[4] * m_[3] * m_[13] + m_[12] * m_[1] * m_[7]  - m_[12] * m_[3] * m_[5];
                inv_[14] = -m_[0] * m_[5]  * m_[14] + m_[0] * m_[6]  * m_[13] + m_[4] * m_[1] * m_[14] - m_[4] * m_[2] * m_[13] - m_[12] * m_[1] * m_[6]  + m_[12] * m_[2] * m_[5];
                inv_[3]  = -m_[1] * m_[6]  * m_[11] + m_[1] * m_[7]  * m_[10] + m_[5] * m_[2] * m_[11] - m_[5] * m_[3] * m_[10] - m_[9]  * m_[2] * m_[7]  + m_[9]  * m_[3] * m_[6];
                inv_[7]  =  m_[0] * m_[6]  * m_[11] - m_[0] * m_[7]  * m_[10] - m_[4] * m_[2] * m_[11] + m_[4] * m_[3] * m_[10] + m_[8]  * m_[2] * m_[7]  - m_[8]  * m_[3] * m_[6];
                inv_[11] = -m_[0] * m_[5]  * m_[11] + m_[0] * m_[7]  * m_[9]  + m_[4] * m_[1] * m_[11] - m_[4] * m_[3] * m_[9]  - m_[8]  * m_[1] * m_[7]  + m_[8]  * m_[3] * m_[5];
                inv_[15] =  m_[0] * m_[5]  * m_[10] - m_[0] * m_[6]  * m_[9]  - m_[4] * m_[1] * m_[10] + m_[4] * m_[2] * m_[9]  + m_[8]  * m_[1] * m_[6]  - m_[8]  * m_[2] * m_[5];

                const float det_ = m_[0] * inv_[0] + m_[1] * inv_[4] + m_[2] * inv_[8] + m_[3] * inv_[12];

                assert(det_ != 0.0f);

                const float inv_det_ = 1.0f / det_;
                for (std::size_t i_ = 0; i_ < 16; ++i_)
                    inv_[i_] *= inv_det_;

                return Matrix4(inv_);
            }

            Matrix4 Matrix4::Scaling(const Vector3& scale_factors_a)
            {
                return Matrix4(
                    scale_factors_a.x_,
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f,
                    scale_factors_a.y_,
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f,
                    scale_factors_a.z_,
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f,
                    1.0f);
            }

            Matrix4 Matrix4::Scaling(const float factor_a)
            {
                return Matrix4(
                    factor_a,
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f,
                    factor_a,
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f,
                    factor_a,
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f,
                    1.0f);
            }

            Matrix4 Matrix4::Translation(const Vector3& translation_a)
            {
                return Matrix4(
                    1.0f,
                    0.0f,
                    0.0f,
                    translation_a.x_,
                    0.0f,
                    1.0f,
                    0.0f,
                    translation_a.y_,
                    0.0f,
                    0.0f,
                    1.0f,
                    translation_a.z_,
                    0.0f,
                    0.0f,
                    0.0f,
                    1.0f);
            }

            Matrix4 Matrix4::AngleAxis(const float angle_a, const Vector3& axis_a)
            {
                const float c_ = std::cos(angle_a);
                const float s_ = std::sin(angle_a);

                const float nc_ = 1.0f - c_;

                const float nc_xy_ = nc_ * axis_a.x_ * axis_a.y_;
                const float nc_yz_ = nc_ * axis_a.y_ * axis_a.z_;
                const float nc_xz_ = nc_ * axis_a.x_ * axis_a.z_;

                const float sx_ = s_ * axis_a.x_;
                const float sy_ = s_ * axis_a.y_;
                const float sz_ = s_ * axis_a.z_;

                return Matrix4(
                    nc_ * axis_a.x_ * axis_a.x_ + c_,
                    nc_xy_ - sz_,
                    nc_xz_ + sy_,
                    0.0f,
                    nc_xy_ + sz_,
                    nc_ * axis_a.y_ * axis_a.y_ + c_,
                    nc_yz_ - sx_,
                    0.0f,
                    nc_xz_ - sy_,
                    nc_yz_ + sx_,
                    nc_ * axis_a.z_ * axis_a.z_ + c_,
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f,
                    1.0f);
            }

            Matrix4 Matrix4::Euler(const Vector3& euler_angles_a)
            {
                const float s1_ = std::sin(euler_angles_a.x_);
                const float s2_ = std::sin(euler_angles_a.y_);
                const float s3_ = std::sin(euler_angles_a.z_);

                const float c1_ = std::cos(euler_angles_a.x_);
                const float c2_ = std::cos(euler_angles_a.y_);
                const float c3_ = std::cos(euler_angles_a.z_);

                return Matrix4(
                    c2_ * c3_,
                    -c2_ * s3_,
                    s2_,
                    0.0f,
                    c1_ * s3_ + c3_ * s1_ * s2_,
                    c1_ * c3_ - s1_ * s2_ * s3_,
                    -c2_ * s1_,
                    0.0f,
                    s1_ * s3_ - c1_ * c3_ * s2_,
                    c3_ * s1_ + c1_ * s2_ * s3_,
                    c1_ * c2_,
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f,
                    1.0f);
            }

            Matrix4 Matrix4::FromToRotation(const Vector3& from_direction_a, const Vector3& to_direction_a)
            {
                assert(from_direction_a.SqrMagnitude() > 0.0f && to_direction_a.SqrMagnitude() > 0.0f);
                const Vector3 unit_from_ = from_direction_a.Normalized();
                const Vector3 unit_to_ = to_direction_a.Normalized();
                const float d_ = Dot(unit_from_, unit_to_);

                if (d_ >= 1.0f) 
                {
                    return Matrix4::IDENTITY;
                } 
                
                else if (d_ <= -1.0f) 
                {
                    Vector3 axis_ = Cross(unit_from_, Vector3(1.0f, 0.0f, 0.0f));
                    if (axis_.SqrMagnitude() < 1e-6) 
                    {
                        axis_ = Cross(unit_from_, Vector3(0.0f, 1.0f, 0.0f));
                    }

                    return AngleAxis(3.14159265358979323846f, axis_.Normalized());
                } 
                
                else 
                {
                    Vector3 unit_axis_ = Cross(from_direction_a, to_direction_a);
                    unit_axis_.Normalize();

                    const float theta_ = Angle(from_direction_a, to_direction_a);

                    return AngleAxis(theta_, unit_axis_);
                }
            }

            Matrix4 Matrix4::LookRotation(const Vector3& forward_a, const Vector3& upwards_a)
            {
                assert(Cross(forward_a, upwards_a).SqrMagnitude() != 0.0f);

                const Vector3 z_axis_ = forward_a.Normalized();
                const Vector3 x_axis_ = Cross(upwards_a, z_axis_).Normalized();
                const Vector3 y_axis_ = Cross(z_axis_, x_axis_).Normalized();

                return Matrix4(
                    x_axis_.x_,
                    y_axis_.x_,
                    z_axis_.x_,
                    0.0f,
                    x_axis_.y_,
                    y_axis_.y_,
                    z_axis_.y_,
                    0.0f,
                    x_axis_.z_,
                    y_axis_.z_,
                    z_axis_.z_,
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f,
                    1.0f);
            }

            Matrix4 Matrix4::LookRotation(const Vector3& target_a, const Vector3& eye_a, const Vector3& upwards_a)
            {
                const Vector3 forward_ = target_a - eye_a;

                assert(Cross(forward_, upwards_a).SqrMagnitude() != 0.0f);

                const Vector3 z_axis_ = forward_.Normalized();
                const Vector3 x_axis_ = Cross(upwards_a, z_axis_).Normalized();
                const Vector3 y_axis_ = Cross(z_axis_, x_axis_).Normalized();

                return Matrix4(
                    x_axis_.x_,
                    y_axis_.x_,
                    z_axis_.x_,
                    0.0f,
                    x_axis_.y_,
                    y_axis_.y_,
                    z_axis_.y_,
                    0.0f,
                    x_axis_.z_,
                    y_axis_.z_,
                    z_axis_.z_,
                    0.0f,
                    -Dot(x_axis_, eye_a),
                    -Dot(y_axis_, eye_a),
                    -Dot(z_axis_, eye_a),
                    1.0f);
            }
        } // namespace Math
    } // namespace Aid
} // namespace CE_Kernel