//
//  Thorns.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "Thorns.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "EntityUtils.h"

void Thorns::create(std::vector<Thorns>& items, float x, EntityAnchor anchor)
{
    items.push_back(Thorns(x, 0));
    
    EntityUtils::applyAnchor(items.at(items.size() - 1), anchor);
    
    items.at(items.size() - 1).updateBounds();
}

Thorns::Thorns(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    resetBounds(width * 0.50f, height * 0.50f);
}

Thorns Thorns::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    
    return Thorns(x, y, width, height);
}