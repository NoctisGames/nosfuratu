//
//  SpriteTesterActionsPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__SpriteTesterActionsPanel__
#define __nosfuratu__SpriteTesterActionsPanel__

#include "PhysicalEntity.h"

#include "GameConstants.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "RTTI.h"

#define SPRITE_TESTER_ACTIONS_PANEL_RC_UNHANDLED -1
#define SPRITE_TESTER_ACTIONS_PANEL_RC_HANDLED 0
#define SPRITE_TESTER_ACTIONS_PANEL_RC_RESET 1
#define SPRITE_TESTER_ACTIONS_PANEL_RC_EXIT 2
#define SPRITE_TESTER_ACTIONS_PANEL_RC_LOAD 3

class SpriteTesterActionsPanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    SpriteTesterActionsPanel(float x = -3.76608187134503f / 2 + 0.94736842105263f, float y = CAM_HEIGHT / 2, float width = 3.76608187134503f, float height = CAM_HEIGHT);
    
    int handleTouch(TouchEvent& te, Vector2D& touchPoint);
    
private:
    std::unique_ptr<Rectangle> m_resetButton;
    std::unique_ptr<Rectangle> m_exitButton;
    std::unique_ptr<Rectangle> m_loadButton;
    std::unique_ptr<Rectangle> m_openButton;
    std::unique_ptr<Rectangle> m_closeButton;
    
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__SpriteTesterActionsPanel__) */
