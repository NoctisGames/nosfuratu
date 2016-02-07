//
//  LogVerticalTall.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "LogVerticalTall.h"
#include "Assets.h"

LogVerticalTall* LogVerticalTall::create(int gridX, int gridY, int type)
{
    return new LogVerticalTall(gridX, gridY);
}

LogVerticalTall::LogVerticalTall(float x, float y, float width, float height) : DestructiblePhysicalEntity(x, y, width, height), m_isBlowingUp(false)
{
    updateBounds();
}

void LogVerticalTall::update(float deltaTime)
{
    if (m_isBlowingUp)
    {
        Entity::update(deltaTime);
        
        if (m_fStateTime > 0.30f)
        {
            m_isRequestingDeletion = true;
        }
    }
}

void LogVerticalTall::triggerHit()
{
    if (!m_isBlowingUp)
    {
        m_isBlowingUp = true;
        m_fStateTime = 0;
        updateBounds();
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_BREAK_LOG);
    }
}

void LogVerticalTall::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.09302325581395f, getHeight() * 0.01953125f);
    
    m_bounds->setWidth(getWidth() * 0.26356589147287f);
    m_bounds->setHeight(getHeight() * 0.6640625f);
    
    if (m_isBlowingUp)
    {
        // Bye bye!
        lowerLeft.setY(1337);
    }
}

int LogVerticalTall::getType()
{
    return -1;
}

GroundSoundType LogVerticalTall::getGroundSoundType()
{
    return GROUND_SOUND_GRASS;
}
