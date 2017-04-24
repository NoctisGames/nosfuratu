//
//  AudioEngineHelperFactory.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "AudioEngineHelperFactory.h"

#if defined __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #include "IOSAudioEngineHelper.h"
    #elif TARGET_OS_OSX
        #include "MacAudioEngineHelper.h"
    #endif
#elif defined __ANDROID__
    #include "AndroidAudioEngineHelper.h"
#elif defined _WIN32
    #include <winapifamily.h>
    #if defined (WINAPI_FAMILY_SYSTEM)
        #include "WinAudioEngineHelper.h"
    #else
        #include "Win81AudioEngineHelper.h"
    #endif
#endif

#include <assert.h>

AudioEngineHelperFactory* AudioEngineHelperFactory::getInstance()
{
    static AudioEngineHelperFactory instance = AudioEngineHelperFactory();
    return &instance;
}

IAudioEngineHelper* AudioEngineHelperFactory::createAudioEngineHelper()
{
#if defined __APPLE__
    #if TARGET_OS_IPHONE
        return IOSAudioEngineHelper::getInstance();
    #elif TARGET_OS_OSX
        return MacAudioEngineHelper::getInstance();
    #endif
#elif defined __ANDROID__
    return AndroidAudioEngineHelper::getInstance();
#elif defined _WIN32
    #if defined (WINAPI_FAMILY_SYSTEM)
        return WinAudioEngineHelper::getInstance();
    #else
        return Win81AudioEngineHelper::getInstance();
    #endif
#endif
    
    assert(false);
}

AudioEngineHelperFactory::AudioEngineHelperFactory()
{
    // Hide Constructor for Singleton
}
