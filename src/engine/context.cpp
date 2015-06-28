
#include "context.hpp"

Context::Context(sf::Window& window, ResourceManager& resourceManager, MusicPlayer& mPlayer, SoundPlayer& sPlayer, Engine& ecEngine, bool& isQuitting, std::unique_ptr<Connection>& connection, sf::Color& clearColor) :
window(&window),
renderWindow(dynamic_cast<sf::RenderWindow*>(&window)),
resourceManager(&resourceManager),
mPlayer(&mPlayer),
sPlayer(&sPlayer),
ecEngine(&ecEngine),
isQuitting(&isQuitting),
connection(&connection),
clearColor(&clearColor)
{}
