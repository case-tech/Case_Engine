[English](#en) | [Русский](#ru)

---

<a id="en"></a>
# ChannelGroup Class

Audio module component for managing group of audio channels in FMOD.

## Features
- Channel group volume control
- Bulk channel operations
- Group hierarchy support
- Direct access to FMOD channel group object

## Basic API Methods
#### Class ChannelGroup
- `explicit ChannelGroup(FMOD_CHANNELGROUP* group_a)` - Constructor (internal use)
- `void AddChannel(
        std::shared_ptr<CE_Kernel::Audio::Channel::Channel> channel_a
)` - Add a channel to the group
- `void SetVolume(float volume_a)` - Set group volume (0.0 - 1.0 range)
- `void StopAll()` - Stop all channels in group
- `FMOD_CHANNELGROUP* GetPtr() const` - Get raw FMOD group pointer

## Example
#### cpp
```cpp
#include <Kernel/Audio/ChannelGroup/ChannelGroup.hpp>

// Typical usage with AudioSystem:
auto music_group_ = audio_sys_.CreateChannelGroup("Music");
auto sfx_group_ = audio_sys_.CreateChannelGroup("SFX");

// Add channels to groups
auto music_channel_ = audio_sys_.PlaySound(bg_music_);
music_group_->AddChannel(music_channel_);

// Control group volume
music_group_->SetVolume(0.6f);
sfx_group_->SetVolume(1.0f);

// Stop all group channels
sfx_group_->StopAll(); 
```

---

<a id="ru"></a>
# ChannelGroup класс

Компонент Audio модуля для управления группами аудио каналов в FMOD.

## Возможности
- Управление громкостью группы каналов
- Групповые операции с каналами
- Поддержка иерархии групп
- Прямой доступ к объекту FMOD channel group

## Основные методы API
#### Класс ChannelGroup
- `explicit ChannelGroup(FMOD_CHANNELGROUP* group_a)` - Конструктор (внутреннее использование)
- `void AddChannel(
         std::shared_ptr<CE_Kernel::Audio::Channel::Channel> channel_a
)` - Добавление канала в группу
- `void SetVolume(float volume_a)` - Установка громкости группы (0.0-1.0)
- `void StopAll()` - Остановка всех каналов группы
- `FMOD_CHANNELGROUP* GetPtr() const` - Получение указателя на FMOD group

## Пример использования
#### cpp
```cpp
#include <Kernel/Audio/ChannelGroup/ChannelGroup.hpp>

// Типичное использование с AudioSystem:
auto music_group_ = audio_sys_.CreateChannelGroup("Music");
auto sfx_group_ = audio_sys_.CreateChannelGroup("SFX");

// Добавление каналов в группы
auto music_channel_ = audio_sys_.PlaySound(bg_music_);
music_group_->AddChannel(music_channel_);

// Управление громкостью групп
music_group_->SetVolume(0.6f);
sfx_group_->SetVolume(1.0f);

// Остановка всех каналов группы
sfx_group_->StopAll();
```