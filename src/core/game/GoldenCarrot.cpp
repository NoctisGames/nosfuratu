//
//  GoldenCarrot.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "GoldenCarrot.h"

GoldenCarrot* GoldenCarrot::create(float x, float y, int type)
{
    return new GoldenCarrot(x, y);
}

GoldenCarrot::GoldenCarrot(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    updateBounds();
}

int GoldenCarrot::getType()
{
    return -1;
}