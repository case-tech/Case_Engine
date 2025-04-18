#pragma once
#include "Aid/Math/Vector/Vector2.hpp"
#include <CUtils/UnitTests.hpp>
#include <sstream>
#include <cmath>

using namespace CUtils;

UTEST(Vector2Test, DefaultConstructor)
{
    CE_Kernel::Aid::Math::Vector2 v_;
    ASSERT_EQ(v_.x_, 0.0f);
    ASSERT_EQ(v_.y_, 0.0f);
}

UTEST(Vector2Test, ParamConstructor)
{
    CE_Kernel::Aid::Math::Vector2 v_(1.5f, 2.5f);
    ASSERT_EQ(v_.x_, 1.5f);
    ASSERT_EQ(v_.y_, 2.5f);
}

UTEST(Vector2Test, EqualityOperators)
{
    CE_Kernel::Aid::Math::Vector2 v1_(1.0f, 2.0f);
    CE_Kernel::Aid::Math::Vector2 v2_(1.0f, 2.0f);
    
    ASSERT_EQ(v1_ == v2_, 1.0f);
    ASSERT_EQ(v1_ != v2_, 0.0f);
    
    CE_Kernel::Aid::Math::Vector2 v3_(3.0f, 4.0f);
    ASSERT_EQ(v1_ == v3_, 0.0f);
    ASSERT_EQ(v1_ != v3_, 1.0f);
}

UTEST(Vector2Test, ArithmeticOperators)
{
    CE_Kernel::Aid::Math::Vector2 v1_(1.0f, 2.0f);
    CE_Kernel::Aid::Math::Vector2 v2_(3.0f, 4.0f);
    
    CE_Kernel::Aid::Math::Vector2 add_ = v1_ + v2_;
    ASSERT_EQ(add_.x_, 4.0f);
    ASSERT_EQ(add_.y_, 6.0f);
    
    CE_Kernel::Aid::Math::Vector2 sub_ = v2_ - v1_;
    ASSERT_EQ(sub_.x_, 2.0f);
    ASSERT_EQ(sub_.y_, 2.0f);
    
    CE_Kernel::Aid::Math::Vector2 neg_ = -v1_;
    ASSERT_EQ(neg_.x_, -1.0f);
    ASSERT_EQ(neg_.y_, -2.0f);
}

UTEST(Vector2Test, ScalarOperations)
{
    CE_Kernel::Aid::Math::Vector2 v_(2.0f, 4.0f);
    
    CE_Kernel::Aid::Math::Vector2 mul1_ = v_ * 3.0f;
    ASSERT_EQ(mul1_.x_, 6.0f);
    ASSERT_EQ(mul1_.y_, 12.0f);
    
    CE_Kernel::Aid::Math::Vector2 mul2_ = 2.0f * v_;
    ASSERT_EQ(mul2_.x_, 4.0f);
    ASSERT_EQ(mul2_.y_, 8.0f);
    
    CE_Kernel::Aid::Math::Vector2 div_ = v_ / 2.0f;
    ASSERT_EQ(div_.x_, 1.0f);
    ASSERT_EQ(div_.y_, 2.0f);
}

UTEST(Vector2Test, MagnitudeMethods)
{
    CE_Kernel::Aid::Math::Vector2 v_(3.0f, 4.0f);
    ASSERT_EQ(v_.SqrMagnitude(), 25.0f);
    ASSERT_EQ(v_.Magnitude(), 5.0f);
}

UTEST(Vector2Test, Normalization)
{
    CE_Kernel::Aid::Math::Vector2 v_(3.0f, 4.0f);
    CE_Kernel::Aid::Math::Vector2 norm_ = v_.Normalized();
    
    const float tolerance_ = 0.0001f;
    ASSERT_LE(std::abs(norm_.x_ - 0.6f), tolerance_);
    ASSERT_LE(std::abs(norm_.y_ - 0.8f), tolerance_);
    
    v_.Normalize();
    ASSERT_LE(std::abs(v_.x_ - 0.6f), tolerance_);
    ASSERT_LE(std::abs(v_.y_ - 0.8f), tolerance_);
}

UTEST(Vector2Test, ScaleFunction)
{
    CE_Kernel::Aid::Math::Vector2 v1_(1.0f, 2.0f);
    CE_Kernel::Aid::Math::Vector2 v2_(2.0f, 3.0f);
    CE_Kernel::Aid::Math::Vector2 scaled_ = 
        CE_Kernel::Aid::Math::Scale(v1_, v2_);
    
    ASSERT_EQ(scaled_.x_, 2.0f);
    ASSERT_EQ(scaled_.y_, 6.0f);
}

UTEST(Vector2Test, DotProduct)
{
    CE_Kernel::Aid::Math::Vector2 v1_(1.0f, 2.0f);
    CE_Kernel::Aid::Math::Vector2 v2_(3.0f, 4.0f);
    float dot_ = CE_Kernel::Aid::Math::Dot(v1_, v2_);
    
    ASSERT_EQ(dot_, 11.0f);
}

UTEST(Vector2Test, DistanceFunctions)
{
    CE_Kernel::Aid::Math::Vector2 p1_(1.0f, 2.0f);
    CE_Kernel::Aid::Math::Vector2 p2_(4.0f, 6.0f);
    
    float sqr_dist_ = CE_Kernel::Aid::Math::SqrDistance(p1_, p2_);
    ASSERT_EQ(sqr_dist_, 25.0f);
    
    float dist_ = CE_Kernel::Aid::Math::Distance(p1_, p2_);
    ASSERT_LE(std::abs(dist_ - 5.0f), 0.0001f);
}

UTEST(Vector2Test, OutputOperator)
{
    CE_Kernel::Aid::Math::Vector2 v_(1.5f, 2.5f);
    std::stringstream ss_;
    ss_ << v_;
    ASSERT_STREQ("(1.5, 2.5)", ss_.str().c_str());
}
