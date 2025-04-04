#include "../Kernel/Audio/Audio.hpp"

#include <chrono>
#include <thread>

using namespace CE_Kernel::Audio;

int main()
{
    AudioSystem::AudioSystem audio_sys_;
    if (!audio_sys_.Initialize()) {
        return -1;
    }

    audio_sys_.Set3DSettings(1.0f, 1.0f, 1.0f);

    auto background_music_ = audio_sys_.CreateSound("/mnt/DATA_DISK/Case_Technologies/Products/Case_Engine/Resources/Audios/test1.mp3");
    auto gun_music_ = audio_sys_.CreateSound(
        "/mnt/DATA_DISK/Case_Technologies/Products/Case_Engine/Resources/Audios/gun.wav");

    auto effects_group_ = audio_sys_.CreateChannelGroup("Effects");

    auto effect_ = audio_sys_.CreateDSPEffect(DSPEffect::DSPType::Echo);
    if (effect_) {
        effect_->SetParameter(0, 0.5f);
        effect_->SetParameter(1, 0.7f);
    }

    auto music_ = audio_sys_.PlaySound(background_music_);
    if (music_) {
        music_->SetVolume(0.6f);
    }

    for (int i = 0; i < 10; ++i) {
        audio_sys_.Update();

        for (;;) {
            if (i % 3 == 0) {
                auto shot_ = audio_sys_.PlaySound(gun_music_);
                if (shot_) {
                    FMOD_VECTOR pos_ = {i * 2.0f, 0.0f, 0.0f};
                    shot_->Set3DAttributes(pos_);

                    effects_group_->AddChannel(shot_);

                    if (effect_) {
                        effect_->ConnectToChannel(shot_.get());
                    }
                }
                break;
            }
        }

        if (i == 3) {
            effects_group_->SetVolume(0.5f);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    effects_group_->StopAll();
    if (music_ && music_->IsPlaying()) {
        music_->Stop();
    }

    audio_sys_.Shutdown();

    return 0;
}
