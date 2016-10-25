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
#include "Game.h"
#include "OverlapTester.h"

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

CollectibleItem::CollectibleItem(int gridX, int gridY, int gridWidth, int gridHeight, int collectSoundId, CollectibleItemType type) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight), m_type(type), m_color(1, 1, 1, 1), m_iCollectSoundId(collectSoundId), m_isCollected(false), m_isOnScreen(false), m_game(nullptr)
{
    updateBounds();
    
    m_fOriginalY = m_position->getY();

	static float stateTimeSeed = 0;

	m_fStateTime = stateTimeSeed;

	stateTimeSeed += 0.08f;
    
    resize();
}

void CollectibleItem::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_isCollected || !m_isOnScreen)
    {
        return;
    }
    
    float baseTime = fmodf(m_fStateTime, 0.80f);
    if (baseTime < 0.40f)
    {
        m_position->setY(m_fOriginalY + (baseTime / 3));
    }
    else
    {
        baseTime -= 0.40f;
        m_position->setY(m_fOriginalY + (0.40f / 3) - (baseTime / 3));
    }
}

void CollectibleItem::updateBounds()
{
    if (m_isOnScreen || !m_game)
    {
        return;
    }
    
    Rectangle& camBounds = *m_game->getCameraBounds();
    
    if (camBounds.getWidth() > CAM_WIDTH)
    {
        return;
    }
    
    if (OverlapTester::doRectanglesOverlap(camBounds, getMainBounds()))
    {
        if (!m_isOnScreen)
        {
            m_isOnScreen = true;
        }
    }
    else if (m_isOnScreen)
    {
        m_isOnScreen = false;
    }
}

void CollectibleItem::resize()
{
    // Override in Subclass
}

void CollectibleItem::collect()
{
    if (!m_isCollected)
    {
        m_fStateTime = 0;
        m_isCollected = true;
        
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

void Carrot::updateBounds()
{
    CollectibleItem::updateBounds();
    
    m_fWidth = 6 * GRID_CELL_SIZE;
    m_fHeight = 7 * GRID_CELL_SIZE;
    
    GridLockedPhysicalEntity::updateBounds();
    
    resize();
}

void Carrot::snapToGrid(int gridCellSizeScalar)
{
    m_fWidth = 6 * GRID_CELL_SIZE;
    m_fHeight = 7 * GRID_CELL_SIZE;
    
    GridLockedPhysicalEntity::snapToGrid(gridCellSizeScalar);
    
    resize();
}

void Carrot::resize()
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
            m_fWidth -= m_fStateTime / 3;
            m_fHeight -= m_fStateTime / 3;
            
            if (m_fWidth < 0.1f)
            {
                m_fWidth = 0.1f;
                m_color.alpha = 0.2f;
            }
            
            if (m_fHeight < 0.1f)
            {
                m_fHeight = 0.1f;
                m_color.alpha = 0.2f;
            }
        }
        
        if (m_fStateTime > 0.55f)
        {
            m_isRequestingDeletion = true;
        }
    }
}

void GoldenCarrot::updateBounds()
{
    CollectibleItem::updateBounds();
    
    if (m_isCollected)
    {
        return;
    }
    
    m_fWidth = 6 * GRID_CELL_SIZE;
    m_fHeight = 8 * GRID_CELL_SIZE;
    
    GridLockedPhysicalEntity::updateBounds();
    
    resize();
}

void GoldenCarrot::snapToGrid(int gridCellSizeScalar)
{
    m_fWidth = 6 * GRID_CELL_SIZE;
    m_fHeight = 8 * GRID_CELL_SIZE;
    
    GridLockedPhysicalEntity::snapToGrid(gridCellSizeScalar);
    
    resize();
}

void GoldenCarrot::resize()
{
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
