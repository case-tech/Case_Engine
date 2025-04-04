#include "AudioSystem.hpp"
#include "../DSP/DSPEffect.hpp"
#include <iostream>

namespace CE_Kernel
{
    namespace Audio
    {
        namespace AudioSystem
        {
            AudioSystem::AudioSystem() = default;

            AudioSystem::~AudioSystem()
            {
                Shutdown();
            }

            bool AudioSystem::Initialize()
            {
                FMOD_RESULT result_;
                result_ = FMOD_System_Create(&system_, FMOD_VERSION);
                if (result_ != FMOD_OK)
                    return false;

                result_ = FMOD_System_Init(system_, 512, FMOD_INIT_NORMAL, nullptr);
                return result_ == FMOD_OK;
            }

            void AudioSystem::Update()
            {
                FMOD_System_Update(system_);
            }

            void AudioSystem::Shutdown()
            {
                if (system_) 
		{
                    FMOD_System_Close(system_);
                    FMOD_System_Release(system_);
                    system_ = nullptr;
                }
            }

            std::shared_ptr<CE_Kernel::Audio::Sound::Sound> AudioSystem::CreateSound(
			    const std::string& file_path_a, bool is_3d_a, bool loop_a)
            {
                FMOD_MODE mode_ = FMOD_DEFAULT;
                mode_ |= is_3d_a ? FMOD_3D : FMOD_2D;
                mode_ |= loop_a ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

                FMOD_SOUND* sound_ = nullptr;
                FMOD_RESULT result_ = FMOD_System_CreateSound(
				system_, file_path_a.c_str(), mode_, nullptr, &sound_);

                if (result_ != FMOD_OK)
                    return nullptr;

                return std::make_shared<CE_Kernel::Audio::Sound::Sound>(sound_);
            }

            std::shared_ptr<CE_Kernel::Audio::Channel::Channel> AudioSystem::PlaySound(
			    std::shared_ptr<CE_Kernel::Audio::Sound::Sound> sound_a, bool start_paused_a)
            {
                FMOD_CHANNEL* channel_ = nullptr;
                FMOD_RESULT result_ = FMOD_System_PlaySound(
				system_, sound_a->GetPtr(), nullptr, start_paused_a, &channel_);

                if (result_ != FMOD_OK)
                    return nullptr;

                return std::make_shared<CE_Kernel::Audio::Channel::Channel>(channel_);
            }

            std::shared_ptr<CE_Kernel::Audio::ChannelGroup::ChannelGroup> AudioSystem::CreateChannelGroup(
			const std::string& name_a)
            {
                FMOD_CHANNELGROUP* group_ = nullptr;
                FMOD_RESULT result_ = FMOD_System_CreateChannelGroup(system_, name_a.c_str(), &group_);

                if (result_ != FMOD_OK)
                    return nullptr;

                auto channel_group_ = 
			std::make_shared<CE_Kernel::Audio::ChannelGroup::ChannelGroup>(group_);
                channel_groups_[name_a] = channel_group_;
                return channel_group_;
            }

            void AudioSystem::Set3DSettings(
			    float doppler_scale_a, float distance_factor_a, float rolloff_scale_a)
            {
                FMOD_System_Set3DSettings(
				system_, doppler_scale_a, distance_factor_a, rolloff_scale_a);
            }

	    std::shared_ptr<CE_Kernel::Audio::DSPEffect::DSPEffect> AudioSystem::CreateDSPEffect(CE_Kernel::Audio::DSPEffect::DSPType type_a)
	    {
	    	FMOD_DSP* dsp_ = nullptr;
		FMOD_DSP_TYPE fmod_type_;

		switch (type_a)
		{
		case CE_Kernel::Audio::DSPEffect::DSPType::Echo:
			fmod_type_ = FMOD_DSP_TYPE_ECHO;
			break;
		case CE_Kernel::Audio::DSPEffect::DSPType::Reverb:
			fmod_type_ = FMOD_DSP_TYPE_SFXREVERB;
			break;
		case CE_Kernel::Audio::DSPEffect::DSPType::LowPass:
			fmod_type_ = FMOD_DSP_TYPE_LOWPASS;
			break;
		case CE_Kernel::Audio::DSPEffect::DSPType::HighPass:
			fmod_type_ = FMOD_DSP_TYPE_HIGHPASS;
			break;
		default:
			return nullptr;
		}

		FMOD_RESULT result_ = FMOD_System_CreateDSPByType(system_, fmod_type_, &dsp_);
		if (result_ != FMOD_OK || !dsp_)
		{
		    return nullptr;
		}

		return std::make_shared<CE_Kernel::Audio::DSPEffect::DSPEffect>(dsp_);
	    }

            void AudioSystem::FMODCheck(FMOD_RESULT result_a)
            {
                if (result_a != FMOD_OK) 
		{
		    std::cout << "FMOD Error" << std::endl;
                }
            }

        } // namespace AudioSystem
    }     // namespace Audio
} // namespace CE_Kernel
