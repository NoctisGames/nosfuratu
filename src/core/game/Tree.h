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

#include <vector>

class Tree : public PhysicalEntity
{
public:
    static void create(std::vector<Tree>& items, float x, float y, TreeType treeType);
    
    Tree(float x, float y, float width, float height, TreeType treeType);
    
    TreeType getTreeType();
    
    static Tree deserialize(rapidjson::Value& v);
    
    virtual void serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w);
    
private:
    TreeType m_treeType;
};

#endif /* defined(__nosfuratu__Tree__) */
