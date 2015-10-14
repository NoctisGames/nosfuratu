//
//  LogVerticalTall.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "LogVerticalTall.h"

LogVerticalTall* LogVerticalTall::create(float x, float y, int type)
{
    return new LogVerticalTall(x, y);
}

LogVerticalTall::LogVerticalTall(float x, float y, float width, float height) : DestructiblePhysicalEntity(x, y, width, height)
{
    updateBounds();
}

void LogVerticalTall::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.18461538461538f, 0);
    
    m_bounds->setWidth(getWidth() * 0.43076923076923f);
    m_bounds->setHeight(getHeight() * 0.90384615384615f);
}

int LogVerticalTall::getType()
{
    return -1;
}