
find_library(SFML_GameEngine_LIBRARY SFML_GameEngine
    PATHS "@CMAKE_INSTALL_PREFIX@/lib"
    NO_DEFAULT_PATH
)
find_library(SFML_GameEngine_LIBRARIES SFML_GameEngine
    PATHS "@CMAKE_INSTALL_PREFIX@/lib"
    NO_DEFAULT_PATH
)

find_path(SFML_GameEngine_INCLUDE_DIR engine/game.hpp
    PATHS "@CMAKE_INSTALL_PREFIX@/include"
    NO_DEFAULT_PATH
)
find_path(SFML_GameEngine_INCLUDE_DIRS engine/game.hpp
    PATHS "@CMAKE_INSTALL_PREFIX@/include"
    NO_DEFAULT_PATH
)
