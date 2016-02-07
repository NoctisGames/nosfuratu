//
//  GroundPlatform.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/4/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__GroundPlatform__
#define __nosfuratu__GroundPlatform__

#include "LandPhysicalEntity.h"

typedef enum
{
    GroundPlatformType_GrassDefault,
    GroundPlatformType_GrassEndLeft,
    GroundPlatformType_GrassCenter,
    GroundPlatformType_GrassEndRight,
    GroundPlatformType_CaveEndLeft,
    GroundPlatformType_CaveCenter,
    GroundPlatformType_CaveEndRight
} GroundPlatformType;

#define GROUND_PLATFORM_HEIGHT 1.162450066577896f

class GroundPlatform : public LandPhysicalEntity
{
public:
    static GroundPlatform* create(int gridX, int gridY, int type);
    
    GroundPlatform(float x, float y, float width, float height, GroundPlatformType type);
    
    virtual void updateBounds();
    
    GroundPlatformType getEnumType();
    
    int getType();
    
    virtual GroundSoundType getGroundSoundType();
    
private:
    GroundPlatformType m_type;
};

class GroundPlatformGrassDefault : public GroundPlatform
{
public:
    GroundPlatformGrassDefault(float x, float y, float width = 2.7205992509363295f, float height = GROUND_PLATFORM_HEIGHT) : GroundPlatform(x, y, width, height, GroundPlatformType_GrassDefault) {}
};

class GroundPlatformGrassEndLeft : public GroundPlatform
{
public:
    GroundPlatformGrassEndLeft(float x, float y, float width = 0.6831460674157304f, float height = GROUND_PLATFORM_HEIGHT) : GroundPlatform(x, y, width, height, GroundPlatformType_GrassEndLeft) {}
};

class GroundPlatformGrassCenter : public GroundPlatform
{
public:
    GroundPlatformGrassCenter(float x, float y, float width = 2.6846441947565545f, float height = GROUND_PLATFORM_HEIGHT) : GroundPlatform(x, y, width, height, GroundPlatformType_GrassCenter) {}
};

class GroundPlatformGrassEndRight : public GroundPlatform
{
public:
    GroundPlatformGrassEndRight(float x, float y, float width = 0.6831460674157304f, float height = GROUND_PLATFORM_HEIGHT) : GroundPlatform(x, y, width, height, GroundPlatformType_GrassEndRight) {}
};

class GroundPlatformCaveEndLeft : public GroundPlatform
{
public:
    GroundPlatformCaveEndLeft(float x, float y, float width = 0.7670411985018727f, float height = GROUND_PLATFORM_HEIGHT) : GroundPlatform(x, y, width, height, GroundPlatformType_CaveEndLeft) {}
};

class GroundPlatformCaveCenter : public GroundPlatform
{
public:
    GroundPlatformCaveCenter(float x, float y, float width = 2.6846441947565545f, float height = GROUND_PLATFORM_HEIGHT) : GroundPlatform(x, y, width, height, GroundPlatformType_CaveCenter) {}
};

class GroundPlatformCaveEndRight : public GroundPlatform
{
public:
    GroundPlatformCaveEndRight(float x, float y, float width = 0.7670411985018727f, float height = GROUND_PLATFORM_HEIGHT) : GroundPlatform(x, y, width, height, GroundPlatformType_CaveEndRight) {}
};

#endif /* defined(__nosfuratu__GroundPlatform__) */
