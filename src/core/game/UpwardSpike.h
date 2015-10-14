//
//  UpwardSpike.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__UpwardSpike__
#define __nosfuratu__UpwardSpike__

#include "PhysicalEntity.h"

typedef enum
{
    UpwardSpikeType_MetalGrass,
    UpwardSpikeType_WoodGrass,
    UpwardSpikeType_MetalCave
} UpwardSpikeType;

class UpwardSpike : public PhysicalEntity
{
public:
    static UpwardSpike* create(float x, float y, int type);
    
    UpwardSpike(float x, float y, float width, float height, UpwardSpikeType type);
    
    virtual void updateBounds();
    
    UpwardSpikeType getEnumType();
    
    int getType();
    
private:
    UpwardSpikeType m_type;
};

class UpwardSpikeMetalGrass : public UpwardSpike
{
public:
    UpwardSpikeMetalGrass(float x, float y, float width = 0.6549707602339181f, float height = 1.0294627383015598f) : UpwardSpike(x, y, width, height, UpwardSpikeType_MetalGrass) {}
};

class UpwardSpikeWoodGrass : public UpwardSpike
{
public:
    UpwardSpikeWoodGrass(float x, float y, float width = 0.9590643274853801f, float height = 1.0762564991334487f) : UpwardSpike(x, y, width, height, UpwardSpikeType_WoodGrass) {}
};

class UpwardSpikeMetalCave : public UpwardSpike
{
public:
    UpwardSpikeMetalCave(float x, float y, float width = 0.42105263157894735f, float height = 1.123050259965338f) : UpwardSpike(x, y, width, height, UpwardSpikeType_MetalCave) {}
};

#endif /* defined(__nosfuratu__UpwardSpike__) */
