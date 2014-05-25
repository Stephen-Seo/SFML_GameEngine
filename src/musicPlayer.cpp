
#include "musicPlayer.hpp"

MusicPlayer::MusicPlayer()
: mMusic(), mFilenames(), mVolume(100.0f)
{
//load music here
}

void MusicPlayer::play(Music::ID theme)
{
    std::string filename = mFilenames[theme];

    if(!mMusic.openFromFile(filename))
        throw std::runtime_error("Music " + filename + " could not be loaded.");

    mMusic.setVolume(mVolume);
    mMusic.setLoop(true);
    mMusic.play();
}

void MusicPlayer::stop()
{
    mMusic.stop();
}

void MusicPlayer::setPaused(bool paused)
{
    if(paused)
        mMusic.pause();
    else
        mMusic.play();
}

void MusicPlayer::setVolume(float volume)
{
    mMusic.setVolume(volume);
}

float MusicPlayer::getVolume() const
{
    return mMusic.getVolume();
}

sf::SoundSource::Status MusicPlayer::getStatus()
{
    return mMusic.getStatus();
}
