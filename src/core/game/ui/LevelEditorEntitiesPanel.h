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
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "OverlapTester.h"
#include "Midground.h"
#include "Ground.h"
#include "ExitGround.h"
#include "Hole.h"
#include "ForegroundObject.h"
#include "Enemy.h"
#include "CollectibleItem.h"
#include "Jon.h"

#include <vector>
#include <math.h>

class Game;

#define LEVEL_EDITOR_ENTITIES_PANEL_RC_UNHANDLED -1
#define LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED 0
#define LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED 1

class LevelEditorEntitiesPanel : public PhysicalEntity
{
public:
    LevelEditorEntitiesPanel(float x = CAM_WIDTH + (3.76608187134503f - 1.89473684210526) / 2, float y = CAM_HEIGHT / 2, float width = 3.76608187134503f, float height = CAM_HEIGHT);
    
    void initForLevel(int world, int level);
    
    int handleTouch(TouchEvent& te, Vector2D& touchPoint, Game& game, Vector2D& camPos, PhysicalEntity** lastAddedEntity);
    
    std::vector<Midground *>& getMidgrounds();
    
    std::vector<Ground *>& getGrounds();
    
    std::vector<Ground *>& getPits();
    
    std::vector<ExitGround *>& getExitGrounds();
    
    std::vector<Hole *>& getHoles();
    
    std::vector<ForegroundObject *>& getForegroundObjects();
    
    std::vector<ForegroundObject *>& getMidBossForegroundObjects();
    
    std::vector<CountHissWithMina *>& getCountHissWithMinas();
    
    std::vector<Enemy *>& getEnemies();
    
    std::vector<CollectibleItem *>& getCollectibleItems();
    
    std::vector<Jon *>& getJons();
    
    std::vector<ExtraForegroundObject *>& getExtraForegroundObjects();
    
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
            
            Rectangle tempBounds = Rectangle(x, y, width, height);
            if (OverlapTester::isPointInRectangle(touchPoint, tempBounds))
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
    
    template<typename T>
    static int boxInAll(std::vector<T*>& items, float eX, float eY, float eWidth, float eHeight, int index)
    {
        float size = fminf(eWidth, eHeight);
        
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            T* item = *i;
            
            item->getPosition().set(eX, eY + (index++ * eHeight));
            item->setWidth(eWidth);
            item->setHeight(eHeight);
            
            if (item->getWidth() > item->getHeight())
            {
                item->setHeight(item->getHeight() / item->getWidth());
                item->setHeight(item->getHeight() * size);
                item->setWidth(size);
            }
            else
            {
                item->setWidth(item->getWidth() / item->getHeight());
                item->setWidth(item->getWidth() * size);
                item->setHeight(size);
            }
        }
        
        return index;
    }
    
    std::vector<Midground *> m_midgrounds;
    std::vector<Ground *> m_grounds;
    std::vector<Ground *> m_pits;
    std::vector<ExitGround *> m_exitGrounds;
    std::vector<Hole *> m_holes;
    std::vector<ForegroundObject *> m_foregroundObjects;
    std::vector<ForegroundObject *> m_midBossForegroundObjects;
    std::vector<CountHissWithMina *> m_countHissWithMinas;
    std::vector<Enemy *> m_enemies;
    std::vector<CollectibleItem *> m_collectibleItems;
    std::vector<Jon *> m_jons;
    std::vector<ExtraForegroundObject *> m_extraForegroundObjects;
    
    std::unique_ptr<Rectangle> m_openButton;
    std::unique_ptr<Rectangle> m_closeButton;
    std::unique_ptr<Vector2D> m_touchPointDown;
    std::unique_ptr<Vector2D> m_touchPointDown2;
    
    float m_fEntitiesCameraPos;
    float m_fEntitiesHeight;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__LevelEditorEntitiesPanel__) */
