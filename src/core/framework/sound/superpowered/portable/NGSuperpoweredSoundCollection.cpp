//
//  NGSuperpoweredSoundManager.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "NGSuperpoweredSoundCollection.h"

#include "NGSuperpoweredSound.h"

#include "NGSTDUtil.h"

NGSuperpoweredSoundCollection::NGSuperpoweredSoundCollection(int soundId, const char *path, unsigned int sampleRate, int numInstances, int fileOffset, int fileLength) :
m_iSoundIndex(0),
m_iSoundId(soundId),
m_iNumInstances(numInstances)
{
    for (int i = 0; i < m_iNumInstances; i++)
    {
        m_sounds.push_back(new NGSuperpoweredSound(soundId, path, sampleRate, fileOffset, fileLength));
    }
}

NGSuperpoweredSoundCollection::~NGSuperpoweredSoundCollection()
{
    NGSTDUtil::cleanUpVectorOfPointers(m_sounds);
}

NGSuperpoweredSound* NGSuperpoweredSoundCollection::getSound()
{
    NGSuperpoweredSound* ret = m_sounds[m_iSoundIndex++];
    if (m_iSoundIndex >= m_iNumInstances)
    {
        m_iSoundIndex = 0;
    }
    
    return ret;
}

std::vector<NGSuperpoweredSound *>& NGSuperpoweredSoundCollection::getSounds()
{
    return m_sounds;
}

int NGSuperpoweredSoundCollection::getSoundIndex()
{
    return m_iSoundIndex;
}

int NGSuperpoweredSoundCollection::getSoundId()
{
    return m_iSoundId;
}

int NGSuperpoweredSoundCollection::getNumInstances()
{
    return m_iNumInstances;
}
