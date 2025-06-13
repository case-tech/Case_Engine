// Copyright (c) 2025 Case Technologies
// Test for AudioSystem

#pragma once
#include <CUtils/UnitTests.hpp>
#include <Audio/AudioSystem/AudioSystem.hpp>
#include <cmath>
#include <thread>

using namespace CUtils;

UTEST(AudioSystemTests, InitializeShutdown)
{
    CE_Kernel::Audio::AudioSystem::AudioSystem audio_system_;
    ASSERT_TRUE(audio_system_.Initialize());
}

UTEST(AudioSystemTests, CreateSound)
{
    CE_Kernel::Audio::AudioSystem::AudioSystem audio_system_;
    ASSERT_TRUE(audio_system_.Initialize());

    auto valid_sound_ = audio_system_.CreateSound("../../../../../Resources/Audios/test1.mp3", false, false);
    ASSERT_TRUE(valid_sound_ != nullptr);

    auto invalid_sound_ = audio_system_.CreateSound("invalid_file.wav", true, true);
    ASSERT_TRUE(invalid_sound_ == nullptr);
}

UTEST(AudioSystemTests, ChannelGroupManagement)
{
    CE_Kernel::Audio::AudioSystem::AudioSystem audio_system_;
    ASSERT_TRUE(audio_system_.Initialize());

    auto group_ = audio_system_.CreateChannelGroup("Effects");
    ASSERT_TRUE(group_ != nullptr);

    auto duplicate_group_ = audio_system_.CreateChannelGroup("Effects");
    ASSERT_TRUE(duplicate_group_ != nullptr);
}

UTEST(AudioSystemTests, DSPEffects)
{
    CE_Kernel::Audio::AudioSystem::AudioSystem audio_system_;
    ASSERT_TRUE(audio_system_.Initialize());

    using DSPType = CE_Kernel::Audio::DSPEffect::DSPType;

    auto echo_ = audio_system_.CreateDSPEffect(DSPType::Echo);
    ASSERT_TRUE(echo_ != nullptr);

    auto reverb_ = audio_system_.CreateDSPEffect(DSPType::Reverb);
    ASSERT_TRUE(reverb_ != nullptr);

    auto invalid_ = audio_system_.CreateDSPEffect(static_cast<DSPType>(999));
    ASSERT_TRUE(invalid_ == nullptr);
}

UTEST(AudioSystemTests, 3DSettings)
{
    CE_Kernel::Audio::AudioSystem::AudioSystem audio_system_;
    ASSERT_TRUE(audio_system_.Initialize());

    audio_system_.Set3DSettings();
    audio_system_.Set3DSettings(2.0f, 0.5f, 3.0f);
    audio_system_.Set3DSettings(0.0f, (std::numeric_limits<float>::max)(), 0.001f);
}

UTEST(AudioSystemTests, ResourceCleanup)
{
    {
        CE_Kernel::Audio::AudioSystem::AudioSystem audio_system_;
        ASSERT_TRUE(audio_system_.Initialize());
        auto sound_ = audio_system_.CreateSound("test.wav");
        auto channel_ = audio_system_.PlaySound(sound_);
        auto group_ = audio_system_.CreateChannelGroup("Group");
        auto dsp_ = audio_system_.CreateDSPEffect(
                CE_Kernel::Audio::DSPEffect::DSPType::LowPass);
    }
}