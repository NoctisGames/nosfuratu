//
//  LevelEditorEntitiesPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__LevelEditorEntitiesPanel__
#define __nosfuratu__LevelEditorEntitiesPanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Tree.h"
#include "Ground.h"
#include "Hole.h"
#include "CaveExit.h"
#include "LogVerticalTall.h"
#include "LogVerticalShort.h"
#include "Thorns.h"
#include "Stump.h"
#include "SideSpike.h"
#include "UpwardSpike.h"
#include "JumpSpring.h"
#include "Rock.h"
#include "GroundPlatform.h"
#include "EndSign.h"
#include "Carrot.h"
#include "GoldenCarrot.h"
#include "OverlapTester.h"
#include "Jon.h"
#include "SnakeEnemy.h"
#include "SnakeGrunt.h"
#include "SnakeHorned.h"

#include <vector>

class Game;

#define LEVEL_EDITOR_ENTITIES_PANEL_RC_UNHANDLED -1
#define LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED 0
#define LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED 1

class LevelEditorEntitiesPanel : public PhysicalEntity
{
public:
    LevelEditorEntitiesPanel(float x = CAM_WIDTH + (3.76608187134503f - 1.89473684210526) / 2, float y = CAM_HEIGHT / 2, float width = 3.76608187134503f, float height = CAM_HEIGHT);
    
    int handleTouch(TouchEvent& te, Vector2D& touchPoint, Game& game, Vector2D& camPos, PhysicalEntity** lastAddedEntity);
    
    std::vector<std::unique_ptr<Jon>>& getJons();
    
    std::vector<std::unique_ptr<Ground>>& getGrounds();
    
    float getEntitiesCameraPos();
    
    bool isOpen();
    
private:
    template<typename T>
    static bool isTouchingEntityForPlacement(std::vector<std::unique_ptr<T>>& items, std::vector<std::unique_ptr<T>>& gameItems, float x, float y, PhysicalEntity** lastAddedEntity, Vector2D& touchPoint)
    {
        int retVal = -1;
        int index = 0;
        for (typename std::vector<std::unique_ptr<T>>::iterator i = items.begin(); i != items.end(); i++, index++)
        {
            std::unique_ptr<T>& upItem = *i;
            T* item = upItem.get();
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
            gameItems.push_back(std::unique_ptr<T>(pT));
            
            *lastAddedEntity = pT;
        }
        
        return retVal != -1;
    }
    
    template<typename T>
    static void boxInAll(std::vector<std::unique_ptr<T>>& items, float size)
    {
        for (typename std::vector<std::unique_ptr<T>>::iterator i = items.begin(); i != items.end(); i++)
        {
            std::unique_ptr<T>& upItem = *i;
            T* item = upItem.get();
            item->boxIn(size);
        }
    }
    
    std::vector<std::unique_ptr<Jon>> m_jons;
    std::vector<std::unique_ptr<Ground>> m_grounds;
    
    std::unique_ptr<Rectangle> m_openButton;
    std::unique_ptr<Rectangle> m_closeButton;
    std::unique_ptr<Vector2D> m_touchPointDown;
    std::unique_ptr<Vector2D> m_touchPointDown2;
    float m_fEntitiesCameraPos;
    float m_fEntitiesHeight;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__LevelEditorEntitiesPanel__) */
