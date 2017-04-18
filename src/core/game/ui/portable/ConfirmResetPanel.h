//
//  ConfirmResetPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/31/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__ConfirmResetPanel__
#define __nosfuratu__ConfirmResetPanel__

#include "PhysicalEntity.h"

#include "GameConstants.h"

#include "RTTI.h"

class ScreenEvent;
class Vector2D;
class NGRect;

#define CONFIRM_RESET_PANEL_RC_UNHANDLED 0
#define CONFIRM_RESET_PANEL_RC_CANCEL 1
#define CONFIRM_RESET_PANEL_RC_RESET 2

class ConfirmResetPanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    ConfirmResetPanel(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH / 3, float height = CAM_HEIGHT / 3);
    
    virtual ~ConfirmResetPanel();
    
    void open();
    
    void close();
    
    int handleTouch(ScreenEvent& te, Vector2D& touchPoint);
    
    bool isOpen();
    
private:
    NGRect* m_cancelButton;
    NGRect* m_resetButton;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__ConfirmResetPanel__) */
