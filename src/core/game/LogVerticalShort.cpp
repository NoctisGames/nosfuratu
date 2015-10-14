//
//  LogVerticalShort.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "LogVerticalShort.h"

LogVerticalShort* LogVerticalShort::create(float x, float y, int type)
{
    return new LogVerticalShort(x, y);
}

LogVerticalShort::LogVerticalShort(float x, float y, float width, float height) : DestructiblePhysicalEntity(x, y, width, height)
{
    updateBounds();
}

void LogVerticalShort::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.18461538461538f, 0);
    
    m_bounds->setWidth(getWidth() * 0.43076923076923f);
    m_bounds->setHeight(getHeight() * 0.90384615384615f);
}

int LogVerticalShort::getType()
{
    return -1;
}