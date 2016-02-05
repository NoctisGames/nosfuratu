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

#include <math.h>

LevelEditorEntitiesPanel::LevelEditorEntitiesPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_isOpen(false), m_fEntitiesCameraPos(0), m_fEntitiesHeight(0)
{
    m_openButton = std::unique_ptr<Rectangle>(new Rectangle(CAM_WIDTH - 1, height * 0.49081920903955f, 1, 1));
    m_closeButton = std::unique_ptr<Rectangle>(new Rectangle(CAM_WIDTH - width, height * 0.49081920903955f, 1, 1));
    m_touchPointDown = std::unique_ptr<Vector2D>(new Vector2D());
    m_touchPointDown2 = std::unique_ptr<Vector2D>(new Vector2D());
    
    float eWidth = width * 0.6f;
    float eHeight = height / 6;
    float size = fminf(eWidth, eHeight);
    float eX = CAM_WIDTH - getWidth() / 2 + 0.4f;
    float eY = eHeight / 2;
    int i = 0;
    
    m_jons.push_back(std::unique_ptr<Jon>(new Jon(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundGrassWithCaveLarge(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundGrassWithCaveMedium(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundGrassWithCaveSmall(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundGrassWithCaveEndLeft(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundGrassWithCaveEndRight(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundGrassWithoutCaveLarge(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundGrassWithoutCaveMedium(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundGrassWithoutCaveSmall(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundGrassWithoutCaveEndLeft(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundGrassWithoutCaveEndRight(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundCaveLarge(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundCaveMedium(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundCaveSmall(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundCaveEndLeft(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundCaveEndRight(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundCaveRaisedLarge(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundCaveRaisedMedium(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundCaveRaisedSmall(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundCaveRaisedEndLeft(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_grounds.push_back(std::unique_ptr<Ground>(new GroundCaveRaisedEndRight(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    
    boxInAll(m_grounds, size);
    
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
                    
                    float x = camPos.getX() + ZOOMED_OUT_CAM_WIDTH / 2;
                    float y = GAME_HEIGHT / 2;
                    
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

std::vector<std::unique_ptr<Jon>>& LevelEditorEntitiesPanel::getJons()
{
    return m_jons;
}

std::vector<std::unique_ptr<Ground>>& LevelEditorEntitiesPanel::getGrounds()
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