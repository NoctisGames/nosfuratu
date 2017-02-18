//
//  ConfirmExitPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/31/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__ConfirmExitPanel__
#define __nosfuratu__ConfirmExitPanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "ScreenEvent.h"
#include "Vector2D.h"
#include "NGRect.h"
#include "RTTI.h"

#define CONFIRM_EXIT_PANEL_RC_UNHANDLED 0
#define CONFIRM_EXIT_PANEL_RC_CANCEL 1
#define CONFIRM_EXIT_PANEL_RC_EXIT 2

class ConfirmExitPanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    ConfirmExitPanel(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH / 3, float height = CAM_HEIGHT / 3);
    
    void open();
    
    void close();
    
    int handleTouch(ScreenEvent& te, Vector2D& touchPoint);
    
    bool isOpen();
    
private:
    std::unique_ptr<NGRect> m_cancelButton;
    std::unique_ptr<NGRect> m_exitButton;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__ConfirmExitPanel__) */
