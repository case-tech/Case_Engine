#pragma once
#include "Aid/Math/Vector/Vector4.hpp"
#include "Aid/Math/Vector/Vector3.hpp"
#include <CUtils/UnitTests.hpp>
#include <sstream>

using namespace CUtils;

UTEST(Vector4Test, DefaultConstructor)
{
    CE_Kernel::Aid::Math::Vector4 v_;
    ASSERT_EQ(v_.x_, 0.0f);
    ASSERT_EQ(v_.y_, 0.0f);
    ASSERT_EQ(v_.z_, 0.0f);
    ASSERT_EQ(v_.w_, 0.0f);
}

UTEST(Vector4Test, ParamConstructor)
{
    CE_Kernel::Aid::Math::Vector4 v_(1.0f, 2.0f, 3.0f, 4.0f);
    ASSERT_EQ(v_.x_, 1.0f);
    ASSERT_EQ(v_.y_, 2.0f);
    ASSERT_EQ(v_.z_, 3.0f);
    ASSERT_EQ(v_.w_, 4.0f);
}

UTEST(Vector4Test, FromVector3)
{
    CE_Kernel::Aid::Math::Vector3 v3_(1.0f, 2.0f, 3.0f);
    CE_Kernel::Aid::Math::Vector4 v4_1_(v3_);

    ASSERT_EQ(v4_1_.x_, 1.0f);
    ASSERT_EQ(v4_1_.y_, 2.0f);
    ASSERT_EQ(v4_1_.z_, 3.0f);
    ASSERT_EQ(v4_1_.w_, 0.0f);

    CE_Kernel::Aid::Math::Vector4 v4_2_(v3_, 4.0f);
    ASSERT_EQ(v4_2_.w_, 4.0f);
}

UTEST(Vector4Test, EqualityOperators)
{
    CE_Kernel::Aid::Math::Vector4 v1_(1.0f, 2.0f, 3.0f, 4.0f);
    CE_Kernel::Aid::Math::Vector4 v2_(1.0f, 2.0f, 3.0f, 4.0f);

    ASSERT_EQ(v1_ == v2_, 1.0f);
    ASSERT_EQ(v1_ != v2_, 0.0f);

    CE_Kernel::Aid::Math::Vector4 v3_(5.0f, 6.0f, 7.0f, 8.0f);
    ASSERT_EQ(v1_ == v3_, 0.0f);
    ASSERT_EQ(v1_ != v3_, 1.0f);
}

UTEST(Vector4Test, AdditionOperator)
{
    CE_Kernel::Aid::Math::Vector4 v1_(1.0f, 2.0f, 3.0f, 4.0f);
    CE_Kernel::Aid::Math::Vector4 v2_(5.0f, 6.0f, 7.0f, 8.0f);
    CE_Kernel::Aid::Math::Vector4 result_ = v1_ + v2_;

    ASSERT_EQ(result_.x_, 6.0f);
    ASSERT_EQ(result_.y_, 8.0f);
    ASSERT_EQ(result_.z_, 10.0f);
    ASSERT_EQ(result_.w_, 12.0f);
}

UTEST(Vector4Test, SubstractionOperator)
{
    CE_Kernel::Aid::Math::Vector4 v1_(5.0f, 6.0f, 7.0f, 8.0f);
    CE_Kernel::Aid::Math::Vector4 v2_(1.0f, 2.0f, 3.0f, 4.0f);
    CE_Kernel::Aid::Math::Vector4 result_ = v1_ - v2_;

    ASSERT_EQ(result_.x_, 4.0f);
    ASSERT_EQ(result_.y_, 4.0f);
    ASSERT_EQ(result_.z_, 4.0f);
    ASSERT_EQ(result_.w_, 4.0f);
}

UTEST(Vector4Test, ScalarMultiplicationOperator)
{
    CE_Kernel::Aid::Math::Vector4 v_(2.0f, 3.0f, 4.0f, 5.0f);
    CE_Kernel::Aid::Math::Vector4 result1_ = v_ * 2.0f;

    ASSERT_EQ(result1_.x_, 4.0f);
    ASSERT_EQ(result1_.y_, 6.0f);
    ASSERT_EQ(result1_.z_, 8.0f);
    ASSERT_EQ(result1_.w_, 10.0f);

    CE_Kernel::Aid::Math::Vector4 result2_ = 3.0f * v_;
    ASSERT_EQ(result2_.x_, 6.0f);
    ASSERT_EQ(result2_.y_, 9.0f);
    ASSERT_EQ(result2_.z_, 12.0f);
    ASSERT_EQ(result2_.w_, 15.0f);
}

