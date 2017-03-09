//
//  NGAudioEngineHelperFactory.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__NGAudioEngineHelperFactory__
#define __noctisgames__NGAudioEngineHelperFactory__

class INGAudioEngineHelper;

#define NG_AUDIO_ENGINE_HELPER_FACTORY (NGAudioEngineHelperFactory::getInstance())

class NGAudioEngineHelperFactory
{
public:
	static NGAudioEngineHelperFactory* getInstance();
    
    INGAudioEngineHelper* createNGAudioEngineHelper();

private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    NGAudioEngineHelperFactory();
    NGAudioEngineHelperFactory(const NGAudioEngineHelperFactory&);
    NGAudioEngineHelperFactory& operator=(const NGAudioEngineHelperFactory&);
};

#endif /* defined(__noctisgames__NGAudioEngineHelperFactory__) */
