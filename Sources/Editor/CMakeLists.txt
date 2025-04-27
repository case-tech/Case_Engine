<<<<<<< HEAD
=======
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

>>>>>>> aa4b252 (Add open project)
set(CheckSystem
    CheckSystem/CheckSystem.cpp
    CheckSystem/CheckSystem.hpp
)

set(SRC
    Editor.cpp
    Editor.hpp)

add_executable(Editor 
               ${SRC}
<<<<<<< HEAD
               ${CheckSystem})
=======
               ${CheckSystem}
               ${Managers}
               ${SandBox})
>>>>>>> aa4b252 (Add open project)

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

<<<<<<< HEAD
target_link_libraries(Editor
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/Linux/x64/Debug/libCUtils.a"
)
=======
if(LINUX)
target_link_libraries(Editor
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/Linux/x64/Debug/libCUtils.a"
)
elseif(WIN32)
target_link_libraries(Editor
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/Win/x64/Debug/CUtils.lib"
)
endif()
>>>>>>> aa4b252 (Add open project)

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
