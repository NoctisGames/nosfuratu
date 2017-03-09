//
//  WinAudioEngineHelper.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "WinAudioEngineHelper.h"

#include "SoundWrapper.h"

#include "WinSoundCollection.h"

WinAudioEngineHelper* WinAudioEngineHelper::getInstance()
{
    static WinAudioEngineHelper *instance = new WinAudioEngineHelper();
    return instance;
}

void WinAudioEngineHelper::update()
{
    // Empty
}

void WinAudioEngineHelper::pause()
{
    // Empty
}

void WinAudioEngineHelper::resume()
{
    // Empty
}

SoundWrapper* WinAudioEngineHelper::loadSound(int soundId, const char *path, int numInstances)
{
    // TODO
    return new SoundWrapper(nullptr);
}

SoundWrapper* WinAudioEngineHelper::loadMusic(const char* path)
{
    // TODO
    return new SoundWrapper(nullptr);
}

WinAudioEngineHelper::WinAudioEngineHelper() : IAudioEngineHelper()
{
    // TODO
}

WinAudioEngineHelper::~WinAudioEngineHelper()
{
    // TODO
}
