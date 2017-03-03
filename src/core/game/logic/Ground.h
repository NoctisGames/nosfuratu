//
//  Ground.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Ground__
#define __nosfuratu__Ground__

#include "GridLockedPhysicalEntity.h"

#include "GroundType.h"
#include "GroundSoundType.h"

#include "RTTI.h"

class Jon;

class Ground : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static Ground* create(int gridX, int gridY, int type);
    
    Ground(int gridX, int gridY, int gridWidth, int gridHeight, float boundsY, float boundsHeight, GroundType type, GroundSoundType groundSoundType);
    
    bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

    virtual int getEntityLandingPriority();
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    bool isJonBlockedAbove(Jon& jon, float deltaTime);
    
    virtual bool canObjectBePlacedOn();
    
    virtual bool canObjectBePlacedUnder();
    
    GroundType getType();
    
    GroundSoundType getGroundSoundType();
    
private:
    GroundType m_type;
    GroundSoundType m_groundSoundType;
};

class GrassPit : public Ground
{
    RTTI_DECL;
    
public:
    GrassPit(int gridX, int gridY, int gridWidth, int gridHeight, float boundsY, float boundsHeight, GroundType type, GroundSoundType groundSoundType);
    
    bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool canObjectBePlacedOn();
    
    virtual bool canObjectBePlacedUnder();
};

class PitTunnel : public Ground
{
    RTTI_DECL;
    
public:
    PitTunnel(int gridX, int gridY, int gridWidth, int gridHeight, float boundsY, float boundsHeight, GroundType type, GroundSoundType groundSoundType);
    
    bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual int getEntityLandingPriority();
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);
    
    virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool canObjectBePlacedOn();
    
    virtual bool canObjectBePlacedUnder();
};

#endif /* defined(__nosfuratu__Ground__) */
