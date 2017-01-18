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
#include "RTTI.h"

typedef enum
{
    ExitGroundType_GrassWithCaveSmallExitMid,
    ExitGroundType_GrassWithCaveSmallExitEnd,
    ExitGroundType_CaveSmallExit,
    
    ExitGroundType_CaveDeepSmallWaterfall
} ExitGroundType;

class ExitGround : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static ExitGround* create(int gridX, int gridY, int type);
    
    ExitGround(int gridX, int gridY, int gridWidth, int gridHeight, float boundsHeight, bool hasCover, ExitGroundType type, GroundSoundType groundSoundType);
    
    virtual void update(float deltaTime);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

	virtual int getEntityLandingPriority() { return 0; }
    
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

class CaveDeepSmallWaterfall : public ExitGround
{
    RTTI_DECL;
    
public:
    CaveDeepSmallWaterfall(int gridX, int gridY, int gridWidth, int gridHeight, float boundsHeight, bool hasCover, ExitGroundType type, GroundSoundType groundSoundType) : ExitGround(gridX, gridY, gridWidth, gridHeight, boundsHeight, hasCover, type, groundSoundType) {}
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool canObjectBePlacedOn();
};

#endif /* defined(__nosfuratu__ExitGround__) */
