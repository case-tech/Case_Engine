set(UI
	SandBox/UI/Az/Az.cpp
	SandBox/UI/Az/az_impl_dx11.cpp
	SandBox/UI/Az/az_impl_opengl3.cpp
	SandBox/UI/Az/az_impl_win32.cpp
	SandBox/UI/Az/AzCurveEdit.cpp
	SandBox/UI/Az/AzDraw.cpp
	SandBox/UI/Az/AzGradient.cpp
	SandBox/UI/Az/AzGuizmo.cpp
	SandBox/UI/Az/AzSequencer.cpp
	SandBox/UI/Az/AzTables.cpp
	SandBox/UI/Az/AzWidgets.cpp
	SandBox/UI/Az/AzGraphEditor.cpp)

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

source_group("CheckSystem" FILES ${CheckSystem})
source_group("Managers"    FILES ${Managers})
source_group("SandBox"     FILES ${SandBox})
source_group("SandBox/UI"  FILES ${UI})
source_group(" " 		   FILES ${SRC})

add_executable(Editor 
               ${SRC}
               ${CheckSystem}
               ${Managers}
			   ${UI}
               ${SandBox})

# ------------------------------------------------------
# ------------------COMPILE-OPTIONS---------------------
# ------------------------------------------------------

target_compile_options(Editor PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/wd4099>
    $<$<CXX_COMPILER_ID:MSVC>:/wd4311>
	$<$<CXX_COMPILER_ID:MSVC>:/wd4302>
	$<$<CXX_COMPILER_ID:MSVC>:/wd4245>
	$<$<CXX_COMPILER_ID:MSVC>:/wd4189>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wno-unused-variable>)


target_link_libraries(Editor PRIVATE
    Audio
    Aid
    Graphics
    Physics
)

target_include_directories(Editor
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Include"
	PRIVATE "${CMAKE_SOURCE_DIR}/External/GLEW/Include"
	PRIVATE "${CMAKE_SOURCE_DIR}/External/Azurite/Include"
    PRIVATE "${CMAKE_SOURCE_DIR}/Sources/Kernel"
)

if(LINUX)
target_link_libraries(Editor
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/Linux/x64/Debug/libCUtils.a"
)
elseif(WIN32)
target_link_libraries(Editor
    PRIVATE "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/Win/x64/Release/CUtils.lib"
	PRIVATE "${CMAKE_SOURCE_DIR}/External/GLEW/Lib/Win/Release/x64/glew32.lib"
    PRIVATE "${CMAKE_SOURCE_DIR}/External/GLEW/Lib/Win/Release/x64/glew32s.lib"
    opengl32.lib
	dwmapi.lib
	d3d11.lib
	DXGI
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
        "$<TARGET_FILE_DIR:Editor>/fmod.dll"
    VERBATIM
)

endif()

set_target_properties(Editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTAzE_OUTPUT_DIRECTORY}/Editor"
)