//
//  LevelEditorActionsPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "LevelEditorActionsPanel.h"

#include "Rectangle.h"
#include "GameConstants.h"
#include "OverlapTester.h"

LevelEditorActionsPanel::LevelEditorActionsPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_iBoundsLevelRequested(4), m_showBounds(true), m_isOpen(false)
{
    m_toggleBoundsButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.09908256880734f, height * 0.87070254110613f, width * 0.68990825688073f, height * 0.09118086696562f));
    m_markerButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.09908256880734f, height * 0.76382660687593f, width * 0.68990825688073f, height * 0.09118086696562f));
    m_offsetButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.09908256880734f, height * 0.66816143497758f, width * 0.68990825688073f, height * 0.09118086696562f));
    m_resetButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.09908256880734f, height * 0.57100149476831f, width * 0.68990825688073f, height * 0.09118086696562f));
    m_exitButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.09908256880734f, height * 0.44544095665172f, width * 0.68990825688073f, height * 0.09118086696562f));
    m_testButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.09908256880734f, height * 0.26158445440957f, width * 0.68990825688073f, height * 0.09118086696562f));
    m_loadButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.09908256880734f, height * 0.13976083707025f, width * 0.68990825688073f, height * 0.09118086696562f));
    m_saveButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.09908256880734f, height * 0.01718983557549f, width * 0.68990825688073f, height * 0.09118086696562f));
    m_openButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0, height * 0.49081920903955f, 1, 1));
    m_closeButton = std::unique_ptr<Rectangle>(new Rectangle(width * 0.83305227655987f, height * 0.49081920903955f, 1, 1));
}

int LevelEditorActionsPanel::handleTouch(TouchEvent& te, Vector2D& touchPoint)
{
    if (m_isOpen)
    {
		if (OverlapTester::isPointInRectangle(touchPoint, *m_toggleBoundsButton))
		{
			if (te.getTouchType() == UP)
			{
				if (m_showBounds)
				{
					m_iBoundsLevelRequested++;

					if (m_iBoundsLevelRequested == 3)
					{
						m_iBoundsLevelRequested++;
					}

					if (m_iBoundsLevelRequested == 5)
					{
						m_iBoundsLevelRequested = 8;
					}

					if (m_iBoundsLevelRequested == 9)
					{
						m_iBoundsLevelRequested = 16;
					}

					if (m_iBoundsLevelRequested == 17)
					{
						m_iBoundsLevelRequested = 1;
						m_showBounds = false;
					}
				}
				else
				{
					m_showBounds = true;
				}
			}

			return LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED;
		}
		else if (OverlapTester::isPointInRectangle(touchPoint, *m_markerButton))
		{
			if (te.getTouchType() == UP)
			{
				return LEVEL_EDITOR_ACTIONS_PANEL_RC_MARKER;
			}
			
			return LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED;
		}
		else if (OverlapTester::isPointInRectangle(touchPoint, *m_offsetButton))
		{
			if (te.getTouchType() == UP)
			{
				return LEVEL_EDITOR_ACTIONS_PANEL_RC_OFFSET;
			}

			return LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED;
		}
		else if (OverlapTester::isPointInRectangle(touchPoint, *m_resetButton))
		{
			if (te.getTouchType() == UP)
			{
				return LEVEL_EDITOR_ACTIONS_PANEL_RC_RESET;
			}

			return LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED;
		}
		else if (OverlapTester::isPointInRectangle(touchPoint, *m_exitButton))
		{
			if (te.getTouchType() == UP)
			{
				return LEVEL_EDITOR_ACTIONS_PANEL_RC_EXIT;
			}

			return LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED;
		}
		else if (OverlapTester::isPointInRectangle(touchPoint, *m_testButton))
		{
			if (te.getTouchType() == UP)
			{
				return LEVEL_EDITOR_ACTIONS_PANEL_RC_TEST;
			}

			return LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED;
		}
		else if (OverlapTester::isPointInRectangle(touchPoint, *m_loadButton))
		{
			if (te.getTouchType() == UP)
			{
				return LEVEL_EDITOR_ACTIONS_PANEL_RC_LOAD;
			}

			return LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED;
		}
		else if (OverlapTester::isPointInRectangle(touchPoint, *m_saveButton))
		{
			if (te.getTouchType() == UP)
			{
				return LEVEL_EDITOR_ACTIONS_PANEL_RC_SAVE;
			}

			return LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED;
		}
		else if (OverlapTester::isPointInRectangle(touchPoint, *m_closeButton))
		{
			if (te.getTouchType() == UP)
			{
				m_position->setX(-3.76608187134503f / 2 + 0.94736842105263f);

				m_isOpen = false;
			}

			return LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED;
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

			return LEVEL_EDITOR_ACTIONS_PANEL_RC_HANDLED;
		}
    }
    
    return LEVEL_EDITOR_ACTIONS_PANEL_RC_UNHANDLED;
}

int LevelEditorActionsPanel::boundsLevelRequested()
{
    return m_iBoundsLevelRequested;
}

bool LevelEditorActionsPanel::showBounds()
{
    return m_showBounds;
}

RTTI_IMPL(LevelEditorActionsPanel, PhysicalEntity);
