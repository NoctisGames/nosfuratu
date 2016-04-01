//
//  ConfirmExitPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/31/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "ConfirmExitPanel.h"
#include "OverlapTester.h"

ConfirmExitPanel::ConfirmExitPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_isOpen(false)
{
    float l = x - width / 2;
    float b = y - height / 2;
    
    m_cancelButton = std::unique_ptr<Rectangle>(new Rectangle(l + width * 0.07647058823529f, b + height * 0.36274509803922f, width * 0.3921568627451f, height * 0.3921568627451f));
    m_exitButton = std::unique_ptr<Rectangle>(new Rectangle(l + width * 0.55490196078431f, b + height * 0.36274509803922f, width * 0.3921568627451f, height * 0.3921568627451f));
}

void ConfirmExitPanel::open()
{
    m_isOpen = true;
}

void ConfirmExitPanel::close()
{
    m_isOpen = false;
}

int ConfirmExitPanel::handleTouch(TouchEvent& te, Vector2D& touchPoint)
{
    if (te.getTouchType() == Touch_Type::UP)
    {
        if (OverlapTester::isPointInRectangle(touchPoint, *m_cancelButton))
        {
            m_isOpen = false;
            
            return CONFIRM_EXIT_PANEL_RC_CANCEL;
        }
        else if (OverlapTester::isPointInRectangle(touchPoint, *m_exitButton))
        {
            return CONFIRM_EXIT_PANEL_RC_EXIT;
        }
    }
    
    return CONFIRM_EXIT_PANEL_RC_UNHANDLED;
}

bool ConfirmExitPanel::isOpen()
{
    return m_isOpen;
}