//
//  MacAudioEngineHelper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__MacAudioEngineHelper__
#define __noctisgames__MacAudioEngineHelper__

#include "IAudioEngineHelper.h"

class SuperpoweredSoundManager;

class MacAudioEngineHelper : public IAudioEngineHelper
{
public:
	static MacAudioEngineHelper* getInstance();
    
    virtual void update(int flags = 0);
    
    virtual void pause();
    
    virtual void resume();
    
    virtual ISoundWrapper* loadSound(int soundId, const char *path, int numInstances = 1);
    
    virtual ISoundWrapper* loadMusic(const char* path);

private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    MacAudioEngineHelper();
    virtual ~MacAudioEngineHelper();
    MacAudioEngineHelper(const MacAudioEngineHelper&);
    MacAudioEngineHelper& operator=(const MacAudioEngineHelper&);
};

#endif /* defined(__noctisgames__MacAudioEngineHelper__) */
