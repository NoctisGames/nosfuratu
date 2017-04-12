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

#include "RTTI.h"

class Vector2D;
class NGRect;
class ScreenEvent;

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
    
    int handleTouch(ScreenEvent& te, Vector2D& touchPoint);
    
    int boundsLevelRequested();
    
    bool showBounds();
    
private:
    NGRect* m_toggleBoundsButton;
    NGRect* m_markerButton;
    NGRect* m_offsetButton;
    NGRect* m_resetButton;
    NGRect* m_exitButton;
    NGRect* m_testButton;
    NGRect* m_loadButton;
    NGRect* m_saveButton;
    NGRect* m_openButton;
    NGRect* m_closeButton;
    
    int m_iBoundsLevelRequested;
    bool m_showBounds;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__LevelEditorActionsPanel__) */
