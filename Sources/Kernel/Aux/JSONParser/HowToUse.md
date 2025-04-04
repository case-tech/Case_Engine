[English](#en) | [Русский](#ru)

---
# JSON Parser {#en}

Aux module component for parsing .json files.

## Features 
- Full support for the JSON standard (ECMA-404) 
- Strict format validity check 
- Clear object-oriented API 
- Exceptions for error handling 
- Supported data types: 
    - Objects (key-value) 
    - Arrays 
    - Line 
    - Numbers (Integers/Fractions/Exponents) 
    - Boolean values 
    - Null

## Basic API methods
#### Class JSONParser
```static JSONValue* Parse(const string& json_a)``` - parsing from a string

```static JSONValue* ParseFromFile(const string& path_to_file_a)``` - parsing from a file

#### Class JSONObject
```JSONValue* Get(const string& key_a)``` - Get value by key

```bool Has(const string& key_a)``` - Checking the availability of the key

```void Add(string key_a, JSONValue* value_a)``` - Adding a key-value pair

#### Class JSONArray
```JSONValue* At(size_t index_a)``` - Accessing an item by index

```size_t Size()``` - Number of elements

```void Add(JSONValue* value_a)``` - Adding an Item

#### Types of Values
```JSONString::value()``` - get string

```JSONNumber::value()``` - get double

```JSONBool::value()``` - get bool

## Example
#### cpp
``` cpp
#include <Kernel/Aux/JSONParser/JSONParser.hpp>

int main()
{
    try
    {
        auto config_ = CE_Kernel::Aux::JSONParser::JSONParser::ParseFromFile("config.json");

        if (auto* root_ = dynamic_cast<CE_Kernel::Aux::JSONParser::JSONObject*>(config_.get()))
        {
            if (auto* timeout_ = root_->Get("timeout"))
            {
                if (timeout_->type() == CE_Kernel::Aux::JSONParser::JSONValue::Number)
                {
                    auto* num_ = dynamic_cast<CE_Kernel::Aux::JSONParser::JSONNumber*>(timeout_);
                    std::cout << "Timeout: " << num_->Value() << " seconds\n";
                }
            }

            if (auto* servers_ = root_->Get("servers"))
            {
                if (servers_->type() == CE_Kernel::Aux::JSONParser::JSONValue::Array)
                {
                    auto* arr_ = dynamic_cast<CE_Kernel::Aux::JSONParser::JSONArray*>(servers_);
                    std::cout << "Servers:\n";
                    for (size_t i = 0; i < arr_->Size(); ++i)
                    {
                        if (auto* server_ = arr_->At(i)) {
                            if (server_->type() == CE_Kernel::Aux::JSONParser::JSONValue::String)
                            {
                                auto* str_ = dynamic_cast<CE_Kernel::Aux::JSONParser::JSONString*>(server_);
                                std::cout << " - " << str_->Value() << "\n";
                            }
                        }
                    }
                }
            }
        }
    }
    catch (const CE_Kernel::Aux::JSONParser::JSONParseException& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

#### config.json
``` json
{
    "timeout": 15.5,
    "servers": [
        "api.example.com",
        "backup.example.com",
        "cdn.example.com"
    ]
}
```
---

# JSON Parser (Парсер JSON) {#ru}

Компонент модуля Aux для парсинга файлов .json.

## Возможности
- Полная поддержка стандарта JSON (ECMA-404)
- Строгая проверка корректности формата
- Четкий объектно-ориентированный API
- Использование исключений для обработки ошибок
- Поддерживаемые типы данных:
    - Объекты (ключ-значение)
    - Массивы
    - Строки
    - Числа (целые/дробные/экспоненциальная запись)
    - Логические значения
    - Null

## Основные методы API
#### Класс JSONParser
```static JSONValue* Parse(const string& json_a)``` - парсинг из строки

```static JSONValue* ParseFromFile(const string& path_to_file_a)``` - парсинг из файла

#### Класс JSONObject
```JSONValue* Get(const string& key_a)``` - Получить значение по ключу

```bool Has(const string& key_a)``` - Проверка наличия ключа

```void Add(string key_a, JSONValue* value_a)``` - Добавление пары ключ-значение

#### Класс JSONArray
```JSONValue* At(size_t index_a)``` - Доступ к элементу по индексу

```size_t Size()``` - Количество элементов

```void Add(JSONValue* value_a)``` - Добавление элемента

#### Типы значений
```JSONString::value()``` - получить строку

```JSONNumber::value()``` - получить число (double)

```JSONBool::value()``` - получить логическое значение (bool)

## Пример использования
#### cpp
``` cpp
#include <Kernel/Aux/JSONParser/JSONParser.hpp>

int main()
{
    try
    {
        auto config_ = CE_Kernel::Aux::JSONParser::JSONParser::ParseFromFile("config.json");

        if (auto* root_ = dynamic_cast<CE_Kernel::Aux::JSONParser::JSONObject*>(config_.get()))
        {
            if (auto* timeout_ = root_->Get("timeout"))
            {
                if (timeout_->type() == CE_Kernel::Aux::JSONParser::JSONValue::Number)
                {
                    auto* num_ = dynamic_cast<CE_Kernel::Aux::JSONParser::JSONNumber*>(timeout_);
                    std::cout << "Timeout: " << num_->Value() << " seconds\n";
                }
            }

            if (auto* servers_ = root_->Get("servers"))
            {
                if (servers_->type() == CE_Kernel::Aux::JSONParser::JSONValue::Array)
                {
                    auto* arr_ = dynamic_cast<CE_Kernel::Aux::JSONParser::JSONArray*>(servers_);
                    std::cout << "Servers:\n";
                    for (size_t i = 0; i < arr_->Size(); ++i)
                    {
                        if (auto* server_ = arr_->At(i)) {
                            if (server_->type() == CE_Kernel::Aux::JSONParser::JSONValue::String)
                            {
                                auto* str_ = dynamic_cast<CE_Kernel::Aux::JSONParser::JSONString*>(server_);
                                std::cout << " - " << str_->Value() << "\n";
                            }
                        }
                    }
                }
            }
        }
    }
    catch (const CE_Kernel::Aux::JSONParser::JSONParseException& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

#### config.json
``` json
{
    "timeout": 15.5,
    "servers": [
        "api.example.com",
        "backup.example.com",
        "cdn.example.com"
    ]
}
```
