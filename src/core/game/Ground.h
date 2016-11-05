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

class CaveExtraDeepEndLeft : public Ground
{
    RTTI_DECL;
    
public:
    CaveExtraDeepEndLeft(int gridX) : Ground(gridX, 0, 8, 8, 0, 0.875f, GroundType_CaveExtraDeepEndLeft, GROUND_SOUND_CAVE) {}
};

class CaveExtraDeepSmall : public Ground
{
    RTTI_DECL;
    
public:
    CaveExtraDeepSmall(int gridX) : Ground(gridX, 0, 32, 8, 0, 0.875f, GroundType_CaveExtraDeepSmall, GROUND_SOUND_CAVE) {}
};

class CaveExtraDeepMedium : public Ground
{
    RTTI_DECL;
    
public:
    CaveExtraDeepMedium(int gridX) : Ground(gridX, 0, 64, 8, 0, 0.875f, GroundType_CaveExtraDeepMedium, GROUND_SOUND_CAVE) {}
};

class CaveExtraDeepLarge : public Ground
{
    RTTI_DECL;
    
public:
    CaveExtraDeepLarge(int gridX) : Ground(gridX, 0, 128, 8, 0, 0.875f, GroundType_CaveExtraDeepLarge, GROUND_SOUND_CAVE) {}
};

class CaveExtraDeepEndRight : public Ground
{
    RTTI_DECL;
    
public:
    CaveExtraDeepEndRight(int gridX) : Ground(gridX, 0, 8, 8, 0, 0.875f, GroundType_CaveExtraDeepEndRight, GROUND_SOUND_CAVE) {}
};

class CaveDeepEndLeft : public Ground
{
    RTTI_DECL;
    
public:
    CaveDeepEndLeft(int gridX) : Ground(gridX, 20, 8, 8, 0, 0.875f, GroundType_CaveDeepEndLeft, GROUND_SOUND_CAVE) {}
};

class CaveDeepSmall : public Ground
{
    RTTI_DECL;
    
public:
    CaveDeepSmall(int gridX) : Ground(gridX, 20, 32, 8, 0, 0.875f, GroundType_CaveDeepSmall, GROUND_SOUND_CAVE) {}
};

class CaveDeepMedium : public Ground
{
    RTTI_DECL;
    
public:
    CaveDeepMedium(int gridX) : Ground(gridX, 20, 64, 8, 0, 0.875f, GroundType_CaveDeepMedium, GROUND_SOUND_CAVE) {}
};

class CaveDeepLarge : public Ground
{
    RTTI_DECL;
    
public:
    CaveDeepLarge(int gridX) : Ground(gridX, 20, 128, 8, 0, 0.875f, GroundType_CaveDeepLarge, GROUND_SOUND_CAVE) {}
};

class CaveDeepEndRight : public Ground
{
    RTTI_DECL;
    
public:
    CaveDeepEndRight(int gridX) : Ground(gridX, 20, 8, 8, 0, 0.875f, GroundType_CaveDeepEndRight, GROUND_SOUND_CAVE) {}
};

class CaveEndLeft : public Ground
{
    RTTI_DECL;
    
public:
    CaveEndLeft(int gridX) : Ground(gridX, 50, 8, 24, 0.08333333333333f, 0.20833333333334f, GroundType_CaveEndLeft, GROUND_SOUND_CAVE) {}
};

class CaveSmall : public Ground
{
    RTTI_DECL;
    
public:
    CaveSmall(int gridX) : Ground(gridX, 50, 32, 24, 0.08333333333333f, 0.20833333333334f, GroundType_CaveSmall, GROUND_SOUND_CAVE) {}
};

class CaveMedium : public Ground
{
    RTTI_DECL;
    
public:
    CaveMedium(int gridX) : Ground(gridX, 50, 64, 24, 0.08333333333333f, 0.20833333333334f, GroundType_CaveMedium, GROUND_SOUND_CAVE) {}
};

class CaveLarge : public Ground
{
    RTTI_DECL;
    
public:
    CaveLarge(int gridX) : Ground(gridX, 50, 128, 24, 0.08333333333333f, 0.20833333333334f, GroundType_CaveLarge, GROUND_SOUND_CAVE) {}
};

class CaveEndRight : public Ground
{
    RTTI_DECL;
    
public:
    CaveEndRight(int gridX) : Ground(gridX, 50, 8, 24, 0.08333333333333f, 0.20833333333334f, GroundType_CaveEndRight, GROUND_SOUND_CAVE) {}
};

class CaveRaisedEndLeft : public Ground
{
    RTTI_DECL;
    
public:
    CaveRaisedEndLeft(int gridX) : Ground(gridX, 48, 8, 32, 0, 0.84375f, GroundType_CaveRaisedEndLeft, GROUND_SOUND_CAVE) {}
};

class CaveRaisedSmall : public Ground
{
    RTTI_DECL;
    
public:
    CaveRaisedSmall(int gridX) : Ground(gridX, 48, 32, 32, 0, 0.84375f, GroundType_CaveRaisedSmall, GROUND_SOUND_CAVE) {}
};

class CaveRaisedMedium : public Ground
{
    RTTI_DECL;
    
public:
    CaveRaisedMedium(int gridX) : Ground(gridX, 48, 64, 32, 0, 0.84375f, GroundType_CaveRaisedMedium, GROUND_SOUND_CAVE) {}
};

