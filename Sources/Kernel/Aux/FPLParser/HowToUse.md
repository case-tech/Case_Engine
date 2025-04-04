[English](#en) | [Русский](#ru)

---
# FPL Parser {#en}
Aux module component for parsing FPL (Fast Parsing Language) configuration files.

## Features
- Custom syntax with ```@section``` declarations
- Strict format validation
- Simple hierarchical structure
- Supported data types:
    - Strings
    - Numbers (integers/floats)
    - Booleans
    - Arrays
    - Nested sections
- Error handling via exceptions

## Syntax Rules
- Sections:
    ```
    @section_name 
    {
        key: value  
        nested: @subsection { ... }
    }
    ```
- Key-Value Pairs:
    - key: "string"
    - key: 42
    - key: true
    - key: [1, "two", false]
- Data Types:
    - Strings: "text" or 'text'
    - Numbers: 123, 3.14, -5
    - Booleans: true, false
    - Arrays: [value1, value2]
    - Null: null
- Comments:
    ```
    # This is a comment  
    @server 
    {  
        port: 8080  # Inline comment  
    }  
    ```

## Basic API
#### Class FPLParser
- ```std::map<std::string, std::map<std::string, Value>> Parse(std::istream& input_a)``` - Parses FPL content from a stream. 
- Returns a nested map:
    - Outer map key: Section name (e.g., "database")
    - Inner map: Key-value pairs within the section
- Structure Value
    - Access data via std::variant:
        - ```std::get<std::string>(value.data_)``` - For strings
        - ```std::get<double>(value.data_)``` - For numbers
        - ```std::get<bool>(value.data_)``` - For booleans
        - ```std::get<std::vector<Value>>(value.data_)``` - For arrays
        
## Example
#### cpp
```cpp
#include <Kernel/Aux/FPLParser/FPLParser.hpp>  

int main() 
{  
    try 
    {  
        std::string input_ = R"(  
            @database 
            {  
                name: "my_db"  
                version: 2.5  
                tables: ["users", "products"]  
                backup_enabled: true  
            }  
        )";  

        std::istringstream stream_(input_);  
        CE_Kernel::Aux::FPLParser::FPLParser parser_;  
        auto config_ = parser_.Parse(stream_);  

        if (config_.find("database") != config_.end()) 
        {  
            auto& db_ = config_["database"];  
            std::cout << "Database name: "  
                      << std::get<std::string>(db_["name"].data_) << "\n";  
        }  
    } 
    catch (const std::exception& e) 
    {  
        std::cerr << "Error: " << e.what() << std::endl;  
        return 1;  
    }  
    
    return 0;  
}  
```

---

# Парсер FPL {#ru}
Компонент модуля Aux для парсинга файлов в формате FPL (Fast Parsing Language).

## Возможности
- Кастомный синтаксис с объявлениями ```@section```
- Проверка корректности формата
- Иерархическая структура
- Поддерживаемые типы данных:
    - Строки
    - Числа (целые/дробные)
    - Логические значения
    - Массивы
    - Вложенные секции
- Обработка ошибок через исключения

## Синтаксис
- Секции:
    ```
    @имя_секции 
    {  
        ключ: значение  
        вложенная: @подсекция { ... }  
    }  
    ```
- Пары ключ-значение:
    - ключ: "строка" 
    - ключ: 42 
    - ключ: true 
    - ключ: [1, "два", false]
- Типы данных:
    - Строки: "текст" или 'текст'
    - Числа: 123, 3.14, -5
    - Булевы значения: true, false
    - Массивы: [значение1, значение2]
    - Null: null
- Комментарии:
    ```
    # Это комментарий  
    @server 
    {  
        port: 8080  # Порт сервера  
    }  
    ```
    
## Основные методы
#### Класс FPLParser
- ```std::map<std::string, std::map<std::string, Value>> Parse(std::istream& input_a)``` - Парсит FPL-контент из потока. 
- Возвращает вложенную карту:
    - Ключ внешней карты: Имя секции (например, "database")
    - Внутренняя карта: Пары ключ-значение внутри секции
- Структура Value
- Доступ к данным через std::variant:
    - ```std::get<std::string>(value.data_)``` - Для строк 
    - ```std::get<double>(value.data_)``` - Для чисел 
    - ```std::get<bool>(value.data_)``` - Для булевых значений 
    - ```std::get<std::vector<Value>>(value.data_)``` - Для массивов
    
## Пример
#### cpp
```cpp
#include <Kernel/Aux/FPLParser/FPLParser.hpp>  

int main() 
{  
    try 
    {  
        std::string input_ = R"(  
            @database 
            {  
                name: "my_db"  
                version: 2.5  
                tables: ["users", "products"]  
                backup_enabled: true  
            }  
        )";  

        std::istringstream stream_(input_);  
        CE_Kernel::Aux::FPLParser::FPLParser parser_;  
        auto config_ = parser_.Parse(stream_);  

        if (config_.find("database") != config_.end()) 
        {  
            auto& db_ = config_["database"];  
            std::cout << "Имя базы: "  
                      << std::get<std::string>(db["name"].data_) << "\n";  
        }  
    } 
    catch (const std::exception& e) 
    {  
        std::cerr << "Ошибка: " << e.what() << std::endl;  
        return 1;  
    }
    
    return 0;  
}  
```
