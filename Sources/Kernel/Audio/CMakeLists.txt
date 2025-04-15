# Copyright (c) 2025 Case Technologies
# ----------Audio-Module---------

# Sources
set(Sound
    Sound/Sound.cpp
    Sound/Sound.hpp)

set(Geometry
    Geometry/Geometry.cpp
    Geometry/Geometry.hpp)

set(DSP
    DSP/DSPEffect.cpp
    DSP/DSPEffect.hpp)

set(ChannelGroup
    ChannelGroup/ChannelGroup.cpp
    ChannelGroup/ChannelGroup.hpp)

set(Sound
    Sound/Sound.cpp
    Sound/Sound.hpp)

set(Channel
    Channel/Channel.cpp
    Channel/Channel.hpp)

set(AudioSystem
    AudioSystem/AudioSystem.cpp
    AudioSystem/AudioSystem.hpp)

# Create library
add_library(Audio
            STATIC
            Audio.hpp
            ${AudioSystem}
            ${Channel}
	    ${ChannelGroup}
	    ${DSP}
	    ${Geometry}
            ${Sound})

# Out Paths
set_target_properties(Audio PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

# FMOD linking
target_include_directories(Audio
    PUBLIC "${CMAKE_SOURCE_DIR}/External/FMOD/Include"
)

if (WIN32)
target_link_libraries(Audio
    PUBLIC "${CMAKE_SOURCE_DIR}/External/FMOD/Lib/Win/x64/fmod_vc.lib"
)

elseif (Linux)
target_link_libraries(Audio
    PUBLIC "${CMAKE_SOURCE_DIR}/External/FMOD/Lib/Linux/x86_64/libfmod.so.14"
)
endif()
