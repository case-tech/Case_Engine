#include "ChannelGroup.hpp"

namespace CE_Kernel
{
    namespace Audio
    {
        namespace ChannelGroup
        {
            ChannelGroup::ChannelGroup(FMOD_CHANNELGROUP* group_a)
                : group_(group_a)
            {}

            ChannelGroup::~ChannelGroup()
            {
                StopAll();
                if (group_)
                    FMOD_ChannelGroup_Release(group_);
            }

            void ChannelGroup::AddChannel(
                    std::shared_ptr<CE_Kernel::Audio::Channel::Channel>
                            channel_a)
            {
                if (group_ && channel_a)
                {
                    FMOD_RESULT result_ =
                            FMOD_Channel_SetChannelGroup(channel_a->GetPtr(),
                                                         group_);

                    if (result_ == FMOD_OK)
                    {
                        channels_.push_back(channel_a);
                    }
                }
            }

            void ChannelGroup::SetVolume(float volume_a)
            {
                if (group_)
                    FMOD_ChannelGroup_SetVolume(group_, volume_a);
            }

            void ChannelGroup::StopAll()
            {
                if (group_)
                    FMOD_ChannelGroup_Stop(group_);
                channels_.clear();
            }

        } // namespace ChannelGroup
    } // namespace Audio
} // namespace CE_Kernel
