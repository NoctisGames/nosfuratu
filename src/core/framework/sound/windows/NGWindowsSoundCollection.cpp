//
//  NGWindowsSoundManager.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "NGWindowsSoundCollection.h"

#include "NGSTDUtil.h"

NGWindowsSoundCollection::NGWindowsSoundCollection(int soundId, const char *path, int numInstances) :
m_iSoundIndex(0),
m_iSoundId(soundId),
m_iNumInstances(numInstances)
{
    for (int i = 0; i < m_iNumInstances; i++)
    {
        // TODO
    }
}

NGWindowsSoundCollection::~NGWindowsSoundCollection()
{
    if (m_sound)
    {
        m_sound.reset();
    }
    
    NGSTDUtil::cleanUpVectorOfUniquePointers(m_sounds);
}

DirectX::SoundEffect* NGWindowsSoundCollection::getSound()
{
    std::unique_ptr<DirectX::SoundEffect> ret = m_sounds[m_iSoundIndex++];
    if (m_iSoundIndex >= m_iNumInstances)
    {
        m_iSoundIndex = 0;
    }
    
    return ret.get();
}

std::vector<std::unique_ptr<DirectX::SoundEffect>>& NGWindowsSoundCollection::getSounds()
{
    return m_sounds;
}

int NGWindowsSoundCollection::getSoundIndex()
{
    return m_iSoundIndex;
}

int NGWindowsSoundCollection::getSoundId()
{
    return m_iSoundId;
}

int NGWindowsSoundCollection::getNumInstances()
{
    return m_iNumInstances;
}
