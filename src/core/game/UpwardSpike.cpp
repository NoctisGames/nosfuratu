//
//  UpwardSpike.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "UpwardSpike.h"
#include "EntityUtils.h"

#define upwardSpikeTypeKey "upwardSpikeType"

void UpwardSpike::create(std::vector<UpwardSpike>& items, float x, float y, UpwardSpikeType type)
{
    switch (type)
    {
        case UpwardSpikeType::UPWARD_SPIKE_METAL_GRASS:
            items.push_back(UpwardSpike(x, y, 0.6549707602339181f, 1.0294627383015598f, type));
            break;
        case UpwardSpikeType::UPWARD_SPIKE_WOOD_GRASS:
            items.push_back(UpwardSpike(x, y, 0.9590643274853801f, 1.0762564991334487f, type));
            break;
        case UpwardSpikeType::UPWARD_SPIKE_METAL_CAVE:
        default:
            items.push_back(UpwardSpike(x, y, 0.42105263157894735f, 1.123050259965338f, type));
            break;
    }
}

UpwardSpike::UpwardSpike(float x, float y, float width, float height, UpwardSpikeType type) : PhysicalEntity(x, y, width, height), m_upwardSpikeType(type)
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

UpwardSpikeType UpwardSpike::getUpwardSpikeType()
{
    return m_upwardSpikeType;
}

UpwardSpike UpwardSpike::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    UpwardSpikeType type = (UpwardSpikeType) v[upwardSpikeTypeKey].GetInt();
    
    return UpwardSpike(x, y, width, height, type);
}

void UpwardSpike::serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w)
{
    w.String(upwardSpikeTypeKey);
    w.Int(getUpwardSpikeType());
}