[English](#en) | [Русский](#ru)

---

<a id="en"></a>
# Channel Class

Audio module component for managing audio channels in FMOD.

## Features
- Channel state control (pause/stop)
- Volume adjustment
- 3D spatialization
- Playback status check
- Direct access to FMOD channel object

## Basic API Methods
#### Class Channel
- `explicit Channel(FMOD_CHANNEL* channel_a)` - Constructor (internal use)
- `void SetPaused(bool paused_a)` - Pause/unpause playback
- `void Stop()``` - Immediately stops playback
- `void SetVolume(float volume_a)` - Set volume (0.0 - 1.0 range)
- `void Set3DAttributes(
        const FMOD_VECTOR& position_a,
        const FMOD_VECTOR& velocity_a = {0,0,0}
)` - 3D position in world space and object velocity (optional)

- `bool IsPlaying() const` - Check if channel is active
- `FMOD_CHANNEL* GetPtr() const` - Get raw FMOD channel pointer

## Example
#### cpp
```cpp
#include <Kernel/Audio/Channel/Channel.hpp>

// Typical usage with AudioSystem:
auto channel_ = audio_sys_.PlaySound(sound_);

// Control playback
channel_->SetVolume(0.75f);
channel_->SetPaused(true); 

// 3D positioning
FMOD_VECTOR pos_ = {10.0f, 0.0f, 5.0f};
FMOD_VECTOR vel_ = {0.0f, 0.0f, 0.0f};
channel->Set3DAttributes(pos_, vel_);

// Check status
if (!channel_->IsPlaying())
{
    channel_->Stop();
}
``` 

---

<a id="ru"></a>
# Channel Класс

Компонент Audio модуля для работы с аудио каналами в FMOD.

## Возможности
- Управление состоянием канала (пауза/стоп)
- Регулировка громкости
- Пространственное 3D-позиционирование
- Проверка статуса воспроизведения
- Прямой доступ к объекту FMOD channel

## Основные методы API
#### Класс Channel
- `explicit Channel(FMOD_CHANNEL* channel_a)` - Конструктор (внутреннее использование)
- `void SetPaused(bool paused_a)` - Постановка на паузу/снятие с паузы
- `void Stop()` - Немедленная остановка воспроизведения
- `void SetVolume(float volume_a)` - Установка громкости (0.0 - 1.0)
- `void Set3DAttributes(
        const FMOD_VECTOR& position_a,
        const FMOD_VECTOR& velocity_a = {0,0,0}
)` - 3D позиция в мировом пространстве и скорость объекта (опционально)

- `bool IsPlaying() const` - Проверка активности канала
- `FMOD_CHANNEL* GetPtr() const` - Получение указателя на FMOD channel

## Пример использования
#### cpp
```cpp
#include <Kernel/Audio/Channel/Channel.hpp>

// Использование с AudioSystem:
auto channel_ = audio_sys_.PlaySound(sound_);

// Управление воспроизведением
channel_->SetVolume(0.75f);
channel_->SetPaused(true); 

// 3D-позиционирование
FMOD_VECTOR pos_ = {10.0f, 0.0f, 5.0f};
FMOD_VECTOR vel_ = {0.0f, 0.0f, 0.0f};
channel->Set3DAttributes(pos_, vel_);

// Проверка статуса
if (!channel_->IsPlaying())
{
    channel_->Stop();
}
```