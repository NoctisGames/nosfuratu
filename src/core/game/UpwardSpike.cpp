//
//  UpwardSpike.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "UpwardSpike.h"

UpwardSpike* UpwardSpike::create(float x, float y, int type)
{
    switch ((UpwardSpikeType) type)
    {
        case UpwardSpikeType_MetalGrass:
            return new UpwardSpikeMetalGrass(x, y);
        case UpwardSpikeType_WoodGrass:
            return new UpwardSpikeWoodGrass(x, y);
        case UpwardSpikeType_MetalCave:
        default:
            return new UpwardSpikeMetalCave(x, y);
    }
}

UpwardSpike::UpwardSpike(float x, float y, float width, float height, UpwardSpikeType type) : PhysicalEntity(x, y, width, height), m_type(type)
{
    updateBounds();
}

void UpwardSpike::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.25f, 0);
    
    m_bounds->setWidth(getWidth() * 0.50f);
    m_bounds->setHeight(getHeight() * 0.66f);
}

UpwardSpikeType UpwardSpike::getEnumType()
{
    return m_type;
}

int UpwardSpike::getType()
{
    return m_type;
}