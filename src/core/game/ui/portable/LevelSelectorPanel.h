//
//  LevelSelectorPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/28/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__LevelSelectorPanel__
#define __nosfuratu__LevelSelectorPanel__

#include "PhysicalEntity.h"

#include "GameConstants.h"
#include "ScreenEvent.h"
#include "Vector2D.h"
#include "NGRect.h"

#include "RTTI.h"

#define LEVEL_SELECTOR_PANEL_RC_UNHANDLED -1
#define LEVEL_SELECTOR_PANEL_RC_HANDLED 0
#define LEVEL_SELECTOR_PANEL_RC_CONFIRM 1

class LevelSelectorPanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    LevelSelectorPanel(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH / 3, float height = CAM_HEIGHT / 3);
    
    virtual ~LevelSelectorPanel();
    
    void open();
    
    void close();
    
    int handleTouch(ScreenEvent& te, Vector2D& touchPoint);
    
    Vector2D& getWorldTextPosition();
    
    Vector2D& getLevelTextPosition();
    
    float getTextSize();
    
    int getWorld();
    
    int getLevel();
    
    bool isOpen();
    
private:
    NGRect* m_toggleWorldButton;
    NGRect* m_toggleLevelButton;
    NGRect* m_confirmButton;
    Vector2D* m_worldTextPosition;
    Vector2D* m_levelTextPosition;
    int m_iWorld;
    int m_iLevel;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__LevelSelectorPanel__) */
