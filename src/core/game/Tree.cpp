//
//  Tree.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/12/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Tree.h"
#include "EntityUtils.h"
#include "EntityAnchor.h"

#define treeTypeKey "treeType"

void Tree::create(std::vector<Tree>& items, float x, float y, TreeType treeType)
{
    switch (treeType)
    {
        case TREE_ONE:
            items.push_back(Tree(x, y, 6.315789473684211f, 8.095320623916813f, treeType));
            break;
        case TREE_TWO:
            items.push_back(Tree(x, y, 11.228070175438596f, 10.271230502599654f, treeType));
            break;
        case TREE_THREE:
        default:
            items.push_back(Tree(x, y, 7.9298245614035086f, 9.616117850953206f, treeType));
            break;
    }
}

Tree::Tree(float x, float y, float width, float height, TreeType treeType) : PhysicalEntity(x, y, width, height), m_treeType(treeType)
{
    // Empty
}

TreeType Tree::getTreeType()
{
    return m_treeType;
}

Tree Tree::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    TreeType type = (TreeType) v[treeTypeKey].GetInt();
    
    return Tree(x, y, width, height, type);
}

void Tree::serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w)
{
    w.String(treeTypeKey);
    w.Int(getTreeType());
}