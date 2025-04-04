[English](#en) | [Русский](#ru)

---
# Sound Class {#en}
Audio module component for managing audio assets and 3D sound properties in FMOD.

## Features
- 3D sound distance configuration
- Direct FMOD sound object access
- Resource lifetime management

## Basic API Methods
#### Class Sound
- ```explicit Sound(FMOD_SOUND* sound_a)``` - Constructor (internal use)
- ```void Set3DMinMaxDistance(
    float min_a,
    float max_a    
)``` - Configure 3D distance range (in meters)
- ``` FMOD_SOUND* GetPtr() const``` - Get raw FMOD sound pointer

## Example
#### cpp
``` cpp
#include <Kernel/Audio/Sound/Sound.hpp>

// Create sound via AudioSystem
auto explosion_sound_ = audio_sys_.CreateSound("explosion.wav", true);

// Configure 3D distance range
explosion_sound_->Set3DMinMaxDistance(2.0f, 50.0f);

// Play with spatialization
auto channel_ = audio_sys_.PlaySound(explosion_sound_);
FMOD_VECTOR pos_ = {10.0f, 0.0f, 5.0f};
channel_->Set3DAttributes(pos_);
``` 
---

# Sound Класс {#ru}
Компонент Audio модуля для управления аудиоресурсами и 3D-свойствами звука в FMOD.

## Возможности
- Настройка дистанции 3D-звука
- Прямой доступ к объекту FMOD sound
- Управление временем жизни ресурса

## Основные методы API
#### Класс Sound
- ```explicit Sound(FMOD_SOUND* sound_a)``` - Конструктор (внутреннее использование)
- ```void Set3DMinMaxDistance(
    float min_a,
    float max_a    
)``` - Настройка дистанционного диапазона 3D-звука
- ``` FMOD_SOUND* GetPtr() const``` - Получение указателя на FMOD sound

## Пример использования
#### cpp
``` cpp
#include <Kernel/Audio/Sound/Sound.hpp>

// Создание звука через AudioSystem
auto explosion_sound_ = audio_sys_.CreateSound("explosion.wav", true);

// Настройка дистанционного диапазона
explosion_sound_->Set3DMinMaxDistance(2.0f, 50.0f);

// Воспроизведение с пространственной привязкой
auto channel_ = audio_sys_.PlaySound(explosion_sound_);
FMOD_VECTOR pos_ = {10.0f, 0.0f, 5.0f};
channel_->Set3DAttributes(pos_);
```                             
