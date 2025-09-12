cmake_minimum_required(VERSION 3.20)
project(GammaArray VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/libs)

# Find required packages
find_package(OpenGL REQUIRED)

# Set custom paths for our manually installed libraries
set(GLFW_ROOT ${CMAKE_SOURCE_DIR}/libs/glfw)
set(RTMIDI_ROOT ${CMAKE_SOURCE_DIR}/libs/rtmidi)
set(PORTAUDIO_ROOT ${CMAKE_SOURCE_DIR}/libs/portaudio)

# Find GLFW
find_library(GLFW_LIBRARY
    NAMES glfw3 glfw
    PATHS ${GLFW_ROOT}/lib-vc2022
    NO_DEFAULT_PATH
)

if(GLFW_LIBRARY)
    add_library(glfw STATIC IMPORTED)
    set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION ${GLFW_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${GLFW_ROOT}/include
    )
    message(STATUS "Found GLFW: ${GLFW_LIBRARY}")
else()
    message(FATAL_ERROR "GLFW library not found")
endif()

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
endif()

# Compiler-specific options
if(MSVC)
    # MSVC specific options
    add_compile_options(/W4)
    add_compile_options(/permissive-)
    # Disable specific warnings for third-party libraries
    add_compile_options(/wd4996) # Disable deprecated function warnings
else()
    # GCC/Clang options
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Third-party libraries setup
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)

# ImGui setup
set(IMGUI_DIR ${LIBS_DIR}/imgui)
if(EXISTS ${IMGUI_DIR})
    file(GLOB IMGUI_SOURCES 
        ${IMGUI_DIR}/*.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    )
    add_library(imgui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
    target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
    message(STATUS "Found ImGui: ${IMGUI_DIR}")
endif()

# RtMidi setup
find_library(RTMIDI_LIBRARY
    NAMES rtmidi
    PATHS ${RTMIDI_ROOT}/build/Release
    NO_DEFAULT_PATH
)

if(RTMIDI_LIBRARY)
    add_library(rtmidi STATIC IMPORTED)
    set_target_properties(rtmidi PROPERTIES
        IMPORTED_LOCATION ${RTMIDI_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${RTMIDI_ROOT}
    )
    message(STATUS "Found RtMidi: ${RTMIDI_LIBRARY}")
endif()

# PortAudio setup
find_library(PORTAUDIO_LIBRARY
    NAMES portaudio
    PATHS ${PORTAUDIO_ROOT}/build/Release
    NO_DEFAULT_PATH
)

if(PORTAUDIO_LIBRARY)
    add_library(portaudio STATIC IMPORTED)
    set_target_properties(portaudio PROPERTIES
        IMPORTED_LOCATION ${PORTAUDIO_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${PORTAUDIO_ROOT}/include
    )
    message(STATUS "Found PortAudio: ${PORTAUDIO_LIBRARY}")
endif()

# Collect source files
file(GLOB_RECURSE GAMMA_SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE GAMMA_HEADERS 
    "${CMAKE_SOURCE_DIR}/include/*.h"
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
)

# Create main executable
add_executable(gamma_array ${GAMMA_SOURCES} ${GAMMA_HEADERS})

# Link libraries
target_link_libraries(gamma_array
    OpenGL::GL
    glfw
)

# Conditionally link libraries if they exist
if(TARGET imgui)
    target_link_libraries(gamma_array imgui)
endif()

if(RTMIDI_LIBRARY)
    target_link_libraries(gamma_array rtmidi)
endif()

if(PORTAUDIO_LIBRARY)
    target_link_libraries(gamma_array portaudio)
endif()

# Windows-specific libraries
if(WIN32)
    target_link_libraries(gamma_array
        winmm
        ole32
        oleaut32
        imm32
        version
        setupapi
    )
endif()

# Set target properties
set_target_properties(gamma_array PROPERTIES
    OUTPUT_NAME "GammaArray"
    DEBUG_POSTFIX "_d"
)

# Installation
install(TARGETS gamma_array
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Copy resources if they exist
if(EXISTS ${CMAKE_SOURCE_DIR}/resources)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources
        DESTINATION .
    )
endif()

# Development helpers
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(gamma_array PRIVATE DEBUG_BUILD)
    # Enable debug symbols
    if(MSVC)
        target_compile_options(gamma_array PRIVATE /Zi)
        target_link_options(gamma_array PRIVATE /DEBUG)
    else()
        target_compile_options(gamma_array PRIVATE -g)
    endif()
endif()

# Print configuration summary
message(STATUS "=== Gamma Array Build Configuration ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "=====================================")