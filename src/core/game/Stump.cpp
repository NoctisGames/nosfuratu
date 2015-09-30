//
//  Stump.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "Stump.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "EntityUtils.h"

#define STUMP_WIDTH 3.9298245614035086f
#define STUMP_HEIGHT 3.3457538994800693f

Stump::Stump(float x, EntityAnchor anchor) : PhysicalEntity(x + STUMP_WIDTH / 2, 0, STUMP_WIDTH, STUMP_HEIGHT)
{
    EntityUtils::applyAnchor(*this, anchor);
    
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