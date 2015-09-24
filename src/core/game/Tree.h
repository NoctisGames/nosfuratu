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
#include "TreeType.h"
#include "EntityAnchor.h"

class Tree : public PhysicalEntity
{
public:
    static Tree createTree(float x, TreeType treeType);
    
    Tree(float x, float width, float height, TreeType treeType, EntityAnchor anchor = ANCHOR_GROUND);
    
    TreeType getTreeType();
    
private:
    TreeType m_treeType;
};

#endif /* defined(__nosfuratu__Tree__) */
