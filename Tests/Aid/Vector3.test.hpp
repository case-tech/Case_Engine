#pragma once
#include "Aid/Math/Vector/Vector3.hpp"
#include "Aid/Math/Vector/Vector4.hpp"
#include <CUtils/UnitTests.hpp>
#include <sstream>
#include <cmath>

using namespace CUtils;

UTEST(Vector3Test, DefaultConstructor)
{
    CE_Kernel::Aid::Math::Vector3 v_;
    ASSERT_EQ(v_.x_, 0.0f);
    ASSERT_EQ(v_.y_, 0.0f);
    ASSERT_EQ(v_.z_, 0.0f);
}

UTEST(Vector3Test, ParamConstructor)
{
    CE_Kernel::Aid::Math::Vector3 v_(1.5f, 2.5f, 3.5f);
    ASSERT_EQ(v_.x_, 1.5f);
    ASSERT_EQ(v_.y_, 2.5f);
    ASSERT_EQ(v_.z_, 3.5f);
}

UTEST(Vector3Test, FromVector4)
{
    CE_Kernel::Aid::Math::Vector4 v4_(2.0f, 3.0f, 4.0f, 5.0f);
    CE_Kernel::Aid::Math::Vector3 v3_(v4_);
    
    ASSERT_EQ(v3_.x_, 2.0f);
    ASSERT_EQ(v3_.y_, 3.0f);
    ASSERT_EQ(v3_.z_, 4.0f);
}

UTEST(Vector3Test, EqualityOperators)
{
    CE_Kernel::Aid::Math::Vector3 v1_(1.0f, 2.0f, 3.0f);
    CE_Kernel::Aid::Math::Vector3 v2_(1.0f, 2.0f, 3.0f);
    
    ASSERT_EQ(v1_ == v2_, 1.0f);
    ASSERT_EQ(v1_ != v2_, 0.0f);
    
    CE_Kernel::Aid::Math::Vector3 v3_(4.0f, 5.0f, 6.0f);
    ASSERT_EQ(v1_ == v3_, 0.0f);
    ASSERT_EQ(v1_ != v3_, 1.0f);
}

UTEST(Vector3Test, ArithmeticOperators)
{
    CE_Kernel::Aid::Math::Vector3 v1_(1.0f, 2.0f, 3.0f);
    CE_Kernel::Aid::Math::Vector3 v2_(4.0f, 5.0f, 6.0f);
    
    CE_Kernel::Aid::Math::Vector3 add_ = v1_ + v2_;
    ASSERT_EQ(add_.x_, 5.0f);
    ASSERT_EQ(add_.y_, 7.0f);
    ASSERT_EQ(add_.z_, 9.0f);
    
    CE_Kernel::Aid::Math::Vector3 sub_ = v2_ - v1_;
    ASSERT_EQ(sub_.x_, 3.0f);
    ASSERT_EQ(sub_.y_, 3.0f);
    ASSERT_EQ(sub_.z_, 3.0f);
    
    CE_Kernel::Aid::Math::Vector3 neg_ = -v1_;
    ASSERT_EQ(neg_.x_, -1.0f);
    ASSERT_EQ(neg_.y_, -2.0f);
    ASSERT_EQ(neg_.z_, -3.0f);
}

UTEST(Vector3Test, ScalarOperations)
{
    CE_Kernel::Aid::Math::Vector3 v_(2.0f, 4.0f, 6.0f);
    
    CE_Kernel::Aid::Math::Vector3 mul1_ = v_ * 3.0f;
    ASSERT_EQ(mul1_.x_, 6.0f);
    ASSERT_EQ(mul1_.y_, 12.0f);
    ASSERT_EQ(mul1_.z_, 18.0f);
    
    CE_Kernel::Aid::Math::Vector3 div_ = v_ / 2.0f;
    ASSERT_EQ(div_.x_, 1.0f);
    ASSERT_EQ(div_.y_, 2.0f);
    ASSERT_EQ(div_.z_, 3.0f);
}

