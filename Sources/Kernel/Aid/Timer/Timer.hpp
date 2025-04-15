// Copyright (c) 2025 Case Technologies

#pragma once
#include <CUtils/Timer.hpp>

namespace CE_Kernel
{
    namespace Aid
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
    } // namespace Aid
} // namespace CE_Kernel
