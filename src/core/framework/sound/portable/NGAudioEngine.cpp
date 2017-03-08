//
//  NGAudioEngine.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#include "NGAudioEngine.h"

NGAudioEngine* NGAudioEngine::getInstance()
{
    static NGAudioEngine *instance = new NGAudioEngine();
    return instance;
}

NGAudioEngine::NGAudioEngine()
{
    // Empty
}
