#include "Timer.hpp"

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Timer
        {
            double Timer::GetMillisecondsElapsed()
            {
                return timer_.GetMillisecondsElapsed();
            }

            void Timer::Restart()
            {
                timer_.Restart();
            }

            bool Timer::Stop()
            {
                return timer_.Stop();
            }

            bool Timer::Start()
            {
                return timer_.Start();
            }
        } // namespace Timer
    } // namespace Aux
} // namespace CE_Kernel
