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
#include "ScreenEvent.h"
#include "Vector2D.h"
#include "NGRect.h"
#include "OverlapTester.h"
#include "Midground.h"
#include "Ground.h"
#include "ExitGround.h"
#include "Hole.h"
#include "ForegroundObject.h"
#include "Enemy.h"
#include "CollectibleItem.h"
#include "Jon.h"
#include "EndBossSnake.h"
#include "RTTI.h"
#include "ForegroundCoverObject.h"
#include "CountHissWithMina.h"

#include <memory>
#include <vector>
#include <math.h>

class Game;

#define LEVEL_EDITOR_ENTITIES_PANEL_RC_UNHANDLED -1
#define LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED 0
#define LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED 1

class LevelEditorEntitiesPanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    LevelEditorEntitiesPanel(float x = CAM_WIDTH + (3.76608187134503f - 1.89473684210526) / 2, float y = CAM_HEIGHT / 2, float width = 3.76608187134503f, float height = CAM_HEIGHT);
    
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
    template<typename T>
    static bool isTouchingEntityForPlacement(std::vector<T*>& items, std::vector<T*>& gameItems, float x, float y, PhysicalEntity** lastAddedEntity, Vector2D& touchPoint)
    {
        int retVal = -1;
        int index = 0;
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++, index++)
        {
            T* item = *i;
            float width = item->getWidth();
            float height = item->getHeight();
            float x = item->getPosition().getX() - width / 2;
            float y = item->getPosition().getY() - height / 2;
            
            NGRect tempBounds = NGRect(x, y, width, height);
            if (OverlapTester::isPointInNGRect(touchPoint, tempBounds))
            {
                retVal = index;
                break;
            }
        }
        
        if (retVal != -1)
        {
            T* pT = T::create(x, y, items.at(index)->getType());
            gameItems.push_back(pT);
            
            *lastAddedEntity = pT;
        }
        
        return retVal != -1;
    }
    
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
    
    std::unique_ptr<NGRect> m_openButton;
    std::unique_ptr<NGRect> m_closeButton;
    std::unique_ptr<Vector2D> m_touchPointDown;
    std::unique_ptr<Vector2D> m_touchPointDown2;
    
    float m_fEntitiesCameraPos;
    float m_fEntitiesHeight;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__LevelEditorEntitiesPanel__) */
