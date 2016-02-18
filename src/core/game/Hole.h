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

typedef enum
{
    HoleType_Grass,
    HoleType_Cave
} HoleType;

class Hole : public GridLockedPhysicalEntity
{
public:
    static Hole* create(int gridX, int gridY, int type);
    
    Hole(int gridX, int gridY, int gridWidth, int gridHeight, HoleType type);
    
    virtual void update(float deltaTime);
    
    bool triggerBurrow();
    
    bool hasCover();
    
    HoleCover& getHoleCover();
    
    HoleType getType();
    
private:
    HoleCover* m_holeCover;
    HoleType m_type;
};

class HoleGrass : public Hole
{
public:
    HoleGrass(int gridX) : Hole(gridX, 80, 16, 16, HoleType_Grass) {}
};

class HoleCave : public Hole
{
public:
    HoleCave(int gridX) : Hole(gridX, 32, 17, 24, HoleType_Cave) {}
};

#endif /* defined(__nosfuratu__Hole__) */
