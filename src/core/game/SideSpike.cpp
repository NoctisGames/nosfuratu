//
//  SideSpike.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "SideSpike.h"

SideSpike* SideSpike::create(int gridX, int gridY, int type)
{
    return new SideSpike(gridX, gridY);
}

SideSpike::SideSpike(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    updateBounds();
}

void SideSpike::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.0625f, getHeight() * 0.5f);
    m_bounds->setWidth(getWidth() * 0.91666666666667f);
    m_bounds->setHeight(getHeight() * 0.38888888888889f);
}

int SideSpike::getType()
{
    return -1;
}