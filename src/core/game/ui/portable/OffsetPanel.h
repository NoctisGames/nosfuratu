//
//  OffsetPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/31/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OffsetPanel__
#define __nosfuratu__OffsetPanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "ScreenEvent.h"
#include "Vector2D.h"
#include "NGRect.h"
#include "RTTI.h"

#define OFFSET_PANEL_RC_UNHANDLED -1
#define OFFSET_PANEL_RC_HANDLED 0
#define OFFSET_PANEL_RC_CONFIRM 1

class OffsetPanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    OffsetPanel(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH / 3, float height = CAM_HEIGHT / 3);
    
    void open();
    
    void close();
    
    int handleTouch(ScreenEvent& te, Vector2D& touchPoint);
    
    Vector2D& getLeftTextPosition();
    
    Vector2D& getRightTextPosition();
    
    float getTextSize();
    
    int getOffset();
    
    bool isOpen();
    
private:
    std::unique_ptr<NGRect> m_offsetLeftButton;
    std::unique_ptr<NGRect> m_offsetRightButton;
    std::unique_ptr<NGRect> m_confirmButton;
    std::unique_ptr<Vector2D> m_leftTextPosition;
    std::unique_ptr<Vector2D> m_rightTextPosition;
    int m_iOffset;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__OffsetPanel__) */
