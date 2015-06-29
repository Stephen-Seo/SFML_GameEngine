
#ifndef MUSIC_PLAYER_HPP
#define MUSIC_PLAYER_HPP

#include "resourceIdentifiers.hpp"

#include <unordered_set>
#include <string>
#include <stdexcept>
#include <SFML/Audio.hpp>

class MusicPlayer : private sf::NonCopyable
{
public:
    MusicPlayer();
    void play(const std::string& filename);
    void stop();
    void setPaused(bool paused);
    void setVolume(float volume);
    float getVolume() const;
    sf::SoundSource::Status getStatus();
private:
    sf::Music mMusic;
    float mVolume;
};

#endif
