#pragma once
#include "Aid/Math/Quaternion/Quaternion.hpp"
#include "Aid/Math/Vector/Vector3.hpp"
#include <CUtils/UnitTests.hpp>
#include <sstream>
#include <cmath>

using namespace CUtils;

UTEST(QuaternionTest, DefaultConstructor)
{
    CE_Kernel::Aid::Math::Quaternion q_;
    ASSERT_EQ(q_.w_, 1.0f);
    ASSERT_EQ(q_.x_, 0.0f);
    ASSERT_EQ(q_.y_, 0.0f);
    ASSERT_EQ(q_.z_, 0.0f);
}

UTEST(QuaternionTest, ParamConstructor)
{
    CE_Kernel::Aid::Math::Quaternion q_(2.0f, 3.0f, 4.0f, 5.0f);
    ASSERT_EQ(q_.w_, 2.0f);
    ASSERT_EQ(q_.x_, 3.0f);
    ASSERT_EQ(q_.y_, 4.0f);
    ASSERT_EQ(q_.z_, 5.0f);
}

UTEST(QuaternionTest, EqualityOperators)
{
    CE_Kernel::Aid::Math::Quaternion q1_(1.0f, 0.0f, 0.0f, 0.0f);
    CE_Kernel::Aid::Math::Quaternion q2_(1.0f, 0.0f, 0.0f, 0.0f);
    CE_Kernel::Aid::Math::Quaternion q3_(0.0f, 1.0f, 0.0f, 0.0f);
    
    ASSERT_EQ(q1_ == q2_, 1.0f);
    ASSERT_EQ(q1_ != q3_, 1.0f);
}

UTEST(QuaternionTest, MultiplicationOperator)
{
    CE_Kernel::Aid::Math::Quaternion q1_(1.0f, 2.0f, 3.0f, 4.0f);
    CE_Kernel::Aid::Math::Quaternion q2_(5.0f, 6.0f, 7.0f, 8.0f);
    CE_Kernel::Aid::Math::Quaternion res_ = q1_ * q2_;
    
    ASSERT_EQ(res_.w_, -60.0f);
    ASSERT_EQ(res_.x_, 12.0f);
    ASSERT_EQ(res_.y_, 30.0f);
    ASSERT_EQ(res_.z_, 24.0f);
}

UTEST(QuaternionTest, MagnitudeMethods)
{
    CE_Kernel::Aid::Math::Quaternion q_(3.0f, 4.0f, 0.0f, 0.0f);
    ASSERT_EQ(q_.SqrMagnitude(), 25.0f);
    ASSERT_EQ(q_.Magnitude(), 5.0f);
}

UTEST(QuaternionTest, Normalization)
{
    CE_Kernel::Aid::Math::Quaternion q_(3.0f, 4.0f, 0.0f, 0.0f);
    CE_Kernel::Aid::Math::Quaternion norm_ = q_.Normalized();
    const float mag_ = q_.Magnitude();
    
    ASSERT_EQ(norm_.w_, 3.0f / mag_);
    ASSERT_EQ(norm_.x_, 4.0f / mag_);
    ASSERT_EQ(norm_.y_, 0.0f);
    ASSERT_EQ(norm_.z_, 0.0f);
}

UTEST(QuaternionTest, AngleAxisConversion)
{
    CE_Kernel::Aid::Math::Vector3 axis_(0.0f, 1.0f, 0.0f);
    float angle_ = 3.14159265358979323846;
    CE_Kernel::Aid::Math::Quaternion q_ = 
        CE_Kernel::Aid::Math::Quaternion::AngleAxis(angle_, axis_);
    
    const float tolerance_ = 0.0001f;
    ASSERT_NEAR(q_.w_, std::cos(angle_/2), tolerance_);
    ASSERT_NEAR(q_.x_, 0.0f, tolerance_);
    ASSERT_NEAR(q_.y_, std::sin(angle_/2), tolerance_);
    ASSERT_NEAR(q_.z_, 0.0f, tolerance_);
}

UTEST(QuaternionTest, DotProduct)
{
    CE_Kernel::Aid::Math::Quaternion q1_(1.0f, 2.0f, 3.0f, 4.0f);
    CE_Kernel::Aid::Math::Quaternion q2_(5.0f, 6.0f, 7.0f, 8.0f);
    float dot_ = CE_Kernel::Aid::Math::Dot(q1_, q2_);
    
    ASSERT_EQ(dot_, 70.0f);
}
