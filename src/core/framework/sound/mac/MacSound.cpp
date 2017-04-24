//
//  MacSound.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "MacSound.h"

MacSound::MacSound(int soundId, const char *path, float volume) : ISound(soundId),
m_fVolume(volume),
m_isLooping(false),
m_isPaused(false)
{
    // TODO
}

MacSound::~MacSound()
{
    // TODO
}

void MacSound::play(bool isLooping)
{
    m_isLooping = isLooping;
    m_isPaused = false;
    
    // TODO
}

void MacSound::resume()
{
    if (m_isPaused)
    {
        // TODO
        
        m_isPaused = false;
    }
}

void MacSound::pause()
{
    if (isPlaying())
    {
        // TODO
        
        m_isPaused = true;
    }
}

void MacSound::stop()
{
    m_isLooping = false;
    m_isPaused = false;
    
    // TODO
}

void MacSound::setVolume(float volume)
{
    m_fVolume = volume;
    
    // TODO
}

bool MacSound::isLooping()
{
    return m_isLooping;
}

bool MacSound::isPlaying()
{
    // TODO
    return false;
}

bool MacSound::isPaused()
{
    return m_isPaused;
}
