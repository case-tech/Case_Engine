set(SandBox
    SandBox/SandBox.cpp
    SandBox/SandBox.hpp)

set(Managers
    Managers/PRManager.cpp
    Managers/PRManager.hpp
    Managers/AssetsManager/AssetsManager.cpp
    Managers/AssetsManager/AssetsManager.hpp
    Managers/LogsManager/LogsManager.cpp
    Managers/LogsManager/LogsManager.hpp
    Managers/SettingsManager/SettingsManager.cpp
    Managers/SettingsManager/SettingsManager.hpp)

set(CheckSystem
    CheckSystem/CheckSystem.cpp
    CheckSystem/CheckSystem.hpp
)

set(SRC
    Editor.cpp
    Editor.hpp)

add_executable(Editor 
               ${SRC}
               ${CheckSystem}
               ${CheckSystem}
               ${Managers}
               ${SandBox})

# ------------------------------------------------------
# ------------------COMPILE-OPTIONS---------------------
# ------------------------------------------------------

target_compile_options(Aid PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/wd4099>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wno-unused-variable>)

set_target_properties(Aid PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

target_link_libraries(Editor PRIVATE
    Audio
    Aid
    Graphics
    Physics
)

target_include_directories(Editor
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Include"
	PRIVATE "${CMAKE_SOURCE_DIR}/External/GLEW/Include"
    PRIVATE "${CMAKE_SOURCE_DIR}/Sources/Kernel"
)

if(LINUX)
target_link_libraries(Editor
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/Linux/x64/Debug/libCUtils.a"
)
elseif(WIN32)
target_link_libraries(Editor
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/Win/x64/Debug/CUtils.lib"
	PRIVATE "${CMAKE_SOURCE_DIR}/External/GLEW/Lib/Win/Release/x64/glew32.lib"
    PRIVATE "${CMAKE_SOURCE_DIR}/External/GLEW/Lib/Win/Release/x64/glew32s.lib"
    opengl32.lib
)
endif()

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

add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Editor>"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/External/GLEW/Bin/Release/x64/glew32.dll"
        "$<TARGET_FILE_DIR:Editor>/glew32.dll"
    VERBATIM
)

add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Editor>"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/External/FMOD/Lib/Win/x64/fmod.dll"
        "$<TARGET_FILE_DIR:Editor>/glew32.dll"
    VERBATIM
)

endif()

set_target_properties(Editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Editor"
)