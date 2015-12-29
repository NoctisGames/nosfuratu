//
//  SnakeGrunt.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/28/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "SnakeGrunt.h"

SnakeGrunt* SnakeGrunt::create(float x, float y, int type)
{
    return new SnakeGrunt(x, y);
}

SnakeGrunt::SnakeGrunt(float x, float y, float width, float height) : SnakeEnemy(x, y, width, height)
{
    updateBounds();
}

void SnakeGrunt::update(float deltaTime)
{
    SnakeEnemy::update(deltaTime);
}

void SnakeGrunt::triggerHit()
{
    SnakeEnemy::triggerHit();
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
