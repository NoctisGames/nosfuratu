//
//  Ground.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Ground__
#define __nosfuratu__Ground__

#include "GridLockedPhysicalEntity.h"
#include "GroundSoundType.h"
#include "Jon.h"

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
    GroundType_GrassWithoutCaveEndRight
} GroundType;

class Ground : public GridLockedPhysicalEntity
{
public:
    static Ground* create(int gridX, int gridY, int type);
    
    Ground(int gridX, int gridY, int gridWidth, int gridHeight, float boundsY, float boundsHeight, GroundType type, GroundSoundType groundSoundType);
    
    bool isJonLanding(Jon& jon, float deltaTime);
    
    bool isJonBlockedOnRight(Jon& jon, float deltaTime);
    
    bool isJonBlockedAbove(Jon& jon, float deltaTime);
    
    bool canObjectBePlacedOn();
    
    bool canObjectBePlacedUnder();
    
    GroundType getType();
    
    GroundSoundType getGroundSoundType();
    
private:
    GroundType m_type;
    GroundSoundType m_groundSoundType;
};

class CaveExtraDeepEndLeft : public Ground
{
public:
    CaveExtraDeepEndLeft(int gridX) : Ground(gridX, 0, 8, 8, 0, 0.875f, GroundType_CaveExtraDeepEndLeft, GROUND_SOUND_CAVE) {}
};

class CaveExtraDeepSmall : public Ground
{
public:
    CaveExtraDeepSmall(int gridX) : Ground(gridX, 0, 32, 8, 0, 0.875f, GroundType_CaveExtraDeepSmall, GROUND_SOUND_CAVE) {}
};

class CaveExtraDeepMedium : public Ground
{
public:
    CaveExtraDeepMedium(int gridX) : Ground(gridX, 0, 64, 8, 0, 0.875f, GroundType_CaveExtraDeepMedium, GROUND_SOUND_CAVE) {}
};

class CaveExtraDeepLarge : public Ground
{
public:
    CaveExtraDeepLarge(int gridX) : Ground(gridX, 0, 128, 8, 0, 0.875f, GroundType_CaveExtraDeepLarge, GROUND_SOUND_CAVE) {}
};

class CaveExtraDeepEndRight : public Ground
{
public:
    CaveExtraDeepEndRight(int gridX) : Ground(gridX, 0, 8, 8, 0, 0.875f, GroundType_CaveExtraDeepEndRight, GROUND_SOUND_CAVE) {}
};

class CaveDeepEndLeft : public Ground
{
public:
    CaveDeepEndLeft(int gridX) : Ground(gridX, 20, 8, 8, 0, 0.875f, GroundType_CaveDeepEndLeft, GROUND_SOUND_CAVE) {}
};

class CaveDeepSmall : public Ground
{
public:
    CaveDeepSmall(int gridX) : Ground(gridX, 20, 32, 8, 0, 0.875f, GroundType_CaveDeepSmall, GROUND_SOUND_CAVE) {}
};

class CaveDeepMedium : public Ground
{
public:
    CaveDeepMedium(int gridX) : Ground(gridX, 20, 64, 8, 0, 0.875f, GroundType_CaveDeepMedium, GROUND_SOUND_CAVE) {}
};

class CaveDeepLarge : public Ground
{
public:
    CaveDeepLarge(int gridX) : Ground(gridX, 20, 128, 8, 0, 0.875f, GroundType_CaveDeepLarge, GROUND_SOUND_CAVE) {}
};

class CaveDeepEndRight : public Ground
{
public:
    CaveDeepEndRight(int gridX) : Ground(gridX, 20, 8, 8, 0, 0.875f, GroundType_CaveDeepEndRight, GROUND_SOUND_CAVE) {}
};

class CaveEndLeft : public Ground
{
public:
    CaveEndLeft(int gridX) : Ground(gridX, 48, 8, 26, 0.08333333333333f, 0.20833333333334f, GroundType_CaveEndLeft, GROUND_SOUND_CAVE) {}
};

class CaveSmall : public Ground
{
public:
    CaveSmall(int gridX) : Ground(gridX, 48, 32, 26, 0.08333333333333f, 0.20833333333334f, GroundType_CaveSmall, GROUND_SOUND_CAVE) {}
};

class CaveMedium : public Ground
{
public:
    CaveMedium(int gridX) : Ground(gridX, 48, 64, 26, 0.08333333333333f, 0.20833333333334f, GroundType_CaveMedium, GROUND_SOUND_CAVE) {}
};

