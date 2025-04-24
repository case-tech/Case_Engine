set(CheckSystem
    CheckSystem/CheckSystem.cpp
    CheckSystem/CheckSystem.hpp
)

set(SRC
    Editor.cpp
    Editor.hpp)

add_executable(Editor 
               ${SRC}
               ${CheckSystem})

target_link_libraries(Editor PRIVATE
    Audio
    Aid
    Graphics
    Physics
)

target_include_directories(Editor
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Include"
    PRIVATE "${CMAKE_SOURCE_DIR}/Sources/Kernel"
)

target_link_libraries(Editor
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/Linux/x64/Debug/libCUtils.a"
)

if(LINUX) 
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/Resources/Icons/icon.xpm"
        "$<TARGET_FILE_DIR:Editor>/icon.xpm"
)
else()
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Editor>"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/Resources/Icons/icon.ico"
        "$<TARGET_FILE_DIR:Editor>/icon.ico"
    VERBATIM
)
endif()

set_target_properties(Editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Editor"
)
