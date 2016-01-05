cmake_minimum_required(VERSION 2.6)
project(SFML_GameEngine)

set(SFML_GameEngine_VERSION 1.1)

set(SFML_GameEngine_SOURCES
    engine/animatedSprite.cpp
    engine/connection.cpp
    engine/context.cpp
    engine/game.cpp
    engine/gui.cpp
    engine/musicPlayer.cpp
    engine/networkIdentifiers.cpp
    engine/resourceIdentifiers.cpp
    engine/resourceManager.cpp
    engine/sceneNode.cpp
    engine/soundPlayer.cpp
    engine/state.cpp
    engine/stateStack.cpp
    engine/utility.cpp
    engine/ec/engine.cpp
    engine/ec/entity.cpp
    engine/ec/nMove.cpp
    engine/ec/system.cpp
    )

add_library(SFML_GameEngine
    ${SFML_GameEngine_SOURCES})

if(BUILD_SHARED_LIBS OR (UNIX AND NOT CYGWIN))
    find_package(SFML 2 REQUIRED audio network graphics window system)
else()
    find_package(SFML 2 REQUIRED audio-s network-s graphics-s window-s system-s)
    add_definitions(-DSFML_STATIC)
endif()

include_directories(${CMAKE_SOURCE_DIR})

if(GAME_USE_GLFW)
    message(STATUS "Enabling GLFW")

    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})
    target_link_libraries(SFML_GameEngine ${GLFW_LIBRARIES})

    add_definitions(-DGAME_USE_GLFW)

    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIRS})
    target_link_libraries(SFML_GameEngine ${OPENGL_LIBRARIES})

    find_package(GLEW REQUIRED)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(SFML_GameEngine ${GLEW_LIBRARIES})
else()
    if(GAME_NO_RENDER_WINDOW)
        message(STATUS "Disabling sf::RenderWindow")
        add_definitions(-DGAME_NO_RENDER_WINDOW)
    else()
        message(STATUS "sf::RenderWindow is enabled")
    endif()
endif()

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(SFML_GameEngine ${SFML_LIBRARIES})
endif()

find_package(ResourcePacker 1 REQUIRED)
include_directories(${ResourcePacker_INCLUDE_DIR})
target_link_libraries(SFML_GameEngine ${ResourcePacker_LIBRARIES})

find_package(Rational 1 CONFIG QUIET)
if(Rational_FOUND)
    include_directories(${Rational_INCLUDE_DIR})
    target_link_libraries(SFML_GameEngine ${Rational_LIBRARIES})
    add_definitions(-DRational_FOUND)
else()
    message(STATUS "Rational not found, continuing without it.")
endif()

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -D NDEBUG")

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if(NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
elseif(MSVC)
else()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug', none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

install(TARGETS SFML_GameEngine EXPORT SFML_GameEngineTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include)

include(GenerateExportHeader)
generate_export_header(SFML_GameEngine)
set_property(TARGET SFML_GameEngine PROPERTY VERSION ${SFML_GameEngine_VERSION})
set_property(TARGET SFML_GameEngine PROPERTY SOVERSION 1)
set_property(TARGET SFML_GameEngine PROPERTY INTERFACE_SFML_GameEngine_MAJOR_VERSION 1)
set_property(TARGET SFML_GameEngine APPEND PROPERTY COMPATIBLE_INTERFACE_STRING SFML_GameEngine_MAJOR_VERSION)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/sfml_gameengine_export.h"
    DESTINATION
        include
    COMPONENT
        Devel
)

install(
    FILES
        engine/animatedSprite.hpp
        engine/connection.hpp
        engine/context.hpp
        engine/game.hpp
        engine/gui.hpp
        engine/musicPlayer.hpp
        engine/networkIdentifiers.hpp
        engine/resourceHolder.hpp
        engine/resourceHolder.inl
        engine/resourceIdentifiers.hpp
        engine/resourceManager.hpp
        engine/sceneNode.hpp
        engine/soundPlayer.hpp
        engine/state.hpp
        engine/stateStack.hpp
        engine/utility.hpp
    DESTINATION
        include/engine
    COMPONENT
        Devel
)

install(
    FILES
        engine/ec/cAcceleration.hpp
        engine/ec/component.hpp
        engine/ec/cPosition.hpp
        engine/ec/cVelocity.hpp
        engine/ec/engine.hpp
        engine/ec/entity.hpp
        engine/ec/nMove.hpp
        engine/ec/node.hpp
        engine/ec/system.hpp
    DESTINATION
        include/engine/ec
    COMPONENT
        Devel
)

