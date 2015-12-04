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
        case JumpSpringType_Light:
            return new JumpSpringLight(x, y);
        case JumpSpringType_Medium:
            return new JumpSpringMedium(x, y);
        case JumpSpringType_Heavy:
        default:
            return new JumpSpringHeavy(x, y);
    }
}

JumpSpring::JumpSpring(float x, float y, float width, float height, JumpSpringType type) : PhysicalEntity(x, y, width, height), m_type(type), m_isTriggered(false), m_isBoosting(false)
{
    updateBounds();
}

void JumpSpring::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    switch (m_type)
    {
        case JumpSpringType_Light:
            lowerLeft.add(getWidth() * 0.03333333333333f, 0);
            m_bounds->setWidth(getWidth() * 0.73333333333333f);
            m_bounds->setHeight(getHeight() * 0.89411764705882f);
            break;
        case JumpSpringType_Medium:
            // TODO
            break;
        case JumpSpringType_Heavy:
            // TODO
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
        
        if (m_fStateTime > 0.10f)
        {
            m_isBoosting = true;
        }
        
        if (m_fStateTime > 0.20f)
        {
            m_isTriggered = false;
            m_isBoosting = false;
            m_fStateTime = 0;
        }
    }
}

void JumpSpring::trigger()
{
    m_isTriggered = true;
}

bool JumpSpring::isBoosting()
{
    return m_isBoosting;
}

JumpSpringType JumpSpring::getEnumType()
{
    return m_type;
}

int JumpSpring::getType()
{
    return m_type;
}