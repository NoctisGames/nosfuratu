//
//  SoundWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__SoundWrapper__
#define __noctisgames__SoundWrapper__

#if defined __APPLE__ || defined __ANDROID__
#include "SuperpoweredSoundCollection.h"
#elif defined _WIN32
#include "WinSoundCollection.h"
#endif

struct SoundWrapper
{
#if defined __APPLE__ || defined __ANDROID__
    SuperpoweredSoundCollection* soundCollection;
    
    SoundWrapper(SuperpoweredSoundCollection* soundCollectionIn) : soundCollection(soundCollectionIn) {}
#elif defined _WIN32
    WinSoundCollection *soundCollection;
    
    SoundWrapper(WinSoundCollection* soundCollectionIn) : soundCollection(soundCollectionIn) {}
#endif
};

#endif /* defined(__noctisgames__SoundWrapper__) */
