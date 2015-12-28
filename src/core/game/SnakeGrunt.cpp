//
//  SnakeGrunt.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/27/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "SnakeGrunt.h"

SnakeGrunt* SnakeGrunt::create(float x, float y, int type)
{
    return new SnakeGrunt(x, y);
}

SnakeGrunt::SnakeGrunt(float x, float y, float width, float height) : DestructiblePhysicalEntity(x, y, width, height), m_color(1, 1, 1, 1), m_isDying(false)
{
    updateBounds();
}

void SnakeGrunt::update(float deltaTime)
{
    Entity::update(deltaTime);
}

void SnakeGrunt::triggerHit()
{
    m_isDying = true;
}

void SnakeGrunt::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.30078125f, getHeight() * 0.08203125f);
    
    m_bounds->setWidth(getWidth() * 0.4296875f);
    m_bounds->setHeight(getHeight() * 0.296875f);
    
    if (m_isDying)
    {
        // Bye bye!
        lowerLeft.setY(1337);
    }
}

Color SnakeGrunt::getColor()
{
    return m_color;
}

bool SnakeGrunt::isDying()
{
    return m_isDying;
}

int SnakeGrunt::getType()
{
    return -1;
}
