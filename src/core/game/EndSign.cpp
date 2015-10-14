//
//  EndSign.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "EndSign.h"

EndSign* EndSign::create(float x, float y, int type)
{
    return new EndSign(x, y);
}

EndSign::EndSign(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

int EndSign::getType()
{
    return -1;
}