//
//  NGSuperpoweredAudioEngineHelper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__NGSuperpoweredAudioEngineHelper__
#define __noctisgames__NGSuperpoweredAudioEngineHelper__

#include "INGAudioEngineHelper.h"

class NGSuperpoweredAudioEngineHelper : public INGAudioEngineHelper
{
public:
    static NGSuperpoweredAudioEngineHelper* getNGSuperpoweredAudioEngineHelper();
    
    virtual void loadSound(int soundId, const char* soundName, int numCopies) = 0;
    
    virtual void loadMusic(const char* musicName) = 0;
    
protected:
    NGSuperpoweredAudioEngineHelper();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    NGSuperpoweredAudioEngineHelper(const NGSuperpoweredAudioEngineHelper&);
    NGSuperpoweredAudioEngineHelper& operator=(const NGSuperpoweredAudioEngineHelper&);
};

#endif /* defined(__noctisgames__NGSuperpoweredAudioEngineHelper__) */
