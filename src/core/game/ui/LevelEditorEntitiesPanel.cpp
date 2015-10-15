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
    
    m_trees.push_back(std::unique_ptr<Tree>(new TreeOne(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_trees.push_back(std::unique_ptr<Tree>(new TreeTwo(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_trees.push_back(std::unique_ptr<Tree>(new TreeThree(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    
    m_caveSkeletons.push_back(std::unique_ptr<CaveSkeleton>(new CaveSkeletonOne(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_caveSkeletons.push_back(std::unique_ptr<CaveSkeleton>(new CaveSkeletonTwo(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_caveSkeletons.push_back(std::unique_ptr<CaveSkeleton>(new CaveSkeletonThree(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    
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
    
    m_logVerticalTalls.push_back(std::unique_ptr<LogVerticalTall>(new LogVerticalTall(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_logVerticalShorts.push_back(std::unique_ptr<LogVerticalShort>(new LogVerticalShort(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_thorns.push_back(std::unique_ptr<Thorns>(new Thorns(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_stumps.push_back(std::unique_ptr<Stump>(new Stump(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_sideSpikes.push_back(std::unique_ptr<SideSpike>(new SideSpike(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    
    m_upwardSpikes.push_back(std::unique_ptr<UpwardSpike>(new UpwardSpikeMetalGrass(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_upwardSpikes.push_back(std::unique_ptr<UpwardSpike>(new UpwardSpikeWoodGrass(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_upwardSpikes.push_back(std::unique_ptr<UpwardSpike>(new UpwardSpikeMetalCave(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    
    m_jumpSprings.push_back(std::unique_ptr<JumpSpring>(new JumpSpringGrass(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_jumpSprings.push_back(std::unique_ptr<JumpSpring>(new JumpSpringCave(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    
    m_rocks.push_back(std::unique_ptr<Rock>(new Rock(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_rocks.push_back(std::unique_ptr<CrackedRock>(new CrackedRock(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    
    m_platforms.push_back(std::unique_ptr<GroundPlatform>(new GroundPlatformGrassDefault(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_platforms.push_back(std::unique_ptr<GroundPlatform>(new GroundPlatformGrassCenter(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_platforms.push_back(std::unique_ptr<GroundPlatform>(new GroundPlatformGrassEndLeft(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_platforms.push_back(std::unique_ptr<GroundPlatform>(new GroundPlatformGrassEndRight(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_platforms.push_back(std::unique_ptr<GroundPlatform>(new GroundPlatformCaveCenter(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_platforms.push_back(std::unique_ptr<GroundPlatform>(new GroundPlatformCaveEndLeft(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_platforms.push_back(std::unique_ptr<GroundPlatform>(new GroundPlatformCaveEndRight(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    
    m_carrots.push_back(std::unique_ptr<Carrot>(new Carrot(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_goldenCarrots.push_back(std::unique_ptr<GoldenCarrot>(new GoldenCarrot(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    m_endSigns.push_back(std::unique_ptr<EndSign>(new EndSign(eX, eY + (i++ * eHeight), eWidth, eHeight)));
    
    boxInAll(m_jons, size);
    boxInAll(m_trees, size);
    boxInAll(m_caveSkeletons, size);
    boxInAll(m_grounds, size);
    boxInAll(m_logVerticalTalls, size);
    boxInAll(m_logVerticalShorts, size);
    boxInAll(m_thorns, size);
    boxInAll(m_stumps, size);
    boxInAll(m_sideSpikes, size);
    boxInAll(m_upwardSpikes, size);
    boxInAll(m_jumpSprings, size);
    boxInAll(m_rocks, size);
    boxInAll(m_platforms, size);
    boxInAll(m_carrots, size);
    boxInAll(m_goldenCarrots, size);
    boxInAll(m_endSigns, size);
    
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
                m_fEntitiesCameraPos += (m_touchPointDown->getY() - touchPoint.getY());
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
                    
                    if (isTouchingEntityForPlacement(m_jons, game.getJons(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_trees, game.getTrees(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_caveSkeletons, game.getCaveSkeletons(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_grounds, game.getGrounds(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_logVerticalTalls, game.getLogVerticalTalls(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_logVerticalShorts, game.getLogVerticalShorts(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_thorns, game.getThorns(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_stumps, game.getStumps(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_sideSpikes, game.getSideSpikes(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_upwardSpikes, game.getUpwardSpikes(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_jumpSprings, game.getJumpSprings(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_rocks, game.getRocks(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_platforms, game.getPlatforms(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_carrots, game.getCarrots(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_goldenCarrots, game.getGoldenCarrots(), x, y, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                    else if (isTouchingEntityForPlacement(m_endSigns, game.getEndSigns(), x, y, lastAddedEntity, touchPoint))
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

std::vector<std::unique_ptr<Tree>>& LevelEditorEntitiesPanel::getTrees()
{
    return m_trees;
}

std::vector<std::unique_ptr<CaveSkeleton>>& LevelEditorEntitiesPanel::getCaveSkeletons()
{
    return m_caveSkeletons;
}

std::vector<std::unique_ptr<Ground>>& LevelEditorEntitiesPanel::getGrounds()
{
    return m_grounds;
}

std::vector<std::unique_ptr<LogVerticalTall>>& LevelEditorEntitiesPanel::getLogVerticalTalls()
{
    return m_logVerticalTalls;
}

std::vector<std::unique_ptr<LogVerticalShort>>& LevelEditorEntitiesPanel::getLogVerticalShorts()
{
    return m_logVerticalShorts;
}

std::vector<std::unique_ptr<Thorns>>& LevelEditorEntitiesPanel::getThorns()
{
    return m_thorns;
}

std::vector<std::unique_ptr<Stump>>& LevelEditorEntitiesPanel::getStumps()
{
    return m_stumps;
}

std::vector<std::unique_ptr<SideSpike>>& LevelEditorEntitiesPanel::getSideSpikes()
{
    return m_sideSpikes;
}

std::vector<std::unique_ptr<UpwardSpike>>& LevelEditorEntitiesPanel::getUpwardSpikes()
{
    return m_upwardSpikes;
}

std::vector<std::unique_ptr<JumpSpring>>& LevelEditorEntitiesPanel::getJumpSprings()
{
    return m_jumpSprings;
}

std::vector<std::unique_ptr<Rock>>& LevelEditorEntitiesPanel::getRocks()
{
    return m_rocks;
}

std::vector<std::unique_ptr<GroundPlatform>>& LevelEditorEntitiesPanel::getPlatforms()
{
    return m_platforms;
}

std::vector<std::unique_ptr<EndSign>>& LevelEditorEntitiesPanel::getEndSigns()
{
    return m_endSigns;
}

std::vector<std::unique_ptr<Carrot>>& LevelEditorEntitiesPanel::getCarrots()
{
    return m_carrots;
}

std::vector<std::unique_ptr<GoldenCarrot>>& LevelEditorEntitiesPanel::getGoldenCarrots()
{
    return m_goldenCarrots;
}

float LevelEditorEntitiesPanel::getEntitiesCameraPos()
{
    return m_fEntitiesCameraPos;
}

bool LevelEditorEntitiesPanel::isOpen()
{
    return m_isOpen;
}