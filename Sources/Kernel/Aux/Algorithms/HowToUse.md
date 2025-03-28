[English](#en) | [Русский](#ru)

---

# Content {#en}
- [Interpolator](#en1)
- [StateMachine](#en2)
- [SimplexNoise](#en3)
- [AStar](#en4)
- [Encryption](#en5)
- [NoisePerlin](#en6)

---

# Interpolator Class {#en1}
Aux module component for value interpolation with various easing methods.

## Features
- Supports multiple interpolation algorithms
- Template-based for flexible data types
- Smooth transitions between values
- Supported methods:
    - Linear interpolation (Lerp)
    - Ease-In/Out transitions
    - Cubic interpolation
    - SmoothStep
    - Exponential easing

## Core API Methods
- ```static T Lerp(const T& a_a, const T& b_a, float t_a)``` - Linear interpolation between a and b using t (clamped to [0, 1]).
- ```static T EaseInOut(const T& a_a, const T& b_a, float t_a)``` - Smooth acceleration/deceleration transition.
- ```static T Cubic(const T& a_a, const T& b_a, float t_a)``` - Cubic interpolation for smoother curves.
- ```static T SmoothStep(const T& a_a, const T& b_a, float t_a)``` -  S-shaped curve transition.
- ```static T Exponential(const T& a_a, const T& b_a, float t_a)``` - Exponential easing effect.

## Example
#### cpp
```cpp
#include <Kernel/Aux/Algorithms/Algorithms.hpp>

using namespace CE_Kernel::Aux::Algorithms::Interpolator;

int main() 
{
    // Linear interpolation example
    float start_ = 0.0f;
    float end_ = 100.0f;
    
    // Interpolate at midpoint
    float result_lerp_ = Interpolator<float>::Lerp(start_, end_, 0.5f);
    std::cout << "Lerp: " << result_lerp_ << "\n"; // Output: 50

    // Ease-In-Out example
    float result_ease_ = Interpolator<float>::EaseInOut(start_, end_, 0.5f);
    std::cout << "EaseInOut: " << result_ease_ << "\n"; // Output: 50

    // SmoothStep animation loop example
    for (float t = 0; t <= 1.0f; t += 0.1f) 
    {
        float value_ = Interpolator<float>::SmoothStep(start_, end_, t);
        std::cout << "t=" << t << " => " << value << "\n";
    }

    return 0;
}
```

--- 

# StateMachine Class {#en2}
Aux module component for managing state transitions with conditional logic and history tracking.

## Features
- Hierarchical state management
- Conditional transitions between states
- State history stack for rollbacks
- Customizable state entry/update callbacks
- Type-safe template implementation

## Core API Methods
- ```void AddState(T state_a, std::function<void()> on_update_a)``` - Register a state with update callback.
- ```void AddTransition(T from_a, T to_a, std::function<bool()> condition_a)``` - Define transition rule between states with condition check.
- ```void Update()``` - Process state transitions and execute current state's callback.
- ```bool TryTransition(T new_state_a, bool save_history_a = false)``` - Attempt immediate state change (returns success status).
- ```bool RevertToPreviousState()``` - Rollback to previous state from history (returns success status).
- ```void ClearHistory()``` - Reset state history stack.
- ```T GetCurrentState() const``` - Get active state identifier.

## Example
#### cpp
```cpp
#include <Kernel/Aux/Algorithms/Algorithms.hpp>

enum class AppState { Idle, Loading, Active, Error };

int main() 
{
    CE_Kernel::Aux::Algorithms::StateMachine<AppState> fsm_;

    // Configure states
    fsm_.AddState(AppState::Idle, []() { std::cout << "Idle state\n"; });
    fsm_.AddState(AppState::Loading, []() { std::cout << "Loading...\n"; });
    fsm_.AddState(AppState::Active, []() { std::cout << "Active mode\n"; });

    // Configure transitions
    fsm_.AddTransition(AppState::Idle, AppState::Loading,
        []() { return /* some condition */ true; });
    
    fsm_.AddTransition(AppState::Loading, AppState::Active,
        []() { return /* resource loaded */ true; });

    // Runtime operations
    fsm_.Update();  // Executes Idle callback
    fsm_.TryTransition(AppState::Loading, true);
    fsm_.Update();  // Executes Loading callback
    
    if(fsm_.GetCurrentState() == AppState::Loading) 
    {
        fsm_.TryTransition(AppState::Active);
    }
    
    fsm_.RevertToPreviousState();  // Returns to Loading
    fsm_.ClearHistory();

    return 0;
}
```

---

# SimplexNoise Class {#en3}
Aux module component for procedural noise generation using Simplex algorithm.

## Features
- 2D noise generation
- Multi-octave fractal noise support
- Configurable scale and persistence
- Optimized gradient tables for performance
- Seamless noise patterns

## Core API Methods
- ```static float Generate(float x_a, float y_a, float scale_a = 0.1f, int octaves_a = 4, float persistence_a = 0.5f)``` - Generates fractal noise value at specified coordinates.
- Parameters:
    - x_a, y_a - Input coordinates
    - scale_a - Noise sampling frequency
    - octaves_a - Number of noise layers
    - persistence_a - Amplitude reduction per octave
- Returns: Noise value in range [-1.0, 1.0]

## Example
#### cpp
```cpp
#include <Kernel/Aux/Algorithms/Algorithms.hpp>

int main() 
{
    // Generate terrain heightmap
    constexpr int size_ = 10;
    
    for(int y = 0; y < size; ++y) 
    {
        for(int x = 0; x < size; ++x) 
        {
            float noise_ = CE_Kernel::Aux::Algorithms::SimplexNoise::Generate(
                x * 0.1f, 
                y * 0.1f,
                0.5f,    // scale
                6,       // octaves
                0.7f     // persistence
            );
            
            // Convert to 0-1 range
            float height_ = (noise_ + 1.0f) / 2.0f;
            std::cout << height_ << " ";
        }
        
        std::cout << "\n";
    }

    return 0;
}
```

---

# AStar Class {#en4}
Aux module component for pathfinding using A* algorithm on 2D grids.

## Features
- Optimal pathfinding in grid-based environments
- Customizable heuristic calculation
- Obstacle detection and avoidance
- Path reconstruction
- Priority queue optimization

## Core API Methods
- ```std::vector<std::pair<int, int>> AStarSearch(std::vector<std::vector<int>> grid_a, std::pair<int, int> start_a, std::pair<int, int> end_a)``` - Finds optimal path between start and end coordinates.
- Parameters:
    - grid_a - 2D vector where 0 = walkable, any other value = obstacle
    - start_a - (x,y) starting position
    - end_a - (x,y) target position
- Returns: Vector of (x,y) coordinates representing path (empty if no path)
- Node Structure

## Example
#### cpp
```cpp
#include <Kernel/Aux/Algorithms/Algorithms.hpp>

int main() 
{
    // 5x5 grid (1 = obstacle)
    std::vector<std::vector<int>> grid_ = {
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,0,0,0},
        {0,0,1,0,0}
    };

    auto path_ = CE_Kernel::Aux::Algorithms::AStar::AStarSearch(
        grid_, 
        {0, 0},  // Start 
        {4, 4}   // End
    );

    std::cout << "Path length: " << path_.size() << "\n";
    for(auto [x,y] : path_)
    {
        std::cout << "(" << x << "," << y << ") ";
    }

    return 0;
}
```

--- 

# Encryption Class {#en5}
Aux module component for data encryption/decryption operations (Is the Encryption class of the Cutils Library).

## Features
- Multiple algorithm support via CUtils
- Symmetric encryption handling
- String-based key management
- Exception-safe operations
- Memory-efficient processing

## Core API Methods
- ```Encryption(CUtils::Alg algorithm_a, const std::string& key_a = "")``` - Initializes cipher with selected algorithm and optional key.
- Parameters:
    - algorithm_a - Encryption algorithm identifier from CUtils::Alg
    - key_a - Secret key (algorithm-dependent requirements)
- ```std::string Encrypt(const std::string& plain_text_a)``` - Encrypts UTF-8 string data.
- ```std::string Decrypt(const std::string& cipher_text_a)``` - Decrypts data.
- Algorithms:
    - Caesar
    - Vigenere
    - XOR
    - Substitution
    - Transposition

## Example
#### cpp
```cpp
#include <Kernel/Aux/Algorithms/Algorithms.hpp>

int main() 
{
    std::string plaintext_ = "Hello, World!";

    CUtils::Encryption caesar_encryptor_(CUtils::Alg::Caesar);
    std::string caesar_cipher_ = caesar_encryptor_.Encrypt(plaintext_);
    std::cout << "Caesar Encrypted: " << caesar_cipher_ << std::endl;
    std::cout << "Caesar Decrypted: " << caesar_encryptor_.Decrypt(caesar_cipher_) << std::endl;

    return 0;
}
```

---

# NoisePerlin Class {#en6}
Aux module component for generating natural-looking procedural noise using Perlin algorithm (Is the NoisePerlin class of the Cutils Library).

## Features
- 2D noise generation
- Seed-based initialization
- Smooth gradient transitions
- Deterministic output with same seed
- Native double precision calculation

## Core API Methods
- ```NoisePerlin(unsigned int seed_a = 0)``` - Initializes noise generator with optional seed value.
- Parameters:
    - seed_a - Randomization seed (0 = random initialization)
- ```double Noise(double x_a, double y_a) const``` - Generates Perlin noise value at specified coordinates.

## Example
#### cpp
```cpp
#include <Kernel/Aux/Algorithms/Algorithms.hpp>

int main() 
{
    // Create noise generator with fixed seed
    CE_Kernel::Aux::Algorithms::NoisePerlin noise_(12345);

    // Generate noise map
    constexpr int size_ = 10;
    for(int y = 0; y < size; ++y) 
    {
        for(int x = 0; x < size; ++x) 
        {
            double value_ = noise_.Noise(x * 0.1, y * 0.1);
            std::cout << value_ << " ";
        }
        
        std::cout << "\n";
    }

    return 0;
}
```

---

# Содержимое {#ru}
- [Interpolator](#ru1)
- [StateMachine](#ru2)
- [SimplexNoise](#ru3)
- [AStar](#ru4)
- [Encryption](#ru5)
- [NoisePerlin](#ru6)

--- 

# Класс Interpolator {#ru1}
Компонент модуля Aux для интерполяции значений с различными методами сглаживания.

## Возможности
- Поддержка нескольких алгоритмов интерполяции
- Шаблонный класс для работы с разными типами данных
- Плавные переходы между значениями

## Доступные методы:
- Линейная интерполяция (Lerp)
- Плавное ускорение/замедление (Ease-In/Out)
- Кубическая интерполяция
- S-образный переход (SmoothStep)
- Экспоненциальное сглаживание

## Основные методы API
- ```static T Lerp(const T& a_a, const T& b_a, float t_a)``` - Линейная интерполяция между a и b с параметром t (в диапазоне [0, 1]).
- ```static T EaseInOut(const T& a_a, const T& b_a, float t_a)``` - Плавный переход с ускорением и замедлением.
- ```static T Cubic(const T& a_a, const T& b_a, float t_a)``` - Кубическая интерполяция для создания гладких кривых.
- ```static T SmoothStep(const T& a_a, const T& b_a, float t_a)``` - S-образный переход.
- ```static T Exponential(const T& a, const T& b, float t)``` - Экспоненциальное сглаживание.

## Пример использования
#### cpp
```cpp
#include <Kernel/Aux/Algorithms/Algorithms.hpp>

using namespace CE_Kernel::Aux::Algorithms::Interpolator;

int main() 
{
    // Пример линейной интерполяции
    float start_ = 0.0f;
    float end_ = 100.0f;
    
    // Интерполяция в средней точке
    float result_lerp_ = Interpolator<float>::Lerp(start_, end_, 0.5f);
    std::cout << "Lerp: " << result_lerp_ << "\n"; // Результат: 50

    // Пример Ease-In-Out
    float result_ease_ = Interpolator<float>::EaseInOut(start_, end_, 0.5f);
    std::cout << "EaseInOut: " << result_ease_ << "\n"; // Результат: 50

    // Анимация с SmoothStep
    for (float t = 0; t <= 1.0f; t += 0.1f) 
    {
        float value_ = Interpolator<float>::SmoothStep(start_, end_, t);
        std::cout << "t=" << t << " => " << value_ << "\n";
    }

    return 0;
}
```

---

# Класс StateMachine {#ru2}
Компонент модуля Aux для управления переходами между состояниями с отслеживанием истории.

## Возможности
- Иерархическое управление состояниями
- Условные переходы между состояниями
- История состояний для откатов
- Настраиваемые коллбэки для состояний
- Типобезопасная шаблонная реализация

## Основные методы API
- ```void AddState(T state_a, std::function<void()> on_update_a)``` - Регистрация состояния с коллбэком при обновлении.
- ```void AddTransition(T from_a, T to_a, std::function<bool()> condition_a)``` - Добавление перехода между состояниями с проверкой условия.
- ```void Update()``` - Обработка переходов и выполнение текущего коллбэка.
- ```bool TryTransition(T new_state_a, bool save_history_a = false)``` - Попытка немедленного перехода (возвращает статус успеха).
- ```bool RevertToPreviousState()``` - Возврат к предыдущему состоянию из истории.
- ```void ClearHistory()``` - Очистка истории состояний.
- ```T GetCurrentState() const``` - Получение текущего активного состояния.

## Пример использования
#### cpp
```cpp
#include <Kernel/Aux/Algorithms/Algorithms.hpp>

enum class AppState { Idle, Loading, Active, Error };

int main() 
{
    CE_Kernel::Aux::Algorithms::StateMachine<AppState> fsm_;

    // Настройка состояний
    fsm_.AddState(AppState::Idle, []() { std::cout << "Состояние ожидания\n"; });
    fsm_.AddState(AppState::Loading, []() { std::cout << "Загрузка...\n"; });
    fsm_.AddState(AppState::Active, []() { std::cout << "Активный режим\n"; });

    // Настройка переходов
    fsm_.AddTransition(AppState::Idle, AppState::Loading,
        []() { return /* условие перехода */ true; });
    
    fsm_.AddTransition(AppState::Loading, AppState::Active,
        []() { return /* ресурсы загружены */ true; });

    // Работа конечного автомата
    fsm_.Update();  // Выполняет коллбэк Idle
    fsm_.TryTransition(AppState::Loading, true);
    fsm_.Update();  // Выполняет коллбэк Loading
    
    if(fsm_.GetCurrentState() == AppState::Loading) 
    {
        fsm_.TryTransition(AppState::Active);
    }
    
    fsm_.RevertToPreviousState();  // Возврат к Loading
    fsm_.ClearHistory();

    return 0;
}
```

---

# Класс SimplexNoise {#ru3}
Компонент модуля Aux для процедурной генерации шума по алгоритму Simplex.

## Возможности
- Генерация 2D-шума
- Многооктавный фрактальный шум
- Настройка масштаба и персистентности
- Оптимизированные градиентные таблицы
- Бесшовные текстуры шума

## Основные методы API
- ```static float Generate(float x_a, float y_a, float scale_a = 0.1f, int octaves_a = 4, float persistence_a = 0.5f)``` - Генерирует значение фрактального шума в указанных координатах.
- Параметры:
    - x_a, y_a - Входные координаты
    - scale_a - Частота дискретизации шума
    - octaves_a - Количество слоев шума
    - persistence_a - Уменьшение амплитуды на октаву
    - Возвращает: Значение шума в диапазоне [-1.0, 1.0]

## Пример использования
#### cpp
```cpp
#include <Kernel/Aux/Algorithms/Algorithms.hpp>

int main() 
{
    // Генерация карты высот
    constexpr int size_ = 10;
    
    for(int y = 0; y < size_; ++y)
    {
        for(int x = 0; x < size_; ++x) 
        {
            float noise_ = CE_Kernel::Aux::Algorithms::SimplexNoise::Generate(
                x * 0.1f, 
                y * 0.1f,
                0.5f,    // масштаб
                6,       // октавы
                0.7f     // персистентность
            );
            
            // Нормализация в диапазон 0-1
            float height_ = (noise_ + 1.0f) / 2.0f;
            std::cout << height_ << " ";
        }
        
        std::cout << "\n";
    }

    return 0;
}
```

---

# Класс AStar {#ru4}
Компонент модуля Aux для поиска пути по алгоритму A* в 2D-сетках.

## Возможности
- Оптимальный поиск пути в сеточных средах
- Настраиваемая эвристика
- Обход препятствий
- Восстановление маршрута
- Оптимизация с очередью приоритетов

## Основные методы API
- ```std::vector<std::pair<int, int>> AStarSearch(std::vector<std::vector<int>> grid_a, std::pair<int, int> start_a, std::pair<int, int> end_a)``` - Находит оптимальный путь между точками.
- Параметры:
    - grid_a - 2D-вектор (0 = проходимо, другие значения = препятствия)
    - start_a - Начальная позиция (x,y)
    - end_a - Целевая позиция (x,y)
- Возвращает: Вектор координат (x,y) пути (пустой при отсутствии пути)

## Пример использования
#### cpp
```cpp
#include <Kernel/Aux/Algorithms/Algorithms.hpp>

int main() 
{
    // Сетка 5x5 (1 = препятствие)
    std::vector<std::vector<int>> grid_ = {
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,0,0,0},
        {0,0,1,0,0}
    };

    auto path_ = CE_Kernel::Aux::Algorithms::AStar::AStarSearch(
        grid_, 
        {0, 0},  // Старт 
        {4, 4}   // Финиш
    );

    std::cout << "Длина пути: " << path_.size() << "\n";
    for(auto [x,y] : path_) 
    {
        std::cout << "(" << x << "," << y << ") ";
    }

    return 0;
}
```

---

# Класс Encryption {#ru5}
Компонент модуля Aux для шифрования/дешифрования данных (является обёрткой класса Encryption библиотеки CUtils).

## Возможности
- Поддержка алгоритмов через CUtils
- Симметричное шифрование
- Управление строковыми ключами
- Безопасная обработка ошибок
- Эффективное использование памяти

## Основные методы API
- ```Encryption(CUtils::Alg algorithm_a, const std::string& key_a = "")``` - Инициализация шифра с алгоритмом и опциональным ключом.
- Параметры:
    - algorithm_a - Идентификатор алгоритма из CUtils::Alg
    - key_a - Секретный ключ (требования зависят от алгоритма)
- ```std::string Encrypt(const std::string& plain_text)``` - Шифрование UTF-8 строки.
- ```std::string Decrypt(const std::string& cipher_text)``` Дешифрование данных.
- Алгоритмы:
    - Caesar
    - Vigenere
    - XOR
    - Substitution
    - Transposition

## Пример использования
#### cpp
```cpp
#include <Kernel/Aux/Algorithms/Algorithms.hpp>

int main() 
{
    std::string plaintext_ = "Hello, World!";

    CUtils::Encryption caesar_encryptor_(CUtils::Alg::Caesar);
    std::string caesar_cipher_ = caesar_encryptor_.Encrypt(plaintext_);
    std::cout << "Caesar Encrypted: " << caesar_cipher_ << std::endl;
    std::cout << "Caesar Decrypted: " << caesar_encryptor_.Decrypt(caesar_cipher_) << std::endl;

    return 0;
}
```

---

# Класс NoisePerlin {#ru6}
Компонент модуля Aux для генерации процедурного шума Перлина.

## Возможности
- Генерация 2D-шума
- Инициализация с помощью seed
- Плавные градиентные переходы
- Детерминированный результат с одинаковым seed

## Основные методы API
- ```NoisePerlin(unsigned int seed_a = 0)``` - Инициализация генератора шума.
- Параметры:
    - seed - Значение для инициализации (0 = случайная инициализация)
- ```double Noise(double x_a, double y_a) const``` - Генерирует значение шума Перлина в указанных координатах.
- Возвращает: Значение шума в диапазоне [-1.0, 1.0]

## Пример использования
#### cpp
```cpp
#include <Kernel/Aux/Algorithms/Algorithms.hpp>

int main() 
{
    // Создание генератора с фиксированным seed
    CE_Kernel::Aux::Algorithms::NoisePerlin noise_(12345);

    // Генерация карты шума
    constexpr int size_ = 10;
    for(int y = 0; y < size_; ++y) 
    {
        for(int x = 0; x < size_; ++x) 
        {
            double value_ = noise_.Noise(x * 0.1, y * 0.1);
            std::cout << value_ << " ";
        }
        
        std::cout << "\n";
    }

    return 0;
}
```
