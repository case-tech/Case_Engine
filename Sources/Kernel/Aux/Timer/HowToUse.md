[English](#en) | [Русский](#ru)

---
# Timer Class {#en}

Aux module component for measuring time intervals (wrapper of the Timer class of the CUtils library).

## Features 
- High-precision time measurement in milliseconds 
- Support for pausing, resuming, and restarting the timer 
- Simple and intuitive API 
- Exception-free error handling via return codes 

## Basic API methods
#### Class Timer
- ```double GetMillisecondsElapsed()``` - Get elapsed time in milliseconds since start/reset 
- ```void Restart()``` - Reset timer and start counting again 
- ```bool Stop()``` - Pause timing (returns success status) 
- ```bool Start()``` - Resume paused timer (returns success status) 

## Example
#### cpp
```cpp
#include <Kernel/Aux/Timer/Timer.hpp>

void ProcessData() 
{
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main() 
{
    CE_Kernel::Aux::Timer::Timer timer_;
    timer_.Start();

    ProcessData();
    
    std::cout << "Elapsed: " << timer_.GetMillisecondsElapsed() << " ms\n";
    
    timer_.Restart();
    for(int i = 0; i < 3; ++i) 
    {
        ProcessData();
    }
    
    std::cout << "Total after restart: " << timer_.GetMillisecondsElapsed() << " ms\n";

    timer_.Stop();
    std::cout << "After stop: " << timer.GetMillisecondsElapsed() << " ms\n";
    
    return 0;
}
```

---
# Класс Timer {#ru}
Компонент модуля Aux для измерения временных интервалов (является обёрткой класса Timer библиотеки CUtils).

## Возможности
- Высокоточное измерение времени в миллисекундах
- Поддержка паузы, возобновления и перезапуска таймера
- Простой и интуитивно понятный API
- Обработка ошибок через возвращаемые значения (без исключений)

## Основные методы API
#### Класс Timer
- ```double GetMillisecondsElapsed()``` - Получить прошедшее время в миллисекундах
- ```void Restart()``` - Сбросить и начать отсчёт заново
- ```bool Stop()``` - Приостановить таймер (возвращает статус успеха)
- ```bool Start()``` - Возобновить приостановленный таймер (возвращает статус успеха)

## Пример использования
#### cpp
```cpp
#include <Kernel/Aux/Timer/Timer.hpp>

void ProcessData() 
{
    // Имитация работы
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main() 
{
    CE_Kernel::Aux::Timer::Timer timer_;
    timer_.Start();

    ProcessData();
    
    std::cout << "Прошло: " << timer_.GetMillisecondsElapsed() << " мс\n";
    
    timer_.Restart();
    for(int i = 0; i < 3; ++i) 
    {
        ProcessData();
    }
    std::cout << "Всего после перезапуска: " << timer_.GetMillisecondsElapsed() << " мс\n";

    timer_.Stop();
    std::cout << "После остановки: " << timer_.GetMillisecondsElapsed() << " мс\n";
    
    return 0;
}
```
