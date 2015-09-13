//
//  Tree.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/12/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Tree.h"

Tree::Tree(float x, float y, float width, float height, int treeType) : PhysicalEntity(x, y, width, height, 0), m_iTreeType(treeType)
{
    // Empty
}

int Tree::getTreeType()
{
    return m_iTreeType;
}