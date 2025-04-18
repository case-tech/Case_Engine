#pragma once
#include "Aid/Algorithms/Algorithms.hpp"
#include <CUtils/UnitTests.hpp>

using namespace CUtils;

UTEST(AlgorithmsTest, Interpolation)
{
    float a_ = 10.0f, b_ = 20.0f;
    ASSERT_NEAR(
      CE_Kernel::Aid::Algorithms::Interpolator<float>::Lerp(a_, b_, 0.5f), 15.0f, 0.001f);
}

UTEST(AlgorithmsTest, StateMachineTransitions)
{
    CE_Kernel::Aid::Algorithms::StateMachine<int> sm_;
    sm_.AddState(0, []{});
    sm_.AddState(1, []{});
    sm_.AddTransition(0, 1, []{ return true; });
    
    ASSERT_TRUE(sm_.TryTransition(1));
    ASSERT_EQ(sm_.GetCurrentState(), 1);
}
