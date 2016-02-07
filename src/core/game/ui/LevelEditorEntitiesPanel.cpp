//
//  LevelEditorEntitiesPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "LevelEditorEntitiesPanel.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "Tree.h"
#include "Game.h"
#include "EntityUtils.h"

LevelEditorEntitiesPanel::LevelEditorEntitiesPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_isOpen(false), m_fEntitiesCameraPos(0), m_fEntitiesHeight(0)
{
    m_openButton = std::unique_ptr<Rectangle>(new Rectangle(CAM_WIDTH - 1, height * 0.49081920903955f, 1, 1));
    m_closeButton = std::unique_ptr<Rectangle>(new Rectangle(CAM_WIDTH - width, height * 0.49081920903955f, 1, 1));
    m_touchPointDown = std::unique_ptr<Vector2D>(new Vector2D());
    m_touchPointDown2 = std::unique_ptr<Vector2D>(new Vector2D());
    
    m_jons.push_back(new Jon(0, 0, 1, 1));
    
    m_grounds.push_back(new CaveExtraDeepEndLeft(0, 0, 1, 1));
    m_grounds.push_back(new CaveExtraDeepSmall(0, 0, 1, 1));
    m_grounds.push_back(new CaveExtraDeepMedium(0, 0, 1, 1));
    m_grounds.push_back(new CaveExtraDeepLarge(0, 0, 1, 1));
    m_grounds.push_back(new CaveExtraDeepEndRight(0, 0, 1, 1));
    
    m_grounds.push_back(new CaveDeepEndLeft(0, 0, 1, 1));
    m_grounds.push_back(new CaveDeepSmall(0, 0, 1, 1));
    m_grounds.push_back(new CaveDeepMedium(0, 0, 1, 1));
    m_grounds.push_back(new CaveDeepLarge(0, 0, 1, 1));
    m_grounds.push_back(new CaveDeepEndRight(0, 0, 1, 1));
    
    m_grounds.push_back(new CaveEndLeft(0, 0, 1, 1));
    m_grounds.push_back(new CaveSmall(0, 0, 1, 1));
    m_grounds.push_back(new CaveMedium(0, 0, 1, 1));
    m_grounds.push_back(new CaveLarge(0, 0, 1, 1));
    m_grounds.push_back(new CaveEndRight(0, 0, 1, 1));
    
    m_grounds.push_back(new CaveRaisedEndLeft(0, 0, 1, 1));
    m_grounds.push_back(new CaveRaisedSmall(0, 0, 1, 1));
    m_grounds.push_back(new CaveRaisedMedium(0, 0, 1, 1));
    m_grounds.push_back(new CaveRaisedLarge(0, 0, 1, 1));
    m_grounds.push_back(new CaveRaisedEndRight(0, 0, 1, 1));
    
    m_grounds.push_back(new GrassWithCaveEndLeft(0, 0, 1, 1));
    m_grounds.push_back(new GrassWithCaveSmall(0, 0, 1, 1));
    m_grounds.push_back(new GrassWithCaveMedium(0, 0, 1, 1));
    m_grounds.push_back(new GrassWithCaveLarge(0, 0, 1, 1));
    m_grounds.push_back(new GrassWithCaveEndRight(0, 0, 1, 1));
    
    m_grounds.push_back(new GrassWithoutCaveEndLeft(0, 0, 1, 1));
    m_grounds.push_back(new GrassWithoutCaveSmall(0, 0, 1, 1));
    m_grounds.push_back(new GrassWithoutCaveMedium(0, 0, 1, 1));
    m_grounds.push_back(new GrassWithoutCaveLarge(0, 0, 1, 1));
    m_grounds.push_back(new GrassWithoutCaveEndRight(0, 0, 1, 1));
    
    float eWidth = width * 0.6f;
    float eHeight = height / 6;
    float eX = CAM_WIDTH - width / 2 + 0.4f;
    float eY = eHeight / 2;
    
    int i = boxInAll(m_jons, eX, eY, eWidth, eHeight, 0);
    i = boxInAll(m_grounds, eX, eY, eWidth, eHeight, i);
    
    m_fEntitiesHeight = fmaxf((i * eHeight), height);
}

int LevelEditorEntitiesPanel::handleTouch(TouchEvent& te, Vector2D& touchPoint, Game& game, Vector2D& camPos, PhysicalEntity** lastAddedEntity)
{
    if (m_isOpen && touchPoint.getX() > (CAM_WIDTH - getWidth()))
    {
        switch (te.getTouchType())
        {
            case DOWN:
            {
                if(touchPoint.getX() > m_closeButton->getLowerLeft().getX() + m_closeButton->getWidth())
                {
                    m_touchPointDown->set(touchPoint.getX(), touchPoint.getY());
                    m_touchPointDown2->set(touchPoint.getX(), touchPoint.getY());
                    
                    return LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED;
                }
            }
                break;
            case DRAGGED:
                m_fEntitiesCameraPos += (m_touchPointDown->getY() - touchPoint.getY()) * 3;
                if (m_fEntitiesCameraPos < 0)
                {
                    m_fEntitiesCameraPos = 0;
                }
                else if (m_fEntitiesCameraPos > (m_fEntitiesHeight - getHeight()))
                {
                    m_fEntitiesCameraPos = m_fEntitiesHeight - getHeight();
                }
                
                m_touchPointDown->set(touchPoint.getX(), touchPoint.getY());
                
                return LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED;
            case UP:
            {
                if (OverlapTester::isPointInRectangle(touchPoint, *m_closeButton))
                {
                    m_position->setX(CAM_WIDTH + (3.76608187134503f - 1.894736842105263f) / 2);
                    
                    m_isOpen = false;
                    
                    return LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED;
                }
                else if (touchPoint.getY() < m_touchPointDown2->getY() + 0.5f && touchPoint.getY() > m_touchPointDown2->getY() - 0.5f)
                {
                    touchPoint.add(0, m_fEntitiesCameraPos);
                    
                    float x = camPos.getX() + ZOOMED_OUT_CAM_WIDTH / 2 / GRID_CELL_SIZE;
                    float y = GAME_HEIGHT / 2 / GRID_CELL_SIZE;
                    
                    if (isTouchingEntityForPlacement(m_jons, game.getJons(), x, y, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_grounds, game.getGrounds(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                }
            }
        }
    }
    else
    {
        switch (te.getTouchType())
        {
            case DOWN:
            case DRAGGED:
                return LEVEL_EDITOR_ENTITIES_PANEL_RC_UNHANDLED;
            case UP:
                if (OverlapTester::isPointInRectangle(touchPoint, *m_openButton))
                {
                    m_position->setX(CAM_WIDTH - getWidth() / 2);
                    
                    m_isOpen = true;
                    
                    return LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED;
                }
        }
    }
    
    return LEVEL_EDITOR_ENTITIES_PANEL_RC_UNHANDLED;
}

std::vector<Jon *>& LevelEditorEntitiesPanel::getJons()
{
    return m_jons;
}

std::vector<Ground *>& LevelEditorEntitiesPanel::getGrounds()
{
    return m_grounds;
}

float LevelEditorEntitiesPanel::getEntitiesCameraPos()
{
    return m_fEntitiesCameraPos;
}

bool LevelEditorEntitiesPanel::isOpen()
{
    return m_isOpen;
}