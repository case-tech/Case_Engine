[English](#en) | [Русский](#ru)

---
#Geometry Class {#en}

Audio module component for spatial audio geometry management in FMOD.

## Features
- 3D position control
- Polygon-based occlusion setup
- Direct/Reverb occlusion configuration
- FMOD geometry integration

## Basic API Methods
#### Class Geometry
- ```Geometry(FMOD_GEOMETRY* geom_a)``` - Constructor (use internal)
- ```void SetPosition(const Vector3D& position_a)``` - Set world position of geometry
- ```void AddPolygon(
        float direct_occlusion_a, // 0.0-1.0 (full to no occlusion)
        float reverb_occlusion_a, // 0.0-1.0 (full to no occlusion)
        const std::vector<Vector3D>& vertices_a // Polygon vertices
);```

## Example
#### cpp
``` cpp
#include <Kernel/Audio/Geometry/Geometry.hpp>

// Create geometry (typically via AudioSystem)
FMOD_GEOMETRY* fmod_geo_;
CE_Kernel::Audio::Geometry::Geometry geo_(fmod_geo_);

// Set world position
geo_.SetPosition({5.0f, 0.0f, 3.0f});

// Create quad polygon vertices
std::vector<CE_Kernel::Audio::Geometry::Vector3D> quad_ = {
    {0,0,0}, {2, 0, 0}, {2, 3, 0}, {0, 3, 0}
};

// Add polygon with occlusion settings
geo_.AddPolygon(
    0.8f, 
    0.6f, 
    quad_
);
```

---

#Geometry Класс {#ru}

Компонент Audio модуля для управления пространственной геометрией в FMOD.

## Возможности
- Управление 3D-позицией
- Настройка окклюзии через полигоны
- Конфигурация прямой/реверберационной окклюзии
- Интеграция с FMOD geometry

## Основные методы API
#### Класс Geometry
- ```Geometry(FMOD_GEOMETRY* geom_a)``` - Конструктор (внутреннее использование)
- ```void SetPosition(const Vector3D& position_a)``` - Установка позиции в мировом пространстве
- ```void AddPolygon(
        float direct_occlusion_a, // 0.0-1.0 (полная - отсутствие окклюзии)
        float reverb_occlusion_a, // 0.0-1.0 (полная - отсутствие окклюзии)
        const std::vector<Vector3D>& vertices_a // Вершины полигона
);```

## Пример использования
#### cpp
``` cpp
#include <Kernel/Audio/Geometry/Geometry.hpp>

// Создание геометрии (обычно через AudioSystem)
FMOD_GEOMETRY* fmod_geo_;
CE_Kernel::Audio::Geometry::Geometry geo_(fmod_geo_);

// Установка позиции
geo_.SetPosition({5.0f, 0.0f, 3.0f});

// Создание вершин четрёхугольника
std::vector<CE_Kernel::Audio::Geometry::Vector3D> quad_ = {
    {0,0,0}, {2, 0, 0}, {2, 3, 0}, {0, 3, 0}
};

// Добавление полигона с окклюзией
geo_.AddPolygon(
    0.8f, 
    0.6f, 
    quad_
);
```
