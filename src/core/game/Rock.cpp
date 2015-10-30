//
//  Rock.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "Rock.h"

Rock* Rock::create(float x, float y, int type)
{
    switch ((RockType) type)
    {
        case RockType_Rock:
            return new Rock(x, y);
        case RockType_CrackedRock:
        default:
            return new CrackedRock(x, y);
    }
}

Rock::Rock(float x, float y, float width, float height, bool isCracked, RockType type) : DestructiblePhysicalEntity(x, y, width, height), m_type(type), m_color(1, 1, 1, 1), m_isCracked(isCracked), m_isBlowingUp(false)
{
    updateBounds();
}

void Rock::update(float deltaTime)
{
    Entity::update(deltaTime);
    
    if (m_isBlowingUp)
    {
        updateBounds();
        
        if (m_fStateTime > 0.32f)
        {
            m_color.alpha = 1.96f - m_fStateTime * 3;
            
            if (m_color.alpha < 0)
            {
                m_color.alpha = 0;
                m_isRequestingDeletion = true;
            }
        }
    }
}

void Rock::triggerHit()
{
    if (m_isCracked)
    {
        m_isBlowingUp = true;
        m_fStateTime = 0;
    }
    
    m_isCracked = true;
}

void Rock::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.35416666666667f, getHeight() * 0.21739130434783);
    
    m_bounds->setWidth(getWidth() * 0.27604166666667f);
    m_bounds->setHeight(getHeight() * 0.26086956521739f);
    
    if (m_isBlowingUp)
    {
        // Bye bye!
        lowerLeft.setY(1337);
    }
}

Color Rock::getColor()
{
    return m_color;
}

bool Rock::isCracked()
{
    return m_isCracked;
}

bool Rock::isBlowingUp()
{
    return m_isBlowingUp;
}

RockType Rock::getEnumType()
{
    return m_type;
}

int Rock::getType()
{
    return m_type;
}