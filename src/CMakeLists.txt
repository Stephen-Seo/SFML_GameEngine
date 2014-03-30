cmake_minimum_required(VERSION 2.6)
project(2DPrimer)

file(GLOB_RECURSE 2DPrimer_SOURCES "*.cpp")
add_executable(2DPrimerMain ${2DPrimer_SOURCES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(2DPrimerMain ${SFML_LIBRARIES})
endif()

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-D NDEBUG")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug', none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()
