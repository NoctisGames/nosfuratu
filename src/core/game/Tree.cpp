//
//  Tree.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/12/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Tree.h"
#include "EntityUtils.h"

Tree Tree::createTree(float x, TreeType treeType)
{
    switch (treeType)
    {
        case TREE_ONE:
            return Tree(x, 6.315789473684211f, 8.095320623916813f, treeType);
        case TREE_TWO:
            return Tree(x, 11.228070175438596f, 10.271230502599654f, treeType);
        case TREE_THREE:
        default:
            return Tree(x, 7.9298245614035086f, 9.616117850953206f, treeType);
    }
}

Tree::Tree(float x, float width, float height, TreeType treeType, EntityAnchor anchor) : PhysicalEntity(x + width / 2, 0, width, height), m_treeType(treeType)
{
    EntityUtils::applyAnchor(*this, anchor);
}

TreeType Tree::getTreeType()
{
    return m_treeType;
}