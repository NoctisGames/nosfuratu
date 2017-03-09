//
//  NGSuperpoweredAudioEngineHelper.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#include "NGSuperpoweredAudioEngineHelper.h"

#if defined __APPLE__
#include "AppleSuperpoweredAudioEngineHelper.h"
#elif defined __ANDROID__
#include "AndroidSuperpoweredAudioEngineHelper.h"
#endif

#include <assert.h>

NGSuperpoweredAudioEngineHelper* NGSuperpoweredAudioEngineHelper::getNGSuperpoweredAudioEngineHelper()
{
#if defined __APPLE__
    return AppleSuperpoweredAudioEngineHelper::getInstance();
#elif defined __ANDROID__
    return AndroidSuperpoweredAudioEngineHelper::getInstance();
#endif
    
    assert(false);
}

NGSuperpoweredAudioEngineHelper::NGSuperpoweredAudioEngineHelper()
{
    // Empty
}