class CaveRaisedLarge : public Ground
{
    RTTI_DECL;
    
public:
    CaveRaisedLarge(int gridX) : Ground(gridX, 48, 128, 32, 0, 0.84375f, GroundType_CaveRaisedLarge, GROUND_SOUND_CAVE) {}
};

class CaveRaisedEndRight : public Ground
{
    RTTI_DECL;
    
public:
    CaveRaisedEndRight(int gridX) : Ground(gridX, 48, 8, 32, 0, 0.84375f, GroundType_CaveRaisedEndRight, GROUND_SOUND_CAVE) {}
};

class GrassWithCaveEndLeft : public Ground
{
    RTTI_DECL;
    
public:
    GrassWithCaveEndLeft(int gridX) : Ground(gridX, 90, 8, 12, 0.16666666666667f, 0.5f, GroundType_GrassWithCaveEndLeft, GROUND_SOUND_GRASS) {}
};

class GrassWithCaveSmall : public Ground
{
    RTTI_DECL;
    
public:
    GrassWithCaveSmall(int gridX) : Ground(gridX, 90, 32, 12, 0.16666666666667f, 0.5f, GroundType_GrassWithCaveSmall, GROUND_SOUND_GRASS) {}
};

class GrassWithCaveMedium : public Ground
{
    RTTI_DECL;
    
public:
    GrassWithCaveMedium(int gridX) : Ground(gridX, 90, 64, 12, 0.16666666666667f, 0.5f, GroundType_GrassWithCaveMedium, GROUND_SOUND_GRASS) {}
};

class GrassWithCaveLarge : public Ground
{
    RTTI_DECL;
    
public:
    GrassWithCaveLarge(int gridX) : Ground(gridX, 90, 128, 12, 0.16666666666667f, 0.5f, GroundType_GrassWithCaveLarge, GROUND_SOUND_GRASS) {}
};

class GrassWithCaveEndRight : public Ground
{
    RTTI_DECL;
    
public:
    GrassWithCaveEndRight(int gridX) : Ground(gridX, 90, 8, 12, 0.16666666666667f, 0.5f, GroundType_GrassWithCaveEndRight, GROUND_SOUND_GRASS) {}
};

class GrassWithoutCaveEndLeft : public Ground
{
    RTTI_DECL;
    
public:
    GrassWithoutCaveEndLeft(int gridX) : Ground(gridX, 0, 8, 100, 0, 0.96f, GroundType_GrassWithoutCaveEndLeft, GROUND_SOUND_GRASS) {}
};

class GrassWithoutCaveSmall : public Ground
{
    RTTI_DECL;
    
public:
    GrassWithoutCaveSmall(int gridX) : Ground(gridX, 0, 32, 100, 0, 0.96f, GroundType_GrassWithoutCaveSmall, GROUND_SOUND_GRASS) {}
};

class GrassWithoutCaveMedium : public Ground
{
    RTTI_DECL;
    
public:
    GrassWithoutCaveMedium(int gridX) : Ground(gridX, 0, 64, 100, 0, 0.96f, GroundType_GrassWithoutCaveMedium, GROUND_SOUND_GRASS) {}
};

class GrassWithoutCaveLarge : public Ground
{
    RTTI_DECL;
    
public:
    GrassWithoutCaveLarge(int gridX) : Ground(gridX, 0, 128, 100, 0, 0.96f, GroundType_GrassWithoutCaveLarge, GROUND_SOUND_GRASS) {}
};

class GrassWithoutCaveEndRight : public Ground
{
    RTTI_DECL;
    
public:
    GrassWithoutCaveEndRight(int gridX) : Ground(gridX, 0, 8, 100, 0, 0.96f, GroundType_GrassWithoutCaveEndRight, GROUND_SOUND_GRASS) {}
};

class GrassPit : public Ground
{
    RTTI_DECL;
    
public:
    GrassPit(int gridX, int gridWidth, GroundType type) : Ground(gridX, 0, gridWidth, 100, 0, 0.96f, type, GROUND_SOUND_NONE) {}
    
    virtual bool isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime);

	virtual bool isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime);
    
    virtual bool canObjectBePlacedOn();
    
    virtual bool canObjectBePlacedUnder();
};

class GrassPitSmall : public GrassPit
{
    RTTI_DECL;
    
public:
    GrassPitSmall(int gridX) : GrassPit(gridX, 32, GroundType_GrassPitSmall) {}
};

class GrassPitMedium : public GrassPit
{
    RTTI_DECL;
    
public:
    GrassPitMedium(int gridX) : GrassPit(gridX, 64, GroundType_GrassPitMedium) {}
};

class GrassPitLarge : public GrassPit
{
    RTTI_DECL;
    
public:
    GrassPitLarge(int gridX) : GrassPit(gridX, 128, GroundType_GrassPitLarge) {}
};

class GrassPitExtraLarge : public GrassPit
{
    RTTI_DECL;
    
public:
    GrassPitExtraLarge(int gridX) : GrassPit(gridX, 256, GroundType_GrassPitExtraLarge) {}
};

#endif /* defined(__nosfuratu__Ground__) */
