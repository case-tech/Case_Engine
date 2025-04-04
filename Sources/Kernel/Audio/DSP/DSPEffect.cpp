#include "DSPEffect.hpp"

#include "../Channel/Channel.hpp"
#include "../ChannelGroup/ChannelGroup.hpp"

namespace CE_Kernel
{
    namespace Audio
    {
        namespace DSPEffect
        {
            DSPEffect::DSPEffect(FMOD_DSP* dsp_a)
                : dsp_(dsp_a) {}

            DSPEffect::~DSPEffect()
            {
                if (dsp_)
                    FMOD_DSP_Release(dsp_);
            }

            void DSPEffect::SetParameter(int index_a, float value_a)
            {
                if (dsp_)
                    FMOD_DSP_SetParameterFloat(dsp_, index_a, value_a);
            }

            void DSPEffect::ConnectToChannel(CE_Kernel::Audio::Channel::Channel* channel_a)
            {
                if (dsp_ && channel_a) 
		{
                    FMOD_Channel_AddDSP(channel_a->GetPtr(), 0, dsp_);
                }
            }

            void DSPEffect::ConnectToGroup(
			    CE_Kernel::Audio::ChannelGroup::ChannelGroup* group_a)
            {
                if (dsp_ && group_a) {
                    FMOD_ChannelGroup_AddDSP(group_a->GetPtr(), 0, dsp_);
                }
            }

        } // namespace DSPEffect
    }     // namespace Audio
} // namespace CE_Kernel
