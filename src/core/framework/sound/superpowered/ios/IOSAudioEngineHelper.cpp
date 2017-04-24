//
//  IOSAudioEngineHelper.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "IOSAudioEngineHelper.h"

#include "ISoundWrapper.h"

#include "SuperpoweredSoundWrapper.h"

#include "apple_audio_engine_helper.h"
#include "apple_asset_data_handler.h"

IOSAudioEngineHelper* IOSAudioEngineHelper::getInstance()
{
    static IOSAudioEngineHelper instance = IOSAudioEngineHelper();
    return &instance;
}

void IOSAudioEngineHelper::update(int flags)
{
    // Empty
}

void IOSAudioEngineHelper::pause()
{
    // Empty
}

void IOSAudioEngineHelper::resume()
{
    // Empty
}

ISoundWrapper* IOSAudioEngineHelper::loadSound(int soundId, const char *path, int numInstances)
{
    const char* bundlePath = getBundlePathForSoundWithName(path);
    
    SuperpoweredSoundWrapper* sound = new SuperpoweredSoundWrapper(m_superpoweredSoundManager, soundId, bundlePath, m_iSampleRate, numInstances);
    
    return sound;
}

ISoundWrapper* IOSAudioEngineHelper::loadMusic(const char* path)
{
    return loadSound(1337, path);
}

IOSAudioEngineHelper::IOSAudioEngineHelper() : IAudioEngineHelper(),
m_superpoweredSoundManager(new SuperpoweredSoundManager(44100)),
m_iSampleRate(44100)
{
    initializeWithSuperpoweredSoundManager(m_superpoweredSoundManager, m_iSampleRate);
}

IOSAudioEngineHelper::~IOSAudioEngineHelper()
{
    delete m_superpoweredSoundManager;
}
