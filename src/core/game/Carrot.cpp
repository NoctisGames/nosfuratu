//
//  Carrot.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/4/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Carrot.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"

Carrot::Carrot(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

Carrot Carrot::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    
    return Carrot(x, y, width, height);
}