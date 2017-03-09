//
//  AppleSuperpoweredAudioEngineHelper.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "AppleSuperpoweredAudioEngineHelper.h"

#include "apple_superpowered_audio_engine_helper.h"

AppleSuperpoweredAudioEngineHelper* AppleSuperpoweredAudioEngineHelper::getInstance()
{
    static AppleSuperpoweredAudioEngineHelper *instance = new AppleSuperpoweredAudioEngineHelper();
    return instance;
}

AppleSuperpoweredAudioEngineHelper::AppleSuperpoweredAudioEngineHelper() : NGSuperpoweredAudioEngineHelper()
{
    // Empty
}
