//
//  CollectibleItem.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__CollectibleItem__
#define __nosfuratu__CollectibleItem__

#include "GridLockedPhysicalEntity.h"
#include "Assets.h"

class Game;

typedef enum
{
    CollectibleItemType_Carrot,
    CollectibleItemType_GoldenCarrot,
    CollectibleItemType_DisplayGoldenCarrot
} CollectibleItemType;

class CollectibleItem : public GridLockedPhysicalEntity
{
public:
    static CollectibleItem* create(int gridX, int gridY, int type);
    
    CollectibleItem(int gridX, int gridY, int gridWidth, int gridHeight, int collectSoundId, CollectibleItemType type);
    
    virtual void update(float deltaTime);
    
    void collect();
    
    bool isCollected();
    
    CollectibleItemType getType();
    
private:
    CollectibleItemType m_type;
    int m_iCollectSoundId;
    bool m_isCollected;
};

class Carrot : public CollectibleItem
{
public:
    Carrot(int gridX, int gridY) : CollectibleItem(gridX, gridY, 6, 7, SOUND_COLLECT_CARROT, CollectibleItemType_Carrot) {}
};

class GoldenCarrot : public CollectibleItem
{
public:
    GoldenCarrot(int gridX, int gridY) : CollectibleItem(gridX, gridY, 6, 8, SOUND_COLLECT_GOLDEN_CARROT, CollectibleItemType_GoldenCarrot), m_iIndex(0) {}
    
    void init(Game* game);
    
    int getIndex();
    
private:
    int m_iIndex;
};

class DisplayGoldenCarrot : public CollectibleItem
{
public:
    DisplayGoldenCarrot(int gridX, int gridY) : CollectibleItem(gridX, gridY, 6, 7, SOUND_COLLECT_GOLDEN_CARROT, CollectibleItemType_DisplayGoldenCarrot) {}
};

#endif /* defined(__nosfuratu__CollectibleItem__) */
