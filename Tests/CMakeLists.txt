set(SRC
    Tests.cpp)

add_executable(Tests ${SRC})

target_link_libraries(Tests PRIVATE
    Aid
    Audio
    Graphics
    Physics
     "${CMAKE_SOURCE_DIR}/External/FMOD/Lib/Linux/x86_64/libfmod.so.14"
)

set_target_properties(Tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Tests"
)

target_include_directories(Tests
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Include"
    PRIVATE "${CMAKE_SOURCE_DIR}/Sources/Kernel"
    PRIVATE "${CMAKE_SOURCE_DIR}/External/FMOD/Include" 
)

target_link_libraries(Tests
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/Linux/x64/Debug/libCUtils.a"
)

