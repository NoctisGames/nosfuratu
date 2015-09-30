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

#define CARROT_WIDTH 1.2163742690058479f

Carrot::Carrot(float x, float y) : PhysicalEntity(x + CARROT_WIDTH / 2, y, CARROT_WIDTH, 0.9592720970537262f)
{
    // Empty
}