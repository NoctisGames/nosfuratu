//
//  ConfirmResetPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/31/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "ConfirmResetPanel.h"
#include "OverlapTester.h"

ConfirmResetPanel::ConfirmResetPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_isOpen(false)
{
    float l = x - width / 2;
    float b = y - height / 2;
    
    m_cancelButton = std::unique_ptr<Rectangle>(new Rectangle(l + width * 0.07647058823529f, b + height * 0.36274509803922f, width * 0.3921568627451f, height * 0.3921568627451f));
    m_resetButton = std::unique_ptr<Rectangle>(new Rectangle(l + width * 0.55490196078431f, b + height * 0.36274509803922f, width * 0.3921568627451f, height * 0.3921568627451f));
}

void ConfirmResetPanel::open()
{
    m_isOpen = true;
}

void ConfirmResetPanel::close()
{
    m_isOpen = false;
}

int ConfirmResetPanel::handleTouch(TouchEvent& te, Vector2D& touchPoint)
{
    if (te.getTouchType() == Touch_Type::UP)
    {
        if (OverlapTester::isPointInRectangle(touchPoint, *m_cancelButton))
        {
            m_isOpen = false;
            
            return CONFIRM_RESET_PANEL_RC_CANCEL;
        }
        else if (OverlapTester::isPointInRectangle(touchPoint, *m_resetButton))
        {
            return CONFIRM_RESET_PANEL_RC_RESET;
        }
    }
    
    return CONFIRM_RESET_PANEL_RC_UNHANDLED;
}

bool ConfirmResetPanel::isOpen()
{
    return m_isOpen;
}