//
//  EndSign.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "EndSign.h"

EndSign* EndSign::create(float x, float y, int type)
{
    return new EndSign(x, y);
}

EndSign::EndSign(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

void EndSign::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(0, getHeight() * 0.04166666666667f);
    
    m_bounds->setHeight(getHeight() * 0.9375f);
}

int EndSign::getType()
{
    return -1;
}