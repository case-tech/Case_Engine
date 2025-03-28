// Copyright (c) 2025 Case Technologies

#pragma once
#include <CUtils/Timer.hpp>

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Timer
        {
            class Timer
            {
            public:
                double GetMillisecondsElapsed();
                void Restart();
                bool Stop();
                bool Start();

            private:
                CUtils::Timer timer_;
            };
        } // namespace Timer
    } // namespace Aux
} // namespace CE_Kernel
