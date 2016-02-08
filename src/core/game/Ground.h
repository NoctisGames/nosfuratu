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
} GroundType;

class Ground : public GridLockedPhysicalEntity
{
public:
    static Ground* create(int gridX, int gridY, int type);
    
    Ground(int gridX, int gridY, int gridWidth, int gridHeight, float boundsHeightFactor, GroundType type, GroundSoundType groundSoundType);
    
    virtual void updateBounds();
    
    GroundType getType();
    
    GroundSoundType getGroundSoundType();
    
private:
    GroundType m_type;
    GroundSoundType m_groundSoundType;
    float m_fBoundsHeightFactor;
};

class CaveExtraDeepEndLeft : public Ground
{
public:
    CaveExtraDeepEndLeft(int gridX, int gridY = 0, int gridWidth = 8, int gridHeight = 8, float boundsHeightFactor = 0.875f, GroundType type = GroundType_CaveExtraDeepEndLeft, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveExtraDeepSmall : public Ground
{
public:
    CaveExtraDeepSmall(int gridX, int gridY = 0, int gridWidth = 32, int gridHeight = 8, float boundsHeightFactor = 0.875f, GroundType type = GroundType_CaveExtraDeepSmall, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveExtraDeepMedium : public Ground
{
public:
    CaveExtraDeepMedium(int gridX, int gridY = 0, int gridWidth = 64, int gridHeight = 8, float boundsHeightFactor = 0.875f, GroundType type = GroundType_CaveExtraDeepMedium, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveExtraDeepLarge : public Ground
{
public:
    CaveExtraDeepLarge(int gridX, int gridY = 0, int gridWidth = 128, int gridHeight = 8, float boundsHeightFactor = 0.875f, GroundType type = GroundType_CaveExtraDeepLarge, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveExtraDeepEndRight : public Ground
{
public:
    CaveExtraDeepEndRight(int gridX, int gridY = 0, int gridWidth = 8, int gridHeight = 8, float boundsHeightFactor = 0.875f, GroundType type = GroundType_CaveExtraDeepEndRight, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveDeepEndLeft : public Ground
{
public:
    CaveDeepEndLeft(int gridX, int gridY = 20, int gridWidth = 8, int gridHeight = 8, float boundsHeightFactor = 0.875f, GroundType type = GroundType_CaveDeepEndLeft, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveDeepSmall : public Ground
{
public:
    CaveDeepSmall(int gridX, int gridY = 20, int gridWidth = 32, int gridHeight = 8, float boundsHeightFactor = 0.875f, GroundType type = GroundType_CaveDeepSmall, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveDeepMedium : public Ground
{
public:
    CaveDeepMedium(int gridX, int gridY = 20, int gridWidth = 64, int gridHeight = 8, float boundsHeightFactor = 0.875f, GroundType type = GroundType_CaveDeepMedium, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveDeepLarge : public Ground
{
public:
    CaveDeepLarge(int gridX, int gridY = 20, int gridWidth = 128, int gridHeight = 8, float boundsHeightFactor = 0.875f, GroundType type = GroundType_CaveDeepLarge, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveDeepEndRight : public Ground
{
public:
    CaveDeepEndRight(int gridX, int gridY = 20, int gridWidth = 8, int gridHeight = 8, float boundsHeightFactor = 0.875f, GroundType type = GroundType_CaveDeepEndRight, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveEndLeft : public Ground
{
public:
    CaveEndLeft(int gridX, int gridY = 48, int gridWidth = 8, int gridHeight = 24, float boundsHeightFactor = 0.29166666666667f, GroundType type = GroundType_CaveEndLeft, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveSmall : public Ground
{
public:
    CaveSmall(int gridX, int gridY = 48, int gridWidth = 32, int gridHeight = 24, float boundsHeightFactor = 0.29166666666667f, GroundType type = GroundType_CaveSmall, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveMedium : public Ground
{
public:
    CaveMedium(int gridX, int gridY = 48, int gridWidth = 64, int gridHeight = 24, float boundsHeightFactor = 0.29166666666667f, GroundType type = GroundType_CaveMedium, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveLarge : public Ground
{
public:
    CaveLarge(int gridX, int gridY = 48, int gridWidth = 128, int gridHeight = 24, float boundsHeightFactor = 0.29166666666667f, GroundType type = GroundType_CaveLarge, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveEndRight : public Ground
{
public:
    CaveEndRight(int gridX, int gridY = 48, int gridWidth = 8, int gridHeight = 24, float boundsHeightFactor = 0.29166666666667f, GroundType type = GroundType_CaveEndRight, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveRaisedEndLeft : public Ground
{
public:
    CaveRaisedEndLeft(int gridX, int gridY = 48, int gridWidth = 8, int gridHeight = 32, float boundsHeightFactor = 0.84375f, GroundType type = GroundType_CaveRaisedEndLeft, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveRaisedSmall : public Ground
{
public:
    CaveRaisedSmall(int gridX, int gridY = 48, int gridWidth = 32, int gridHeight = 32, float boundsHeightFactor = 0.84375f, GroundType type = GroundType_CaveRaisedSmall, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveRaisedMedium : public Ground
{
public:
    CaveRaisedMedium(int gridX, int gridY = 48, int gridWidth = 64, int gridHeight = 32, float boundsHeightFactor = 0.84375f, GroundType type = GroundType_CaveRaisedMedium, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveRaisedLarge : public Ground
{
public:
    CaveRaisedLarge(int gridX, int gridY = 48, int gridWidth = 128, int gridHeight = 32, float boundsHeightFactor = 0.84375f, GroundType type = GroundType_CaveRaisedLarge, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class CaveRaisedEndRight : public Ground
{
public:
    CaveRaisedEndRight(int gridX, int gridY = 48, int gridWidth = 8, int gridHeight = 32, float boundsHeightFactor = 0.84375f, GroundType type = GroundType_CaveRaisedEndRight, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class GrassWithCaveEndLeft : public Ground
{
public:
    GrassWithCaveEndLeft(int gridX, int gridY = 88, int gridWidth = 8, int gridHeight = 12, float boundsHeightFactor = 0.66666666666667f, GroundType type = GroundType_GrassWithCaveEndLeft, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class GrassWithCaveSmall : public Ground
{
public:
    GrassWithCaveSmall(int gridX, int gridY = 88, int gridWidth = 32, int gridHeight = 12, float boundsHeightFactor = 0.66666666666667f, GroundType type = GroundType_GrassWithCaveSmall, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class GrassWithCaveMedium : public Ground
{
public:
    GrassWithCaveMedium(int gridX, int gridY = 88, int gridWidth = 64, int gridHeight = 12, float boundsHeightFactor = 0.66666666666667f, GroundType type = GroundType_GrassWithCaveMedium, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class GrassWithCaveLarge : public Ground
{
public:
    GrassWithCaveLarge(int gridX, int gridY = 88, int gridWidth = 128, int gridHeight = 12, float boundsHeightFactor = 0.66666666666667f, GroundType type = GroundType_GrassWithCaveLarge, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class GrassWithCaveEndRight : public Ground
{
public:
    GrassWithCaveEndRight(int gridX, int gridY = 88, int gridWidth = 8, int gridHeight = 12, float boundsHeightFactor = 0.66666666666667f, GroundType type = GroundType_GrassWithCaveEndRight, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class GrassWithoutCaveEndLeft : public Ground
{
public:
    GrassWithoutCaveEndLeft(int gridX, int gridY = 0, int gridWidth = 8, int gridHeight = 100, float boundsHeightFactor = 0.96f, GroundType type = GroundType_GrassWithoutCaveEndLeft, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class GrassWithoutCaveSmall : public Ground
{
public:
    GrassWithoutCaveSmall(int gridX, int gridY = 0, int gridWidth = 32, int gridHeight = 100, float boundsHeightFactor = 0.96f, GroundType type = GroundType_GrassWithoutCaveSmall, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class GrassWithoutCaveMedium : public Ground
{
public:
    GrassWithoutCaveMedium(int gridX, int gridY = 0, int gridWidth = 64, int gridHeight = 100, float boundsHeightFactor = 0.96f, GroundType type = GroundType_GrassWithoutCaveMedium, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class GrassWithoutCaveLarge : public Ground
{
public:
    GrassWithoutCaveLarge(int gridX, int gridY = 0, int gridWidth = 128, int gridHeight = 100, float boundsHeightFactor = 0.96f, GroundType type = GroundType_GrassWithoutCaveLarge, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

class GrassWithoutCaveEndRight : public Ground
{
public:
    GrassWithoutCaveEndRight(int gridX, int gridY = 0, int gridWidth = 8, int gridHeight = 100, float boundsHeightFactor = 0.96f, GroundType type = GroundType_GrassWithoutCaveEndRight, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : Ground(gridX, gridY, gridWidth, gridHeight, boundsHeightFactor, type, groundSoundType) {}
};

#endif /* defined(__nosfuratu__Ground__) */
