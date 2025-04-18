#pragma once
#include "Aid/Math/Matrix/Matrix3.hpp"
#include "Aid/Math/Vector/Vector3.hpp"
#include <CUtils/UnitTests.hpp>
#include <sstream>
#include <cmath>

using namespace CUtils;

UTEST(Matrix3Test, DefaultConstructor)
{
    CE_Kernel::Aid::Math::Matrix3 m_;
    for(int i = 0; i < 9; ++i) 
    {
        if(i % 4 == 0) 
        {
          ASSERT_EQ(m_[i], 1.0f);
        }
        else 
        {
          ASSERT_EQ(m_[i], 0.0f);
        }
    }
}

UTEST(Matrix3Test, ArrayConstructor)
{
    float arr_[9] = {1,2,3,4,5,6,7,8,9};
    CE_Kernel::Aid::Math::Matrix3 m_(arr_);
    for(int i = 0; i < 9; ++i) 
    {
        ASSERT_EQ(m_[i], arr_[i]);
    }
}

UTEST(Matrix3Test, MatrixMultiplication)
{
    CE_Kernel::Aid::Math::Matrix3 A_(2,0,0, 0,2,0, 0,0,2);
    CE_Kernel::Aid::Math::Matrix3 B_ = A_ * A_;
    for(int i = 0; i < 9; ++i) 
    {
        if(i % 4 == 0)
        {
          ASSERT_EQ(B_[i], 4.0f);
        }
        else
        {
          ASSERT_EQ(B_[i], 0.0f);
        }
    }
}

UTEST(Matrix3Test, VectorMultiplication)
{
    CE_Kernel::Aid::Math::Matrix3 m_(1,2,3,4,5,6,7,8,9);
    CE_Kernel::Aid::Math::Vector3 v_(10,11,12);
    CE_Kernel::Aid::Math::Vector3 res_ = m_ * v_;
    ASSERT_EQ(res_.x_, 1*10 + 2*11 + 3*12);
    ASSERT_EQ(res_.y_, 4*10 + 5*11 + 6*12);
    ASSERT_EQ(res_.z_, 7*10 + 8*11 + 9*12);
}

UTEST(Matrix3Test, Transpose)
{
    CE_Kernel::Aid::Math::Matrix3 m_(1,2,3,4,5,6,7,8,9);
    CE_Kernel::Aid::Math::Matrix3 transposed_ = m_.Transposed();
    ASSERT_EQ(transposed_[0], 1);
    ASSERT_EQ(transposed_[1], 4);
    ASSERT_EQ(transposed_[2], 7);
    ASSERT_EQ(transposed_[3], 2);
}

UTEST(Matrix3Test, Determinant)
{
    CE_Kernel::Aid::Math::Matrix3 m_(2,3,4,5,6,7,8,9,1);
    float det_ = 2*(6*1 - 7*9) - 3*(5*1 - 7*8) + 4*(5*9 - 6*8);
    ASSERT_EQ(m_.Determinant(), det_);
}

UTEST(Matrix3Test, LookRotation)
{
    CE_Kernel::Aid::Math::Vector3 forward_(0,0,1);
    CE_Kernel::Aid::Math::Vector3 up_(0,1,0);
    CE_Kernel::Aid::Math::Matrix3 m_ = 
        CE_Kernel::Aid::Math::Matrix3::LookRotation(forward_, up_);
    ASSERT_EQ(m_[8], 1.0f);
    ASSERT_EQ(m_[4], 1.0f);
}
