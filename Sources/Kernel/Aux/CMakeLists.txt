set(FPLParser
    FPLParser/FPLParser.cpp
    FPLParser/FPLParser.hpp)

set(Algorithms
    Algorithms/Algorithms.cpp
    Algorithms/Algorithms.hpp)

set(INIParser
    INIParser/INIParser.cpp
    INIParser/INIParser.hpp)

set(Random
    Random/Random.cpp
    Random/Random.hpp)

set(System
    System/DateTime/DateTime.cpp
    System/DateTime/DateTime.hpp
    System/Filesystem/Filesystem.cpp
    System/Filesystem/Filesystem.hpp
    System/InfoSys/InfoSys.cpp
    System/InfoSys/InfoSys.hpp
)

set(Timer
    Timer/Timer.cpp
    Timer/Timer.hpp
)

set(Math
    Math/Vector/Vector2.cpp
    Math/Vector/Vector2.hpp
    Math/Vector/Vector3.cpp
    Math/Vector/Vector3.hpp
    Math/Vector/Vector4.cpp
    Math/Vector/Vector4.hpp
    Math/Quaternion/Quaternion.cpp
    Math/Quaternion/Quaternion.hpp
    Math/Matrix/Matrix2.cpp
    Math/Matrix/Matrix2.hpp
    Math/Matrix/Matrix3.cpp
    Math/Matrix/Matrix3.hpp
    Math/Matrix/Matrix4.cpp
    Math/Matrix/Matrix4.hpp
)

set(JSONParser
    JSONParser/JSONParser.hpp
    JSONParser/JSONParser.cpp)

set(SRC
    Aux.cpp
    Aux.hpp)

add_library(
    Aux
    STATIC
    ${SRC}
    ${JSONParser}
    ${Math}
    ${Timer}
    ${System}
    ${Random}
    ${INIParser}
    ${Algorithms}
    ${FPLParser})

set_target_properties(Aux PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

target_include_directories(Aux
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Include"
)

target_link_libraries(Aux
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/Linux/x64/Debug/libCUtils.a"
)
