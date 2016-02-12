//
//  Midground.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#include "Midground.h"
#include "EntityUtils.h"

Midground* Midground::create(int gridX, int gridY, int type)
{
    switch ((MidgroundType)type)
    {
        case MidgroundType_TreeOne:
            return new TreeOne(gridX);
        case MidgroundType_TreeTwo:
            return new TreeTwo(gridX);
        case MidgroundType_TreeThree:
            return new TreeThree(gridX);
        case MidgroundType_DeepCaveColumnSmall:
            return new DeepCaveColumnSmall(gridX);
        case MidgroundType_DeepCaveColumnMedium:
            return new DeepCaveColumnMedium(gridX);
        case MidgroundType_DeepCaveColumnBig:
            return new DeepCaveColumnBig(gridX);
    }
    
    assert(false);
}

Midground::Midground(int gridX, int gridY, int gridWidth, int gridHeight, MidgroundType type) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight), m_type(type)
{
    updateBounds();
}

MidgroundType Midground::getType()
{
    return m_type;
}