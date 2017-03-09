//
//  INGAudioEngineHelper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__INGAudioEngineHelper__
#define __noctisgames__INGAudioEngineHelper__

struct SoundWrapper;

class INGAudioEngineHelper
{
public:
    INGAudioEngineHelper();
    
    virtual ~INGAudioEngineHelper();
    
    virtual void update() = 0;
    
    virtual void pause() = 0;
    
    virtual void resume() = 0;
    
    virtual SoundWrapper* loadSound(int soundId, const char *path, int numInstances = 1) = 0;
};

#endif /* defined(__noctisgames__INGAudioEngineHelper__) */
