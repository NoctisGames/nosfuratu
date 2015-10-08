//
//  SideSpike.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "SideSpike.h"
#include "EntityUtils.h"

void SideSpike::create(std::vector<SideSpike>& items, float x, float y)
{
    items.push_back(SideSpike(x, y));
    
    items.at(items.size() - 1).updateBounds();
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

SideSpike SideSpike::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    
    return SideSpike(x, y, width, height);
}