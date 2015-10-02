//
//  SideSpike.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "SideSpike.h"
#include "EntityUtils.h"

SideSpike::SideSpike(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    resetBounds(width * 0.40f, height * 0.40f);
}

SideSpike SideSpike::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    
    return SideSpike(x, y, width, height);
}