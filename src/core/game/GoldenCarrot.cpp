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

#define GOLDEN_CARROT_WIDTH 1.21637426900585f

GoldenCarrot::GoldenCarrot(float x, float y) : PhysicalEntity(x + GOLDEN_CARROT_WIDTH / 2, y, GOLDEN_CARROT_WIDTH, 1.14644714038128f)
{
    // Empty
}