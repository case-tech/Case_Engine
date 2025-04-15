[English](#en) | [Русский](#ru)

---
# Math Library {#en}

Core mathematical classes for 2D/3D transformations, vectors, matrices, and quaternions.

## Features 
- Matrix operations (addition, multiplication, transpose, determinant, inverse)
- Vector operations (dot product, cross product, normalization, interpolation)
- Quaternion rotations and conversions
- Transformations: 
    - Scaling 
    - Translation 
    - Rotation (Euler angles, axis-angle, look-at)
- Support for 2x2, 3x3, and 4x4 matrices
- Type-safe operator overloads (+, -, *, /)
- Static factory methods for common transformations

## Basic API Overview
### Class `Matrix4`
- Constructors: 
  - From float array, individual elements, `Matrix3`, or `Quaternion`
- Operators: `+`, `-`, `*` (matrix-matrix, matrix-vector, scalar multiplication)
- `Transposed()`, `Transpose()`: Transpose operations
- `Determinant()`, `Inverse()`: Matrix inversion
- Static methods: 
  - `Scaling()`, `Translation()`, `AngleAxis()`, `Euler()`, `LookRotation()`

### Class `Vector3`
- Operators: `+`, `-`, `*` (scalar), `/`, `+=`, `-=`
- `Normalized()`, `Normalize()`: Unit vector operations
- `SqrMagnitude()`, `Magnitude()`: Length calculations
- Static utilities: 
  - `Dot()`, `Cross()`, `Lerp()`, `Angle()`

### Class `Quaternion`
- Constructors: From components or axis-angle
- Operators: `*` (quaternion-quaternion, quaternion-vector)
- `Conjugate()`, `Inverse()`: Rotation utilities
- Static methods: 
  - `AngleAxis()`, `Euler()`, `LookRotation()`

### Class `Matrix3`
- Similar functionality to `Matrix4` for 3x3 matrices
- Supports conversions to/from rotation quaternions

### Class `Matrix2`
- 2x2 matrix operations including scaling and rotation

## Example
#### cpp
```cpp
#include <Kernel/Aid/Math/Matrix4.hpp>
#include <Kernel/Aid/Math/Vector3.hpp>
#include <iostream>

using namespace CE_Kernel::Aid::Math;

int main() 
{
    // Create a translation matrix for position (2, 3, 4)
    Matrix4 translation_ = Matrix4::Translation(Vector3(2, 3, 4));
    
    // Create a scaling matrix
    Matrix4 scaling_ = Matrix4::Scaling(Vector3(2.0f, 2.0f, 2.0f));
    
    // Combine transformations: scale first, then translate
    Matrix4 transform_ = translation_ * scaling_;
    
    // Apply transformation to a point
    Vector4 point_(1, 1, 1, 1);
    Vector4 transformed_point_ = transform_ * point_;
    
    std::cout << "Transformed point: " << transformed_point_ << std::endl;
    
    return 0;
}
```
---

# Математическая библиотека {#ru}
Базовые классы для 2D/3D преобразований, работы с векторами, матрицами и кватернионами.

## Возможности
- Операции с матрицами (сложение, умножение, транспонирование, определитель, инверсия)
- Операции с векторами (скалярное/векторное произведение, нормализация, интерполяция)
- Кватернионы для вращений и преобразований
- Преобразования:
    - Масштабирование
    - Перемещение
    - Вращение (углы Эйлера, ось-угол, направление взгляда)
- Поддержка матриц 2x2, 3x3 и 4x4
- Перегруженные операторы (+, -, *, /) с проверкой типов
- Статические методы для стандартных преобразований

## Основные методы API
### Класс `Matrix4`
- Конструкторы:
    - Из массива float, отдельных элементов, `Matrix3` или `Quaternion`
- Операторы: `+`, `-`, `*` (матрица-матрица, матрица-вектор, умножение на скаляр)
- `Transposed()`, `Transpose()`: Транспонирование
- `Determinant()`, `Inverse()`: Инверсия матрицы
- Статические методы:
    - `Scaling()`, `Translation()`, `AngleAxis()`, `Euler()`, `LookRotation()`

### Класс `Vector3`
- Операторы: `+`, `-`, `*` (на скаляр), `/`, `+=`, `-=`
- `Normalized()`, `Normalize()`: Нормализация вектора
- `SqrMagnitude()`, `Magnitude()`: Вычисление длины

- Статические методы:
    - `Dot()`, `Cross()`, `Lerp()`, `Angle()`

### Класс `Quaternion`
- Конструкторы: Из компонентов или оси-угла
- Операторы: `*` (кватернион-кватернион, кватернион-вектор)
- `Conjugate()`, `Inverse()`: Утилиты для вращений
- Статические методы:
    - `AngleAxis()`, `Euler()`, `LookRotation()`
    
### Класс `Matrix3`
- Функционал аналогичен `Matrix4` для матриц 3x3
- Поддержка преобразований в кватернионы

### Класс `Matrix2`
- Операции с матрицами 2x2, включая масштабирование и вращение

### Пример использования
#### cpp
```cpp
#include <Kernel/Aid/Math/Matrix4.hpp>
#include <Kernel/Aid/Math/Vector3.hpp>
#include <iostream>

using namespace CE_Kernel::Aid::Math;

int main() 
{
    // Создание матрицы перемещения в позицию (2, 3, 4)
    Matrix4 translation_ = Matrix4::Translation(Vector3(2, 3, 4));
    
    // Создание матрицы масштабирования
    Matrix4 scaling_ = Matrix4::Scaling(Vector3(2.0f, 2.0f, 2.0f));
    
    // Комбинирование преобразований: сначала масштаб, затем перемещение
    Matrix4 transform_ = translation_ * scaling_;
    
    // Применение преобразования к точке
    Vector4 point_(1, 1, 1, 1);
    Vector4 transformed_point_ = transform_ * point_;
    
    std::cout << "Преобразованная точка: " << transformed_point_ << std::endl;
    
    return 0;
}
```
