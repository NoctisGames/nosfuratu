//
//  ForegroundCoverObject.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/23/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__ForegroundCoverObject__
#define __nosfuratu__ForegroundCoverObject__

#include "GridLockedPhysicalEntity.h"

#include "ForegroundCoverObjectType.h"
#include "GroundSoundType.h"
#include "Color.h"

#include "RTTI.h"

class Jon;
class Game;

class ForegroundCoverObject : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static ForegroundCoverObject* create(int gridX, int gridY, int type);
    
    ForegroundCoverObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundCoverObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual int getEntityLandingPriority();
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingHorizontally(Jon& jon, float deltaTime);
    
    virtual bool isJonHittingFromBelow(Jon& jon, float deltaTime);
    
    virtual bool canObjectBePlacedOn();
    
    ForegroundCoverObjectType getType();
    
    GroundSoundType getGroundSoundType();
    
    void setGame(Game* game);
    
    Color getColor();
    
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
    LandableForegroundCoverObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundCoverObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isJonBlockedAbove(Jon& jon, float deltaTime);
};

#endif /* defined(__nosfuratu__ForegroundCoverObject__) */
