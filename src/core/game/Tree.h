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

class Tree : public PhysicalEntity
{
public:
    Tree(float x, float y, float width, float height, int treeType);
    
    int getTreeType();
    
private:
    int m_iTreeType;
};

#endif /* defined(__nosfuratu__Tree__) */
