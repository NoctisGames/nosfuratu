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

GoldenCarrot::GoldenCarrot(float x, float y) : PhysicalEntity(x + CARROT_WIDTH / 2, y, CARROT_WIDTH, 0.9592720970537262f)
{
    // Empty
}