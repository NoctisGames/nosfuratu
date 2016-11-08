//
//  SpriteTesterEntitiesPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__SpriteTesterEntitiesPanel__
#define __nosfuratu__SpriteTesterEntitiesPanel__

#include "PhysicalEntity.h"

#include "GameConstants.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "OverlapTester.h"
#include "RTTI.h"

#include <vector>
#include <math.h>

class UnknownEntity;

#define SPRITE_TESTER_ENTITIES_PANEL_RC_UNHANDLED -1
#define SPRITE_TESTER_ENTITIES_PANEL_RC_HANDLED 0
#define SPRITE_TESTER_ENTITIES_PANEL_RC_ENTITY_ADDED 1

class SpriteTesterEntitiesPanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    SpriteTesterEntitiesPanel(float x = CAM_WIDTH + (3.76608187134503f - 1.89473684210526) / 2, float y = CAM_HEIGHT / 2, float width = 3.76608187134503f, float height = CAM_HEIGHT);
    
    int handleTouch(TouchEvent& te, Vector2D& touchPoint, Vector2D& camPos, std::vector<UnknownEntity*>& gameItems, UnknownEntity** lastAddedEntity);
    
    std::vector<UnknownEntity *>& getEntities();
    
    float getEntitiesCameraPos();
    
    bool isOpen();
    
private:
    bool isTouchingEntityForPlacement(std::vector<UnknownEntity*>& items, std::vector<UnknownEntity*>& gameItems, float x, float y, UnknownEntity** lastAddedEntity, Vector2D& touchPoint);
    
    std::vector<UnknownEntity *> m_entities;
    
    std::unique_ptr<Rectangle> m_openButton;
    std::unique_ptr<Rectangle> m_closeButton;
    std::unique_ptr<Vector2D> m_touchPointDown;
    std::unique_ptr<Vector2D> m_touchPointDown2;
    
    float m_fEntitiesCameraPos;
    float m_fEntitiesHeight;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__SpriteTesterEntitiesPanel__) */
