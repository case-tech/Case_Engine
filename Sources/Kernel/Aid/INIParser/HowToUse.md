[English](#en) | [Русский](#ru)

---
# INIParser Class {#en}
Aid module component for parsing and manipulating INI files (wrapper of the INIParser class from the CUtils library).

## Features
- Load and save INI files
- Read and modify values by sections and keys
- Retrieve all sections or keys within a section
- Simple error handling via boolean return codes

## Basic API methods
#### Class INIParser
- ```INIParser(const std::string& path_to_file)``` - Constructor with path to the INI file.
- ```bool Load()``` - Load the INI file (returns true on success).
- ```bool Save()``` - Save changes back to the file (returns true on success).
- ```std::string GetValue(const std::string& section_a, const std::string& key_a) const``` - Get a value from the specified section and key. Returns empty string if not found.
- ```void SetValue(const std::string& section_a, const std::string& key_a, const std::string& value_a)``` - Set or update a value in the specified section and key.
- ```std::vector<std::string> GetSections() const``` - Retrieve a list of all sections in the INI file.
- ```std::map<std::string, std::string> GetKeys(const std::string& section) const``` - Retrieve all key-value pairs in a specified section.

## Example
#### cpp
```cpp
#include <Kernel/Aid/INIParser/INIParser.hpp>  

int main()  
{  
    CE_Kernel::Aid::INIParser::INIParser parser_("config.ini");  
    if (!parser_.Load())  
    {  
        std::cerr << "Failed to load INI file!\n";  
        return 1;  
    }  

    // Read a value  
    std::string username_ = parser_.GetValue("User", "Name");  
    std::cout << "Username: " << username_ << "\n";  

    // Modify a value  
    parser_.SetValue("User", "Name", "NewUser");  

    // List all sections  
    std::cout << "Sections:\n";  
    for (const auto& section : parser_.GetSections())  
    {  
        std::cout << "- " << section << "\n";  
    }  

    // Save changes  
    if (!parser_.Save())  
    {  
        std::cerr << "Failed to save changes!\n";  
    }  

    return 0;  
}  
```
---
# Класс INIParser {#ru}
Компонент модуля Aid для работы с INI-файлами (обёртка класса INIParser из библиотеки CUtils).

## Возможности
- Загрузка и сохранение INI-файлов
- Чтение и изменение значений по секциям и ключам
- Получение списка секций или ключей в секции
- Обработка ошибок через возвращаемые булевы значения

## Основные методы API
#### Класс INIParser
- ```INIParser(const std::string& path_to_file)``` - Конструктор с указанием пути к INI-файлу.
- ```bool Load()``` - Загрузить INI-файл (возвращает true при успехе).
- ```bool Save()``` - Сохранить изменения в файл (возвращает true при успехе).
- ```std::string GetValue(const std::string& section_a, const std::string& key_a) const``` - Получить значение по секции и ключу. Возвращает пустую строку, если не найдено.
- ```void SetValue(const std::string& section_a, const std::string& key_a, const std::string& value_a)``` - Установить или обновить значение в указанной секции.
- ```std::vector<std::string> GetSections() const``` - Получить список всех секций в файле.
- ```std::map<std::string, std::string> GetKeys(const std::string& section_a) const``` - Получить все пары ключ-значение в указанной секции.

## Пример использования
#### cpp
```cpp
#include <Kernel/Aid/INIParser/INIParser.hpp>  

int main()  
{  
    CE_Kernel::Aid::INIParser::INIParser parser_("config.ini");  
    if (!parser_.Load())  
    {  
        std::cerr << "Ошибка загрузки INI-файла!\n";  
        return 1;  
    }  

    // Чтение значения  
    std::string username_ = parser_.GetValue("User", "Name");  
    std::cout << "Имя пользователя: " << username_ << "\n";  

    // Изменение значения  
    parser_.SetValue("User", "Name", "НовыйПользователь");  

    // Список секций  
    std::cout << "Секции:\n";  
    for (const auto& section : parser_.GetSections())  
    {  
        std::cout << "- " << section << "\n";  
    }  

    // Сохранение изменений  
    if (!parser_.Save())  
    {  
        std::cerr << "Ошибка сохранения изменений!\n";  
    }  

    return 0;  
}  
```
