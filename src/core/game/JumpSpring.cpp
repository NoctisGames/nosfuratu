//
//  JumpSpring.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "JumpSpring.h"

JumpSpring* JumpSpring::create(int gridX, int gridY, int type)
{
    switch ((JumpSpringType) type)
    {
        case JumpSpringType_Light:
            return new JumpSpringLight(gridX, gridY);
        case JumpSpringType_Medium:
            return new JumpSpringMedium(gridX, gridY);
        case JumpSpringType_Heavy:
        default:
            return new JumpSpringHeavy(gridX, gridY);
    }
}

JumpSpring::JumpSpring(float x, float y, float width, float height, int boostVelocity, JumpSpringType type) : LandPhysicalEntity(x, y, width, height), m_type(type), m_iBoostVelocity(boostVelocity), m_isTriggered(false), m_isBoosting(false)
{
    // Empty
}

void JumpSpring::trigger()
{
    m_isTriggered = true;
}

int JumpSpring::getBoostVelocity()
{
    return m_iBoostVelocity;
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

void JumpSpringLight::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.03333333333333f, 0);
    m_bounds->setWidth(getWidth() * 0.73333333333333f);
    m_bounds->setHeight(getHeight() * 0.6f);
}

void JumpSpringLight::update(float deltaTime)
{
    if (m_isTriggered)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.02f)
        {
            m_isBoosting = true;
        }
        
        if (m_fStateTime > 0.16f)
        {
            m_isTriggered = false;
            m_isBoosting = false;
            m_fStateTime = 0;
        }
    }
    
    updateBounds();
}

void JumpSpringMedium::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.09818181818182f, getHeight() * 0.18439716312057f);
    m_bounds->setWidth(getWidth() * 0.50545454545455f);
    m_bounds->setHeight(getHeight() * 0.34042553191489f);
}

void JumpSpringMedium::update(float deltaTime)
{
    if (m_isTriggered)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.02f)
        {
            m_isBoosting = true;
        }
        
        if (m_fStateTime > 0.28f)
        {
            m_isTriggered = false;
            m_isBoosting = false;
            m_fStateTime = 0;
        }
    }
    
    updateBounds();
}

void JumpSpringHeavy::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    m_bounds->setWidth(getWidth() * 0.57735849056604f);
    m_bounds->setHeight(getHeight() * 0.53881278538813f);
}

void JumpSpringHeavy::update(float deltaTime)
{
    if (m_isTriggered)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.02f)
        {
            m_isBoosting = true;
        }
        
        if (m_fStateTime > 0.28f)
        {
            m_isTriggered = false;
            m_isBoosting = false;
            m_fStateTime = 0;
        }
    }
    
    updateBounds();
}