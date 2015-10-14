//
//  Carrot.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/4/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Carrot.h"

Carrot* Carrot::create(float x, float y, int type)
{
    return new Carrot(x, y);
}

Carrot::Carrot(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    updateBounds();
}

int Carrot::getType()
{
    return -1;
}