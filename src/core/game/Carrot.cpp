//
//  Carrot.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/4/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Carrot.h"
#include "Assets.h"
#include "GameConstants.h"

Carrot* Carrot::create(int gridX, int gridY, int type)
{
    return new Carrot(gridX, gridY);
}

Carrot::Carrot(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_isCollected(false)
{
    updateBounds();
}

void Carrot::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_isCollected && m_fStateTime > 0.25f)
    {
        m_isRequestingDeletion = true;
    }
}

void Carrot::collect()
{
    if (!m_isCollected)
    {
        m_fStateTime = 0;
        m_isCollected = true;
        
        m_position->add(0, 0.7669773635153129f);
        m_fWidth = 1.8696629213483147f;
        m_fHeight = 2.6364846870838883f;
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_CARROT);
    }
}

bool Carrot::isCollected()
{
    return m_isCollected;
}

int Carrot::getType()
{
    return -1;
}