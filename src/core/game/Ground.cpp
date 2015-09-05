//
//  Ground.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Ground.h"

Ground::Ground(float x, float y, float width, float height, int groundType) : PhysicalEntity(x, y, width, height, 0), m_iGroundType(groundType)
{
    resetBounds(width, height * 0.77116704805492f);
}

int Ground::getGroundType()
{
    return m_iGroundType;
}