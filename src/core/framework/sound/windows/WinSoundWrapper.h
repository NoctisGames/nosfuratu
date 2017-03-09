//
//  WinSoundWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__WinSoundWrapper__
#define __noctisgames__WinSoundWrapper__

#include "SoundWrapper.h"

#include "Audio.h"

#include <vector>
#include <memory>

class WinSoundWrapper : public SoundWrapper
{
public:
    WinSoundWrapper(int soundId, const char *path, DirectX::AudioEngine* audioEngine, int numInstances = 1);
    
    virtual ~WinSoundWrapper();
    
    DirectX::SoundEffectInstance* getSound();
    
    std::vector<std::unique_ptr<DirectX::SoundEffectInstance>>& getSounds();
    
private:
    std::unique_ptr<DirectX::SoundEffect> m_sound;
    std::vector<std::unique_ptr<DirectX::SoundEffectInstance>> m_sounds;
};

#endif /* defined(__noctisgames__WinSoundWrapper__) */
