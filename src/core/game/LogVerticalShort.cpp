//
//  LogVerticalShort.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "LogVerticalShort.h"
#include "Assets.h"

LogVerticalShort* LogVerticalShort::create(float x, float y, int type)
{
    return new LogVerticalShort(x, y);
}

LogVerticalShort::LogVerticalShort(float x, float y, float width, float height) : DestructiblePhysicalEntity(x, y, width, height), m_isBlowingUp(false)
{
    updateBounds();
}

void LogVerticalShort::update(float deltaTime)
{
    if (m_isBlowingUp)
    {
        Entity::update(deltaTime);
        
        if (m_fStateTime > 0.28f)
        {
            m_isRequestingDeletion = true;
        }
    }
}

void LogVerticalShort::triggerHit()
{
    if (!m_isBlowingUp)
    {
        m_isBlowingUp = true;
        m_fStateTime = 0;
        updateBounds();
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_BREAK_LOG);
    }
}

void LogVerticalShort::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.0655737704918f, getHeight() * 0.02678571428571f);
    
    m_bounds->setWidth(getWidth() * 0.19016393442623f);
    m_bounds->setHeight(getHeight() * 0.8125f);
    
    if (m_isBlowingUp)
    {
        // Bye bye!
        lowerLeft.setY(1337);
    }
}

int LogVerticalShort::getType()
{
    return -1;
}

GroundSoundType LogVerticalShort::getGroundSoundType()
{
    return GROUND_SOUND_GRASS;
}
