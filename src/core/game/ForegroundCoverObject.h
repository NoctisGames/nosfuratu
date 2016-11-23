//
//  ForegroundCoverObject.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/23/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__ForegroundCoverObject__
#define __nosfuratu__ForegroundCoverObject__

#include "GridLockedPhysicalEntity.h"
#include "RTTI.h"

typedef enum
{
    ForegroundCoverObjectType_Tree,
    ForegroundCoverObjectType_Plant,
    ForegroundCoverObjectType_Bush,
    ForegroundCoverObjectType_Ferns
} ForegroundCoverObjectType;

class ForegroundCoverObject : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static ForegroundCoverObject* create(int gridX, int gridY, int type);
    
    ForegroundCoverObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundCoverObjectType type);
    
    ForegroundCoverObjectType getType();
    
private:
    ForegroundCoverObjectType m_type;
};

#endif /* defined(__nosfuratu__ForegroundCoverObject__) */
