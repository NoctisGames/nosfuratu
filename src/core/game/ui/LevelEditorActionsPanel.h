//
//  LevelEditorActionsPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__LevelEditorActionsPanel__
#define __nosfuratu__LevelEditorActionsPanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "RTTI.h"

#define LEVEL_EDITOR_ACTIONS_PANEL_RC_UNHANDLED -1
#define LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED 0
#define LEVEL_EDITOR_ACTIONS_PANEL_RC_MARKER 1
#define LEVEL_EDITOR_ACTIONS_PANEL_RC_OFFSET 2
#define LEVEL_EDITOR_ACTIONS_PANEL_RC_RESET 3
#define LEVEL_EDITOR_ACTIONS_PANEL_RC_EXIT 4
#define LEVEL_EDITOR_ACTIONS_PANEL_RC_TEST 6
#define LEVEL_EDITOR_ACTIONS_PANEL_RC_LOAD 7
#define LEVEL_EDITOR_ACTIONS_PANEL_RC_SAVE 8

class LevelEditorActionsPanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    LevelEditorActionsPanel(float x = -3.76608187134503f / 2 + 0.94736842105263f, float y = CAM_HEIGHT / 2, float width = 3.76608187134503f, float height = CAM_HEIGHT);
    
    int handleTouch(TouchEvent& te, Vector2D& touchPoint);
    
    int boundsLevelRequested();
    
    bool showBounds();
    
private:
    std::unique_ptr<Rectangle> m_toggleBoundsButton;
    std::unique_ptr<Rectangle> m_markerButton;
    std::unique_ptr<Rectangle> m_offsetButton;
    std::unique_ptr<Rectangle> m_resetButton;
    std::unique_ptr<Rectangle> m_exitButton;
    std::unique_ptr<Rectangle> m_testButton;
    std::unique_ptr<Rectangle> m_loadButton;
    std::unique_ptr<Rectangle> m_saveButton;
    std::unique_ptr<Rectangle> m_openButton;
    std::unique_ptr<Rectangle> m_closeButton;
    
    int m_iBoundsLevelRequested;
    bool m_showBounds;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__LevelEditorActionsPanel__) */
