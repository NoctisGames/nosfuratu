//
//  MacSoundManager.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "MacSoundWrapper.h"

#include "MacSound.h"

#include "NGSTDUtil.h"

MacSoundWrapper::MacSoundWrapper(int soundId, const char *path, int numInstances) : ISoundWrapper(soundId, numInstances)
{
    for (int i = 0; i < m_iNumInstances; ++i)
    {
        m_sounds.push_back(new MacSound(soundId, path));
    }
}
