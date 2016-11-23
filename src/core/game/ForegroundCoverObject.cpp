//
//  ForegroundCoverObject.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 11/23/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "ForegroundCoverObject.h"
#include "EntityUtils.h"

ForegroundCoverObject* ForegroundCoverObject::create(int gridX, int gridY, int type)
{
    ForegroundCoverObjectType fcot = (ForegroundCoverObjectType)type;
    switch (fcot)
    {
        case ForegroundCoverObjectType_Tree:
            return new ForegroundCoverObject(gridX, 85, 28, 51, fcot);
        case ForegroundCoverObjectType_Plant:
            return new ForegroundCoverObject(gridX, 92, 21, 24, fcot);
        case ForegroundCoverObjectType_Bush:
            return new ForegroundCoverObject(gridX, 92, 25, 15, fcot);
        case ForegroundCoverObjectType_Ferns:
            return new ForegroundCoverObject(gridX, 92, 33, 11, fcot);
    }
    
    assert(false);
}

ForegroundCoverObject::ForegroundCoverObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundCoverObjectType type) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight), m_type(type)
{
    updateBounds();
}

ForegroundCoverObjectType ForegroundCoverObject::getType()
{
    return m_type;
}

RTTI_IMPL(ForegroundCoverObject, GridLockedPhysicalEntity);
