//
//  SoundWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__SoundWrapper__
#define __noctisgames__SoundWrapper__

class SoundWrapper
{
public:
    SoundWrapper(int soundId, int numInstances = 1);
    
    virtual ~SoundWrapper();
    
    int getSoundIndex();
    
    int getSoundId();
    
    int getNumInstances();
    
protected:
    int m_iSoundIndex;
    int m_iSoundId;
    int m_iNumInstances;
};

#endif /* defined(__noctisgames__SoundWrapper__) */
