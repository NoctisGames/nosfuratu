//
//  ForegroundObject.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__ForegroundObject__
#define __nosfuratu__ForegroundObject__

#include "GridLockedPhysicalEntity.h"
#include "GroundSoundType.h"
#include "Jon.h"

typedef enum
{
    ForegroundObjectType_GrassPlatformLeft,
    ForegroundObjectType_GrassPlatformCenter,
    ForegroundObjectType_GrassPlatformRight,
    
    ForegroundObjectType_CavePlatformLeft,
    ForegroundObjectType_CavePlatformCenter,
    ForegroundObjectType_CavePlatformRight,
    
    ForegroundObjectType_RockLarge,
    ForegroundObjectType_RockMedium,
    ForegroundObjectType_RockSmall,
    ForegroundObjectType_RockSmallCracked,
    
    ForegroundObjectType_StumpBig,
    ForegroundObjectType_StumpSmall,
    
    ForegroundObjectType_EndSign,
    
    ForegroundObjectType_ThornsLeft,
    ForegroundObjectType_ThornsCenterSmall,
    ForegroundObjectType_ThornsCenterBig,
    ForegroundObjectType_ThornsRight,
    
    ForegroundObjectType_LogVerticalTall,
    ForegroundObjectType_LogVerticalShort,
    
    ForegroundObjectType_JumpSpringLight,
    ForegroundObjectType_JumpSpringMedium,
    ForegroundObjectType_JumpSpringHeavy,
    
    ForegroundObjectType_SpikeGrassSingle,
    ForegroundObjectType_SpikeGrassFour,
    ForegroundObjectType_SpikeGrassEight,
    
    ForegroundObjectType_SpikeCaveSingle,
    ForegroundObjectType_SpikeCaveFour,
    ForegroundObjectType_SpikeCaveEight,
    
    ForegroundObjectType_SpikeCaveCeilingSingle,
    ForegroundObjectType_SpikeCaveCeilingFour,
    ForegroundObjectType_SpikeCaveCeilingEight,
    
    ForegroundObjectType_SpikeWallSingle,
    ForegroundObjectType_SpikeWallFour,
    ForegroundObjectType_SpikeWallEight
} ForegroundObjectType;

class ForegroundObject : public GridLockedPhysicalEntity
{
public:
    static ForegroundObject* create(int gridX, int gridY, int type);
    
    ForegroundObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE);
    
    bool isJonLanding(Jon& jon, float deltaTime);
    
    bool isJonBlockedOnRight(Jon& jon, float deltaTime);
    
    ForegroundObjectType getType();
    
    GroundSoundType getGroundSoundType();
    
private:
    ForegroundObjectType m_type;
    GroundSoundType m_groundSoundType;
};

