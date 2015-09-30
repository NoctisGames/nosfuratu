//
//  JumpSpring.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "JumpSpring.h"
#include "EntityUtils.h"

JumpSpring JumpSpring::create(float x, JumpSpringType type)
{
    switch (type)
    {
        case JumpSpringType::JUMP_SPRING_IN_GRASS:
            return JumpSpring(x, 1.0058479532163742f, 1.0060658578856152f, type, EntityAnchor::ANCHOR_GROUND);
        case JumpSpringType::JUMP_SPRING_IN_CAVE:
        default:
            return JumpSpring(x, 1.2163742690058479f, 1.5441941074523398f, type, EntityAnchor::ANCHOR_CAVE);
    }
}

JumpSpring::JumpSpring(float x, float width, float height, JumpSpringType type, EntityAnchor anchor) : PhysicalEntity(x + width / 2, 0, width, height), m_jumpSpringType(type), m_isTriggered(false)
{
    EntityUtils::applyAnchor(*this, anchor);
    
    resetBounds(width, height * 0.70f);
    
    updateBounds();
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

JumpSpringType JumpSpring::getJumpSpringType()
{
    return m_jumpSpringType;
}

bool JumpSpring::isTriggered()
{
    return m_isTriggered;
}