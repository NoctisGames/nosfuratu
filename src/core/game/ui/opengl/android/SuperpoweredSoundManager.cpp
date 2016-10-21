//
//  SuperpoweredSoundManager.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "SuperpoweredSoundManager.h"

#include "EntityUtils.h"

#define MAX_NUM_SOUND_PLAYERS 11

SuperpoweredSoundManager::SuperpoweredSoundManager(const char *apkPath, unsigned int sampleRate, unsigned int bufferSize)
{
    m_music = new SuperpoweredSound(apkPath, sampleRate, bufferSize);
    
    for (int i = 0; i < MAX_NUM_SOUND_PLAYERS; i++)
    {
        m_sounds.push_back(new SuperpoweredSound(apkPath, sampleRate, bufferSize));
    }
}

SuperpoweredSoundManager::~SuperpoweredSoundManager()
{
    delete m_music;
    EntityUtils::cleanUpVectorOfPointers(m_sounds);
}

void SuperpoweredSoundManager::loadAndPlaySound(int rawResourceId, int fileOffset, int fileLength, float volume, bool isLooping)
{
    for (std::vector<SuperpoweredSound *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        if ((*i)->getRawResourceId() == rawResourceId
            && !(*i)->isPlaying())
        {
            (*i)->loadAndPlaySound(rawResourceId, fileOffset, fileLength, isLooping);
            
            return;
        }
    }
    
    for (std::vector<SuperpoweredSound *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        if (!(*i)->isPlaying())
        {
            (*i)->loadAndPlaySound(rawResourceId, fileOffset, fileLength, isLooping);
            
            return;
        }
    }
}

void SuperpoweredSoundManager::stopSound(int rawResourceId)
{
    for (std::vector<SuperpoweredSound *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        if ((*i)->getRawResourceId() == rawResourceId)
        {
            (*i)->stop();
            
            return;
        }
    }
}

void SuperpoweredSoundManager::loadAndPlayMusic(int rawResourceId, int fileOffset, int fileLength, float volume, bool isLooping)
{
    m_music->loadAndPlaySound(rawResourceId, fileOffset, fileLength, isLooping);
}

void SuperpoweredSoundManager::setMusicVolume(float volume)
{
    m_music->setVolume(volume);
}

void SuperpoweredSoundManager::resumeMusic()
{
    m_music->play();
}

void SuperpoweredSoundManager::pauseMusic()
{
    m_music->pause();
}
