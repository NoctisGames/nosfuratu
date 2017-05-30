//
//  CollectibleItem.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__CollectibleItem__
#define __nosfuratu__CollectibleItem__

#include "GridLockedPhysicalEntity.h"

#include "CollectibleItemType.h"

#include "RTTI.h"

class Game;

class CollectibleItem : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static CollectibleItem* create(int gridX, int gridY, int type);
    
    CollectibleItem(int gridX, int gridY, int gridWidth, int gridHeight, int collectSoundId, CollectibleItemType type, float boundsX = 0, float boundsY = 0, float boundsWidth = 1, float boundsHeight = 1);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    virtual void resize();
    
    void collect();
    
    bool isCollected();
    
    CollectibleItemType getType();
    
    void setGame(Game* game);
    
protected:
    Game* m_game;
    CollectibleItemType m_type;
    float m_fOriginalY;
    int m_iCollectSoundId;
    bool m_isCollected;
    bool m_isOnScreen;
};

class Carrot : public CollectibleItem
{
    RTTI_DECL;
    
public:
    Carrot(int gridX, int gridY);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    virtual void snapToGrid(int gridCellSizeScalar = 1);
    
    virtual void resize();
};

class GoldenCarrotTwinkle : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    GoldenCarrotTwinkle(float x, float y, float seedStateTime);
};

class GoldenCarrot : public CollectibleItem
{
    RTTI_DECL;
    
public:
    GoldenCarrot(int gridX, int gridY);
    
    ~GoldenCarrot();
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    virtual void snapToGrid(int gridCellSizeScalar = 1);
    
    virtual void resize();
    
    void init(int index, int bestLevelStatsFlag);
    
    GoldenCarrotTwinkle& getGoldenCarrotTwinkle();
    
    int getIndex();
    
    bool isPreviouslyCollected();
    
private:
    GoldenCarrotTwinkle* m_goldenCarrotTwinkle;
    int m_iIndex;
    bool m_isPreviouslyCollected;
};

class BigCarrot : public CollectibleItem
{
    RTTI_DECL;
    
public:
    BigCarrot(int gridX, int gridY);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
};

class Vial : public CollectibleItem
{
    RTTI_DECL;
    
public:
    Vial(int gridX, int gridY);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
};

#endif /* defined(__nosfuratu__CollectibleItem__) */
