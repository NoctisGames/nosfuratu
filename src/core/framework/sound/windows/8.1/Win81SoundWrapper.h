//
//  Win81SoundWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Win81SoundWrapper__
#define __noctisgames__Win81SoundWrapper__

#include "ISoundWrapper.h"

#include "Audio.h"

#include <memory>

class Win81SoundWrapper : public ISoundWrapper
{
public:
    Win81SoundWrapper(int soundId, const char *path, DirectX::AudioEngine* audioEngine, int numInstances = 1);
    
    virtual ~Win81SoundWrapper();
    
private:
    std::unique_ptr<DirectX::SoundEffect> m_sound;
};

#endif /* defined(__noctisgames__Win81SoundWrapper__) */
