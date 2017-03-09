//
//  WinAudioEngineHelper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__WinAudioEngineHelper__
#define __noctisgames__WinAudioEngineHelper__

#define WIN_AUDIO_ENGINE_HELPER (WinAudioEngineHelper::getInstance())

#include "IAudioEngineHelper.h"

#include "Audio.h"

#include <memory>

class WinAudioEngineHelper : public IAudioEngineHelper
{
public:
	static WinAudioEngineHelper* getInstance();
    
    virtual void update();
    
    virtual void pause();
    
    virtual void resume();
    
    virtual SoundWrapper* loadSound(int soundId, const char *path, int numInstances = 1);
    
    virtual SoundWrapper* loadMusic(const char* path);

private:
    std::unique_ptr<DirectX::AudioEngine> m_audEngine;
    bool m_retryAudio;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WinAudioEngineHelper();
    virtual ~WinAudioEngineHelper();
    WinAudioEngineHelper(const WinAudioEngineHelper&);
    WinAudioEngineHelper& operator=(const WinAudioEngineHelper&);
};

#endif /* defined(__noctisgames__WinAudioEngineHelper__) */
