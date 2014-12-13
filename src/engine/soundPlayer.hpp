
#ifndef SOUND_PLAYER_HPP
#define SOUND_PLAYER_HPP

#include <list>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"

class SoundPlayer : private sf::NonCopyable
{
public:
    SoundPlayer();
    void play(sf::SoundBuffer& soundBuffer, bool looping=false);
    void play(sf::SoundBuffer& soundBuffer, sf::Vector2f position, bool looping=false);
    void removeStoppedSounds();
    void stopAllSounds();
    void stopLoopingSounds();
    void setListenerPosition(sf::Vector2f position);
    sf::Vector2f getListenerPosition() const;
private:
    std::list<sf::Sound> mSounds;
};

#endif
