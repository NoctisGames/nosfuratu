//
//  NGAudioEngine.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#include "NGAudioEngine.h"

#include "SoundWrapper.h"
#include "IAudioEngineHelper.h"

#include "AudioEngineHelperFactory.h"
#include "NGSTDUtil.h"

#include <assert.h>

#define MAX_SOUNDS_TO_PLAY_PER_FRAME 3

NGAudioEngine* NGAudioEngine::getInstance()
{
    static NGAudioEngine *instance = new NGAudioEngine();
    return instance;
}

void NGAudioEngine::update()
{
    m_iNumSoundsPlayedThisFrame = 0;
    
    // TODO
}

void NGAudioEngine::pause()
{
    // TODO
}

void NGAudioEngine::resume()
{
    // TODO
}

void NGAudioEngine::loadSound(int soundId, const char *path, int numInstances)
{
    SoundWrapper* sound = m_audioEngineHelper->loadSound(soundId, path, numInstances);
    
    m_sounds.insert(std::make_pair(soundId, sound));
}

void NGAudioEngine::playSound(int soundId, bool isLooping, float volume)
{
    if (m_isSoundDisabled
        || soundId <= 0
        || m_iNumSoundsPlayedThisFrame >= MAX_SOUNDS_TO_PLAY_PER_FRAME)
    {
        return;
    }
    
    for (std::vector<SuperpoweredSoundWrapper *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        if ((*i)->getSoundId() == soundId)
        {
            SuperpoweredSound* sound = (*i)->getSound();
            
            sound->play(isLooping);
            
            // TODO, set active sound (Superpowered only)
            
            return;
        }
    }
}

void NGAudioEngine::stopSound(int soundId)
{
    if (m_isSoundDisabled)
    {
        return;
    }
    
    for (std::vector<SuperpoweredSoundWrapper *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        if ((*i)->getSoundId() == soundId)
        {
            for (int j = 0; j < (*i)->getNumInstances(); j++)
            {
                SuperpoweredSound* sound = (*i)->getSound();
                if (sound->isPlaying())
                {
                    sound->stop();
                    
                    return;
                }
            }
            
            return;
        }
    }
}

void NGAudioEngine::pauseSound(int soundId)
{
    if (m_isSoundDisabled)
    {
        return;
    }
    
    for (std::vector<SuperpoweredSoundWrapper *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        if ((*i)->getSoundId() == soundId)
        {
            for (int j = 0; j < (*i)->getNumInstances(); j++)
            {
                SuperpoweredSound* sound = (*i)->getSound();
                if (sound->isPlaying())
                {
                    sound->stop();
                    
                    return;
                }
            }
            
            return;
        }
    }
}

void NGAudioEngine::resumeSound(int soundId)
{
    if (m_isSoundDisabled)
    {
        return;
    }
    
    for (std::vector<SuperpoweredSoundWrapper *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        if ((*i)->getSoundId() == soundId)
        {
            for (int j = 0; j < (*i)->getNumInstances(); j++)
            {
                SuperpoweredSound* sound = (*i)->getSound();
                if (sound->isPlaying())
                {
                    sound->stop();
                    
                    return;
                }
            }
            
            return;
        }
    }
}

void NGAudioEngine::stopAllSounds()
{
    if (m_isSoundDisabled)
    {
        return;
    }
    
    for (std::vector<SuperpoweredSoundWrapper *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        for (int j = 0; j < (*i)->getNumInstances(); j++)
        {
            SuperpoweredSound* sound = (*i)->getSound();
            sound->stop();
        }
    }
}

void NGAudioEngine::pauseAllSounds()
{
    if (m_isSoundDisabled)
    {
        return;
    }
    
    for (std::vector<SuperpoweredSoundWrapper *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        for (int j = 0; j < (*i)->getNumInstances(); j++)
        {
            SuperpoweredSound* sound = (*i)->getSound();
            sound->pause();
        }
    }
}

void NGAudioEngine::resumeAllSounds()
{
    if (m_isSoundDisabled)
    {
        return;
    }
    
    for (std::vector<SuperpoweredSoundWrapper *>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        for (int j = 0; j < (*i)->getNumInstances(); j++)
        {
            SuperpoweredSound* sound = (*i)->getSound();
            sound->resume();
        }
    }
}

void NGAudioEngine::loadMusic(const char *path)
{
    if (m_isMusicDisabled)
    {
        return;
    }
    
    if (m_music)
    {
        m_music->pause();
        
        delete m_music;
    }
    
    m_music = m_audioEngineHelper->loadMusic(path);
}

void NGAudioEngine::playMusic(float volume, bool isLooping)
{
    if (m_isMusicDisabled)
    {
        return;
    }
    
    if (m_music)
    {
        m_music->play(isLooping);
        m_music->setVolume(volume);
    }
}

void NGAudioEngine::setMusicVolume(float volume)
{
    if (m_isMusicDisabled)
    {
        return;
    }
    
    if (m_music)
    {
        m_music->setVolume(volume);
    }
}

void NGAudioEngine::stopMusic()
{
    if (m_isMusicDisabled)
    {
        return;
    }
    
    if (m_music)
    {
        m_music->stop();
    }
}

void NGAudioEngine::pauseMusic()
{
    if (m_isMusicDisabled)
    {
        return;
    }
    
    if (m_music)
    {
        m_music->pause();
    }
}

void NGAudioEngine::resumeMusic()
{
    if (m_isMusicDisabled)
    {
        return;
    }
    
    if (m_music && m_music->isPaused())
    {
        m_music->resume();
    }
}

bool NGAudioEngine::isMusicDisabled()
{
    return m_isMusicDisabled;
}

void NGAudioEngine::setMusicDisabled(bool isMusicDisabled)
{
    if (m_isMusicDisabled && !isMusicDisabled)
    {
        m_isMusicDisabled = false;
        playMusic();
    }
    else if (!m_isMusicDisabled && isMusicDisabled)
    {
        stopMusic();
        m_isMusicDisabled = true;
    }
}

bool NGAudioEngine::isSoundDisabled()
{
    return m_isSoundDisabled;
}

void NGAudioEngine::setSoundDisabled(bool isSoundDisabled)
{
    m_isSoundDisabled = isSoundDisabled;
}

SoundWrapper* NGAudioEngine::findSound(int soundId)
{
    auto q = m_sounds.find(soundId);
    
    assert(q != m_sounds.end());
    
    SoundWrapper* sound = q->second;
    
    return sound;
}

NGAudioEngine::NGAudioEngine() :
m_music(nullptr),
m_audioEngineHelper(NG_AUDIO_ENGINE_HELPER_FACTORY->createAudioEngineHelper()),
m_iNumSoundsPlayedThisFrame(0),
m_isMusicDisabled(false),
m_isSoundDisabled(false)
{
    // Empty
}

NGAudioEngine::~NGAudioEngine()
{
    NGSTDUtil::cleanUpMapOfPointerValues(m_sounds);
    
    delete m_music;
    
    delete m_audioEngineHelper;
}
