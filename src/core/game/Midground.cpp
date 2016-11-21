//
//  Midground.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "Midground.h"
#include "EntityUtils.h"

Midground* Midground::create(int gridX, int gridY, int type)
{
    MidgroundType mt = (MidgroundType)type;
    switch (mt)
    {
        case MidgroundType_TreeOne:
            return new Midground(gridX, 96, 62, 54, mt);
        case MidgroundType_TreeTwo:
        case MidgroundType_TreeThree:
            return new Midground(gridX, 96, 70, 50, mt);
        case MidgroundType_DeepCaveColumnSmall:
            return new Midground(gridX, 0, 6, 18, mt);
        case MidgroundType_DeepCaveColumnMedium:
            return new Midground(gridX, 0, 8, 52, mt);
        case MidgroundType_DeepCaveColumnBig:
            return new Midground(gridX, 0, 16, 52, mt);
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

RTTI_IMPL(Midground, GridLockedPhysicalEntity);
