//
//  MacSound.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "MacSound.h"

#include "AudioPlayerOsx.h"

MacSound::MacSound(int soundId, const char *path, float volume) : ISound(soundId),
m_player(AudioPlayerOsx::file(path)),
m_fVolume(volume),
m_isLooping(false),
m_isPaused(false)
{
    // TODO
}

MacSound::~MacSound()
{
    m_player->stop();
    
    delete m_player;
}

void MacSound::play(bool isLooping)
{
    m_isLooping = isLooping;
    m_isPaused = false;
    
    m_player->setLooping(isLooping);
    m_player->play();
}

void MacSound::resume()
{
    if (m_isPaused)
    {
        m_player->play();
        
        m_isPaused = false;
    }
}

void MacSound::pause()
{
    if (isPlaying())
    {
        m_player->pause();
        
        m_isPaused = true;
    }
}

void MacSound::stop()
{
    m_isLooping = false;
    m_isPaused = false;
    
    m_player->stop();
}

void MacSound::setVolume(float volume)
{
    m_fVolume = volume;
    
    m_player->setVolume(volume);
}

bool MacSound::isLooping()
{
    return m_isLooping;
}

bool MacSound::isPlaying()
{
    return m_player->isPlaying();
}

bool MacSound::isPaused()
{
    return m_isPaused;
}
