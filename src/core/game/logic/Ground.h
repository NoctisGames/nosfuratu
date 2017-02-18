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
#include "GroundSoundType.h"
#include "Jon.h"
#include "RTTI.h"

typedef enum
{
    GroundType_CaveExtraDeepEndLeft,
    GroundType_CaveExtraDeepSmall,
    GroundType_CaveExtraDeepMedium,
    GroundType_CaveExtraDeepLarge,
    GroundType_CaveExtraDeepEndRight,
    
    GroundType_CaveDeepEndLeft,
    GroundType_CaveDeepSmall,
    GroundType_CaveDeepMedium,
    GroundType_CaveDeepLarge,
    GroundType_CaveDeepEndRight,
    
    GroundType_CaveEndLeft,
    GroundType_CaveSmall,
    GroundType_CaveMedium,
    GroundType_CaveLarge,
    GroundType_CaveEndRight,
    
    GroundType_CaveRaisedEndLeft,
    GroundType_CaveRaisedSmall,
    GroundType_CaveRaisedMedium,
    GroundType_CaveRaisedLarge,
    GroundType_CaveRaisedEndRight,
    
    GroundType_GrassWithCaveEndLeft,
    GroundType_GrassWithCaveSmall,
    GroundType_GrassWithCaveMedium,
    GroundType_GrassWithCaveLarge,
    GroundType_GrassWithCaveEndRight,
    
    GroundType_GrassWithoutCaveEndLeft,
    GroundType_GrassWithoutCaveSmall,
    GroundType_GrassWithoutCaveMedium,
    GroundType_GrassWithoutCaveLarge,
    GroundType_GrassWithoutCaveEndRight,
    
    GroundType_GrassPitSmall,
    GroundType_GrassPitMedium,
    GroundType_GrassPitLarge,
    GroundType_GrassPitExtraLarge
} GroundType;

class Ground : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static Ground* create(int gridX, int gridY, int type);
    
    Ground(int gridX, int gridY, int gridWidth, int gridHeight, float boundsY, float boundsHeight, GroundType type, GroundSoundType groundSoundType);
    
    bool isEntityLanding(PhysicalEntity* entity, float deltaTime);

	virtual int getEntityLandingPriority() { return 0; }
    
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
    GrassPit(int gridX, int gridY, int gridWidth, int gridHeight, float boundsY, float boundsHeight, GroundType type, GroundSoundType groundSoundType) : Ground(gridX, gridY, gridWidth, gridHeight, boundsY, boundsHeight, type, groundSoundType) {}
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool canObjectBePlacedOn();
    
    virtual bool canObjectBePlacedUnder();
};

#endif /* defined(__nosfuratu__Ground__) */
