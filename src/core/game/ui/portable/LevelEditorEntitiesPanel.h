//
//  LevelEditorEntitiesPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__LevelEditorEntitiesPanel__
#define __nosfuratu__LevelEditorEntitiesPanel__

#include "PhysicalEntity.h"

#include "GameConstants.h"

#include "RTTI.h"

#include <vector>

class Game;
class ScreenEvent;
class Vector2D;
class NGRect;
class Midground;
class Ground;
class ExitGround;
class Hole;
class ForegroundObject;
class ExtraForegroundObject;
class Enemy;
class CollectibleItem;
class Jon;
class EndBossSnake;
class ForegroundCoverObject;
class CountHissWithMina;

#define LEVEL_EDITOR_ENTITIES_PANEL_RC_UNHANDLED -1
#define LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED 0
#define LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED 1

class LevelEditorEntitiesPanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    LevelEditorEntitiesPanel(float x = CAM_WIDTH + (3.76608187134503f - 1.89473684210526) / 2, float y = CAM_HEIGHT / 2, float width = 3.76608187134503f, float height = CAM_HEIGHT);
    
    virtual ~LevelEditorEntitiesPanel();
    
    void initForLevel(int world, int level);
    
    int handleTouch(ScreenEvent& te, Vector2D& touchPoint, Game& game, Vector2D& camPos, PhysicalEntity** lastAddedEntity);
    
    std::vector<Midground *>& getMidgrounds();
    
    std::vector<Ground *>& getGrounds();
    
    std::vector<Ground *>& getPits();
    
    std::vector<ExitGround *>& getExitGrounds();
    
    std::vector<Hole *>& getHoles();
    
    std::vector<ForegroundObject *>& getForegroundObjects();
    
    std::vector<ForegroundObject *>& getMidBossForegroundObjects();
    
    std::vector<ForegroundObject *>& getEndBossForegroundObjects();
    
    std::vector<CountHissWithMina *>& getCountHissWithMinas();
    
    std::vector<EndBossSnake *>& getEndBossSnakes();
    
    std::vector<Enemy *>& getEnemies();
    
    std::vector<CollectibleItem *>& getCollectibleItems();
    
    std::vector<Jon *>& getJons();
    
    std::vector<ExtraForegroundObject *>& getExtraForegroundObjects();
    
    std::vector<ForegroundCoverObject *>& getForegroundCoverObjects();
    
    float getEntitiesCameraPos();
    
    bool isOpen();
    
private:
    std::vector<Midground *> m_midgrounds;
    std::vector<Ground *> m_grounds;
    std::vector<Ground *> m_pits;
    std::vector<ExitGround *> m_exitGrounds;
    std::vector<Hole *> m_holes;
    std::vector<ForegroundObject *> m_foregroundObjects;
    std::vector<ForegroundObject *> m_midBossForegroundObjects;
    std::vector<ForegroundObject *> m_endBossForegroundObjects;
    std::vector<CountHissWithMina *> m_countHissWithMinas;
    std::vector<EndBossSnake *> m_endBossSnakes;
    std::vector<Enemy *> m_enemies;
    std::vector<CollectibleItem *> m_collectibleItems;
    std::vector<Jon *> m_jons;
    std::vector<ExtraForegroundObject *> m_extraForegroundObjects;
    std::vector<ForegroundCoverObject *> m_foregroundCoverObjects;
    
    NGRect* m_openButton;
    NGRect* m_closeButton;
    Vector2D* m_touchPointDown;
    Vector2D* m_touchPointDown2;
    
    float m_fEntitiesCameraPos;
    float m_fEntitiesHeight;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__LevelEditorEntitiesPanel__) */
