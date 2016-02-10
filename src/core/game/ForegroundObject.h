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
    
    ForegroundObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType = GROUND_SOUND_NONE, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual void updateBounds();
    
    bool isJonLanding(Jon& jon, float deltaTime);
    
    bool isJonBlockedOnRight(Jon& jon, float deltaTime);
    
    ForegroundObjectType getType();
    
    GroundSoundType getGroundSoundType();
    
private:
    ForegroundObjectType m_type;
    GroundSoundType m_groundSoundType;
    float m_fBoundsX;
    float m_fBoundsY;
    float m_fBoundsWidth;
    float m_fBoundsHeight;
};

class GrassPlatformLeft : public ForegroundObject
{
public:
    GrassPlatformLeft(int gridX, int gridY) : ForegroundObject(gridX, gridY, 4, 6, ForegroundObjectType_GrassPlatformLeft, GROUND_SOUND_GRASS) {}
};

class GrassPlatformCenter : public ForegroundObject
{
public:
    GrassPlatformCenter(int gridX, int gridY) : ForegroundObject(gridX, gridY, 14, 6, ForegroundObjectType_GrassPlatformCenter, GROUND_SOUND_GRASS) {}
};

class GrassPlatformRight : public ForegroundObject
{
public:
    GrassPlatformRight(int gridX, int gridY) : ForegroundObject(gridX, gridY, 4, 6, ForegroundObjectType_GrassPlatformRight, GROUND_SOUND_GRASS) {}
};

class CavePlatformLeft : public ForegroundObject
{
public:
    CavePlatformLeft(int gridX, int gridY) : ForegroundObject(gridX, gridY, 4, 6, ForegroundObjectType_CavePlatformLeft, GROUND_SOUND_CAVE) {}
};

class CavePlatformCenter : public ForegroundObject
{
public:
    CavePlatformCenter(int gridX, int gridY) : ForegroundObject(gridX, gridY, 14, 6, ForegroundObjectType_CavePlatformCenter, GROUND_SOUND_CAVE) {}
};

class CavePlatformRight : public ForegroundObject
{
public:
    CavePlatformRight(int gridX, int gridY) : ForegroundObject(gridX, gridY, 4, 6, ForegroundObjectType_CavePlatformRight, GROUND_SOUND_CAVE) {}
};

class RockLarge : public ForegroundObject
{
public:
    RockLarge(int gridX, int gridY) : ForegroundObject(gridX, gridY, 44, 32, ForegroundObjectType_RockLarge, GROUND_SOUND_CAVE) {}
};

class RockMedium : public ForegroundObject
{
public:
    RockMedium(int gridX, int gridY) : ForegroundObject(gridX, gridY, 24, 24, ForegroundObjectType_RockMedium, GROUND_SOUND_CAVE) {}
};

class RockSmall : public ForegroundObject
{
public:
    RockSmall(int gridX, int gridY) : ForegroundObject(gridX, gridY, 24, 23, ForegroundObjectType_RockSmall, GROUND_SOUND_CAVE) {}
};

class RockSmallCracked : public ForegroundObject
{
public:
    RockSmallCracked(int gridX, int gridY) : ForegroundObject(gridX, gridY, 24, 23, ForegroundObjectType_RockSmallCracked, GROUND_SOUND_CAVE) {}
};

class StumpBig : public ForegroundObject
{
public:
    StumpBig(int gridX, int gridY) : ForegroundObject(gridX, gridY, 16, 18, ForegroundObjectType_StumpBig, GROUND_SOUND_GRASS) {}
};

class StumpSmall : public ForegroundObject
{
public:
    StumpSmall(int gridX, int gridY) : ForegroundObject(gridX, gridY, 20, 17, ForegroundObjectType_StumpSmall, GROUND_SOUND_GRASS) {}
};

class EndSign : public ForegroundObject
{
public:
    EndSign(int gridX, int gridY) : ForegroundObject(gridX, gridY, 4, 6, ForegroundObjectType_EndSign) {}
};

class ThornsLeft : public ForegroundObject
{
public:
    ThornsLeft(int gridX, int gridY) : ForegroundObject(gridX, gridY, 5, 7, ForegroundObjectType_ThornsLeft) {}
};

class ThornsCenterSmall : public ForegroundObject
{
public:
    ThornsCenterSmall(int gridX, int gridY) : ForegroundObject(gridX, gridY, 11, 7, ForegroundObjectType_ThornsCenterSmall) {}
};

class ThornsCenterBig : public ForegroundObject
{
public:
    ThornsCenterBig(int gridX, int gridY) : ForegroundObject(gridX, gridY, 22, 7, ForegroundObjectType_ThornsCenterBig) {}
};

