#pragma once
#include "Aid/Math/Matrix/Matrix2.hpp"
#include "Aid/Math/Vector/Vector2.hpp"
#include <CUtils/UnitTests.hpp>
#include <sstream>
#include <cmath>

using namespace CUtils;

UTEST(Matrix2Test, DefaultConstructor)
{
    CE_Kernel::Aid::Math::Matrix2 m_;
    ASSERT_EQ(m_[0], 1.0f);
    ASSERT_EQ(m_[1], 0.0f);
    ASSERT_EQ(m_[2], 0.0f);
    ASSERT_EQ(m_[3], 1.0f);
}

UTEST(Matrix2Test, ArrayConstructor)
{
    float arr_[4] = {1,2,3,4};
    CE_Kernel::Aid::Math::Matrix2 m_(arr_);
    for(int i = 0; i < 4; ++i) 
    {
        ASSERT_EQ(m_[i], arr_[i]);
    }
}

UTEST(Matrix2Test, ComponentConstructor)
{
    CE_Kernel::Aid::Math::Matrix2 m_(1,2,3,4);
    ASSERT_EQ(m_[0], 1);
    ASSERT_EQ(m_[1], 2);
    ASSERT_EQ(m_[2], 3);
    ASSERT_EQ(m_[3], 4);
}

UTEST(Matrix2Test, MatrixAddition)
{
    CE_Kernel::Aid::Math::Matrix2 A_(1,2,3,4);
    CE_Kernel::Aid::Math::Matrix2 B_(5,6,7,8);
    CE_Kernel::Aid::Math::Matrix2 C_ = A_ + B_;
    ASSERT_EQ(C_[0], 6);
    ASSERT_EQ(C_[1], 8);
    ASSERT_EQ(C_[2], 10);
    ASSERT_EQ(C_[3], 12);
}

UTEST(Matrix2Test, ScalarMultiplication)
{
    CE_Kernel::Aid::Math::Matrix2 A_(1,2,3,4);
    CE_Kernel::Aid::Math::Matrix2 B_ = A_ * 2;
    ASSERT_EQ(B_[0], 2);
    ASSERT_EQ(B_[1], 4);
    ASSERT_EQ(B_[2], 6);
    ASSERT_EQ(B_[3], 8);
}

UTEST(Matrix2Test, VectorMultiplication)
{
    CE_Kernel::Aid::Math::Matrix2 m_(1,2,3,4);
    CE_Kernel::Aid::Math::Vector2 v_(5,6);
    CE_Kernel::Aid::Math::Vector2 res_ = m_ * v_;
    ASSERT_EQ(res_.x_, 1*5 + 2*6);
    ASSERT_EQ(res_.y_, 3*5 + 4*6);
}

UTEST(Matrix2Test, Transpose)
{
    CE_Kernel::Aid::Math::Matrix2 m_(1,2,3,4);
    CE_Kernel::Aid::Math::Matrix2 transposed_ = m_.Transposed();
    ASSERT_EQ(transposed_[0], 1);
    ASSERT_EQ(transposed_[1], 3);
    ASSERT_EQ(transposed_[2], 2);
    ASSERT_EQ(transposed_[3], 4);
}

UTEST(Matrix2Test, Determinant)
{
    CE_Kernel::Aid::Math::Matrix2 m_(2,3,4,5);
    ASSERT_EQ(m_.Determinant(), 2*5 - 3*4);
}

UTEST(Matrix2Test, ScalingMatrix)
{
    CE_Kernel::Aid::Math::Vector2 scale_(2,3);
    CE_Kernel::Aid::Math::Matrix2 m_ = 
        CE_Kernel::Aid::Math::Matrix2::Scaling(scale_);
    ASSERT_EQ(m_[0], 2);
    ASSERT_EQ(m_[3], 3);
}
