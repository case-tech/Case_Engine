#include "Matrix3.hpp"

#include <cassert>
#include <cmath>
#include <cstring>

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Math
        {
            const Matrix3 Matrix3::IDENTITY = Matrix3();
            const Matrix3 Matrix3::ZERO = Matrix3({0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});

            Matrix3::Matrix3()
                : m_{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f} {}

            Matrix3::Matrix3(const float arr_a[9])
            {
                std::memcpy(m_, arr_a, 9 * sizeof(float));
            }

            Matrix3::Matrix3(float entry00_a, float entry01_a, float entry02_a, 
                             float entry10_a, float entry11_a, float entry12_a, 
                             float entry20_a, float entry21_a, float entry22_a)
                              : m_{entry00_a, entry01_a, entry02_a, entry10_a, 
                                   entry11_a, entry12_a, entry20_a, entry21_a, entry22_a} {}

            float Matrix3::operator[](std::size_t index_a) const
            {
                assert(index_a < 9);
                return m_[index_a];
            }

            bool operator==(const Matrix3& A_a, const Matrix3& B_a)
            {
                const float epsilon_ = 1e-6;
                for (std::size_t i = 0; i < 9; ++i) 
                {
                    if (std::abs(A_a[i] - B_a[i]) > epsilon_)
                        return false;
                }

                return true;
            }

            bool operator!=(const Matrix3& A_a, const Matrix3& B_a)
            {
                return !(A_a == B_a);
            }

            Matrix3 operator+(const Matrix3& A_a, const Matrix3& B_a)
            {
                return Matrix3(
                    A_a[0] + B_a[0],
                    A_a[1] + B_a[1],
                    A_a[2] + B_a[2],
                    A_a[3] + B_a[3],
                    A_a[4] + B_a[4],
                    A_a[5] + B_a[5],
                    A_a[6] + B_a[6],
                    A_a[7] + B_a[7],
                    A_a[8] + B_a[8]);
            }

            Matrix3 operator-(const Matrix3& lhs_a, const Matrix3& rhs_a)
            {
                return Matrix3(
                    lhs_a[0] - rhs_a[0],
                    lhs_a[1] - rhs_a[1],
                    lhs_a[2] - rhs_a[2],
                    lhs_a[3] - rhs_a[3],
                    lhs_a[4] - rhs_a[4],
                    lhs_a[5] - rhs_a[5],
                    lhs_a[6] - rhs_a[6],
                    lhs_a[7] - rhs_a[7],
                    lhs_a[8] - rhs_a[8]);
            }

            Matrix3 operator-(const Matrix3& A_a)
            {
                return Matrix3(
                    -A_a[0],
                    -A_a[1],
                    -A_a[2],
                    -A_a[3],
                    -A_a[4],
                    -A_a[5],
                    -A_a[6],
                    -A_a[7],
                    -A_a[8]);
            }

            Matrix3 operator*(const Matrix3& A_a, const float s_a)
            {
                return Matrix3(
                    A_a[0] * s_a,
                    A_a[1] * s_a,
                    A_a[2] * s_a,
                    A_a[3] * s_a,
                    A_a[4] * s_a,
                    A_a[5] * s_a,
                    A_a[6] * s_a,
                    A_a[7] * s_a,
                    A_a[8] * s_a);
            }

            Matrix3 operator*(const float s_a, const Matrix3& A_a)
            {
                return A_a * s_a;
            }

            Vector3 operator*(const Matrix3& lhs_a, const Vector3& rhs_a)
            {
                return Vector3(
                    lhs_a[0] * rhs_a.x_ + lhs_a[1] * rhs_a.y_ + lhs_a[2] * rhs_a.z_,
                    lhs_a[3] * rhs_a.x_ + lhs_a[4] * rhs_a.y_ + lhs_a[5] * rhs_a.z_,
                    lhs_a[6] * rhs_a.x_ + lhs_a[7] * rhs_a.y_ + lhs_a[8] * rhs_a.z_);
            }

            Vector3 operator*(const Vector3& lhs_a, const Matrix3& rhs_a)
            {
                return Vector3(
                    lhs_a.x_ * rhs_a[0] + lhs_a.y_ * rhs_a[3] + lhs_a.z_ * rhs_a[6],
                    lhs_a.x_ * rhs_a[1] + lhs_a.y_ * rhs_a[4] + lhs_a.z_ * rhs_a[7],
                    lhs_a.x_ * rhs_a[2] + lhs_a.y_ * rhs_a[5] + lhs_a.z_ * rhs_a[8]);
            }

            Matrix3 operator*(const Matrix3& lhs_a, const Matrix3& rhs_a)
            {
                return Matrix3(
                    lhs_a[0] * rhs_a[0] + lhs_a[1] * rhs_a[3] + lhs_a[2] * rhs_a[6],
                    lhs_a[0] * rhs_a[1] + lhs_a[1] * rhs_a[4] + lhs_a[2] * rhs_a[7],
                    lhs_a[0] * rhs_a[2] + lhs_a[1] * rhs_a[5] + lhs_a[2] * rhs_a[8],

                    lhs_a[3] * rhs_a[0] + lhs_a[4] * rhs_a[3] + lhs_a[5] * rhs_a[6],
                    lhs_a[3] * rhs_a[1] + lhs_a[4] * rhs_a[4] + lhs_a[5] * rhs_a[7],
                    lhs_a[3] * rhs_a[2] + lhs_a[4] * rhs_a[5] + lhs_a[5] * rhs_a[8],

                    lhs_a[6] * rhs_a[0] + lhs_a[7] * rhs_a[3] + lhs_a[8] * rhs_a[6],
                    lhs_a[6] * rhs_a[1] + lhs_a[7] * rhs_a[4] + lhs_a[8] * rhs_a[7],
                    lhs_a[6] * rhs_a[2] + lhs_a[7] * rhs_a[5] + lhs_a[8] * rhs_a[8]);
            }

            std::ostream& operator<<(std::ostream& out_a, const Matrix3& A_a)
            {
                std::string string_matrix_[9];
                std::size_t column_lengths_[3] = {0, 0, 0};

                for (std::size_t i = 0; i < 3; ++i) 
                {
                    for (std::size_t j = 0; j < 3; ++j) 
                    {
                        const std::string str_ = std::to_string(A_a[3 * i + j]);

                        const std::size_t len_ = str_.length();
                        if (len_ > column_lengths_[j])
                            column_lengths_[j] = len_;

                        string_matrix_[3 * i + j] = str_;
                    }
                }

                const std::size_t total_length_ = column_lengths_[0] + column_lengths_[1] + column_lengths_[2] + 2;

                out_a << "┌─";
                for (std::size_t i = 0; i < total_length_; ++i)
                    out_a << " ";
                out_a << "─┐" << std::endl;

                for (std::size_t i = 0; i < 3; ++i) 
                {
                    out_a << "│";

                    for (std::size_t j = 0; j < 3; ++j) 
                    {
                        const std::string str_ = string_matrix_[3 * i + j];
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

            Matrix3 Matrix3::Transposed() const
            {
                return Matrix3(
                    m_[0],
                    m_[3],
                    m_[6],
                    m_[1],
                    m_[4],
                    m_[7],
                    m_[2],
                    m_[5],
                    m_[8]);
            }

            void Matrix3::Transpose()
            {
                std::swap(m_[1], m_[3]);
                std::swap(m_[2], m_[6]);
                std::swap(m_[5], m_[7]);
            }

            float Matrix3::Determinant() const
            {
                return m_[0] * m_[4] * m_[8] + m_[1] * m_[5] * m_[6] + m_[2] * m_[3] * m_[7]
                       - m_[6] * m_[4] * m_[2] - m_[7] * m_[5] * m_[0] - m_[8] * m_[3] * m_[1];
            }

            Matrix3 Matrix3::Inverse() const
            {
                const float det_ = Determinant();
                assert(det_ != 0.0f);

                const float inv_det_ = 1.0f / det_;
                return Matrix3(
                    (m_[4] * m_[8] - m_[5] * m_[7]) * inv_det_,
                    (m_[7] * m_[2] - m_[8] * m_[1]) * inv_det_,
                    (m_[1] * m_[5] - m_[2] * m_[4]) * inv_det_,
                    (m_[6] * m_[5] - m_[3] * m_[8]) * inv_det_,
                    (m_[0] * m_[8] - m_[6] * m_[2]) * inv_det_,
                    (m_[3] * m_[2] - m_[0] * m_[5]) * inv_det_,
                    (m_[3] * m_[7] - m_[6] * m_[4]) * inv_det_,
                    (m_[6] * m_[1] - m_[0] * m_[7]) * inv_det_,
                    (m_[0] * m_[4] - m_[3] * m_[1]) * inv_det_);
            }

            Matrix3 Matrix3::AngleAxis(const float angle_a, const Vector3& axis_a)
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

                return Matrix3(
                    nc_ * axis_a.x_ * axis_a.x_ + c_,
                    nc_xy_ - sz_,
                    nc_xz_ + sy_,
                    nc_xy_ + sz_,
                    nc_ * axis_a.y_ * axis_a.y_ + c_,
                    nc_yz_ - sx_,
                    nc_xz_ - sy_,
                    nc_yz_ + sx_,
                    nc_ * axis_a.z_ * axis_a.z_ + c_);
            }

            Matrix3 Matrix3::Euler(const Vector3& euler_angles_a)
            {
                const float s1_ = std::sin(euler_angles_a.x_);
                const float s2_ = std::sin(euler_angles_a.y_);
                const float s3_ = std::sin(euler_angles_a.z_);

                const float c1_ = std::cos(euler_angles_a.x_);
                const float c2_ = std::cos(euler_angles_a.y_);
                const float c3_ = std::cos(euler_angles_a.z_);

                return Matrix3(
                    c2_ * c3_,
                    -c2_ * s3_,
                    s2_,
                    c1_ * s3_ + c3_ * s1_ * s2_,
                    c1_ * c3_ - s1_ * s2_ * s3_,
                    -c2_ * s1_,
                    s1_ * s3_ - c1_ * c3_ * s2_,
                    c3_ * s1_ + c1_ * s2_ * s3_,
                    c1_ * c2_);
            }

            Matrix3 Matrix3::FromToRotation(const Vector3& from_direction_a, const Vector3& to_direction_a)
            {
                assert(from_direction_a.SqrMagnitude() > 0.0f && to_direction_a.SqrMagnitude() > 0.0f);
                const Vector3 unit_from_ = from_direction_a.Normalized();
                const Vector3 unit_to_ = to_direction_a.Normalized();
                const float d_ = Dot(unit_from_, unit_to_);

                if (d_ >= 1.0f) 
                {
                    return Matrix3::IDENTITY;
                } 
                
                else if (d_ <= -1.0f) 
                {
                    Vector3 axis_ = Cross(unit_from_, Vector3(1.0f, 0.0f, 0.0f));
                    if (axis_.SqrMagnitude() < 1e-6) 
                    {
                        axis_ = Cross(unit_from_, Vector3(0.0f, 1.0f, 0.0f));
                    }

                    return AngleAxis(M_PI, axis_.Normalized());
                } 
                
                else 
                {
                    Vector3 unit_axis_ = Cross(from_direction_a, to_direction_a);
                    unit_axis_.Normalize();

                    const float theta_ = Angle(from_direction_a, to_direction_a);

                    return AngleAxis(theta_, unit_axis_);
                }
            }

            Matrix3 Matrix3::LookRotation(const Vector3& forward_a, const Vector3& upwards_a)
            {
                assert(Cross(forward_a, upwards_a).SqrMagnitude() != 0.0f);

                const Vector3 z_axis_ = forward_a.Normalized();
                const Vector3 x_axis_ = Cross(upwards_a, z_axis_).Normalized();
                const Vector3 y_axis_ = Cross(z_axis_, x_axis_).Normalized();

                return Matrix3(
                    x_axis_.x_,
                    y_axis_.x_,
                    z_axis_.x_,
                    x_axis_.y_,
                    y_axis_.y_,
                    z_axis_.y_,
                    x_axis_.z_,
                    y_axis_.z_,
                    z_axis_.z_);
            }
        } // namespace Math
    } // namespace Aux
} // namespace CE_Kernel
