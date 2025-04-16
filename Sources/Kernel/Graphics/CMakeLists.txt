set(Control
    Window/Control/Control.cpp
    Window/Control/Control.hpp)

set (Window
     Window/Window.cpp
     Window/Window.hpp)

set(SRC
    Graphics.cpp
    Graphics.hpp)

add_library(Graphics STATIC 
	   ${SRC}
	   ${Window}
	   ${Control}
)

if (UNIX AND NOT APPLE)
    execute_process(
        COMMAND bash -c "echo $XDG_SESSION_TYPE"
        OUTPUT_VARIABLE WINDOW_SYSTEM
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if (WINDOW_SYSTEM STREQUAL "x11")
        find_package(X11 REQUIRED)
        if (X11_FOUND)
            include_directories(${X11_INCLUDE_DIR})
             target_link_libraries(Graphics
            PUBLIC
            ${X11_LIBRARIES} 
            ${X11_Xpm_LIB}
        )
        endif()
    endif()
endif()

set_target_properties(Graphics PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
