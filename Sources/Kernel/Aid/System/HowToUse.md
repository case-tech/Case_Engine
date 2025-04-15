[English](#en) | [Русский](#ru)

---
# System Utilities {#en}

Module components for system information, filesystem operations, and datetime handling 
(wrapper of the SystemConfiguration, Filesystem and DateTime classes of the CUtils library).

## DateTime Class
### Features
- Get current date/time in string format
- Format time using custom patterns
- Convert between `time_t` and string representations

### Key Methods
- ```std::string GetCurrentDateTime() const``` - Returns "YYYY-MM-DD HH:MM:SS"
- ```std::string GetCurrentDate() const``` - Returns "YYYY-MM-DD"
- ```std::string GetCurrentTime() const``` - Returns "HH:MM:SS"
- ```void SetTime(time_t time_a)``` - Set internal time
- ```std::string Format(const std::string& format_a) const``` - Custom format (e.g., "%d/%m/%Y")

### Example
#### cpp
```cpp
#include <Kernel/Aid/System/DateTime.hpp>

void PrintTime() 
{
    CE_Kernel::Aid::System::DateTime dt_;
    std::cout << "Current datetime: " << dt_.GetCurrentDateTime() << "\n";
    
    dt.SetTime(1718067600);  // 2024-06-11 12:00:00 UTC
    std::cout << "Formatted: " << dt_.Format("%A, %d %B %Y") << "\n";
    // Output: "Tuesday, 11 June 2024"
}
```

## Filesystem Class
### Features
- Create/delete directories and files
- File/directory existence checks
- Recursive file copying
- Directory listing

### Key Methods
- ```bool CreateDirectory(const std::string& path_a)```
- ```bool Remove(const std::string& path_a)```
- ```bool Exists(const std::string& path_a) const```
- ```std::vector<std::string> ListDirectory(const std::string& path_a) const```
- ```uintmax_t FileSize(const std::string& path_a) const;``` - Returns file size in bytes

### Example
```cpp
#include <Kernel/Aid/System/Filesystem.hpp>

void ManageFiles() 
{
    CE_Kernel::Aid::System::Filesystem fs_;
    
    if (!fs_.Exists("/tmp/logs")) 
    {
        fs_.CreateDirectory("/tmp/logs");
    }
    
    auto files_ = fs_.ListDirectory("/home/user/documents");
    std::cout << "Documents directory contains " << files_.size() << " items\n";
    
    if (fs_.IsFile("data.bin")) 
    {
        std::cout << "File size: " << fs_.FileSize("data.bin") << " bytes\n";
    }
}
```

## InfoSys Class
### Features
- Retrieve OS details
- Get hardware information (CPU, RAM, disk)

### Key Methods (All Static)
- ```static std::string GetOSName()``` - e.g., "Linux"
- ```static std::string GetOSVersion()``` - e.g., "5.15.0-101-generic"
- ```static std::string GetCPUInfo()``` - e.g., "AMD Ryzen 9 7900X"
- ```static std::string GetMemoryInfo()``` - e.g., "32GB DDR5"
- ```static std::string GetDiskInfo()``` - e.g., "1TB NVMe SSD"

### Example
```cpp
#include <Kernel/Aid/System/InfoSys.hpp>

void PrintSystemInfo() 
{
    std::cout << "OS: "      << CE_Kernel::Aid::System::InfoSys::GetOSName() 
              << "OSVersion" << CE_Kernel::Aid::System::InfoSys::GetOSVersion()  << "\n"
              << "CPU: "     << CE_Kernel::Aid::System::InfoSys::GetCPUInfo()    << "\n"
              << "Memory: "  << CE_Kernel::Aid::System::InfoSys::GetMemoryInfo() << "\n";
}
```

---
# Системные утилиты {#ru}
Компоненты для работы с системной информацией, файловой системой и временем
(являются обёрткой классов SystemConfiguration, Filesystem и DateTime библиотеки CUtils).

## Класс DateTime
### Возможности
- Получение текущей даты/времени в строковом формате
- Форматирование времени
- Конвертация между time_t и строковыми представлениями

### Основные методы
- ```std::string GetCurrentDateTime() const``` - Возвращает "ГГГГ-ММ-ДД ЧЧ:ММ:СС"
- ```std::string GetCurrentDate() const``` - Возвращает "ГГГГ-ММ-ДД"
- ```std::string GetCurrentTime() const``` - Возвращает "ЧЧ:ММ:СС"
- ```void SetTime(time_t time_a)``` - Установка времени
- ```std::string Format(const std::string& format_a) const``` - Пользовательский формат

### Пример
```cpp
#include <Kernel/Aid/System/DateTime.hpp>

void PrintTime() 
{
    CE_Kernel::Aid::System::DateTime dt_;
    std::cout << "Текущее время: " << dt_.GetCurrentDateTime() << "\n";
    
    dt_.SetTime(1718067600);  // 11 июня 2024 12:00:00 UTC
    std::cout << "Форматировано: " << dt_.Format("%d.%m.%Y %H:%M") << "\n";
    // Вывод: "11.06.2024 12:00"
}
```

## Класс Filesystem
### Возможности
- Создание/удаление файлов и директорий
- Проверка существования объектов
- Получение списка файлов

### Основные методы
- ```bool CreateDirectory(const std::string& path_a)``` - Создать директорию
- ```bool Remove(const std::string& path_a)``` - Удалить объект
- ```bool Exists(const std::string& path_a) const``` - Проверить существование
- ```std::vector<std::string> ListDirectory(const std::string& path_a) const``` - Список файлов

### Пример
```cpp
#include <Kernel/Aid/System/Filesystem.hpp>

void ManageFiles() 
{
    CE_Kernel::Aid::System::Filesystem fs_;
    
    if (!fs_.Exists("/tmp/logs")) 
    {
        fs_.CreateDirectory("/tmp/logs");
    }
    
    auto files_ = fs_.ListDirectory("/home/user/documents");
    std::cout << "В директории documents: " << files_.size() << " объектов\n";
}
```

## Класс InfoSys
### Возможности
- Получение информации об ОС
- Данные об аппаратном обеспечении

### Основные методы (все статические)
- ```static std::string GetOSName()``` - Пример: "Linux"
- ```static std::string GetOSVersion()``` - Пример: "5.15.0-101-generic"
- ```static std::string GetCPUInfo()``` - Пример: "AMD Ryzen 9 7900X"
- ```static std::string GetMemoryInfo()``` - Пример: "32GB DDR5"

### Пример
```cpp
#include <Kernel/Aid/System/InfoSys.hpp>

void PrintSystemInfo() 
{
    std::cout << "ОС: "        << CE_Kernel::Aid::System::InfoSys::GetOSName() 
              << "OSVersion"   << CE_Kernel::Aid::System::InfoSys::GetOSVersion() << "\n"
              << "Процессор: " << CE_Kernel::Aid::System::InfoSys::GetCPUInfo()   << "\n";
}
```
