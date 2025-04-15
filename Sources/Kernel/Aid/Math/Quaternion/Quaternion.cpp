#include "Quaternion.hpp"
#include "../Vector/Vector3.hpp"

#include <cassert>
#include <cmath>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace Math
        {
            const Quaternion Quaternion::IDENTITY = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

            Quaternion::Quaternion()
                : w_(1.0f), x_(0.0f), y_(0.0f), z_(0.0f) {}

            Quaternion::Quaternion(float w_a, float x_a, float y_a, float z_a)
                : w_(w_a), x_(x_a), y_(y_a), z_(z_a) {}

            Quaternion::Quaternion(const float s_a, const Vector3& v_a)
                : w_(s_a), x_(v_a.x_), y_(v_a.y_), z_(v_a.z_) {}

            float operator==(const Quaternion& q1_a, const Quaternion& q2_a)
            {
                const float epsilon_ = 1e-6f;
                return std::abs(q1_a.w_ - q2_a.w_) < epsilon_ && 
                       std::abs(q1_a.x_ - q2_a.x_) < epsilon_ && 
                       std::abs(q1_a.y_ - q2_a.y_) < epsilon_ && 
                       std::abs(q1_a.z_ - q2_a.z_) < epsilon_;
            }

            float operator!=(const Quaternion& q1_a, const Quaternion& q2_a)
            {
                return !(q1_a == q2_a);
            }

            Quaternion operator*(const Quaternion& lhs_a, const Quaternion& rhs_a)
            {
                return Quaternion(
                    lhs_a.w_ * rhs_a.w_ - lhs_a.x_ * rhs_a.x_ - lhs_a.y_ * rhs_a.y_ - lhs_a.z_ * rhs_a.z_,
                    lhs_a.w_ * rhs_a.x_ + lhs_a.x_ * rhs_a.w_ + lhs_a.y_ * rhs_a.z_ - lhs_a.z_ * rhs_a.y_,
                    lhs_a.w_ * rhs_a.y_ - lhs_a.x_ * rhs_a.z_ + lhs_a.y_ * rhs_a.w_ + lhs_a.z_ * rhs_a.x_,
                    lhs_a.w_ * rhs_a.z_ + lhs_a.x_ * rhs_a.y_ - lhs_a.y_ * rhs_a.x_ + lhs_a.z_ * rhs_a.w_);
            }

            Vector3 operator*(const Quaternion& q_a, const Vector3& v_a)
            {
                const Vector3 qv_ = Vector3(q_a.x_, q_a.y_, q_a.z_);
                const Vector3 t_ = 2.0f * Cross(qv_, v_a);
                return v_a + q_a.w_ * t_ + Cross(qv_, t_);
            }

            std::ostream& operator<<(std::ostream& out_a, const Quaternion& q_a)
            {
                out_a << q_a.w_ << " + " << q_a.x_ << "i + " << q_a.y_ << "j + " << q_a.z_ << "k";
                return out_a;
            }

            float Quaternion::SqrMagnitude() const
            {
                return w_ * w_ + x_ * x_ + y_ * y_ + z_ * z_;
            }

            float Quaternion::Magnitude() const
            {
                return sqrt(SqrMagnitude());
            }

            Quaternion Quaternion::Normalized() const
            {
                assert(Magnitude() > 0.0f);
                const float inv_norm_ = 1.0f / Magnitude();

                return Quaternion(w_ * inv_norm_, x_ * inv_norm_, y_ * inv_norm_, z_ * inv_norm_);
            }

            void Quaternion::RotateToWards(const Quaternion& target_a, float max_radians_delta_a)
            {
                const Quaternion relative_rotation_ = Conjugate() * target_a;

                assert(std::abs(relative_rotation_.w_) <= 1.0f);
                const float angle_ = 2.0f * std::acos(relative_rotation_.w_);
                const Vector3 axis_(relative_rotation_.x_, relative_rotation_.y_, relative_rotation_.z_);

                if (angle_ > max_radians_delta_a)
                {
                    const Quaternion delta_ = Quaternion::AngleAxis(max_radians_delta_a, axis_.Normalized());
                    (*this) = delta_ * (*this);
                }

                else
                {
                    (*this) = target_a;
                }
            }

            void Quaternion::Normalize()
            {
                assert(Magnitude() > 0.0f);
                const float inv_norm_ = 1.0f / Magnitude();

                w_ *= inv_norm_;
                x_ *= inv_norm_;
                y_ *= inv_norm_;
                z_ *= inv_norm_;
            }

            Quaternion Quaternion::AngleAxis(const float angle_a, const Vector3& axis_a)
            {
                assert(std::abs(axis_a.Magnitude() - 1.0f) < 1e-6);

                const float half_angle_ = 0.5f * angle_a;
                return Quaternion(std::cos(half_angle_), axis_a * std::sin(half_angle_));
            }

            Quaternion Quaternion::Euler(const Vector3& euler_angles_a)
            {
                const float half_phi_ = 0.5f * euler_angles_a.x_;
                const float half_theta_ = 0.5f * euler_angles_a.y_;
                const float half_psi_ = 0.5f * euler_angles_a.z_;

                const float cos_half_phi_ = std::cos(half_phi_);
                const float sin_half_phi_ = std::sin(half_phi_);
                const float cos_half_theta_ = std::cos(half_theta_);
                const float sin_half_theta_ = std::sin(half_theta_);
                const float cos_half_psi_ = std::cos(half_psi_);
                const float sin_half_psi_ = std::sin(half_psi_);

                return Quaternion(
                    cos_half_phi_ * cos_half_theta_ * cos_half_psi_ - sin_half_phi_ * sin_half_theta_ * sin_half_psi_,
                    sin_half_phi_ * cos_half_theta_ * cos_half_psi_ + cos_half_phi_ * sin_half_theta_ * sin_half_psi_,
                    cos_half_phi_ * sin_half_theta_ * cos_half_psi_ - sin_half_phi_ * cos_half_theta_ * sin_half_psi_,
                    cos_half_phi_ * cos_half_theta_ * sin_half_psi_ + sin_half_phi_ * sin_half_theta_ * cos_half_psi_);
            }

            Quaternion Quaternion::FromToRotation(const Vector3& from_direction_a, const Vector3& to_direction_a)
            {
                assert(from_direction_a.SqrMagnitude() > 0.0f && to_direction_a.SqrMagnitude() > 0.0f);
                const Vector3 unit_from_ = from_direction_a.Normalized();
                const Vector3 unit_to_ = to_direction_a.Normalized();
                const float d_ = Dot(unit_from_, unit_to_);

                if (d_ >= 1.0f) {
                    return Quaternion::IDENTITY;
                }

                else if (d_ <= -1.0f) {
                    Vector3 axis_ = Cross(unit_from_, Vector3(1.0f, 0.0f, 0.0f));
                    if (axis_.SqrMagnitude() < 1e-6) {
                        axis_ = Cross(unit_from_, Vector3(0.0f, 1.0f, 0.0f));
                    }

                    return AngleAxis(3.14159265358979323846f, axis_.Normalized());
                }

                else {
                    const float s_ = sqrt(unit_from_.SqrMagnitude() * unit_to_.SqrMagnitude())
                                     + Dot(unit_from_, unit_to_);

                    const Vector3 v_ = Cross(unit_from_, unit_to_);

                    return Quaternion(s_, v_).Normalized();
                }
            }

            Quaternion Quaternion::LookRotation(const Vector3& forward_a)
            {
                assert(forward_a.SqrMagnitude() > 0.0f);
                return Quaternion::FromToRotation(Vector3(0.0f, 0.0f, 1.0f), forward_a);
            }

            Quaternion Quaternion::LookRotation(const Vector3& forward_a, const Vector3& upwards_a)
            {
                const Quaternion q1_ = Quaternion::LookRotation(forward_a);

                if (Cross(forward_a, upwards_a).SqrMagnitude() < 1e-6) {
                    return q1_;
                }

                const Vector3 new_up_ = q1_ * Vector3(0.0f, 1.0f, 0.0f);

                const Quaternion q2_ = FromToRotation(new_up_, upwards_a);

                return q2_ * q1_;
            }

            Quaternion Quaternion::Conjugate() const
            {
                return Quaternion(w_, -x_, -y_, -z_);
            }

            Quaternion Quaternion::Inverse() const
            {
                const float sqr_ = SqrMagnitude();
                assert(sqr_ > 0.0f);

                const float inv_sqr_ = 1.0f / sqr_;
                return Quaternion(w_ * inv_sqr_, -x_ * inv_sqr_, -y_ * inv_sqr_, -z_ * inv_sqr_);
            }

            float Dot(const Quaternion& lhs_a, const Quaternion& rhs_a)
            {
                return lhs_a.w_ * rhs_a.w_ + lhs_a.x_ * rhs_a.x_ + lhs_a.y_ * rhs_a.y_ + lhs_a.z_ * rhs_a.z_;
            }

            float Angle(const Quaternion& from_a, const Quaternion& to_a)
            {
                const Quaternion relative_rotation_ = from_a.Conjugate() * to_a;
                assert(std::abs(relative_rotation_.w_) <= 1.0f);
                return 2.0f * std::acos(relative_rotation_.w_);
            }
        } // namespace Math
    } // namespace Aid
} // namespace CE_Kernel
