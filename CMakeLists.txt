cmake_minimum_required(VERSION 3.21)
project(DarkFantasyRPG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------
# raylib is fetched automatically so the project builds from a fresh
# checkout without requiring a system-wide install. If your machine
# already has raylib installed and you prefer that, replace this
# FetchContent block with find_package(raylib REQUIRED).
include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(raylib)

# ------------------------------------------------------------------
# Source files
# ------------------------------------------------------------------
set(GAME_SOURCES
    src/main.cpp

    src/core/Application.cpp

    src/input/InputSystem.cpp

    src/camera/GameCamera.cpp

    src/world/World.cpp

    src/entities/Player.cpp

    src/rendering/Renderer.cpp
)

add_executable(DarkFantasyRPG ${GAME_SOURCES})

target_include_directories(DarkFantasyRPG PRIVATE src)

target_link_libraries(DarkFantasyRPG PRIVATE raylib)

target_compile_features(DarkFantasyRPG PRIVATE cxx_std_20)

if (MSVC)
    target_compile_options(DarkFantasyRPG PRIVATE /W4)
    # Make Visual Studio use DarkFantasyRPG as the startup project
    # and give it a sensible working directory for asset loading.
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DarkFantasyRPG)
    set_target_properties(DarkFantasyRPG PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
else()
    target_compile_options(DarkFantasyRPG PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Group sources in the IDE so the generated Visual Studio filters
# mirror the actual folder structure on disk.
foreach(_source IN ITEMS ${GAME_SOURCES})
    get_filename_component(_source_dir "${_source}" DIRECTORY)
    string(REPLACE "src" "" _group "${_source_dir}")
    string(REPLACE "/" "\\" _group "${_group}")
    source_group("${_group}" FILES "${_source}")
endforeach()
