// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Channel/Channel.hpp"
#include "../ChannelGroup/ChannelGroup.hpp"

#include <FMOD/fmod.h>

namespace CE_Kernel
{
    namespace Audio
    {
        namespace DSPEffect
        {
            enum class DSPType
            {
                Echo,
                Reverb,
                LowPass,
                HighPass
            };

            class DSPEffect
            {
            public:
                explicit DSPEffect(FMOD_DSP* dsp_a);
                ~DSPEffect();

                void SetParameter(int index_a, float value_a);
                void ConnectToChannel(
                        CE_Kernel::Audio::Channel::Channel* channel_a);
                void ConnectToGroup(
                        CE_Kernel::Audio::ChannelGroup::ChannelGroup* group_a);

            private:
                FMOD_DSP* dsp_ = nullptr;
            };
        } // namespace DSPEffect
    } // namespace Audio
} // namespace CE_Kernel
