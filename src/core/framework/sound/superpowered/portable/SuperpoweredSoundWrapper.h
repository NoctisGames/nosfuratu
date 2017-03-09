//
//  SuperpoweredSoundManager.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__SuperpoweredSoundWrapper__
#define __noctisgames__SuperpoweredSoundWrapper__

#include "SoundWrapper.h"

#include <vector>

class SuperpoweredSound;

class SuperpoweredSoundWrapper : public SoundWrapper
{
public:
    SuperpoweredSoundWrapper(int soundId, const char *path, unsigned int sampleRate, int numInstances = 1, int fileOffset = -1, int fileLength = -1);
    
    virtual ~SuperpoweredSoundWrapper();
    
    SuperpoweredSound* getSound();
    
    std::vector<SuperpoweredSound *>& getSounds();
    
private:
    std::vector<SuperpoweredSound *> m_sounds;
};

#endif /* defined(__noctisgames__SuperpoweredSoundWrapper__) */
