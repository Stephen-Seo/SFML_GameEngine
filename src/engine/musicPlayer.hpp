
#ifndef MUSIC_PLAYER_HPP
#define MUSIC_PLAYER_HPP

#include "resourceIdentifiers.hpp"

#include <map>
#include <string>
#include <stdexcept>
#include <SFML/Audio.hpp>

class MusicPlayer : private sf::NonCopyable
{
public:
    MusicPlayer();
    void play(Music::ID theme);
    void stop();
    void setPaused(bool paused);
    void setVolume(float volume);
    float getVolume() const;
    sf::SoundSource::Status getStatus();
private:
    sf::Music mMusic;
    std::map<Music::ID, std::string> mFilenames;
    float mVolume;
};

#endif
