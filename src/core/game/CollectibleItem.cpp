//
//  CollectibleItem.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "CollectibleItem.h"

#include "GameConstants.h"
#include "FlagUtil.h"
#include "MathUtil.h"

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

CollectibleItem::CollectibleItem(int gridX, int gridY, int gridWidth, int gridHeight, int collectSoundId, CollectibleItemType type) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight), m_type(type), m_color(1, 1, 1, 1), m_iCollectSoundId(collectSoundId), m_isCollected(false)
{
    updateBounds();

	static float stateTimeSeed = 0;

	m_fStateTime = stateTimeSeed;

	stateTimeSeed += 0.08f;
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
        
        onCollected();
        
        Assets::getInstance()->addSoundIdToPlayQueue(m_iCollectSoundId);
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

void Carrot::update(float deltaTime)
{
    CollectibleItem::update(deltaTime);
    
    if (m_isCollected)
    {
        if (m_fStateTime > 0.4f)
        {
            m_isRequestingDeletion = true;
        }
    }
}

void Carrot::onCollected()
{
    m_fWidth = 1.916015625f;
    m_fHeight = 1.96875f;
}

void GoldenCarrot::update(float deltaTime)
{
    CollectibleItem::update(deltaTime);
    
    if (m_isCollected)
    {
        if (m_isPreviouslyCollected)
        {
            m_color.alpha = 1 - (0.5f / m_fStateTime);
            m_color.alpha = clamp(m_color.alpha, 1, 0);
        }
        
        if (m_fStateTime > 0.55f)
        {
            m_isRequestingDeletion = true;
        }
    }
}

void GoldenCarrot::onCollected()
{
    if (m_isPreviouslyCollected)
    {
        return;
    }
    
    m_fWidth = 2.232421875f;
    m_fHeight = 1.96875f;
}

void GoldenCarrot::init(int index, int bestLevelStatsFlag)
{
    m_iIndex = index;
    
    if ((m_iIndex == 0 && FlagUtil::isFlagSet(bestLevelStatsFlag, FLAG_FIRST_GOLDEN_CARROT_COLLECTED))
        || (m_iIndex == 1 && FlagUtil::isFlagSet(bestLevelStatsFlag, FLAG_SECOND_GOLDEN_CARROT_COLLECTED))
        || (m_iIndex == 2 && FlagUtil::isFlagSet(bestLevelStatsFlag, FLAG_THIRD_GOLDEN_CARROT_COLLECTED)))
    {
        m_isPreviouslyCollected = true;
    }
}

int GoldenCarrot::getIndex()
{
    return m_iIndex;
}

bool GoldenCarrot::isPreviouslyCollected()
{
    return m_isPreviouslyCollected;
}