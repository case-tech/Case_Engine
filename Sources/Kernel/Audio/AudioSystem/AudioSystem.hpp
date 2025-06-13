// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Channel/Channel.hpp"
#include "../ChannelGroup/ChannelGroup.hpp"
#include "../DSP/DSPEffect.hpp"
#include "../Sound/Sound.hpp"

#include <FMOD/fmod.h>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace CE_Kernel
{
    namespace Audio
    {
        namespace AudioSystem
        {
            class AudioSystem
            {
            public:
                AudioSystem();
                ~AudioSystem();

                bool Initialize();
                void Update();
                void Shutdown();

                std::shared_ptr<CE_Kernel::Audio::Sound::Sound> CreateSound(
                        const std::string& file_path_a,
                        bool is_3d_a = false,
                        bool loop_a = false);

                std::shared_ptr<CE_Kernel::Audio::Channel::Channel> PlaySound(
                        std::shared_ptr<CE_Kernel::Audio::Sound::Sound> sound_a,
                        bool start_paused_a = false);

                std::shared_ptr<CE_Kernel::Audio::ChannelGroup::ChannelGroup>
                CreateChannelGroup(const std::string& name_a);
                std::shared_ptr<CE_Kernel::Audio::DSPEffect::DSPEffect>
                CreateDSPEffect(CE_Kernel::Audio::DSPEffect::DSPType type_a);

                void Set3DSettings(float doppler_scale_a = 1.0f,
                                   float distance_factor_a = 1.0f,
                                   float rolloff_scale_a = 1.0f);

            private:
                static void FMODCheck(FMOD_RESULT result_a);

            private:
                FMOD_SYSTEM* system_ = nullptr;
                std::unordered_map<
                        std::string,
                        std::shared_ptr<
                                CE_Kernel::Audio::ChannelGroup::ChannelGroup>>
                        channel_groups_;
            };
        } // namespace AudioSystem
    } // namespace Audio
} // namespace CE_Kernel
