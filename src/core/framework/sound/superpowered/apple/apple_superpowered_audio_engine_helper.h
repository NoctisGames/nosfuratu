//
//  apple_superpowered_audio_engine_helper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__apple_superpowered_audio_engine_helper__
#define __noctisgames__apple_superpowered_audio_engine_helper__

#include "NGSuperpoweredSoundManager.h"

void initialize(NGSuperpoweredSoundManager* superpoweredSoundManager);

const char * getBundlePathForSoundWithName(const char* soundName);

#endif /* defined(__noctisgames__apple_superpowered_audio_engine_helper__) */
