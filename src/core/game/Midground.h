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
    TreeOne(int gridX) : Midground(gridX, 96, 32, 42, MidgroundType_TreeOne) {}
};

class TreeTwo : public Midground
{
public:
    TreeTwo(int gridX) : Midground(gridX, 96, 41, 50, MidgroundType_TreeTwo) {}
};

class TreeThree : public Midground
{
public:
    TreeThree(int gridX) : Midground(gridX, 96, 58, 53, MidgroundType_TreeThree) {}
};

class DeepCaveColumnSmall : public Midground
{
public:
    DeepCaveColumnSmall(int gridX) : Midground(gridX, 0, 6, 18, MidgroundType_DeepCaveColumnSmall) {}
};

class DeepCaveColumnMedium : public Midground
{
public:
    DeepCaveColumnMedium(int gridX) : Midground(gridX, 0, 8, 52, MidgroundType_DeepCaveColumnMedium) {}
};

class DeepCaveColumnBig : public Midground
{
public:
    DeepCaveColumnBig(int gridX) : Midground(gridX, 0, 16, 52, MidgroundType_DeepCaveColumnBig) {}
};

#endif /* defined(__nosfuratu__Midground__) */
