//
//  Midground.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Midground__
#define __nosfuratu__Midground__

#include "GridLockedPhysicalEntity.h"
#include "RTTI.h"

typedef enum
{
    MidgroundType_TreeOne,
    MidgroundType_TreeTwo,
    MidgroundType_TreeThree,
    MidgroundType_DeepCaveColumnSmall,
    MidgroundType_DeepCaveColumnMedium,
    MidgroundType_DeepCaveColumnBig,
    MidgroundType_Plant,
    MidgroundType_Metal_Tower_Section,
    MidgroundType_Billboard_Count_Hiss,
    MidgroundType_Billboard_Slag_Town,
    MidgroundType_Billboard_Jon_Wanted
} MidgroundType;

class Midground : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static Midground* create(int gridX, int gridY, int type);
    
    Midground(int gridX, int gridY, int gridWidth, int gridHeight, MidgroundType type);
    
    MidgroundType getType();
    
private:
    MidgroundType m_type;
};

#endif /* defined(__nosfuratu__Midground__) */
