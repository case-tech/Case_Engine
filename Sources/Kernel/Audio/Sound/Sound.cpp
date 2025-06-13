#include "Sound.hpp"

namespace CE_Kernel
{
    namespace Audio
    {
        namespace Sound
        {
            Sound::Sound(FMOD_SOUND* sound_a) : sound_(sound_a) {};

            Sound::~Sound()
            {
                if (sound_)
                    FMOD_Sound_Release(sound_);
            }

            void Sound::Set3DMinMaxDistance(float min_a, float max_a)
            {
                if (sound_)
                {
                    FMOD_Sound_Set3DMinMaxDistance(sound_, min_a, max_a);
                }
            }
        } // namespace Sound
    } // namespace Audio
} // namespace CE_Kernel
