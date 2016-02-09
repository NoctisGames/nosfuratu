//
//  ExitGround.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__ExitGround__
#define __nosfuratu__ExitGround__

#include "GridLockedPhysicalEntity.h"
#include "GroundSoundType.h"

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
    
    ExitGround(int gridX, int gridY, int gridWidth, int gridHeight, float boundsHeightFactor, ExitGroundType type, GroundSoundType groundSoundType);
    
    virtual void updateBounds();
    
    ExitGroundType getType();
    
    GroundSoundType getGroundSoundType();
    
private:
    ExitGroundType m_type;
    GroundSoundType m_groundSoundType;
    float m_fBoundsHeightFactor;
};

class GrassWithCaveSmallExitMid : public ExitGround
{
public:
    GrassWithCaveSmallExitMid(int gridX, int gridY = 88, int gridWidth = 32, int gridHeight = 14, float boundsHeightFactor = 0.71428571428571f, ExitGroundType type = ExitGroundType_GrassWithCaveSmallExitMid, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : ExitGround(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class GrassWithCaveSmallExitEnd : public ExitGround
{
public:
    GrassWithCaveSmallExitEnd(int gridX, int gridY = 88, int gridWidth = 32, int gridHeight = 14, float boundsHeightFactor = 0.71428571428571f, ExitGroundType type = ExitGroundType_GrassWithCaveSmallExitEnd, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : ExitGround(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveSmallExit : public ExitGround
{
public:
    CaveSmallExit(int gridX, int gridY = 48, int gridWidth = 32, int gridHeight = 24, float boundsHeightFactor = 0.29166666666667f, ExitGroundType type = ExitGroundType_CaveSmallExit, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : ExitGround(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveDeepSmallWaterfall : public ExitGround
{
public:
    CaveDeepSmallWaterfall(int gridX, int gridY = 0, int gridWidth = 32, int gridHeight = 28, float boundsHeightFactor = 0.96428571428571f, ExitGroundType type = ExitGroundType_CaveDeepSmallWaterfall, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : ExitGround(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

#endif /* defined(__nosfuratu__ExitGround__) */
