//
//  NGAudioEngineHelperFactory.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "NGAudioEngineHelperFactory.h"

NGAudioEngineHelperFactory* NGAudioEngineHelperFactory::getInstance()
{
    static NGAudioEngineHelperFactory *instance = new NGAudioEngineHelperFactory();
    return instance;
}

#if defined __APPLE__
#include "OpenGLNGAudioEngineHelper.h"
INGAudioEngineHelper* NGAudioEngineHelperFactory::createNGAudioEngineHelper() { return new OpenGLNGAudioEngineHelper(); }
#elif defined __ANDROID__
#include "OpenGLNGAudioEngineHelper.h"
INGAudioEngineHelper* NGAudioEngineHelperFactory::createNGAudioEngineHelper() { return new OpenGLNGAudioEngineHelper(); }
#elif defined _WIN32
#include "Direct3DNGAudioEngineHelper.h"
INGAudioEngineHelper* NGAudioEngineHelperFactory::createNGAudioEngineHelper() { return new Direct3DNGAudioEngineHelper(); }
#endif

NGAudioEngineHelperFactory::NGAudioEngineHelperFactory()
{
    // Hide Constructor for Singleton
}
