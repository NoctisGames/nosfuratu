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
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    switch (m_type)
    {
        case JumpSpringType_Grass:
            lowerLeft.add(getWidth() * 0.25581395348837f, getHeight() * 0.04651162790698f);
            m_bounds->setWidth(getWidth() * 0.48837209302326f);
            m_bounds->setHeight(getHeight() * 0.72093023255814f);
            break;
        case JumpSpringType_Cave:
            lowerLeft.add(getWidth() * 0.03846153846154f, getHeight() * 0);
            m_bounds->setWidth(getWidth() * 0.92307692307692f);
            m_bounds->setHeight(getHeight() * 0.81818181818182f);
            break;
        default:
            break;
    }
}

void JumpSpring::update(float deltaTime)
{
    if (m_isTriggered)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.12f)
        {
            m_isTriggered = false;
            m_fStateTime = 0;
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