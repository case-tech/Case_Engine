
set(SRC
    Tests.cpp)

add_executable(Tests ${SRC})

target_link_libraries(Tests PRIVATE
    Audio
    Graphics
    Physics
)

set_target_properties(Tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Tests"
)

add_test(NAME EngineTests COMMAND Tests)
