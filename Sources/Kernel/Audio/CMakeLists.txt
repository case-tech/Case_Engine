# Copyright (c) 2025 Case Technologies
# Audio Case Engine Core Module

# ------------------------------------------------------
# -----------------------SOURCES------------------------
# ------------------------------------------------------

set(AudioSystem
    AudioSystem/AudioSystem.cpp
    AudioSystem/AudioSystem.hpp)

set(Channel
    Channel/Channel.cpp
    Channel/Channel.hpp)

set(ChannelGroup
    ChannelGroup/ChannelGroup.cpp
    ChannelGroup/ChannelGroup.hpp)

set(DSP
    DSP/DSPEffect.cpp
    DSP/DSPEffect.hpp)

set(Geometry
    Geometry/Geometry.cpp
    Geometry/Geometry.hpp)

set(Sound
    Sound/Sound.cpp
    Sound/Sound.hpp)

# ------------------------------------------------------
# -----------------------GROUPS-------------------------
# ------------------------------------------------------

source_group("AudioSystem"  FILES ${AudioSystem} )
source_group("Channel" 		FILES ${Channel}     )
source_group("ChannelGroup" FILES ${ChannelGroup})
source_group("DSP" 			FILES ${DSP} 		 )
source_group("Geometry" 	FILES ${Geometry}	 )
source_group("Sound" 		FILES ${Sound}		 )

# ------------------------------------------------------
# -------------------CREATE-MODULE----------------------
# ------------------------------------------------------

add_library(Audio
            STATIC
            ${AudioSystem}
            ${Channel}
			${ChannelGroup}
			${DSP}
			${Geometry}
            ${Sound})

# ------------------------------------------------------
# ------------------COMPILE-OPTIONS---------------------
# ------------------------------------------------------

set_target_properties(Audio PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

# ------------------------------------------------------
# ---------------- CONNECTING LIBRARIES -----------------
# ------------------------------------------------------

target_include_directories(Audio PRIVATE
    "${CMAKE_SOURCE_DIR}/External/FMOD/Include"
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_DIR "x64")
else()
    set(ARCH_DIR "x86")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_DIR)
if(NOT BUILD_TYPE_DIR MATCHES "^(debug|release)$")
    set(BUILD_TYPE_DIR "release")
endif()

if(WIN32)
    set(OS_DIR "Win")
    set(FMOD_LIB "fmod_vc.lib")
	set(FMODL_LIB "fmodl_vc.lib")
    
    set(FMOD_PATH "${CMAKE_SOURCE_DIR}/External/FMOD/Lib/${OS_DIR}/${ARCH_DIR}/${FMOD_LIB}")
	set(FMODL_PATH "${CMAKE_SOURCE_DIR}/External/FMOD/Lib/${OS_DIR}/${ARCH_DIR}/${FMODL_LIB}")

elseif(LINUX)
    set(OS_DIR "Linux")
    set(FMOD_LIB "libfmod.a")
    set(FMODL_LIB "libfmodL.a")
    
    set(FMOD_PATH "${CMAKE_SOURCE_DIR}/External/FMOD/Lib/${OS_DIR}/${ARCH_DIR}/${FMOD_LIB}")
    set(FMODL_PATH "${CMAKE_SOURCE_DIR}/External/FMOD/Lib/${OS_DIR}/${ARCH_DIR}/${FMODL_LIB}")

elseif(APPLE)
    set(OS_DIR "Mac")
    set(FMOD_LIB "libfmod.a")
    set(FMODL_LIB "libfmodL.a")
    
    set(FMOD_PATH "${CMAKE_SOURCE_DIR}/External/FMOD/Lib/${OS_DIR}/${ARCH_DIR}/${FMOD_LIB}")
    set(FMODL_PATH "${CMAKE_SOURCE_DIR}/External/FMOD/Lib/${OS_DIR}/${ARCH_DIR}/${FMODL_LIB}")
endif()

target_link_libraries(Audio PRIVATE
    ${FMOD_PATH}
    ${FMODL_PATH})