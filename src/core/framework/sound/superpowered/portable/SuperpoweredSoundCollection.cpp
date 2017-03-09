//
//  SuperpoweredSoundManager.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "SuperpoweredSoundCollection.h"

#include "SuperpoweredSound.h"

#include "NGSTDUtil.h"

SuperpoweredSoundCollection::SuperpoweredSoundCollection(int soundId, const char *path, unsigned int sampleRate, int numInstances, int fileOffset, int fileLength) :
m_iSoundIndex(0),
m_iSoundId(soundId),
m_iNumInstances(numInstances)
{
    for (int i = 0; i < m_iNumInstances; i++)
    {
        m_sounds.push_back(new SuperpoweredSound(soundId, path, sampleRate, fileOffset, fileLength));
    }
}

SuperpoweredSoundCollection::~SuperpoweredSoundCollection()
{
    NGSTDUtil::cleanUpVectorOfPointers(m_sounds);
}

SuperpoweredSound* SuperpoweredSoundCollection::getSound()
{
    SuperpoweredSound* ret = m_sounds[m_iSoundIndex++];
    if (m_iSoundIndex >= m_iNumInstances)
    {
        m_iSoundIndex = 0;
    }
    
    return ret;
}

std::vector<SuperpoweredSound *>& SuperpoweredSoundCollection::getSounds()
{
    return m_sounds;
}

int SuperpoweredSoundCollection::getSoundIndex()
{
    return m_iSoundIndex;
}

int SuperpoweredSoundCollection::getSoundId()
{
    return m_iSoundId;
}

int SuperpoweredSoundCollection::getNumInstances()
{
    return m_iNumInstances;
}
