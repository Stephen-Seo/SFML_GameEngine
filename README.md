

# Compiling

## Building with CMake

CMake gui recommended. (point CMake gui to the src directory as the source directory.)
Otherwise:

To build project, make directory "build" and cd to it. Then use the commands:
cmake ../src
make

To setup Makefile with SFML-2 in a non-standard directory:
cmake -D SFML\_ROOT=${Directory} ../src

To set install directory, use -D CMAKE\_INSTALL\_PREFIX=${Directory}

To find ResourcePacker if installed in non standard directory,
use -D CMAKE\_PREFIX\_PATH=${pathToResourcePackerRoot}

To add Eclipse project files, add the option:
-G"Eclipse CDT4 - Unix Makefiles"

# Usage / Notes to be aware of

## State Management

This game engine uses a state stack system to switch between different game states.

A new state must inherit from the `State` class located at `src/state.hpp`.

- Create a new state object that derives from `State`

This state must also be registered with the game engine.  
A new enum value must be added in `src/stateIdentifiers.hpp`, then the new state class must be registered to link it to that enum value.

- Add an enum value representing the recently created State in `src/stateIdentifiers.hpp`
- Register the state to this value in `src/game.cpp` in function `Game::registerStates()`
    - For example, `stateStack.registerState<NewState>(States::NEW_STATE);` (assuming the created state is called `NewState` and its corresponding enum value is `NEW_STATE`)

To start the game with a state, push a state to the state stack during initialization.

- Push a state to the state stack in the end of `Game::registerStates()`
    - For example, `stateStack.pushState(States::NEW_STATE);`

Note that each state has protected functions requesting the stack to push a state, pop a state, or clear the stack.  
This is how you can switch between states during operation of a state.

Also, `State::update(sf::Time dt)` and `State::handleEvent(const sf::Event& event)` are meant to return booleans.  
True indicates that update should also be called further down the state stack. This will continue to happen until a state returns False after updating or until the end of the stack has been reached. The same goes for handleEvent.

However, please note that every state on the stack will be drawn.

## Resource Management

Each resource must have a corresponding enum value to be registered with.

- The enum values should be added in `src/resourceIdentifiers.hpp`
    - The `Textures` enum values are for SFML2 supported image files
    - The `Fonts` enum values are for SFML2 supported font files
    - The `Music` enum values are for SFML2 supported audio files
    - The `Sound` enum values are for SFML2 supported audio files

Important Note: Use Music if you intend to stream audio from a file, and Sound if you intend on loading the file entirely in memory first. If the ResourcePacker is being used, all audio must be loaded as SoundBuffers via the Sound enum values.

After listing the enum values for each resource needed, the resources must be registered with the ResourceManager.

- Register resources linking resource enum values to filenames via the ResourceManager, this is done in `src/game.cpp` in the function `Game::registerResources()`
    - For example, `resourceManager.registerTexture(Textures::BACKGROUND_0, "background_0.png");`

Finally, a State must list what resources it requires and request these resources from the Resource Manager. A State cannot use a resource if it is not loaded in memory (unless it is a Music resource that is to be streamed from file).

A State has protected variables `TextureSet tset`, `FontSet fset`, and `SoundSet sset`. Each of these sets holds the corresponding enum value. TextureSet holds `Textures::ID` values from `src/resourceIdentifiers.hpp`.  
After setting these resource identifier holders, they will be sent to the resourceManager to load the required resources when requesting that they be loaded.

- Add the required resources by their identifiers to `tset`, `fset`, and `sset`
- Request these resources to be loaded from the Resource Manager
    - For example, `getContext().resourceManager->loadResources(getNeededResources());` (from the constructor of a State object)

To unload unnecessary resources, a single call to Resource Manager will cause it to query all states in the state stack to unload anything that it deems unnecessary (based on what each State has placed in their resource identifier sets)

- Unload necesssary resources with `getContext().resourceManager->unloadCheckResources();`


