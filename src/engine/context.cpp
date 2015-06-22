
#include "context.hpp"

Context::Context(sf::RenderWindow& window, ResourceManager& resourceManager, MusicPlayer& mPlayer, SoundPlayer& sPlayer, Engine& ecEngine, bool& isQuitting, std::unique_ptr<Connection>& connection) :
window(&window),
resourceManager(&resourceManager),
mPlayer(&mPlayer),
sPlayer(&sPlayer),
ecEngine(&ecEngine),
isQuitting(&isQuitting),
connection(&connection)
{}
