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

typedef enum
{
    CollectibleItemType_Carrot,
    CollectibleItemType_GoldenCarrot
} CollectibleItemType;

class Game;

class CollectibleItem : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static CollectibleItem* create(int gridX, int gridY, int type);
    
    CollectibleItem(int gridX, int gridY, int gridWidth, int gridHeight, int collectSoundId, CollectibleItemType type);
    
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

#endif /* defined(__nosfuratu__CollectibleItem__) */
