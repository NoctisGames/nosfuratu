//
//  SnakeGrunt.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/28/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "Enemy.h"

SnakeGrunt::SnakeGrunt(int gridX, int gridY, int gridWidth, int gridHeight, EnemyType type, SpiritType spiritType) : Enemy(gridX, gridY, gridWidth, gridHeight, type, spiritType)
{
    updateBounds();
}

void SnakeGrunt::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    m_bounds->setWidth(getWidth() * 0.8f);
    m_bounds->setHeight(getHeight() * 0.8f);
    
    if (m_isDying)
    {
        // Bye bye!
        lowerLeft.setY(1337);
    }
}