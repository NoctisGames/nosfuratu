//
//  Tree.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/12/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Tree.h"

Tree* Tree::create(int gridX, int gridY, int type)
{
    switch ((TreeType) type)
    {
        case TreeType_One:
            return new TreeOne(gridX, gridY);
        case TreeType_Two:
            return new TreeTwo(gridX, gridY);
        case TreeType_Three:
        default:
            return new TreeThree(gridX, gridY);
    }
}

Tree::Tree(float x, float y, float width, float height, TreeType type) : PhysicalEntity(x, y, width, height), m_type(type)
{
    // Empty
}

TreeType Tree::getEnumType()
{
    return m_type;
}

int Tree::getType()
{
    return m_type;
}