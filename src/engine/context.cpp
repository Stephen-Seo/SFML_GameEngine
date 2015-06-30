
#include "context.hpp"

Context::Context(
#ifdef GAME_USE_GLFW
GLFWwindow* window,
#else
sf::Window* window,
#endif
ResourceManager& resourceManager, MusicPlayer& mPlayer, SoundPlayer& sPlayer, Engine& ecEngine, bool& isQuitting, std::unique_ptr<Connection>& connection, sf::Color& clearColor) :
window(window),
resourceManager(&resourceManager),
mPlayer(&mPlayer),
sPlayer(&sPlayer),
ecEngine(&ecEngine),
isQuitting(&isQuitting),
connection(&connection),
clearColor(&clearColor)
{}
