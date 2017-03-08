//
//  NGAudioEngine.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__NGAudioEngine__
#define __noctisgames__NGAudioEngine__

class NGAudioEngine
{
public:
    static NGAudioEngine* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    NGAudioEngine();
    NGAudioEngine(const NGAudioEngine&);
    NGAudioEngine& operator=(const NGAudioEngine&);
};

#endif /* defined(__noctisgames__NGAudioEngine__) */
