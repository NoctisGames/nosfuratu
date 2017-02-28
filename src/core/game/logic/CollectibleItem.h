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
#include "Color.h"
#include "RTTI.h"
#include "GameConstants.h"

#include <memory>

typedef enum
{
    CollectibleItemType_Carrot,
    CollectibleItemType_GoldenCarrot,
    CollectibleItemType_BigCarrot,
    CollectibleItemType_Vial
} CollectibleItemType;

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
    
    Color& getColor() { return m_color; }
    
    void setGame(Game* game) { m_game = game; }
    
protected:
    Game* m_game;
    CollectibleItemType m_type;
    Color m_color;
    float m_fOriginalY;
    int m_iCollectSoundId;
    bool m_isCollected;
    bool m_isOnScreen;
};

class Carrot : public CollectibleItem
{
    RTTI_DECL;
    
public:
    Carrot(int gridX, int gridY) : CollectibleItem(gridX, gridY, 6, 7, SOUND_COLLECT_CARROT, CollectibleItemType_Carrot) {}
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    virtual void snapToGrid(int gridCellSizeScalar = 1);
    
    virtual void resize();
};

class GoldenCarrotTwinkle : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    GoldenCarrotTwinkle(float x, float y, float seedStateTime) : PhysicalEntity(x, y, 0.65039062500001f, 0.68554687500001f)
    {
        m_fStateTime = seedStateTime;
    }
};

class GoldenCarrot : public CollectibleItem
{
    RTTI_DECL;
    
public:
    GoldenCarrot(int gridX, int gridY) : CollectibleItem(gridX, gridY, 6, 8, SOUND_COLLECT_GOLDEN_CARROT, CollectibleItemType_GoldenCarrot), m_iIndex(0), m_isPreviouslyCollected(false)
    {
        float x = m_position.getX();
        float y = m_position.getY();
        float w = m_fWidth;
        float h = m_fHeight;
        float l = x - w / 2;
        float b = y - (h / 2);
        
        m_goldenCarrotTwinkle = std::unique_ptr<GoldenCarrotTwinkle>(new GoldenCarrotTwinkle(0.53543307086614f * w + l, 0.60714285714286f * h + b, m_fStateTime));
    }
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    virtual void snapToGrid(int gridCellSizeScalar = 1);
    
    virtual void resize();
    
    void init(int index, int bestLevelStatsFlag);
    
    GoldenCarrotTwinkle& getGoldenCarrotTwinkle() { return *m_goldenCarrotTwinkle; }
    
    int getIndex();
    
    bool isPreviouslyCollected();
    
private:
    std::unique_ptr<GoldenCarrotTwinkle> m_goldenCarrotTwinkle;
    int m_iIndex;
    bool m_isPreviouslyCollected;
};

class BigCarrot : public CollectibleItem
{
    RTTI_DECL;
    
public:
    BigCarrot(int gridX, int gridY) : CollectibleItem(gridX, gridY, 27, 27, SOUND_COLLECT_BIG_CARROT, CollectibleItemType_BigCarrot, 0.405092592592593f, 0.259259259259259f, 0.428240740740741f, 0.481481481481481f) {}
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
};

class Vial : public CollectibleItem
{
    RTTI_DECL;
    
public:
    Vial(int gridX, int gridY) : CollectibleItem(gridX, gridY, 20, 21, SOUND_COLLECT_VIAL, CollectibleItemType_Vial, 0.43125f, 0.25297619047619f, 0.25f, 0.339285714285714f) {}
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
};

#endif /* defined(__nosfuratu__CollectibleItem__) */
