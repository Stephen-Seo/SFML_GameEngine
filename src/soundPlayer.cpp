
#include "soundPlayer.hpp"

namespace
{
    const float Listenerz = 300.0f;
    const float Attenuation = 8.0f;
    const float MinDistance2D = 200.0f;
    const float MinDistance3D =
        std::sqrt(MinDistance2D*MinDistance2D + Listenerz*Listenerz);
}

SoundPlayer::SoundPlayer()
: mSoundBuffers(), mSounds()
{
//load sounds here
}

void SoundPlayer::play(Sound::ID effect)
{
    play(effect, getListenerPosition());
}

void SoundPlayer::play(Sound::ID effect, sf::Vector2f position)
{
    mSounds.push_back(sf::Sound(mSoundBuffers.get(effect)));
    sf::Sound& sound = mSounds.back();

    sound.setBuffer(mSoundBuffers.get(effect));
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

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
    sf::Listener::setPosition(position.x, -position.y, Listenerz);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
    sf::Vector3f pos = sf::Listener::getPosition();
    return sf::Vector2f(pos.x,-pos.y);
}
