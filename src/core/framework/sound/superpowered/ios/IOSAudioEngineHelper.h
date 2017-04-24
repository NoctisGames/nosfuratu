//
//  IOSAudioEngineHelper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__IOSAudioEngineHelper__
#define __noctisgames__IOSAudioEngineHelper__

#include "IAudioEngineHelper.h"

class SuperpoweredSoundManager;

class IOSAudioEngineHelper : public IAudioEngineHelper
{
public:
	static IOSAudioEngineHelper* getInstance();
    
    virtual void update(int flags = 0);
    
    virtual void pause();
    
    virtual void resume();
    
    virtual ISoundWrapper* loadSound(int soundId, const char *path, int numInstances = 1);
    
    virtual ISoundWrapper* loadMusic(const char* path);

private:
    SuperpoweredSoundManager* m_superpoweredSoundManager;
    int m_iSampleRate;

    // ctor, copy ctor, and assignment should be private in a Singleton
    IOSAudioEngineHelper();
    virtual ~IOSAudioEngineHelper();
    IOSAudioEngineHelper(const IOSAudioEngineHelper&);
    IOSAudioEngineHelper& operator=(const IOSAudioEngineHelper&);
};

#endif /* defined(__noctisgames__IOSAudioEngineHelper__) */
