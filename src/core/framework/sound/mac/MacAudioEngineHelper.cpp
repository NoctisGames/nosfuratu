//
//  MacAudioEngineHelper.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "MacAudioEngineHelper.h"

#include "ISoundWrapper.h"

#include "apple_asset_data_handler.h"

MacAudioEngineHelper* MacAudioEngineHelper::getInstance()
{
    static MacAudioEngineHelper instance = MacAudioEngineHelper();
    return &instance;
}

void MacAudioEngineHelper::update(int flags)
{
    // Empty
}

void MacAudioEngineHelper::pause()
{
    // Empty
}

void MacAudioEngineHelper::resume()
{
    // Empty
}

ISoundWrapper* MacAudioEngineHelper::loadSound(int soundId, const char *path, int numInstances)
{
    const char* bundlePath = getBundlePathForSoundWithName(path);
    
    // TODO
    
    return nullptr;
}

ISoundWrapper* MacAudioEngineHelper::loadMusic(const char* path)
{
    return loadSound(1337, path);
}

MacAudioEngineHelper::MacAudioEngineHelper() : IAudioEngineHelper()
{
    // Empty
}

MacAudioEngineHelper::~MacAudioEngineHelper()
{
    // Empty
}
