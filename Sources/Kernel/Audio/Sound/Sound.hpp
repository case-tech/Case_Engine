// Copyright (c) 2025 Case Technologies

#pragma once
#include <FMOD/fmod.h>
#include <memory>

namespace CE_Kernel
{
    namespace Audio
    {
        namespace Sound
        {
            class Sound
            {
            public:
                explicit Sound(FMOD_SOUND* sound_a);
                ~Sound();

                void Set3DMinMaxDistance(float min_a, float max_a);
                FMOD_SOUND* GetPtr() const { return sound_; }

            private:
                FMOD_SOUND* sound_ = nullptr;
            };
        } // namespace Sound
    }     // namespace Audio
} // namespace CE_Kernel
