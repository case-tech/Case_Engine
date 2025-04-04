
set(SRC
    Editor.cpp
    Editor.hpp)

add_executable(Editor ${SRC})

target_link_libraries(Editor PRIVATE
    Audio
    Aux
    Graphics
    Physics
)

set_target_properties(Editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Editor"
)
