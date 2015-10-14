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
    static Tree* create(float x, float y, int type);
    
    Tree(float x, float y, float width, float height, TreeType type);
    
    TreeType getEnumType();
    
    int getType();
    
private:
    TreeType m_type;
};

class TreeOne : public Tree
{
public:
    TreeOne(float x, float y, float width = 6.315789473684211f, float height = 8.095320623916813f) : Tree(x, y, width, height, TreeType_One) {}
};

class TreeTwo : public Tree
{
public:
    TreeTwo(float x, float y, float width = 11.228070175438596f, float height = 10.271230502599654f) : Tree(x, y, width, height, TreeType_Two) {}
};

class TreeThree : public Tree
{
public:
    TreeThree(float x, float y, float width = 7.9298245614035086f, float height = 9.616117850953206f) : Tree(x, y, width, height, TreeType_Three) {}
};

#endif /* defined(__nosfuratu__Tree__) */
