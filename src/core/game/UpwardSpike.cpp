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
    
    switch (m_type)
    {
        case UpwardSpikeType_MetalGrass:
            lowerLeft.add(getWidth() * 0.28571428571429f, getHeight() * 0.06818181818182f);
            m_bounds->setWidth(getWidth() * 0.28571428571429f);
            m_bounds->setHeight(getHeight() * 0.88636363636364f);
            break;
        case UpwardSpikeType_WoodGrass:
            lowerLeft.add(getWidth() * 0.31707317073171f, getHeight() * 0.06521739130435f);
            m_bounds->setWidth(getWidth() * 0.29268292682927f);
            m_bounds->setHeight(getHeight() * 0.8695652173913f);
            break;
        case UpwardSpikeType_MetalCave:
            lowerLeft.add(getWidth() * 0.11111111111111f, getHeight() * 0.02083333333333f);
            m_bounds->setWidth(getWidth() * 0.38888888888889f);
            m_bounds->setHeight(getHeight() * 0.89583333333333f);
            break;
        default:
            break;
    }
}

UpwardSpikeType UpwardSpike::getEnumType()
{
    return m_type;
}

int UpwardSpike::getType()
{
    return m_type;
}