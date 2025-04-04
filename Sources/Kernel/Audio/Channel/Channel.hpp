// Copyright (c) 2025 Case Technologies

#pragma once
#include <FMOD/fmod.h>

namespace CE_Kernel
{
    namespace Audio
    {
        namespace Channel
        {
            class Channel
            {
            public:
                explicit Channel(FMOD_CHANNEL* channel_a);
		~Channel();

		void SetPaused(bool paused_a);
		void Stop();
		void SetVolume(float volume_a);
		void Set3DAttributes(const FMOD_VECTOR& position_a, const FMOD_VECTOR& velocity_a = {0,0,0});

		bool IsPlaying() const;

		FMOD_CHANNEL* GetPtr() const { return channel_; }

	    private:
		FMOD_CHANNEL* channel_ = nullptr;
            };
        } // namespace Channel
    }     // namespace Audio
} // namespace CE_Kernel
