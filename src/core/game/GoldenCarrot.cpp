//
//  GoldenCarrot.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "GoldenCarrot.h"
#include "Assets.h"
#include "GameConstants.h"

GoldenCarrot* GoldenCarrot::create(int gridX, int gridY, int type)
{
    return new GoldenCarrot(gridX, gridY);
}

GoldenCarrot::GoldenCarrot(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_isCollected(false)
{
    resetBounds(width * 0.5f, height * 0.5f);
    
    updateBounds();
}

void GoldenCarrot::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_isCollected && m_fStateTime > 0.40f)
    {
        m_isRequestingDeletion = true;
    }
}

void GoldenCarrot::collect()
{
    if (!m_isCollected)
    {
        m_fStateTime = 0;
        m_isCollected = true;
        
        m_position->add(0, 0.34753661784287615f);
        m_fWidth = 2.660674157303371f;
        m_fHeight = 2.03728362183755f;
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_GOLDEN_CARROT);
    }
}

bool GoldenCarrot::isCollected()
{
    return m_isCollected;
}

int GoldenCarrot::getType()
{
    return -1;
}