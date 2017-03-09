//
//  AppleSuperpoweredAudioEngineHelper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__AppleSuperpoweredAudioEngineHelper__
#define __noctisgames__AppleSuperpoweredAudioEngineHelper__

#include "NGSuperpoweredAudioEngineHelper.h"

class AppleSuperpoweredAudioEngineHelper : public NGSuperpoweredAudioEngineHelper
{
public:
	static AppleSuperpoweredAudioEngineHelper* getInstance();

private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    AppleSuperpoweredAudioEngineHelper();
    AppleSuperpoweredAudioEngineHelper(const AppleSuperpoweredAudioEngineHelper&);
    AppleSuperpoweredAudioEngineHelper& operator=(const AppleSuperpoweredAudioEngineHelper&);
};

#endif /* defined(__noctisgames__AppleSuperpoweredAudioEngineHelper__) */