UTEST(Vector4Test, ScalarDivisionOperator)
{
    CE_Kernel::Aid::Math::Vector4 v_(6.0f, 8.0f, 10.0f, 12.0f);
    CE_Kernel::Aid::Math::Vector4 result_ = v_ / 2.0f;

    ASSERT_EQ(result_.x_, 3.0f);
    ASSERT_EQ(result_.y_, 4.0f);
    ASSERT_EQ(result_.z_, 5.0f);
    ASSERT_EQ(result_.w_, 6.0f);
}

UTEST(Vector4Test, MagnitudeMethods)
{
    CE_Kernel::Aid::Math::Vector4 v_(3.0f, 4.0f, 0.0f, 0.0f);
    ASSERT_EQ(v_.SqrMagnitude(), 25.0f);
    ASSERT_EQ(v_.Magnitude(), 5.0f);
}

UTEST(Vector4Test, NormalizeMethods)
{
    CE_Kernel::Aid::Math::Vector4 v_(3.0f, 4.0f, 0.0f, 0.0f);
    CE_Kernel::Aid::Math::Vector4 normalized_ = v_.Normalized();
    ASSERT_EQ(normalized_.x_, 3.0f / 5.0f);
    ASSERT_EQ(normalized_.y_, 4.0f / 5.0f);
    ASSERT_EQ(normalized_.z_, 0.0f);
    ASSERT_EQ(normalized_.w_, 0.0f);

    v_.Normalize();
    ASSERT_EQ(v_.x_, 3.0f / 5.0f);
    ASSERT_EQ(v_.y_, 4.0f / 5.0f);
    ASSERT_EQ(v_.z_, 0.0f);
    ASSERT_EQ(v_.w_, 0.0f);
}

UTEST(Vector4Test, ScaleFunction)
{
    CE_Kernel::Aid::Math::Vector4 v1_(1.0f, 2.0f, 3.0f, 4.0f);
    CE_Kernel::Aid::Math::Vector4 v2_(2.0f, 3.0f, 4.0f, 5.0f);
    CE_Kernel::Aid::Math::Vector4 result_ 
        = CE_Kernel::Aid::Math::Scale(v1_, v2_);

    ASSERT_EQ(result_.x_, 2.0f);
    ASSERT_EQ(result_.y_, 6.0f);
    ASSERT_EQ(result_.z_, 12.0f);
    ASSERT_EQ(result_.w_, 20.0f);
}

UTEST(Vector4Test, DotProductFunction)
{
    CE_Kernel::Aid::Math::Vector4 v1_(1.0f, 2.0f, 3.0f, 4.0f);
    CE_Kernel::Aid::Math::Vector4 v2_(5.0f, 6.0f, 7.0f, 8.0f);
    float dot_ = CE_Kernel::Aid::Math::Dot(v1_, v2_);
    ASSERT_EQ(dot_, 70.0f);
}

UTEST(Vector4Test, DistanceFunctions)
{
    CE_Kernel::Aid::Math::Vector4 p1_(1.0f, 2.0f, 3.0f, 4.0f);
    CE_Kernel::Aid::Math::Vector4 p2_(5.0f, 6.0f, 7.0f, 8.0f);
    float sqr_dist_ = CE_Kernel::Aid::Math::SqrDistance(p1_, p2_);
    ASSERT_EQ(sqr_dist_, 64.0f);
    ASSERT_EQ(CE_Kernel::Aid::Math::Distance(p1_, p2_), 8.0f);
}

UTEST(Vector4Test, OutputOperator)
{
    CE_Kernel::Aid::Math::Vector4 v_(1.5f, 2.5f, 3.5f, 4.5f);
    std::stringstream ss_;
    ss_ << v_;
    ASSERT_STREQ("(1.5, 2.5, 3.5, 4.5)", ss_.str().c_str());
}
