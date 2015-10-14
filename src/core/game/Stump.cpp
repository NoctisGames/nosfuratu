//
//  Stump.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "Stump.h"

Stump* Stump::create(float x, float y, int type)
{
    return new Stump(x, y);
}

Stump::Stump(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    updateBounds();
}

void Stump::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.24404761904762f, 0);
    
    m_bounds->setWidth(getWidth() * 0.29761904761905f);
    m_bounds->setHeight(getHeight() * 0.81118881118881f);
}

int Stump::getType()
{
    return -1;
}