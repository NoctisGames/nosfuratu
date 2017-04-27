//
//  GameHudItem.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/1/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "GameHudItem.h"

GameHudItem GameHudItem::createCarrot()
{
    return GameHudItem(0);
}

GameHudItem GameHudItem::createGoldenCarrot()
{
    return GameHudItem(1);
}

GameHudItem GameHudItem::createVial()
{
    return GameHudItem(2);
}

GameHudItem::GameHudItem(int type) : PhysicalEntity(0, 0, 1, 1), m_iType(type)
{
    // Empty
}

bool GameHudItem::isCarrot()
{
    return m_iType == 0;
}

bool GameHudItem::isGolden()
{
    return m_iType == 1;
}

bool GameHudItem::isVial()
{
    return m_iType == 2;
}

RTTI_IMPL(GameHudItem, PhysicalEntity);
