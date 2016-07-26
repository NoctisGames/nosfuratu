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
    HoleType_GrassTileLeft,
    HoleType_GrassTileCenter,
    HoleType_GrassTileRight,
    HoleType_Grass,
    HoleType_Cave
} HoleType;

class Hole : public GridLockedPhysicalEntity
{
public:
    static Hole* create(int gridX, int gridY, int type);
    
    Hole(int gridX, int gridY, int gridWidth, int gridHeight, HoleType type, HoleCoverType holeCoverType);
    
    virtual void update(float deltaTime);
    
    bool triggerBurrow(float jonDownwardsVelocity);
    
    bool hasCover();
    
    bool isCoverBreaking();
    
    HoleCover& getHoleCover();
    
    HoleType getType();
    
private:
    HoleCover* m_holeCover;
    HoleType m_type;
};

class HoleGrassTileLeft : public Hole
{
public:
    HoleGrassTileLeft(int gridX) : Hole(gridX, 73, 16, 23, HoleType_GrassTileLeft, HoleCoverType_GrassTileLeft) {}
};

class HoleGrassTileCenter : public Hole
{
public:
    HoleGrassTileCenter(int gridX) : Hole(gridX, 73, 16, 23, HoleType_GrassTileCenter, HoleCoverType_GrassTileCenter) {}
};

class HoleGrassTileRight : public Hole
{
public:
    HoleGrassTileRight(int gridX) : Hole(gridX, 73, 16, 23, HoleType_GrassTileRight, HoleCoverType_GrassTileRight) {}
};

class HoleGrass : public Hole
{
public:
    HoleGrass(int gridX) : Hole(gridX, 80, 16, 16, HoleType_Grass, HoleCoverType_Grass) {}
};

class HoleCave : public Hole
{
public:
    HoleCave(int gridX) : Hole(gridX, 32, 17, 24, HoleType_Cave, HoleCoverType_Cave) {}
};

#endif /* defined(__nosfuratu__Hole__) */
