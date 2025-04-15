[English](#en) | [Русский](#ru)

---
# Random Utilities {#en}

Component for generating random numbers with various distributions (wrapper of the Random classe of the CUtils library).

## Features
- Uniform distribution (integers/doubles)
- Normal (Gaussian) distribution
- Binomial distribution
- Bernoulli trials
- Poisson distribution

## Key Methods
- ```int RandomInt(int min_a, int max_a)``` - [min_a, max_a] inclusive
- ```double RandomDouble(double min_a, double max_a)``` - [min_a, max_a)
- ```double NormalDouble(double mean_a, double stddev_a)``` - Normal distribution
- ```int BinomialInt(int t_a, double p_a)``` - t_a trials, p_a success probability
- ```bool BernoulliBool(double p_a)``` - Returns true with probability p_a
- ```int PoissonInt(double mean_a)``` - Poisson distribution

## Example
```cpp
#include <Kernel/Aid/Random/Random.hpp>

void GenerateData() 
{
    CE_Kernel::Aid::Random::Random random_;
    
    // Random user ID between 1000 and 9999
    int user_id_ = random_.RandomInt(1000, 9999);
    
    // Simulate 10 coin flips
    int heads_ = random_.BinomialInt(10, 0.5);
    
    // Generate sensor noise (μ=0, σ=0.1)
    double noise_ = random_.NormalDouble(0.0, 0.1);
    
    // 5% chance of emergency shutdown
    if (random_.BernoulliBool(0.05)) 
    {
        trigger_emergency_stop();
    }
}
```

---
# Генерация случайных чисел {#ru}

Компонент для работы со случайными числами и распределениями (является обёрткой класса Random библиотеки CUtils).

## Возможности
- Равномерное распределение (целые/дробные числа)
- Нормальное распределение
- Биномиальное распределение
- Испытания Бернулли
- Распределение Пуассона

## Основные методы
- ```int RandomInt(int min_a, int max_a)``` - Целое в [min_a, max_a]
- ```double RandomDouble(double min_a, double max_a)``` - Дробное в [min_a, max_a)
- ```double NormalDouble(double mean_a, double stddev_a)``` - Нормальное распределение
- ```int BinomialInt(int t_a, double p_a)``` - t_a испытаний, вероятность p_a
- ```bool BernoulliBool(double p_a)``` - Вероятность true = p_a
- ```int PoissonInt(double mean_a)``` - Распределение Пуассона

## Пример
```cpp
#include <Kernel/Aid/Random/Random.hpp>

void GenerateData() 
{
    CE_Kernel::Aid::Random::Random random_;
    
    // Случайная температура от -20.0 до 45.0
    double temp_ = random_.RandomDouble(-20.0, 45.0);
    
    // Симулировать 100 кликов (CTR 1.5%)
    int clicks_ = random_.BinomialInt(100, 0.015);
    
    // Генерация времени ожидания (λ=3.0)
    int delay_ms_ = random_.PoissonInt(3.0) * 100;
    
    // 30% шанс бонуса
    if (random_.BernoulliBool(0.3)) 
    {
        apply_bonus();
    }
}
```
