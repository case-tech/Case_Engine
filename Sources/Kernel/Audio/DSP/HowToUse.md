[English](#en) | [Русский](#ru)

---

<a id="en"></a>
# DSPEffect Class

Audio module component for applying DSP effects to audio channels/groups in FMOD.

## Features
- Multiple effect types support (Echo, Reverb, Low/High Pass)
- Parameter customization
- Flexible routing to channels/groups
- Direct FMOD DSP integration

## Basic API Methods
#### Class DSPEffect
- `explicit DSPEffect(FMOD_DSP* dsp_a)` - Constructor (internal use)
--`void SetParameter(int index_a, float value_a)` - Set effect-specific parameter (index depends on DSP type)
- `void ConnectToChannel(CE_Kernel::Audio::Channel::Channel* channel_a)` - Connect effect to individual channel
- `void ConnectToGroup(CE_Kernel::Audio::ChannelGroup::ChannelGroup* group_a)` - Connect effect to entire channel group

## Example
#### cpp
```cpp
#include <Kernel/Audio/DSP/DSPEffect.hpp>

// create reverb effect via AudioSystem
auto reverb_ = audio_sys_.CreateDSPEffect(CE_Kernel::Audio::DSPEffect::DSPType::Reverb);

// Configure reverb parameters
reverb_->SetParameter(0, 0.7f); // decay time
reverb_->SetParameter(1, 0.5f); // Wet level

// Apply to music group
reverb_->ConnectToGroup(music_group_.get());

// Create low-pass filter for voice channel
auto filter_ = audio_sys_.CreateDSPEffect(CE_Kernel::Audio::DSPEffect::DSPType::LowPass);
filter_->SetParameter(0, 5000.0f); // Cutoff frequency
filter_->ConnectToChannel(voice_channel_.get());
```

---

<a id="ru"></a>
# DSPEffect Класс

Компонент Audio модуля для применения DSP-эффектов к каналам/группам в FMOD.

## Возможности
- Поддержка различных эффектов (Эхо, Реверберация, ФНЧ/ФВЧ)
- Настройка параметров эффектов
- Гибкое подключение к каналам/группам
- Прямая интеграция с FMOD DSP

## Основные методы API
#### Класс DSPEffect
- `explicit DSPEffect(FMOD_DSP* dsp_a)` - Конструктор (внутреннее использование)
- `void SetParameter(int index_a, float value_a)` - Установка параметра эффектов (индекс зависит от типа DSP)
- `void ConnectToChannel(CE_Kernel::Audio::Channel::Channel* channel_a)` - Подключение эффекта к отдельному каналу
- `void ConnectToGroup(CE_Kernel::Audio::ChannelGroup::ChannelGroup* group_a)` - Подключение эффекта к группе каналов

## Пример использования
#### cpp
```cpp
#include <Kernel/Audio/DSP/DSPEffect.hpp>

// Создание реверберации через AudioSystem
auto reverb_ = audio_sys_.CreateDSPEffect(CE_Kernel::Audio::DSPEffect::DSPType::Reverb);

// Настройка параметров
reverb_->SetParameter(0, 0.7f); // Время затухания
reverb_->SetParameter(1, 0.5f); // Уровень эффекта

// Применение к группе музыки
reverb_->ConnectToGroup(music_group_.get());

// Создание ФНЧ для голосового канала
auto filter_ = audio_sys_.CreateDSPEffect(CE_Kernel::Audio::DSPEffect::DSPType::LowPass);
filter_->SetParameter(0, 5000.0f); // Частота среза
filter_->ConnectToChannel(voice_channel_.get());
```