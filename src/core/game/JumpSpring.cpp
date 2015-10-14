//
//  JumpSpring.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "JumpSpring.h"

JumpSpring* JumpSpring::create(float x, float y, int type)
{
    switch ((JumpSpringType) type)
    {
        case JumpSpringType_Grass:
            return new JumpSpringGrass(x, y);
        case JumpSpringType_Cave:
        default:
            return new JumpSpringCave(x, y);
    }
}

JumpSpring::JumpSpring(float x, float y, float width, float height, JumpSpringType type) : PhysicalEntity(x, y, width, height), m_type(type), m_isTriggered(false)
{
    updateBounds();
}

void JumpSpring::updateBounds()
{
    m_bounds->setHeight(getHeight());
    
    PhysicalEntity::updateBounds();
    
    m_bounds->setHeight(getHeight() * 0.70f);
}

void JumpSpring::update(float deltaTime)
{
    if (m_isTriggered)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.20f)
        {
            m_isTriggered = false;
        }
    }
}

void JumpSpring::trigger()
{
    m_isTriggered = true;
}

bool JumpSpring::isTriggered()
{
    return m_isTriggered;
}

JumpSpringType JumpSpring::getEnumType()
{
    return m_type;
}

int JumpSpring::getType()
{
    return m_type;
}