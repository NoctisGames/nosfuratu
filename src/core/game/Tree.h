//
//  Tree.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/12/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Tree__
#define __nosfuratu__Tree__

#include "PhysicalEntity.h"

typedef enum
{
    TreeType_One,
    TreeType_Two,
    TreeType_Three
} TreeType;

class Tree : public PhysicalEntity
{
public:
    static Tree* create(int gridX, int gridY, int type);
    
    Tree(float x, float y, float width, float height, TreeType type);
    
    TreeType getEnumType();
    
    int getType();
    
private:
    TreeType m_type;
};

class TreeOne : public Tree
{
public:
    TreeOne(float x, float y, float width = 6.112359550561798f, float height = 8.101198402130493f) : Tree(x, y, width, height, TreeType_One) {}
};

class TreeTwo : public Tree
{
public:
    TreeTwo(float x, float y, float width = 11.217977528089888f, float height = 10.25832223701731f) : Tree(x, y, width, height, TreeType_Two) {}
};

class TreeThree : public Tree
{
public:
    TreeThree(float x, float y, float width = 7.910112359550562f, float height = 9.63515312916112f) : Tree(x, y, width, height, TreeType_Three) {}
};

#endif /* defined(__nosfuratu__Tree__) */
