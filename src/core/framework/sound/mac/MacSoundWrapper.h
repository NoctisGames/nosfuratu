//
//  MacSoundManager.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__MacSoundWrapper__
#define __noctisgames__MacSoundWrapper__

#include "ISoundWrapper.h"

#include <vector>

class MacSound;

class MacSoundWrapper : public ISoundWrapper
{
public:
    MacSoundWrapper(int soundId, const char *path, int numInstances = 1);
};

#endif /* defined(__noctisgames__MacSoundWrapper__) */
