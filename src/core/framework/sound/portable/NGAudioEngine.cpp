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
    
    // TODO
}

void NGAudioEngine::pauseSound(int soundId)
{
    if (m_isSoundDisabled)
    {
        return;
    }
    
    // TODO
}

void NGAudioEngine::stopSound(int soundId)
{
    if (m_isSoundDisabled)
    {
        return;
    }
    
    // TODO
}

void NGAudioEngine::pauseAllSounds()
{
    if (m_isSoundDisabled)
    {
        return;
    }
    
    // TODO
}

void NGAudioEngine::stopAllSounds()
{
    if (m_isSoundDisabled)
    {
        return;
    }
    
    // TODO
}

void NGAudioEngine::resumeAllSounds()
{
    if (m_isSoundDisabled)
    {
        return;
    }
    
    // TODO
}

void NGAudioEngine::loadMusic(const char *path)
{
    if (m_isMusicDisabled)
    {
        return;
    }
    
    if (m_music)
    {
        delete m_music;
    }
    
    m_music = m_audioEngineHelper->loadMusic(path);
}

void NGAudioEngine::playMusic(bool isLooping, float volume)
{
    if (m_isMusicDisabled)
    {
        return;
    }
    
    // TODO
}

void NGAudioEngine::setMusicVolume(float volume)
{
    if (m_isMusicDisabled)
    {
        return;
    }
    
    // TODO
}

void NGAudioEngine::pauseMusic()
{
    if (m_isMusicDisabled)
    {
        return;
    }
    
    // TODO
}

void NGAudioEngine::stopMusic()
{
    if (m_isMusicDisabled)
    {
        return;
    }
    
    // TODO
}

void NGAudioEngine::resumeMusic()
{
    if (m_isMusicDisabled)
    {
        return;
    }
    
    // TODO
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
