
set(SRC
    Graphics.cpp
    Graphics.hpp)

add_library(Graphics STATIC ${SRC})

set_target_properties(Graphics PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
