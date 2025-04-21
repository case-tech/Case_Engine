cmake_minimum_required(VERSION 4.0.0)
project(HLSL2C)

set(CMAKE_CXX_STANDARD 17) # Обязательно C++17
# Укажите абсолютные пути к вашей установке ANTLR C++ Runtime
set(ANTLR_INCLUDE_DIR "/home/sasa/antlr4/runtime/Cpp/runtime/src/")  # Путь к заголовочным файлам
set(ANTLR_LIB_DIR "/home/sasa/antlr4/runtime/Cpp/dist/")                        # Путь к библиотекам

# Добавляем директории с заголовками
include_directories(
    ${ANTLR_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Добавляем директорию с библиотеками
link_directories(
    ${ANTLR_LIB_DIR}
)

# Список исходных файлов вашего проекта
set(SOURCES
    test.cpp
    HLSLBaseListener.cpp
        HLSLBaseVisitor.cpp
    HLSLLexer.cpp
    HLSLListener.cpp
        HLSLVisitor.cpp
    HLSLParser.cpp
)

# Создание исполняемого файла
add_executable(HLSL2C ${SOURCES})

# Явное указание библиотек ANTLR
target_link_libraries(HLSL2C
    antlr4-runtime
    ${ANTLR_LIB_DIR}/libantlr4-runtime.a  # Или .so для динамической линковки
)
