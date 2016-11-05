//
//  GameMarker.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/26/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#include "GameMarker.h"

GameMarker* GameMarker::create(int gridX, int gridY, int type)
{
    return new GameMarker(gridX, type);
}

GameMarker::GameMarker(int gridX, int type) : GridLockedPhysicalEntity(gridX, 0, 2, 256),
m_type(type)
{
    // Empty
}

int GameMarker::getType()
{
    return m_type;
}

RTTI_IMPL(GameMarker, GridLockedPhysicalEntity);
