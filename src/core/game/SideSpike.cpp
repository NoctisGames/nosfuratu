//
//  SideSpike.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "SideSpike.h"

SideSpike* SideSpike::create(float x, float y, int type)
{
    return new SideSpike(x, y);
}

SideSpike::SideSpike(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    updateBounds();
}

void SideSpike::updateBounds()
{
    m_bounds->setHeight(getHeight());
    
    PhysicalEntity::updateBounds();
    
    m_bounds->setHeight(getHeight() * 0.60f);
}

int SideSpike::getType()
{
    return -1;
}