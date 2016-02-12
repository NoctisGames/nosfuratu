//
//  CollectibleItem.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#include "CollectibleItem.h"
#include "GameConstants.h"

CollectibleItem* CollectibleItem::create(int gridX, int gridY, int type)
{
    switch ((CollectibleItemType)type)
    {
        case CollectibleItemType_Carrot:
            return new Carrot(gridX, gridY);
        case CollectibleItemType_GoldenCarrot:
            return new GoldenCarrot(gridX, gridY);
    }
    
    assert(false);
}

CollectibleItem::CollectibleItem(int gridX, int gridY, int gridWidth, int gridHeight, int collectSoundId, CollectibleItemType type) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight), m_type(type), m_iCollectSoundId(collectSoundId), m_isCollected(false)
{
    updateBounds();
}

void CollectibleItem::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
}

void CollectibleItem::collect()
{
    if (!m_isCollected)
    {
        m_fStateTime = 0;
        m_isCollected = true;
        
        Assets::getInstance()->addSoundIdToPlayQueue(m_iCollectSoundId);
        
        m_isRequestingDeletion = true;
    }
}

bool CollectibleItem::isCollected()
{
    return m_isCollected;
}

CollectibleItemType CollectibleItem::getType()
{
    return m_type;
}