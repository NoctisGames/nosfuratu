//
//  LevelSelectorPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/28/15.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__LevelSelectorPanel__
#define __nosfuratu__LevelSelectorPanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"

#define LEVEL_SELECTOR_PANEL_MODE_SAVE 1
#define LEVEL_SELECTOR_PANEL_MODE_LOAD 2

#define LEVEL_SELECTOR_PANEL_RC_UNHANDLED -1
#define LEVEL_SELECTOR_PANEL_RC_HANDLED 0
#define LEVEL_SELECTOR_PANEL_RC_CONFIRM_SAVE 1
#define LEVEL_SELECTOR_PANEL_RC_CONFIRM_LOAD 2

class LevelSelectorPanel : public PhysicalEntity
{
public:
    LevelSelectorPanel(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH / 3, float height = CAM_HEIGHT / 3);
    
    void openForMode(int mode);
    
    int handleTouch(TouchEvent& te, Vector2D& touchPoint);
    
    Vector2D& getWorldTextPosition();
    
    Vector2D& getLevelTextPosition();
    
    float getTextSize();
    
    int getWorld();
    
    int getLevel();
    
    bool isOpen();
    
private:
    std::unique_ptr<Rectangle> m_toggleWorldButton;
    std::unique_ptr<Rectangle> m_toggleLevelButton;
    std::unique_ptr<Rectangle> m_confirmButton;
    std::unique_ptr<Vector2D> m_worldTextPosition;
    std::unique_ptr<Vector2D> m_levelTextPosition;
    int m_iMode;
    int m_iWorld;
    int m_iLevel;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__LevelSelectorPanel__) */