class ThornsRight : public ForegroundObject
{
public:
    ThornsRight(int gridX, int gridY) : ForegroundObject(gridX, gridY, 5, 7, ForegroundObjectType_ThornsRight) {}
};

class LogVerticalTall : public ForegroundObject
{
public:
    LogVerticalTall(int gridX, int gridY) : ForegroundObject(gridX, gridY, 8, 11, ForegroundObjectType_LogVerticalTall, GROUND_SOUND_GRASS) {}
};

class LogVerticalShort : public ForegroundObject
{
public:
    LogVerticalShort(int gridX, int gridY) : ForegroundObject(gridX, gridY, 8, 6, ForegroundObjectType_LogVerticalShort, GROUND_SOUND_GRASS) {}
};

class JumpSpringLight : public ForegroundObject
{
public:
    JumpSpringLight(int gridX, int gridY) : ForegroundObject(gridX, gridY, 6, 5, ForegroundObjectType_JumpSpringLight) {}
};

class JumpSpringMedium : public ForegroundObject
{
public:
    JumpSpringMedium(int gridX, int gridY) : ForegroundObject(gridX, gridY, 17, 9, ForegroundObjectType_JumpSpringMedium) {}
};

class JumpSpringHeavy : public ForegroundObject
{
public:
    JumpSpringHeavy(int gridX, int gridY) : ForegroundObject(gridX, gridY, 17, 14, ForegroundObjectType_JumpSpringHeavy) {}
};

class SpikeGrassSingle : public ForegroundObject
{
public:
    SpikeGrassSingle(int gridX, int gridY) : ForegroundObject(gridX, gridY, 6, 6, ForegroundObjectType_SpikeGrassSingle) {}
};

class SpikeGrassFour : public ForegroundObject
{
public:
    SpikeGrassFour(int gridX, int gridY) : ForegroundObject(gridX, gridY, 18, 6, ForegroundObjectType_SpikeGrassFour) {}
};

class SpikeGrassEight : public ForegroundObject
{
public:
    SpikeGrassEight(int gridX, int gridY) : ForegroundObject(gridX, gridY, 34, 6, ForegroundObjectType_SpikeGrassEight) {}
};

class SpikeCaveSingle : public ForegroundObject
{
public:
    SpikeCaveSingle(int gridX, int gridY) : ForegroundObject(gridX, gridY, 6, 6, ForegroundObjectType_SpikeCaveSingle) {}
};

class SpikeCaveFour : public ForegroundObject
{
public:
    SpikeCaveFour(int gridX, int gridY) : ForegroundObject(gridX, gridY, 18, 6, ForegroundObjectType_SpikeCaveFour) {}
};

class SpikeCaveEight : public ForegroundObject
{
public:
    SpikeCaveEight(int gridX, int gridY) : ForegroundObject(gridX, gridY, 34, 6, ForegroundObjectType_SpikeCaveEight) {}
};

class SpikeCaveCeilingSingle : public ForegroundObject
{
public:
    SpikeCaveCeilingSingle(int gridX, int gridY) : ForegroundObject(gridX, gridY, 6, 6, ForegroundObjectType_SpikeCaveCeilingSingle) {}
};

class SpikeCaveCeilingFour : public ForegroundObject
{
public:
    SpikeCaveCeilingFour(int gridX, int gridY) : ForegroundObject(gridX, gridY, 18, 6, ForegroundObjectType_SpikeCaveCeilingFour) {}
};

class SpikeCaveCeilingEight : public ForegroundObject
{
public:
    SpikeCaveCeilingEight(int gridX, int gridY) : ForegroundObject(gridX, gridY, 34, 6, ForegroundObjectType_SpikeCaveCeilingEight) {}
};

class SpikeWallSingle : public ForegroundObject
{
public:
    SpikeWallSingle(int gridX, int gridY) : ForegroundObject(gridX, gridY, 6, 4, ForegroundObjectType_SpikeWallSingle) {}
};

class SpikeWallFour : public ForegroundObject
{
public:
    SpikeWallFour(int gridX, int gridY) : ForegroundObject(gridX, gridY, 6, 16, ForegroundObjectType_SpikeWallFour) {}
};

class SpikeWallEight : public ForegroundObject
{
public:
    SpikeWallEight(int gridX, int gridY) : ForegroundObject(gridX, gridY, 6, 34, ForegroundObjectType_SpikeWallEight) {}
};

#endif /* defined(__nosfuratu__ForegroundObject__) */
