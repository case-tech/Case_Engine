
set(SRC
    Physics.cpp
    Physics.hpp)

add_library(Physics STATIC ${SRC})

set_target_properties(Physics PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
