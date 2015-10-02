//
//  EndSign.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "EndSign.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "EntityUtils.h"

void EndSign::create(std::vector<EndSign>& items, float x, EntityAnchor anchor)
{
    items.push_back(EndSign(x, 0));
    
    EntityUtils::applyAnchor(items.at(items.size() - 1), anchor);
    
    items.at(items.size() - 1).updateBounds();
}

EndSign::EndSign(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

EndSign EndSign::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    
    return EndSign(x, y, width, height);
}