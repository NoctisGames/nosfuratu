//
//  SuperpoweredSoundManager.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "SoundWrapper.h"

SoundWrapper::SoundWrapper(int soundId, int numInstances) :
m_iSoundIndex(0),
m_iSoundId(soundId),
m_iNumInstances(numInstances)
{
    // Empty
}

SoundWrapper::~SoundWrapper()
{
    // Empty
}

int SoundWrapper::getSoundIndex()
{
    return m_iSoundIndex;
}

int SoundWrapper::getSoundId()
{
    return m_iSoundId;
}

int SoundWrapper::getNumInstances()
{
    return m_iNumInstances;
}
