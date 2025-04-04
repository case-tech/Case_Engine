[English](#en) | [Русский](#ru)

---

# AudioSystem {#en}

Audio module component for audio management using FMOD.

## Features
- Audio system initialization and shutdown
- Sound creation and playback
- Channel group management
- DSP effects support
- 3D audio settings configuration

## Basic API Methods
#### Class AudioSystem
- `bool Initialize()` - Initializes the audio system
- `void Update()` - Updates the audio system 
- `void Shutdown()` - Shuts down the audio system

- `std::shared_ptr<CE_Kernel::Audio::Channel::Channel> PlaySound(
       std::shared_ptr<CE_Kernel::Audio::Sound::Sound> sound_a,
       bool start_paused_a = false
);` - Plays a Sound and returns a Channel

- `std::shared_ptr<CE_Kernel::Audio::ChannelGroup::ChannelGroup> createChannelGroup(
    const std::string& name_a
);` - Creates a ChannelGroup with a specified name

- `std::shared_ptr<CE_Kernel::Audio::DSPEffect::DSPEffect> CreateDSPEffect(
    CE_Kernel::Audio::DSPEffect::DSPType type_a
);` - Creates a DSP effect of specified type

- `void Set3DSettings(
    float doppler_scale_a = 1.0f,
    float distance_factor_a = 1.0f,
    float rolloff_scale_a = 1.0f
);` - Configures global 3D audio settings

## Example
#### cpp
```cpp
#include <Kernel/Audio/AudioSystem/AudioSystem.hpp>

int main()
{
    CE_Kernel::Audio::AudioSystem::AudioSystem audio_sys_;
    if (!audio_sys_.Initialize())
    {
        std::cerr << "Failed to initialize audio system!" << std::endl;
        return 1;
    }

    auto sound_ = audio_sys_.CreateSound("explosion.wav", true, true);
    auto channel_ = audio_sys_.PlaySound(sound_);
    auto test_group_ = audio_sys_.CreateChannelGroup("Test");

    audio_sys_.Set3DSettings(1.0f, 0.5f, 2.0f);

    while (true)
    {
        audio_sys_.Update();
        // ... other logic
    }

    audio_sys_.Shutdown();
    return 0;
}
```

---

# AudioSystem {#ru}

Компонент Audio модуля для управления аудио с использованием FMOD.

## Возможности
- Инициализация и завершение работы аудиосистемы
- Создание и воспроизведение звуков
- Управление группами каналов
- Поддержка DSP-эффектов
- Настройка параметров 3D-аудио

## Основные методы API
#### Класс AudioSystem
- `bool Initialize()` - Инициализирует аудиосистему
- `void Update()` - Обновляет состояние аудиосистемы
- `void Shutdown()` - Завершает работу аудиосистемы

- `std::shared_ptr<CE_Kernel::Audio::Channel::Channel> PlaySound(
       std::shared_ptr<CE_Kernel::Audio::Sound::Sound> sound_a,
       bool start_paused_a = false
);` - Создаёт объект Sound из файла

- `std::shared_ptr<CE_Kernel::Audio::ChannelGroup::ChannelGroup> createChannelGroup(
    const std::string& name_a
);` - Создаёт ChannelGroup с указанным именем

- `std::shared_ptr<CE_Kernel::Audio::DSPEffect::DSPEffect> CreateDSPEffect(
    CE_Kernel::Audio::DSPEffect::DSPType type_a
);` - Создаёт DSP-эффект указанного типа

- `void Set3DSettings(
    float doppler_scale_a = 1.0f,
    float distance_factor_a = 1.0f,
    float rolloff_scale_a = 1.0f
);` - Настраивает глобальные параметры 3D-аудио

## Пример использования
#### cpp
```cpp
#include <Kernel/Audio/AudioSystem/AudioSystem.hpp>

int main()
{
    CE_Kernel::Audio::AudioSystem::AudioSystem audio_sys_;
    if (!audio_sys_.Initialize())
    {
        std::cerr << "Failed to initialize audio system!" << std::endl;
        return 1;
    }

    auto sound_ = audio_sys_.CreateSound("explosion.wav", true, true);
    auto channel_ = audio_sys_.PlaySound(sound_);
    auto test_group_ = audio_sys_.CreateChannelGroup("Test");

    audio_sys_.Set3DSettings(1.0f, 0.5f, 2.0f);

    while (true)
    {
        audio_sys_.Update();
        // ... другая логика
    }

    audio_sys_.Shutdown();
    return 0;
}
```