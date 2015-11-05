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
#include "CaveSkeleton.h"
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
    
    std::vector<std::unique_ptr<Tree>>& getTrees();
    
    std::vector<std::unique_ptr<CaveSkeleton>>& getCaveSkeletons();
    
    std::vector<std::unique_ptr<Ground>>& getGrounds();
    
    std::vector<std::unique_ptr<Hole>>& getHoles();
    
    std::vector<std::unique_ptr<CaveExit>>& getCaveExits();
    
    std::vector<std::unique_ptr<LogVerticalTall>>& getLogVerticalTalls();
    
    std::vector<std::unique_ptr<LogVerticalShort>>& getLogVerticalShorts();
    
    std::vector<std::unique_ptr<Thorns>>& getThorns();
    
    std::vector<std::unique_ptr<Stump>>& getStumps();
    
    std::vector<std::unique_ptr<SideSpike>>& getSideSpikes();
    
    std::vector<std::unique_ptr<UpwardSpike>>& getUpwardSpikes();
    
    std::vector<std::unique_ptr<JumpSpring>>& getJumpSprings();
    
    std::vector<std::unique_ptr<Rock>>& getRocks();
    
    std::vector<std::unique_ptr<GroundPlatform>>& getPlatforms();
    
    std::vector<std::unique_ptr<EndSign>>& getEndSigns();
    
    std::vector<std::unique_ptr<Carrot>>& getCarrots();
    
    std::vector<std::unique_ptr<GoldenCarrot>>& getGoldenCarrots();
    
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
    std::vector<std::unique_ptr<Tree>> m_trees;
    std::vector<std::unique_ptr<CaveSkeleton>> m_caveSkeletons;
    std::vector<std::unique_ptr<Ground>> m_grounds;
    std::vector<std::unique_ptr<Hole>> m_holes;
    std::vector<std::unique_ptr<CaveExit>> m_caveExits;
    std::vector<std::unique_ptr<LogVerticalTall>> m_logVerticalTalls;
    std::vector<std::unique_ptr<LogVerticalShort>> m_logVerticalShorts;
    std::vector<std::unique_ptr<Thorns>> m_thorns;
    std::vector<std::unique_ptr<Stump>> m_stumps;
    std::vector<std::unique_ptr<SideSpike>> m_sideSpikes;
    std::vector<std::unique_ptr<UpwardSpike>> m_upwardSpikes;
    std::vector<std::unique_ptr<JumpSpring>> m_jumpSprings;
    std::vector<std::unique_ptr<Rock>> m_rocks;
    std::vector<std::unique_ptr<GroundPlatform>> m_platforms;
    std::vector<std::unique_ptr<EndSign>> m_endSigns;
    std::vector<std::unique_ptr<Carrot>> m_carrots;
    std::vector<std::unique_ptr<GoldenCarrot>> m_goldenCarrots;
    
    std::unique_ptr<Rectangle> m_openButton;
    std::unique_ptr<Rectangle> m_closeButton;
    std::unique_ptr<Vector2D> m_touchPointDown;
    std::unique_ptr<Vector2D> m_touchPointDown2;
    float m_fEntitiesCameraPos;
    float m_fEntitiesHeight;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__LevelEditorEntitiesPanel__) */
