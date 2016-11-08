//
//  SpriteTesterActionsPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 11/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "SpriteTesterActionsPanel.h"

#include "Rectangle.h"
#include "GameConstants.h"
#include "OverlapTester.h"

SpriteTesterActionsPanel::SpriteTesterActionsPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_isOpen(false)
{
    m_resetButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.09908256880734f, height * 0.57100149476831f, width * 0.68990825688073f, height * 0.09118086696562f));
    m_exitButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.09908256880734f, height * 0.44544095665172f, width * 0.68990825688073f, height * 0.09118086696562f));
    m_loadButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.09908256880734f, height * 0.13976083707025f, width * 0.68990825688073f, height * 0.09118086696562f));
    m_openButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0, height * 0.49081920903955f, 1, 1));
    m_closeButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.83305227655987f, height * 0.49081920903955f, 1, 1));
}

int SpriteTesterActionsPanel::handleTouch(TouchEvent& te, Vector2D& touchPoint)
{
    if (m_isOpen)
    {
		if (OverlapTester::isPointInRectangle(touchPoint, *m_resetButton))
		{
			if (te.getTouchType() == UP)
			{
				return SPRITE_TESTER_ACTIONS_PANEL_RC_RESET;
			}

			return SPRITE_TESTER_ACTIONS_PANEL_RC_HANDLED;
		}
		else if (OverlapTester::isPointInRectangle(touchPoint, *m_exitButton))
		{
			if (te.getTouchType() == UP)
			{
				return SPRITE_TESTER_ACTIONS_PANEL_RC_EXIT;
			}

			return SPRITE_TESTER_ACTIONS_PANEL_RC_HANDLED;
		}
		else if (OverlapTester::isPointInRectangle(touchPoint, *m_loadButton))
		{
			if (te.getTouchType() == UP)
			{
				return SPRITE_TESTER_ACTIONS_PANEL_RC_LOAD;
			}

			return SPRITE_TESTER_ACTIONS_PANEL_RC_HANDLED;
		}
		else if (OverlapTester::isPointInRectangle(touchPoint, *m_closeButton))
		{
			if (te.getTouchType() == UP)
			{
				m_position->setX(-3.76608187134503f / 2 + 0.94736842105263f);

				m_isOpen = false;
			}

			return SPRITE_TESTER_ACTIONS_PANEL_RC_HANDLED;
		}
    }
    else
    {
		if (OverlapTester::isPointInRectangle(touchPoint, *m_openButton))
		{
			if (te.getTouchType() == UP)
			{
				m_position->setX(getWidth() / 2);

				m_isOpen = true;
			}

			return SPRITE_TESTER_ACTIONS_PANEL_RC_HANDLED;
		}
    }
    
    return SPRITE_TESTER_ACTIONS_PANEL_RC_UNHANDLED;
}

RTTI_IMPL(SpriteTesterActionsPanel, PhysicalEntity);
