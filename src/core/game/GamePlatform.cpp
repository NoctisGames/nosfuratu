//
//  GamePlatform.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/4/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "GamePlatform.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"

GamePlatform::GamePlatform(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height, 0)
{
    resetBounds(width, height * 0.74489795918367f);
}