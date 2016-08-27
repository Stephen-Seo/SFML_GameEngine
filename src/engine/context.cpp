
#include "context.hpp"

Context::Context(
sf::RenderWindow& window,
ResourceManager& resourceManager,
MusicPlayer& mPlayer,
SoundPlayer& sPlayer,
bool& isQuitting,
std::unique_ptr<Connection>& connection,
sf::Color& clearColor,
std::mt19937& randomEngine,
void* extraContext) :
window(&window),
resourceManager(&resourceManager),
mPlayer(&mPlayer),
sPlayer(&sPlayer),
isQuitting(&isQuitting),
connection(&connection),
clearColor(&clearColor),
randomEngine(&randomEngine),
extraContext(extraContext)
{}
