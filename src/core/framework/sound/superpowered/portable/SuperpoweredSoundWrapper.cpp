//
//  SuperpoweredSoundManager.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "SuperpoweredSoundWrapper.h"

#include "SuperpoweredSound.h"

#include "NGSTDUtil.h"

SuperpoweredSoundWrapper::SuperpoweredSoundWrapper(int soundId, const char *path, unsigned int sampleRate, int numInstances, int fileOffset, int fileLength) : SoundWrapper(soundId, numInstances)
{
    for (int i = 0; i < m_iNumInstances; i++)
    {
        m_sounds.push_back(new SuperpoweredSound(soundId, path, sampleRate, fileOffset, fileLength));
    }
}

SuperpoweredSoundWrapper::~SuperpoweredSoundWrapper()
{
    NGSTDUtil::cleanUpVectorOfPointers(m_sounds);
}

SuperpoweredSound* SuperpoweredSoundWrapper::getSound()
{
    SuperpoweredSound* ret = m_sounds[m_iSoundIndex++];
    if (m_iSoundIndex >= m_iNumInstances)
    {
        m_iSoundIndex = 0;
    }
    
    return ret;
}

std::vector<SuperpoweredSound *>& SuperpoweredSoundWrapper::getSounds()
{
    return m_sounds;
}
