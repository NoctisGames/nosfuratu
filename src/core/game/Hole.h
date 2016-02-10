//
//  Hole.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
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
    
    void triggerBurrow();
    
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
    HoleGrass(int gridX, int gridY = 80, int gridWidth = 16, int gridHeight = 16, HoleType type = HoleType_Grass) : Hole(gridX, gridY, gridWidth, gridHeight, type) {}
};

class HoleCave : public Hole
{
public:
    HoleCave(int gridX, int gridY = 32, int gridWidth = 17, int gridHeight = 24, HoleType type = HoleType_Cave) : Hole(gridX, gridY, gridWidth, gridHeight, type) {}
};

#endif /* defined(__nosfuratu__Hole__) */