class CaveLarge : public Ground
{
public:
    CaveLarge(int gridX) : Ground(gridX, 48, 128, 26, 0.08333333333333f, 0.20833333333334f, GroundType_CaveLarge, GROUND_SOUND_CAVE) {}
};

class CaveEndRight : public Ground
{
public:
    CaveEndRight(int gridX) : Ground(gridX, 48, 8, 26, 0.08333333333333f, 0.20833333333334f, GroundType_CaveEndRight, GROUND_SOUND_CAVE) {}
};

class CaveRaisedEndLeft : public Ground
{
public:
    CaveRaisedEndLeft(int gridX) : Ground(gridX, 48, 8, 32, 0, 0.84375f, GroundType_CaveRaisedEndLeft, GROUND_SOUND_CAVE) {}
};

class CaveRaisedSmall : public Ground
{
public:
    CaveRaisedSmall(int gridX) : Ground(gridX, 48, 32, 32, 0, 0.84375f, GroundType_CaveRaisedSmall, GROUND_SOUND_CAVE) {}
};

class CaveRaisedMedium : public Ground
{
public:
    CaveRaisedMedium(int gridX) : Ground(gridX, 48, 64, 32, 0, 0.84375f, GroundType_CaveRaisedMedium, GROUND_SOUND_CAVE) {}
};

class CaveRaisedLarge : public Ground
{
public:
    CaveRaisedLarge(int gridX) : Ground(gridX, 48, 128, 32, 0, 0.84375f, GroundType_CaveRaisedLarge, GROUND_SOUND_CAVE) {}
};

class CaveRaisedEndRight : public Ground
{
public:
    CaveRaisedEndRight(int gridX) : Ground(gridX, 48, 8, 32, 0, 0.84375f, GroundType_CaveRaisedEndRight, GROUND_SOUND_CAVE) {}
};

class GrassWithCaveEndLeft : public Ground
{
public:
    GrassWithCaveEndLeft(int gridX) : Ground(gridX, 88, 8, 14, 0.16666666666667f, 0.5f, GroundType_GrassWithCaveEndLeft, GROUND_SOUND_GRASS) {}
};

class GrassWithCaveSmall : public Ground
{
public:
    GrassWithCaveSmall(int gridX) : Ground(gridX, 88, 32, 14, 0.16666666666667f, 0.5f, GroundType_GrassWithCaveSmall, GROUND_SOUND_GRASS) {}
};

class GrassWithCaveMedium : public Ground
{
public:
    GrassWithCaveMedium(int gridX) : Ground(gridX, 88, 64, 14, 0.16666666666667f, 0.5f, GroundType_GrassWithCaveMedium, GROUND_SOUND_GRASS) {}
};

class GrassWithCaveLarge : public Ground
{
public:
    GrassWithCaveLarge(int gridX) : Ground(gridX, 88, 128, 14, 0.16666666666667f, 0.5f, GroundType_GrassWithCaveLarge, GROUND_SOUND_GRASS) {}
};

class GrassWithCaveEndRight : public Ground
{
public:
    GrassWithCaveEndRight(int gridX) : Ground(gridX, 88, 8, 14, 0.16666666666667f, 0.5f, GroundType_GrassWithCaveEndRight, GROUND_SOUND_GRASS) {}
};

class GrassWithoutCaveEndLeft : public Ground
{
public:
    GrassWithoutCaveEndLeft(int gridX) : Ground(gridX, 0, 8, 100, 0, 0.96f, GroundType_GrassWithoutCaveEndLeft, GROUND_SOUND_GRASS) {}
};

class GrassWithoutCaveSmall : public Ground
{
public:
    GrassWithoutCaveSmall(int gridX) : Ground(gridX, 0, 32, 100, 0, 0.96f, GroundType_GrassWithoutCaveSmall, GROUND_SOUND_GRASS) {}
};

class GrassWithoutCaveMedium : public Ground
{
public:
    GrassWithoutCaveMedium(int gridX) : Ground(gridX, 0, 64, 100, 0, 0.96f, GroundType_GrassWithoutCaveMedium, GROUND_SOUND_GRASS) {}
};

class GrassWithoutCaveLarge : public Ground
{
public:
    GrassWithoutCaveLarge(int gridX) : Ground(gridX, 0, 128, 100, 0, 0.96f, GroundType_GrassWithoutCaveLarge, GROUND_SOUND_GRASS) {}
};

class GrassWithoutCaveEndRight : public Ground
{
public:
    GrassWithoutCaveEndRight(int gridX) : Ground(gridX, 0, 8, 100, 0, 0.96f, GroundType_GrassWithoutCaveEndRight, GROUND_SOUND_GRASS) {}
};

#endif /* defined(__nosfuratu__Ground__) */
