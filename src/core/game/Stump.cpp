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

Stump::Stump(float x, float y, float width, float height) : LandPhysicalEntity(x, y, width, height)
{
    updateBounds();
}

void Stump::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.25595238095238f, getHeight() * 0.04195804195804f);
    
    m_bounds->setWidth(getWidth() * 0.29166666666667f);
    m_bounds->setHeight(getHeight() * 0.78321678321678f);
}

int Stump::getType()
{
    return -1;
}

GroundSoundType Stump::getGroundSoundType()
{
    return GROUND_SOUND_GRASS;
}