if(WIN32)
    install(FILES "${SFML_ROOT}/bin/libsndfile-1.dll" DESTINATION SFML_GameEngine)
    install(FILES "${SFML_ROOT}/bin/openal32.dll" DESTINATION SFML_GameEngine)
    if(ResourcePacker_FOUND AND BUILD_SHARED_LIBS)
        install(FILES "${ResourcePacker_LIBRARY}" DESTINATION SFML_GameEngine)
    endif()
    if(CMAKE_BUILD_TYPE MATCHES "Debug" AND BUILD_SHARED_LIBS)
        install(FILES "${SFML_ROOT}/bin/sfml-audio-d-2.dll" DESTINATION SFML_GameEngine)
        install(FILES "${SFML_ROOT}/bin/sfml-graphics-d-2.dll" DESTINATION SFML_GameEngine)
        install(FILES "${SFML_ROOT}/bin/sfml-network-d-2.dll" DESTINATION SFML_GameEngine)
        install(FILES "${SFML_ROOT}/bin/sfml-system-d-2.dll" DESTINATION SFML_GameEngine)
        install(FILES "${SFML_ROOT}/bin/sfml-window-d-2.dll" DESTINATION SFML_GameEngine)
    elseif(BUILD_SHARED_LIBS)
        install(FILES "${SFML_ROOT}/bin/sfml-audio-2.dll" DESTINATION SFML_GameEngine)
        install(FILES "${SFML_ROOT}/bin/sfml-graphics-2.dll" DESTINATION SFML_GameEngine)
        install(FILES "${SFML_ROOT}/bin/sfml-network-2.dll" DESTINATION SFML_GameEngine)
        install(FILES "${SFML_ROOT}/bin/sfml-system-2.dll" DESTINATION SFML_GameEngine)
        install(FILES "${SFML_ROOT}/bin/sfml-window-2.dll" DESTINATION SFML_GameEngine)
    endif()
endif()


if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(NetworkingTest_SOURCES
        engine/connection.cpp
        engine/networkIdentifiers.cpp
        ../NetworkingTest/Main.cpp)

    add_executable(NetworkingTest
        ${NetworkingTest_SOURCES})

    target_link_libraries(NetworkingTest ${SFML_LIBRARIES})

    if(GAME_USE_GLFW)
        set(GLTest_SOURCES
            ../GLTest/Main.cpp
            ../GLTest/TestState.cpp)

        add_executable(GLTest
            ${GLTest_SOURCES})

        target_link_libraries(GLTest SFML_GameEngine)

        include_directories(${OPENGL_INCLUDE_DIRS})
        target_link_libraries(SFML_GameEngine ${OPENGL_LIBRARIES})

        include_directories(${GLEW_INCLUDE_DIRS})
        target_link_libraries(SFML_GameEngine ${GLEW_LIBRARIES})
    endif()
endif()


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SFML_GameEngine/SFML_GameEngineConfigVersion.cmake"
    VERSION ${SFML_GameEngine_VERSION}
    COMPATIBILITY AnyNewerVersion
)

export(EXPORT SFML_GameEngineTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/SFML_GameEngine/SFML_GameEngineTargets.cmake"
)

configure_file(cmake/SFML_GameEngineConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/SFML_GameEngine/SFML_GameEngineConfig.cmake"
    @ONLY
)

export(PACKAGE SFML_GameEngine)

set(ConfigPackageLocation lib/cmake/SFML_GameEngine)

install(EXPORT SFML_GameEngineTargets
    FILE
        SFML_GameEngineTargets.cmake
    DESTINATION
        ${ConfigPackageLocation}
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/SFML_GameEngine/SFML_GameEngineConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/SFML_GameEngine/SFML_GameEngineConfigVersion.cmake"
    DESTINATION
        ${ConfigPackageLocation}
    COMPONENT
        Devel
)

################################################################################
#   Begin Unit Test files
################################################################################

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    find_package(GTest REQUIRED)

    if(GTEST_FOUND)
        set(Unit_Tests_SOURCES
            ../UnitTests/TestEC.cpp
            ../UnitTests/TestUtility.cpp
            ../UnitTests/TestStateStack.cpp
            ../UnitTests/main.cpp)

        if(WIN32)
            add_executable(Unit_Tests WIN32
                ${Unit_Tests_SOURCES})
        else()
            add_executable(Unit_Tests
                ${Unit_Tests_SOURCES})
        endif()


        enable_testing()

        include_directories(${GTEST_INCLUDE_DIRS})
        target_link_libraries(Unit_Tests ${GTEST_BOTH_LIBRARIES})
        target_link_libraries(Unit_Tests ${SFML_LIBRARIES})
        target_link_libraries(Unit_Tests SFML_GameEngine)

        if(ResourcePacker_FOUND)
            target_link_libraries(Unit_Tests ${ResourcePacker_LIBRARIES})
            add_definitions(-DResourcePacker_FOUND)
        endif()

        if(Rational_FOUND)
            target_link_libraries(Unit_Tests ${Rational_LIBRARIES})
            add_definitions(-DRational_FOUND)
        endif()


        add_test(NAME UnitTests COMMAND Unit_Tests)

        file(COPY ../UnitTestRes DESTINATION ${CMAKE_BINARY_DIR})
    endif()
endif()
