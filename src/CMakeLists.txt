cmake_minimum_required(VERSION 2.6)
project(Game_Project)

set(Game_Project_SOURCES
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

if(WIN32)
    add_executable(Game_Project WIN32
        ${Game_Project_SOURCES}
        main.cpp)
else()
    add_executable(Game_Project
        ${Game_Project_SOURCES}
        main.cpp)
endif()

if(BUILD_SHARED_LIBS OR (UNIX AND NOT CYGWIN))
    find_package(SFML 2 REQUIRED audio network graphics window system)
else()
    find_package(SFML 2 REQUIRED audio-s network-s graphics-s window-s system-s)
    add_definitions(-DSFML_STATIC)
endif()

include_directories(${CMAKE_SOURCE_DIR})

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(Game_Project ${SFML_LIBRARIES})
endif()

find_package(ResourcePacker 1 CONFIG QUIET)
if(ResourcePacker_FOUND)
    include_directories(${ResourcePacker_INCLUDE_DIR})
    target_link_libraries(Game_Project ${ResourcePacker_LIBRARIES})
    add_definitions(-DResourcePacker_FOUND)
else()
    message(STATUS "ResourcePacker not found, continuing without it.")
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

install(TARGETS Game_Project DESTINATION Game_Project)

if(WIN32)
    install(FILES "${SFML_ROOT}/bin/libsndfile-1.dll" DESTINATION Game_Project)
    install(FILES "${SFML_ROOT}/bin/openal32.dll" DESTINATION Game_Project)
    if(ResourcePacker_FOUND AND BUILD_SHARED_LIBS)
        install(FILES "${ResourcePacker_LIBRARY}" DESTINATION Game_Project)
    endif()
    if(CMAKE_BUILD_TYPE MATCHES "Debug" AND BUILD_SHARED_LIBS)
        install(FILES "${SFML_ROOT}/bin/sfml-audio-d-2.dll" DESTINATION Game_Project)
        install(FILES "${SFML_ROOT}/bin/sfml-graphics-d-2.dll" DESTINATION Game_Project)
        install(FILES "${SFML_ROOT}/bin/sfml-network-d-2.dll" DESTINATION Game_Project)
        install(FILES "${SFML_ROOT}/bin/sfml-system-d-2.dll" DESTINATION Game_Project)
        install(FILES "${SFML_ROOT}/bin/sfml-window-d-2.dll" DESTINATION Game_Project)
    elseif(BUILD_SHARED_LIBS)
        install(FILES "${SFML_ROOT}/bin/sfml-audio-2.dll" DESTINATION Game_Project)
        install(FILES "${SFML_ROOT}/bin/sfml-graphics-2.dll" DESTINATION Game_Project)
        install(FILES "${SFML_ROOT}/bin/sfml-network-2.dll" DESTINATION Game_Project)
        install(FILES "${SFML_ROOT}/bin/sfml-system-2.dll" DESTINATION Game_Project)
        install(FILES "${SFML_ROOT}/bin/sfml-window-2.dll" DESTINATION Game_Project)
    endif()
endif()


################################################################################
#   Begin Unit Test files
################################################################################

if(SFML_GameEngine_TEST)
    set(Unit_Tests_SOURCES
        ../UnitTests/TestEC.cpp
        ../UnitTests/TestUtility.cpp
        ../UnitTests/TestStateStack.cpp
        ../UnitTests/main.cpp)

    if(WIN32)
        add_executable(Unit_Tests WIN32
            ${Unit_Tests_SOURCES}
            ${Game_Project_SOURCES})
    else()
        add_executable(Unit_Tests
            ${Unit_Tests_SOURCES}
            ${Game_Project_SOURCES})
    endif()


    enable_testing()

    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    target_link_libraries(Unit_Tests ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(Unit_Tests ${SFML_LIBRARIES})


    add_test(NAME UnitTests COMMAND Unit_Tests)

    file(COPY ../UnitTestRes DESTINATION ${CMAKE_BINARY_DIR})
endif()
