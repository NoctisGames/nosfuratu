//
//  Midground.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Midground__
#define __nosfuratu__Midground__

#include "GridLockedPhysicalEntity.h"

typedef enum
{
    MidgroundType_TreeOne,
    MidgroundType_TreeTwo,
    MidgroundType_TreeThree,
    MidgroundType_DeepCaveColumnSmall,
    MidgroundType_DeepCaveColumnMedium,
    MidgroundType_DeepCaveColumnBig,
} MidgroundType;

class Midground : public GridLockedPhysicalEntity
{
public:
    static Midground* create(int gridX, int gridY, int type);
    
    Midground(int gridX, int gridY, int gridWidth, int gridHeight, MidgroundType type);
    
    MidgroundType getType();
    
private:
    MidgroundType m_type;
};

class TreeOne : public Midground
{
public:
    TreeOne(int gridX, int gridY = 96, int gridWidth = 32, int gridHeight = 42) : Midground(gridX, gridY, gridWidth, gridHeight, MidgroundType_TreeOne) {}
};

class TreeTwo : public Midground
{
public:
    TreeTwo(int gridX, int gridY = 96, int gridWidth = 41, int gridHeight = 50) : Midground(gridX, gridY, gridWidth, gridHeight, MidgroundType_TreeTwo) {}
};

class TreeThree : public Midground
{
public:
    TreeThree(int gridX, int gridY = 96, int gridWidth = 58, int gridHeight = 53) : Midground(gridX, gridY, gridWidth, gridHeight, MidgroundType_TreeThree) {}
};

class DeepCaveColumnSmall : public Midground
{
public:
    DeepCaveColumnSmall(int gridX, int gridY = 0, int gridWidth = 6, int gridHeight = 18) : Midground(gridX, gridY, gridWidth, gridHeight, MidgroundType_DeepCaveColumnSmall) {}
};

class DeepCaveColumnMedium : public Midground
{
public:
    DeepCaveColumnMedium(int gridX, int gridY = 0, int gridWidth = 8, int gridHeight = 52) : Midground(gridX, gridY, gridWidth, gridHeight, MidgroundType_DeepCaveColumnMedium) {}
};

class DeepCaveColumnBig : public Midground
{
public:
    DeepCaveColumnBig(int gridX, int gridY = 0, int gridWidth = 16, int gridHeight = 52) : Midground(gridX, gridY, gridWidth, gridHeight, MidgroundType_DeepCaveColumnBig) {}
};

#endif /* defined(__nosfuratu__Midground__) */
