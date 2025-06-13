#include "Channel.hpp"

namespace CE_Kernel
{
    namespace Audio
    {
        namespace Channel
        {
            Channel::Channel(FMOD_CHANNEL* channel_a) : channel_(channel_a)
            {}

            Channel::~Channel()
            {
                Stop();
            }

            void Channel::SetPaused(bool paused_a)
            {
                if (channel_)
                    FMOD_Channel_SetPaused(channel_, paused_a);
            }

            void Channel::Stop()
            {
                if (channel_)
                {
                    FMOD_Channel_Stop(channel_);
                    channel_ = nullptr;
                }
            }

            void Channel::SetVolume(float volume_a)
            {
                if (channel_)
                    FMOD_Channel_SetVolume(channel_, volume_a);
            }

            bool Channel::IsPlaying() const
            {
                if (!channel_)
                    return false;
                FMOD_BOOL is_playing_;
                FMOD_Channel_IsPlaying(channel_, &is_playing_);
                return is_playing_;
            }

            void Channel::Set3DAttributes(const FMOD_VECTOR& position_a,
                                          const FMOD_VECTOR& velocity_a)
            {
                if (channel_)
                    FMOD_Channel_Set3DAttributes(channel_,
                                                 &position_a,
                                                 &velocity_a);
            }
        } // namespace Channel
    } // namespace Audio
} // namespace CE_Kernel