class GrassPlatformLeft : public ForegroundObject
{
public:
    GrassPlatformLeft(int gridX, int gridY, int gridWidth = 4, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_GrassPlatformLeft, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class GrassPlatformCenter : public ForegroundObject
{
public:
    GrassPlatformCenter(int gridX, int gridY, int gridWidth = 14, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_GrassPlatformCenter, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class GrassPlatformRight : public ForegroundObject
{
public:
    GrassPlatformRight(int gridX, int gridY, int gridWidth = 4, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_GrassPlatformRight, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class CavePlatformLeft : public ForegroundObject
{
public:
    CavePlatformLeft(int gridX, int gridY, int gridWidth = 4, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_CavePlatformLeft, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class CavePlatformCenter : public ForegroundObject
{
public:
    CavePlatformCenter(int gridX, int gridY, int gridWidth = 14, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_CavePlatformCenter, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class CavePlatformRight : public ForegroundObject
{
public:
    CavePlatformRight(int gridX, int gridY, int gridWidth = 4, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_CavePlatformRight, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class RockLarge : public ForegroundObject
{
public:
    RockLarge(int gridX, int gridY, int gridWidth = 44, int gridHeight = 32, ForegroundObjectType type = ForegroundObjectType_RockLarge, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class RockMedium : public ForegroundObject
{
public:
    RockMedium(int gridX, int gridY, int gridWidth = 24, int gridHeight = 24, ForegroundObjectType type = ForegroundObjectType_RockMedium, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class RockSmall : public ForegroundObject
{
public:
    RockSmall(int gridX, int gridY, int gridWidth = 24, int gridHeight = 23, ForegroundObjectType type = ForegroundObjectType_RockSmall, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class RockSmallCracked : public ForegroundObject
{
public:
    RockSmallCracked(int gridX, int gridY, int gridWidth = 24, int gridHeight = 23, ForegroundObjectType type = ForegroundObjectType_RockSmallCracked, GroundSoundType groundSoundType = GROUND_SOUND_CAVE) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class StumpBig : public ForegroundObject
{
public:
    StumpBig(int gridX, int gridY, int gridWidth = 16, int gridHeight = 18, ForegroundObjectType type = ForegroundObjectType_StumpBig, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class StumpSmall : public ForegroundObject
{
public:
    StumpSmall(int gridX, int gridY, int gridWidth = 20, int gridHeight = 17, ForegroundObjectType type = ForegroundObjectType_StumpSmall, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class EndSign : public ForegroundObject
{
public:
    EndSign(int gridX, int gridY, int gridWidth = 4, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_EndSign) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class ThornsLeft : public ForegroundObject
{
public:
    ThornsLeft(int gridX, int gridY, int gridWidth = 5, int gridHeight = 7, ForegroundObjectType type = ForegroundObjectType_ThornsLeft) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class ThornsCenterSmall : public ForegroundObject
{
public:
    ThornsCenterSmall(int gridX, int gridY, int gridWidth = 11, int gridHeight = 7, ForegroundObjectType type = ForegroundObjectType_ThornsCenterSmall) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class ThornsCenterBig : public ForegroundObject
{
public:
    ThornsCenterBig(int gridX, int gridY, int gridWidth = 22, int gridHeight = 7, ForegroundObjectType type = ForegroundObjectType_ThornsCenterBig) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class ThornsRight : public ForegroundObject
{
public:
    ThornsRight(int gridX, int gridY, int gridWidth = 5, int gridHeight = 7, ForegroundObjectType type = ForegroundObjectType_ThornsRight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class LogVerticalTall : public ForegroundObject
{
public:
    LogVerticalTall(int gridX, int gridY, int gridWidth = 8, int gridHeight = 11, ForegroundObjectType type = ForegroundObjectType_LogVerticalTall, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class LogVerticalShort : public ForegroundObject
{
public:
    LogVerticalShort(int gridX, int gridY, int gridWidth = 8, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_LogVerticalShort, GroundSoundType groundSoundType = GROUND_SOUND_GRASS) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType) {}
};

class JumpSpringLight : public ForegroundObject
{
public:
    JumpSpringLight(int gridX, int gridY, int gridWidth = 6, int gridHeight = 5, ForegroundObjectType type = ForegroundObjectType_JumpSpringLight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class JumpSpringMedium : public ForegroundObject
{
public:
    JumpSpringMedium(int gridX, int gridY, int gridWidth = 17, int gridHeight = 9, ForegroundObjectType type = ForegroundObjectType_JumpSpringMedium) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class JumpSpringHeavy : public ForegroundObject
{
public:
    JumpSpringHeavy(int gridX, int gridY, int gridWidth = 17, int gridHeight = 14, ForegroundObjectType type = ForegroundObjectType_JumpSpringHeavy) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class SpikeGrassSingle : public ForegroundObject
{
public:
    SpikeGrassSingle(int gridX, int gridY, int gridWidth = 6, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_SpikeGrassSingle) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class SpikeGrassFour : public ForegroundObject
{
public:
    SpikeGrassFour(int gridX, int gridY, int gridWidth = 18, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_SpikeGrassFour) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class SpikeGrassEight : public ForegroundObject
{
public:
    SpikeGrassEight(int gridX, int gridY, int gridWidth = 34, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_SpikeGrassEight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class SpikeCaveSingle : public ForegroundObject
{
public:
    SpikeCaveSingle(int gridX, int gridY, int gridWidth = 6, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_SpikeCaveSingle) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class SpikeCaveFour : public ForegroundObject
{
public:
    SpikeCaveFour(int gridX, int gridY, int gridWidth = 18, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_SpikeCaveFour) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class SpikeCaveEight : public ForegroundObject
{
public:
    SpikeCaveEight(int gridX, int gridY, int gridWidth = 34, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_SpikeCaveEight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class SpikeCaveCeilingSingle : public ForegroundObject
{
public:
    SpikeCaveCeilingSingle(int gridX, int gridY, int gridWidth = 6, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_SpikeCaveCeilingSingle) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class SpikeCaveCeilingFour : public ForegroundObject
{
public:
    SpikeCaveCeilingFour(int gridX, int gridY, int gridWidth = 18, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_SpikeCaveCeilingFour) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class SpikeCaveCeilingEight : public ForegroundObject
{
public:
    SpikeCaveCeilingEight(int gridX, int gridY, int gridWidth = 34, int gridHeight = 6, ForegroundObjectType type = ForegroundObjectType_SpikeCaveCeilingEight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class SpikeWallSingle : public ForegroundObject
{
public:
    SpikeWallSingle(int gridX, int gridY, int gridWidth = 6, int gridHeight = 4, ForegroundObjectType type = ForegroundObjectType_SpikeWallSingle) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class SpikeWallFour : public ForegroundObject
{
public:
    SpikeWallFour(int gridX, int gridY, int gridWidth = 6, int gridHeight = 16, ForegroundObjectType type = ForegroundObjectType_SpikeWallFour) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

class SpikeWallEight : public ForegroundObject
{
public:
    SpikeWallEight(int gridX, int gridY, int gridWidth = 6, int gridHeight = 34, ForegroundObjectType type = ForegroundObjectType_SpikeWallEight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type) {}
};

#endif /* defined(__nosfuratu__ForegroundObject__) */
