#pragma once
#include "Aid/Math/Matrix/Matrix4.hpp"
#include "Aid/Math/Vector/Vector3.hpp"
#include "Aid/Math/Vector/Vector4.hpp"
#include "Aid/Math/Quaternion/Quaternion.hpp"
#include <CUtils/UnitTests.hpp>
#include <sstream>
#include <cmath>

using namespace CUtils;

UTEST(Matrix4Test, DefaultConstructor)
{
    CE_Kernel::Aid::Math::Matrix4 m_;
    for(int i = 0; i < 16; ++i) 
    {
        if(i % 5 == 0)
        {
            ASSERT_EQ(m_[i], 1.0f);
        }
        else 
        {
          ASSERT_EQ(m_[i], 0.0f);
        }
    }
}

UTEST(Matrix4Test, ArrayConstructor)
{
    float arr_[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    CE_Kernel::Aid::Math::Matrix4 m_(arr_);
    for(int i = 0; i < 16; ++i)
        ASSERT_EQ(m_[i], arr_[i]);
}

UTEST(Matrix4Test, MatrixMultiplication)
{
    CE_Kernel::Aid::Math::Matrix4 A_(
        2,0,0,0,
        0,2,0,0,
        0,0,2,0,
        0,0,0,2
    );
    
    CE_Kernel::Aid::Math::Matrix4 B_ = A_ * A_;
    for(int i = 0; i < 16; ++i) 
    {
        if(i % 5 == 0) 
        {
            ASSERT_EQ(B_[i], 4.0f);
        }
        else 
        {
            ASSERT_EQ(B_[i], 0.0f);
        }
    }
}

UTEST(Matrix4Test, VectorMultiplication)
{
    CE_Kernel::Aid::Math::Matrix4 m_(
        1,0,0,5,
        0,1,0,6,
        0,0,1,7,
        0,0,0,1
    );
    
    CE_Kernel::Aid::Math::Vector4 v_(2,3,4,1);
    CE_Kernel::Aid::Math::Vector4 res_ = m_ * v_;
    ASSERT_EQ(res_.x_, 7);
    ASSERT_EQ(res_.y_, 9);
    ASSERT_EQ(res_.z_, 11);
}

UTEST(Matrix4Test, ScalingMatrix)
{
    CE_Kernel::Aid::Math::Vector3 scale_(2,3,4);
    CE_Kernel::Aid::Math::Matrix4 m_ = CE_Kernel::Aid::Math::Matrix4::Scaling(scale_);
    ASSERT_EQ(m_[0], 2); 
    ASSERT_EQ(m_[5], 3); 
    ASSERT_EQ(m_[10], 4); 
    ASSERT_EQ(m_[15], 1);
}
