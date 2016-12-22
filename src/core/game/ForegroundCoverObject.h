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
#include "GroundSoundType.h"
#include "Jon.h"
#include "GameConstants.h"
#include "RTTI.h"

typedef enum
{
    ForegroundCoverObjectType_Tree,
    ForegroundCoverObjectType_Plant,
    ForegroundCoverObjectType_Bush,
    ForegroundCoverObjectType_Ferns,
    ForegroundCoverObjectType_Wall,
    ForegroundCoverObjectType_Wall_Bottom,
    ForegroundCoverObjectType_Wall_Window,
    ForegroundCoverObjectType_Wall_Window_Bottom,
    ForegroundCoverObjectType_Roof_Side_Left,
    ForegroundCoverObjectType_Roof_Side_Right,
    ForegroundCoverObjectType_Roof_Plain,
    ForegroundCoverObjectType_Roof_Chimney
} ForegroundCoverObjectType;

class ForegroundCoverObject : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static ForegroundCoverObject* create(int gridX, int gridY, int type);
    
    ForegroundCoverObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundCoverObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime) { return false; }
    
    virtual int getEntityLandingPriority() { return 0; }
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime) { return false; }
    
    virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime) { return false; }
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime) { return false; }
    
    virtual bool isJonHittingHorizontally(Jon& jon, float deltaTime) { return false; }
    
    virtual bool isJonHittingFromBelow(Jon& jon, float deltaTime) { return false; }
    
    virtual bool canObjectBePlacedOn() { return false; }
    
    ForegroundCoverObjectType getType() { return m_type; }
    
    GroundSoundType getGroundSoundType() { return m_groundSoundType; }
    
    void setGame(Game* game) { m_game = game; }
    
    Color getColor() { return m_color; }
    
protected:
    Game* m_game;
    Color m_color;
    
private:
    ForegroundCoverObjectType m_type;
    GroundSoundType m_groundSoundType;
};

class LandableForegroundCoverObject : public ForegroundCoverObject
{
    RTTI_DECL;
    
public:
    LandableForegroundCoverObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundCoverObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1) : ForegroundCoverObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight) {}
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

#endif /* defined(__nosfuratu__ForegroundCoverObject__) */