UTEST(Vector3Test, MagnitudeMethods)
{
    CE_Kernel::Aid::Math::Vector3 v_(3.0f, 4.0f, 0.0f);
    ASSERT_EQ(v_.SqrMagnitude(), 25.0f);
    ASSERT_EQ(v_.Magnitude(), 5.0f);
}

UTEST(Vector3Test, Normalization)
{
    CE_Kernel::Aid::Math::Vector3 v_(3.0f, 4.0f, 0.0f);
    CE_Kernel::Aid::Math::Vector3 norm_ = v_.Normalized();
    
    const float tolerance_ = 0.0001f;
    ASSERT_LE(std::abs(norm_.x_ - 0.6f), tolerance_);
    ASSERT_LE(std::abs(norm_.y_ - 0.8f), tolerance_);
    ASSERT_EQ(norm_.z_, 0.0f);
    
    v_.Normalize();
    ASSERT_LE(std::abs(v_.x_ - 0.6f), tolerance_);
    ASSERT_LE(std::abs(v_.y_ - 0.8f), tolerance_);
    ASSERT_EQ(v_.z_, 0.0f);
}

UTEST(Vector3Test, ScaleFunction)
{
    CE_Kernel::Aid::Math::Vector3 v1_(1.0f, 2.0f, 3.0f);
    CE_Kernel::Aid::Math::Vector3 v2_(2.0f, 3.0f, 4.0f);
    CE_Kernel::Aid::Math::Vector3 scaled_ = 
        CE_Kernel::Aid::Math::Scale(v1_, v2_);
    
    ASSERT_EQ(scaled_.x_, 2.0f);
    ASSERT_EQ(scaled_.y_, 6.0f);
    ASSERT_EQ(scaled_.z_, 12.0f);
}

UTEST(Vector3Test, DotProduct)
{
    CE_Kernel::Aid::Math::Vector3 v1_(1.0f, 2.0f, 3.0f);
    CE_Kernel::Aid::Math::Vector3 v2_(4.0f, 5.0f, 6.0f);
    float dot_ = CE_Kernel::Aid::Math::Dot(v1_, v2_);
    
    ASSERT_EQ(dot_, 32.0f);
}

UTEST(Vector3Test, CrossProduct)
{
    CE_Kernel::Aid::Math::Vector3 v1_(1.0f, 0.0f, 0.0f);
    CE_Kernel::Aid::Math::Vector3 v2_(0.0f, 1.0f, 0.0f);
    CE_Kernel::Aid::Math::Vector3 cross_ = 
        CE_Kernel::Aid::Math::Cross(v1_, v2_);
    
    ASSERT_EQ(cross_.x_, 0.0f);
    ASSERT_EQ(cross_.y_, 0.0f);
    ASSERT_EQ(cross_.z_, 1.0f);
}

UTEST(Vector3Test, DistanceFunctions)
{
    CE_Kernel::Aid::Math::Vector3 p1_(1.0f, 2.0f, 3.0f);
    CE_Kernel::Aid::Math::Vector3 p2_(4.0f, 6.0f, 8.0f);
    
    float sqr_dist_ = CE_Kernel::Aid::Math::SqrDistance(p1_, p2_);
    ASSERT_EQ(sqr_dist_, 50.0f);
    
    float dist_ = CE_Kernel::Aid::Math::Distance(p1_, p2_);
    ASSERT_LE(std::abs(dist_ - std::sqrt(50.0f)), 0.0001f);
}

UTEST(Vector3Test, OutputOperator)
{
    CE_Kernel::Aid::Math::Vector3 v_(1.5f, 2.5f, 3.5f);
    std::stringstream ss_;
    ss_ << v_;
    ASSERT_STREQ("(1.5, 2.5, 3.5)", ss_.str().c_str());
}
