set(LINKER_FLAGS "")
list(APPEND LINKER_FLAGS "-lsfml-graphics")
list(APPEND LINKER_FLAGS "-lsfml-system")
list(APPEND LINKER_FLAGS "-lsfml-window")
list(APPEND LINKER_FLAGS "-lsfml-audio")
list(APPEND LINKER_FLAGS "-lsfml-network")

cmake_minimum_required(VERSION 2.6)
project(2DPrimer)

find_package(SFML 2)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-D NDEBUG")

file(GLOB_RECURSE 2DPrimer_SOURCES "src/*.cpp")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug', none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

add_executable(2DPrimer ${2DPrimer_SOURCES})
target_link_libraries(2DPrimer ${LINKER_FLAGS})
