//
//  OffsetPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/31/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OffsetPanel.h"
#include "OverlapTester.h"

OffsetPanel::OffsetPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_iOffset(0), m_isOpen(false)
{
    float l = x - width / 2;
    float b = y - height / 2;
    
    m_offsetLeftButton = std::unique_ptr<NGRect>(new NGRect(l + width * 0.07647058823529f, b + height * 0.36274509803922f, width * 0.3921568627451f, height * 0.3921568627451f));
    m_offsetRightButton = std::unique_ptr<NGRect>(new NGRect(l + width * 0.55490196078431f, b + height * 0.36274509803922f, width * 0.3921568627451f, height * 0.3921568627451f));
    m_confirmButton = std::unique_ptr<NGRect>(new NGRect(l + width * 0.12941176470588f, b + height * 0.07843137254902f, width * 0.74117647058824f, height * 0.23921568627451f));
    
    m_leftTextPosition = std::unique_ptr<Vector2D>(new Vector2D(m_offsetLeftButton->getLeft() + m_offsetLeftButton->getWidth() - m_offsetLeftButton->getWidth() / 3, m_offsetLeftButton->getBottom() + m_offsetLeftButton->getHeight() / 2));
    m_rightTextPosition = std::unique_ptr<Vector2D>(new Vector2D(m_offsetRightButton->getLeft() + m_offsetRightButton->getWidth() - m_offsetRightButton->getWidth() / 3, m_offsetRightButton->getBottom() + m_offsetRightButton->getHeight() / 2));
}

void OffsetPanel::open()
{
    m_isOpen = true;
}

void OffsetPanel::close()
{
    m_isOpen = false;
}

int OffsetPanel::handleTouch(ScreenEvent& te, Vector2D& touchPoint)
{
    if (te.getType() == ScreenEventType_DOWN
        || te.getType() == ScreenEventType_DRAGGED)
    {
        if (OverlapTester::isPointInNGRect(touchPoint, *m_offsetLeftButton))
        {
            m_iOffset--;
            
            return OFFSET_PANEL_RC_HANDLED;
        }
        else if (OverlapTester::isPointInNGRect(touchPoint, *m_offsetRightButton))
        {
            m_iOffset++;
            
            return OFFSET_PANEL_RC_HANDLED;
        }
        else if (OverlapTester::isPointInNGRect(touchPoint, *m_confirmButton))
        {
			m_iOffset = 0;
            
			m_isOpen = false;

            return OFFSET_PANEL_RC_CONFIRM;
        }
    }
    
    return OFFSET_PANEL_RC_UNHANDLED;
}

Vector2D& OffsetPanel::getLeftTextPosition()
{
    return *m_leftTextPosition;
}

Vector2D& OffsetPanel::getRightTextPosition()
{
    return *m_rightTextPosition;
}

float OffsetPanel::getTextSize()
{
    return m_offsetLeftButton->getWidth() / 3;
}

int OffsetPanel::getOffset()
{
    return m_iOffset;
}

bool OffsetPanel::isOpen()
{
    return m_isOpen;
}

RTTI_IMPL(OffsetPanel, PhysicalEntity);
