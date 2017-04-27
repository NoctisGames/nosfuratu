//
//  ConfirmExitPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/31/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "ConfirmExitPanel.h"

#include "ScreenEvent.h"
#include "Vector2D.h"
#include "NGRect.h"

#include "OverlapTester.h"

ConfirmExitPanel::ConfirmExitPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_isOpen(false)
{
    float l = x - width / 2;
    float b = y - height / 2;
    
    m_cancelButton = new NGRect(l + width * 0.07647058823529f, b + height * 0.36274509803922f, width * 0.3921568627451f, height * 0.3921568627451f);
    m_exitButton = new NGRect(l + width * 0.55490196078431f, b + height * 0.36274509803922f, width * 0.3921568627451f, height * 0.3921568627451f);
}

ConfirmExitPanel::~ConfirmExitPanel()
{
    delete m_cancelButton;
    delete m_exitButton;
}

void ConfirmExitPanel::open()
{
    m_isOpen = true;
}

void ConfirmExitPanel::close()
{
    m_isOpen = false;
}

int ConfirmExitPanel::handleTouch(ScreenEvent& te, Vector2D& touchPoint)
{
    if (te.getType() == ScreenEventType_UP)
    {
        if (OverlapTester::isPointInNGRect(touchPoint, *m_cancelButton))
        {
            m_isOpen = false;
            
            return CONFIRM_EXIT_PANEL_RC_CANCEL;
        }
        else if (OverlapTester::isPointInNGRect(touchPoint, *m_exitButton))
        {
			m_isOpen = false;

            return CONFIRM_EXIT_PANEL_RC_EXIT;
        }
    }
    
    return CONFIRM_EXIT_PANEL_RC_UNHANDLED;
}

bool ConfirmExitPanel::isOpen()
{
    return m_isOpen;
}

RTTI_IMPL(ConfirmExitPanel, PhysicalEntity);
