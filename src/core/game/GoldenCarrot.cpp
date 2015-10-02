//
//  GoldenCarrot.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "GoldenCarrot.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"

GoldenCarrot::GoldenCarrot(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

GoldenCarrot GoldenCarrot::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    
    return GoldenCarrot(x, y, width, height);
}