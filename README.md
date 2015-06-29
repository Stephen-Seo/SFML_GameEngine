

# Compiling

## Building with CMake

CMake gui recommended. (point CMake gui to the src directory as the source directory.)
Otherwise:

To build project, make directory "build" and cd to it. Then use the commands:
`cmake ../src`
make

To setup Makefile with SFML-2 in a non-standard directory:
`cmake -D SFML_ROOT=${Directory} ../src`

To set install directory, use `-D CMAKE_INSTALL_PREFIX=${Directory}`

To find ResourcePacker if installed in non standard directory,
use `-D CMAKE_PREFIX_PATH=${pathToResourcePackerRoot}`

To add Eclipse project files, add the option:
`-G"Eclipse CDT4 - Unix Makefiles"`

To not use sf::RenderWindow and replace it with sf::Window (for direct calls to
OpenGL without the overhead of sf::RenderWindow), use
`-DGAME_NO_RENDER_WINDOW=True`

# Usage / Notes to be aware of

## State Management

This game engine uses a state stack system to switch between different game states.

A new state must inherit from the `State` class located at `src/engine/state.hpp`.

- Create a new state object that derives from `State`

This state must also be registered with the game engine.  

- Register the state with the game

Before the game runs, you must register states with the game via `Game`'s `registerState` public function.  
This can be done in your main function.  
If you have a `class SuperState : public State`, then in your main function, registering it would look like,  
`Game game;
game.registerState<SuperState>("SuperState");`

Note that you can give the state any name for the string parameter, just be sure to remember it by that name.

- Manipulating the state stack

Note that each state has protected functions requesting the stack to push a state, pop a state, or clear the stack.  
This is how you can switch between states during operation of a state.

Also, `State::update(sf::Time dt)` and `State::handleEvent(const sf::Event& event)` are meant to return booleans.  
True indicates that update should also be called further down the state stack. This will continue to happen until a state returns False after updating or until the end of the stack has been reached. The same goes for handleEvent.

However, please note that every state on the stack will be drawn.

## Resource Management

Resource management is simplified.

Simply use the resourceManager's `registerTexture`, `registerFont`, or `registerSound` functions.  
Note that a reference to resourceManager is available in `Context`.

If you need the resources already loaded in the constructor of your state, you can call the resourceManager's `loadResources` function within your constructor.

Important Note: Use Music if you intend to stream audio from a file, and Sound if you intend on loading the file entirely in memory first. If the ResourcePacker is being used, all audio must be loaded as SoundBuffers via the Sound enum values.

After listing the enum values for each resource needed, the resources must be registered with the ResourceManager.

- Register resources linking resource enum values to filenames via the ResourceManager, this is done in `src/game.cpp` in the function `Game::registerResources()`
    - For example, `resourceManager.registerTexture(Textures::BACKGROUND_0, "background_0.png");`

To unload unnecessary resources, simply pop a State off the stack. The StateStack and ResourceManager will work together to find out what resources are still necessary, and unload everything that isn't.

