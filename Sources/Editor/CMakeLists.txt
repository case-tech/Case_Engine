
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

if(LINUX) 
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/Resources/Icons/icon.xpm"
        "$<TARGET_FILE_DIR:Editor>/icon.xpm"
)
else()
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/Resources/Icons/icon.ico"
        "$<TARGET_FILE_DIR:Editor>/icon.ico"
)
endif()

set_target_properties(Editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Editor"
)
