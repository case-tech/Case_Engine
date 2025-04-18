#pragma once
#include "Aid/Timer/Timer.hpp"
#include <CUtils/UnitTests.hpp>
#include <thread>

using namespace CUtils;

UTEST(TimerTest, InitialState)
{
    CE_Kernel::Aid::Timer::Timer timer_;
    ASSERT_LE(timer_.GetMillisecondsElapsed(), 10.0);
}

UTEST(TimerTest, RestartFunctionality)
{
    CE_Kernel::Aid::Timer::Timer timer_;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    timer_.Restart();
    ASSERT_LE(timer_.GetMillisecondsElapsed(), 15.0);
}
