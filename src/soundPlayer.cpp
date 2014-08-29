
#include "soundPlayer.hpp"

#include <algorithm>

namespace
{
    const float Listenerz = 300.0f;
    const float Attenuation = 8.0f;
    const float MinDistance2D = 200.0f;
    const float MinDistance3D =
        std::sqrt(MinDistance2D*MinDistance2D + Listenerz*Listenerz);
}

SoundPlayer::SoundPlayer()
{}

void SoundPlayer::play(sf::SoundBuffer& soundBuffer, bool looping)
{
    play(soundBuffer, getListenerPosition(), looping);
}

void SoundPlayer::play(sf::SoundBuffer& soundBuffer, sf::Vector2f position, bool looping)
{
    mSounds.push_back(sf::Sound(soundBuffer));
    sf::Sound& sound = mSounds.back();

    sound.setLoop(looping);

    sound.setPosition(position.x, -position.y, 0.0f);
    sound.setAttenuation(Attenuation);
    sound.setMinDistance(MinDistance3D);

    sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
    mSounds.remove_if([] (const sf::Sound& s)
    {
        return s.getStatus() == sf::Sound::Stopped;
    } );
}

void SoundPlayer::stopAllSounds()
{
    std::for_each(mSounds.begin(), mSounds.end(), [](sf::Sound& sound) {
        if(sound.getStatus() == sf::Sound::Playing)
        {
            sound.stop();
        }
    });
}

void SoundPlayer::stopLoopingSounds()
{
    std::for_each(mSounds.begin(), mSounds.end(), [](sf::Sound& sound) {
        if(sound.getStatus() == sf::Sound::Playing && sound.getLoop())
        {
            sound.stop();
        }
    });
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
    sf::Listener::setPosition(position.x, -position.y, Listenerz);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
    sf::Vector3f pos = sf::Listener::getPosition();
    return sf::Vector2f(pos.x,-pos.y);
}
