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
#include "NGSuperpoweredSoundCollection.h"
#elif defined _WIN32
#include "NGWindowsSoundCollection.h"
#endif

struct SoundWrapper
{
#if defined __APPLE__ || defined __ANDROID__
    NGSuperpoweredSoundCollection* soundCollection;
    
    SoundWrapper(NGSuperpoweredSoundCollection* soundCollectionIn) : soundCollection(soundCollectionIn) {}
#elif defined _WIN32
    NGWindowsSoundCollection *soundCollection;
    
    SoundWrapper(NGWindowsSoundCollection* soundCollectionIn) : soundCollection(soundCollectionIn) {}
#endif
};

#endif /* defined(__noctisgames__SoundWrapper__) */
