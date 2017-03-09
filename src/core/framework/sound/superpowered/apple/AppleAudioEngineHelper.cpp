//
//  AppleAudioEngineHelper.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "AppleAudioEngineHelper.h"

#include "SoundWrapper.h"

#include "SuperpoweredSoundCollection.h"

#include "apple_audio_engine_helper.h"

AppleAudioEngineHelper* AppleAudioEngineHelper::getInstance()
{
    static AppleAudioEngineHelper *instance = new AppleAudioEngineHelper();
    return instance;
}

void AppleAudioEngineHelper::update()
{
    // Empty
}

void AppleAudioEngineHelper::pause()
{
    // Empty
}

void AppleAudioEngineHelper::resume()
{
    // Empty
}

SoundWrapper* AppleAudioEngineHelper::loadSound(int soundId, const char *path, int numInstances)
{
    const char* bundlePath = getBundlePathForSoundWithName(path);
    SuperpoweredSoundCollection* sound = new SuperpoweredSoundCollection(soundId, bundlePath, m_iSampleRate, numInstances);
    
    return new SoundWrapper(sound);
}

SoundWrapper* AppleAudioEngineHelper::loadMusic(const char* path)
{
    const char* bundlePath = getBundlePathForSoundWithName(path);
    
    SuperpoweredSoundCollection* music = new SuperpoweredSoundCollection(1337, bundlePath, m_iSampleRate);
    
    return new SoundWrapper(music);
}

AppleAudioEngineHelper::AppleAudioEngineHelper() : IAudioEngineHelper(),
m_superpoweredSoundManager(new SuperpoweredSoundManager(44100)),
m_iSampleRate(44100)
{
    initializeWithSuperpoweredSoundManager(m_superpoweredSoundManager, m_iSampleRate);
}

AppleAudioEngineHelper::~AppleAudioEngineHelper()
{
    delete m_superpoweredSoundManager;
}
