//
//  Hole.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Hole__
#define __nosfuratu__Hole__

#include "GridLockedPhysicalEntity.h"
#include "HoleCover.h"
#include "RTTI.h"

typedef enum
{
    HoleType_GrassTileLeft,
    HoleType_GrassTileCenter,
    HoleType_GrassTileRight,
    HoleType_Grass,
    HoleType_Cave
} HoleType;

class Hole : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static Hole* create(int gridX, int gridY, int type);
    
    Hole(int gridX, int gridY, int gridWidth, int gridHeight, HoleType type, HoleCoverType holeCoverType);
    
    virtual void update(float deltaTime);
    
    bool triggerBurrow();
    
    bool hasCover();
    
    bool isCoverBreaking();
    
    HoleCover& getHoleCover();
    
    HoleType getType();
    
private:
    HoleCover* m_holeCover;
    HoleType m_type;
};

#endif /* defined(__nosfuratu__Hole__) */
