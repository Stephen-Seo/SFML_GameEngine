
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
    void play(Sound::ID effect);
    void play(Sound::ID effect, sf::Vector2f position);
    void removeStoppedSounds();
    void setListenerPosition(sf::Vector2f position);
    sf::Vector2f getListenerPosition() const;
private:
    SoundBufferHolder mSoundBuffers;
    std::list<sf::Sound> mSounds;
};

#endif
