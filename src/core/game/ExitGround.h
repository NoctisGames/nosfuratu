//
//  ExitGround.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__ExitGround__
#define __nosfuratu__ExitGround__

#include "GridLockedPhysicalEntity.h"
#include "GroundSoundType.h"
#include "Jon.h"
#include "ExitGroundCover.h"

typedef enum
{
    ExitGroundType_GrassWithCaveSmallExitMid,
    ExitGroundType_GrassWithCaveSmallExitEnd,
    ExitGroundType_CaveSmallExit,
    
    ExitGroundType_CaveDeepSmallWaterfall
} ExitGroundType;

class ExitGround : public GridLockedPhysicalEntity
{
public:
    static ExitGround* create(int gridX, int gridY, int type);
    
    ExitGround(int gridX, int gridY, int gridWidth, int gridHeight, float boundsHeight, bool hasCover, ExitGroundType type, GroundSoundType groundSoundType);
    
    virtual void update(float deltaTime);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    bool isJonBlockedAbove(Jon& jon, float deltaTime);
    
    virtual bool canObjectBePlacedOn();
    
    bool hasCover();
    
    ExitGroundCover& getExitCover();
    
    ExitGroundType getType();
    
    GroundSoundType getGroundSoundType();
    
private:
    ExitGroundCover *m_exitCover;
    ExitGroundType m_type;
    GroundSoundType m_groundSoundType;
};

class GrassWithCaveSmallExitMid : public ExitGround
{
public:
    GrassWithCaveSmallExitMid(int gridX) : ExitGround(gridX, 88, 32, 14, 0.57142857142857f, true, ExitGroundType_GrassWithCaveSmallExitMid, GROUND_SOUND_GRASS) {}
};

class GrassWithCaveSmallExitEnd : public ExitGround
{
public:
    GrassWithCaveSmallExitEnd(int gridX) : ExitGround(gridX, 88, 32, 14, 0.57142857142857f, true, ExitGroundType_GrassWithCaveSmallExitEnd, GROUND_SOUND_GRASS) {}
};

class CaveSmallExit : public ExitGround
{
public:
    CaveSmallExit(int gridX) : ExitGround(gridX, 48, 32, 24, 0.29166666666667f, true, ExitGroundType_CaveSmallExit, GROUND_SOUND_CAVE) {}
};

class CaveDeepSmallWaterfall : public ExitGround
{
public:
    CaveDeepSmallWaterfall(int gridX) : ExitGround(gridX, 0, 32, 28, 0.96428571428571f, false, ExitGroundType_CaveDeepSmallWaterfall, GROUND_SOUND_CAVE) {}
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool canObjectBePlacedOn();
};

#endif /* defined(__nosfuratu__ExitGround__) */
