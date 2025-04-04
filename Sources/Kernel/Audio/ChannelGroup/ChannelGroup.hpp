// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Channel/Channel.hpp"

#include <FMOD/fmod.h>
#include <memory>
#include <vector>

namespace CE_Kernel
{
    namespace Audio
    {
        namespace ChannelGroup
        {
            class ChannelGroup
            {
            public:
                explicit ChannelGroup(FMOD_CHANNELGROUP* group_a);
                ~ChannelGroup();

		void AddChannel(
			std::shared_ptr<CE_Kernel::Audio::Channel::Channel> channel_a);

		void SetVolume(float volume_a);
		void StopAll();

		FMOD_CHANNELGROUP* GetPtr() const { return group_; }
	    
	    private:
		FMOD_CHANNELGROUP* group_ = nullptr;
		std::vector<std::shared_ptr<CE_Kernel::Audio::Channel::Channel>> channels_;
            };
        } // namespace ChannelGroup
    }     // namespace Audio
} // namespace CE_Kernel
