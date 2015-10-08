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
    m_trees = std::unique_ptr<std::vector<Tree>>(new std::vector<Tree>);
    m_caveSkeletons = std::unique_ptr<std::vector<CaveSkeleton>>(new std::vector<CaveSkeleton>);
    m_grounds = std::unique_ptr<std::vector<Ground>>(new std::vector<Ground>);
    m_logVerticalTalls = std::unique_ptr<std::vector<LogVerticalTall>>(new std::vector<LogVerticalTall>);
    m_logVerticalShorts = std::unique_ptr<std::vector<LogVerticalShort>>(new std::vector<LogVerticalShort>);
    m_thorns = std::unique_ptr<std::vector<Thorns>>(new std::vector<Thorns>);
    m_stumps = std::unique_ptr<std::vector<Stump>>(new std::vector<Stump>);
    m_sideSpikes = std::unique_ptr<std::vector<SideSpike>>(new std::vector<SideSpike>);
    m_upwardSpikes = std::unique_ptr<std::vector<UpwardSpike>>(new std::vector<UpwardSpike>);
    m_jumpSprings = std::unique_ptr<std::vector<JumpSpring>>(new std::vector<JumpSpring>);
    m_rocks = std::unique_ptr<std::vector<Rock>>(new std::vector<Rock>);
    m_platforms = std::unique_ptr<std::vector<GroundPlatform>>(new std::vector<GroundPlatform>);
    m_endSigns = std::unique_ptr<std::vector<EndSign>>(new std::vector<EndSign>);
    m_carrots = std::unique_ptr<std::vector<Carrot>>(new std::vector<Carrot>);
    m_goldenCarrots = std::unique_ptr<std::vector<GoldenCarrot>>(new std::vector<GoldenCarrot>);
    
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
    
    Tree::create(*m_trees, eX, eY + (i++ * eHeight), TreeType::TREE_ONE);
    Tree::create(*m_trees, eX, eY + (i++ * eHeight), TreeType::TREE_TWO);
    Tree::create(*m_trees, eX, eY + (i++ * eHeight), TreeType::TREE_THREE);    
    m_caveSkeletons->push_back(CaveSkeleton(eX, eY + (i++ * eHeight), eWidth, eHeight, CaveSkeletonType::CAVE_SKELETON_ONE));
    m_caveSkeletons->push_back(CaveSkeleton(eX, eY + (i++ * eHeight), eWidth, eHeight, CaveSkeletonType::CAVE_SKELETON_TWO));
    m_caveSkeletons->push_back(CaveSkeleton(eX, eY + (i++ * eHeight), eWidth, eHeight, CaveSkeletonType::CAVE_SKELETON_THREE));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_GRASS_WITH_CAVE_LARGE, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_CAVE_LARGE, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_GRASS_WITHOUT_CAVE_LARGE, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_GRASS_WITHOUT_CAVE_END_LEFT, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_GRASS_WITHOUT_CAVE_MEDIUM, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_GRASS_WITHOUT_CAVE_SMALL, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_GRASS_WITHOUT_CAVE_END_RIGHT, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_GRASS_WITH_CAVE_END_LEFT, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_GRASS_WITH_CAVE_MEDIUM, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_GRASS_WITH_CAVE_SMALL, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_GRASS_WITH_CAVE_END_RIGHT, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_CAVE_END_LEFT, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_CAVE_MEDIUM, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_CAVE_SMALL, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_CAVE_END_RIGHT, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_CAVE_RAISED_END_LEFT, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_CAVE_RAISED_MEDIUM, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_CAVE_RAISED_SMALL, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_CAVE_RAISED_END_RIGHT, 1));
    m_grounds->push_back(Ground(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundType::GROUND_CAVE_RAISED_LARGE, 1));
    m_logVerticalTalls->push_back(LogVerticalTall(eX, eY + (i++ * eHeight), eWidth, eHeight));
    m_logVerticalShorts->push_back(LogVerticalShort(eX, eY + (i++ * eHeight), eWidth, eHeight));
    m_thorns->push_back(Thorns(eX, eY + (i++ * eHeight), eWidth, eHeight));
    m_stumps->push_back(Stump(eX, eY + (i++ * eHeight), eWidth, eHeight));
    m_sideSpikes->push_back(SideSpike(eX, eY + (i++ * eHeight), eWidth, eHeight));
    m_upwardSpikes->push_back(UpwardSpike(eX, eY + (i++ * eHeight), eWidth, eHeight, UpwardSpikeType::UPWARD_SPIKE_METAL_CAVE));
    m_upwardSpikes->push_back(UpwardSpike(eX, eY + (i++ * eHeight), eWidth, eHeight, UpwardSpikeType::UPWARD_SPIKE_METAL_GRASS));
    m_upwardSpikes->push_back(UpwardSpike(eX, eY + (i++ * eHeight), eWidth, eHeight, UpwardSpikeType::UPWARD_SPIKE_WOOD_GRASS));
    m_jumpSprings->push_back(JumpSpring(eX, eY + (i++ * eHeight), eWidth, eHeight, JumpSpringType::JUMP_SPRING_IN_GRASS));
    m_jumpSprings->push_back(JumpSpring(eX, eY + (i++ * eHeight), eWidth, eHeight, JumpSpringType::JUMP_SPRING_IN_CAVE));
    m_rocks->push_back(Rock(eX, eY + (i++ * eHeight), eWidth, eHeight, false));
    m_rocks->push_back(Rock(eX, eY + (i++ * eHeight), eWidth, eHeight, true));
    m_platforms->push_back(GroundPlatform(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundPlatformType::GROUND_PLATFORM_GRASS_DEFAULT));
    m_platforms->push_back(GroundPlatform(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundPlatformType::GROUND_PLATFORM_GRASS_END_LEFT));
    m_platforms->push_back(GroundPlatform(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundPlatformType::GROUND_PLATFORM_GRASS_CENTER));
    m_platforms->push_back(GroundPlatform(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundPlatformType::GROUND_PLATFORM_GRASS_END_RIGHT));
    m_platforms->push_back(GroundPlatform(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundPlatformType::GROUND_PLATFORM_CAVE_END_LEFT));
    m_platforms->push_back(GroundPlatform(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundPlatformType::GROUND_PLATFORM_CAVE_CENTER));
    m_platforms->push_back(GroundPlatform(eX, eY + (i++ * eHeight), eWidth, eHeight, GroundPlatformType::GROUND_PLATFORM_CAVE_END_RIGHT));
    m_carrots->push_back(Carrot(eX, eY + (i++ * eHeight), eWidth, eHeight));
    m_goldenCarrots->push_back(GoldenCarrot(eX, eY + (i++ * eHeight), eWidth, eHeight));
    m_endSigns->push_back(EndSign(eX, eY + (i++ * eHeight), eWidth, eHeight));
    
    EntityUtils::boxInAll(*m_trees, size);
    EntityUtils::boxInAll(*m_caveSkeletons, size);
    EntityUtils::boxInAll(*m_grounds, size);
    EntityUtils::boxInAll(*m_logVerticalTalls, size);
    EntityUtils::boxInAll(*m_logVerticalShorts, size);
    EntityUtils::boxInAll(*m_thorns, size);
    EntityUtils::boxInAll(*m_stumps, size);
    EntityUtils::boxInAll(*m_sideSpikes, size);
    EntityUtils::boxInAll(*m_upwardSpikes, size);
    EntityUtils::boxInAll(*m_jumpSprings, size);
    EntityUtils::boxInAll(*m_rocks, size);
    EntityUtils::boxInAll(*m_platforms, size);
    EntityUtils::boxInAll(*m_carrots, size);
    EntityUtils::boxInAll(*m_goldenCarrots, size);
    EntityUtils::boxInAll(*m_endSigns, size);
    
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
                    
                    int index;
                    if ((index = EntityUtils::isTouchingEntityForPlacement(*m_trees, touchPoint)) != -1)
                    {
                        Tree::create(game.getTrees(), x, y, m_trees->at(index).getTreeType());
                        *lastAddedEntity = &game.getTrees().at(game.getTrees().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_caveSkeletons, touchPoint)) != -1)
                    {
                        CaveSkeleton::create(game.getCaveSkeletons(), x, y, m_caveSkeletons->at(index).getCaveSkeletonType());
                        *lastAddedEntity = &game.getCaveSkeletons().at(game.getCaveSkeletons().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_grounds, touchPoint)) != -1)
                    {
                        Ground::create(game.getGrounds(), x, m_grounds->at(index).getGroundType());
                        *lastAddedEntity = &game.getGrounds().at(game.getGrounds().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_logVerticalTalls, touchPoint)) != -1)
                    {
                        LogVerticalTall::create(game.getLogVerticalTalls(), x, y);
                        *lastAddedEntity = &game.getLogVerticalTalls().at(game.getLogVerticalTalls().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_logVerticalShorts, touchPoint)) != -1)
                    {
                        LogVerticalShort::create(game.getLogVerticalShorts(), x, y);
                        *lastAddedEntity = &game.getLogVerticalShorts().at(game.getLogVerticalShorts().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_thorns, touchPoint)) != -1)
                    {
                        Thorns::create(game.getThorns(), x, y);
                        *lastAddedEntity = &game.getThorns().at(game.getThorns().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_stumps, touchPoint)) != -1)
                    {
                        Stump::create(game.getStumps(), x, y);
                        *lastAddedEntity = &game.getStumps().at(game.getStumps().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_sideSpikes, touchPoint)) != -1)
                    {
                        SideSpike::create(game.getSideSpikes(), x, y);
                        *lastAddedEntity = &game.getSideSpikes().at(game.getSideSpikes().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_upwardSpikes, touchPoint)) != -1)
                    {
                        UpwardSpike::create(game.getUpwardSpikes(), x, y, m_upwardSpikes->at(index).getUpwardSpikeType());
                        *lastAddedEntity = &game.getUpwardSpikes().at(game.getUpwardSpikes().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_jumpSprings, touchPoint)) != -1)
                    {
                        JumpSpring::create(game.getJumpSprings(), x, y, m_jumpSprings->at(index).getJumpSpringType());
                        *lastAddedEntity = &game.getJumpSprings().at(game.getJumpSprings().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_rocks, touchPoint)) != -1)
                    {
                        Rock::create(game.getRocks(), x, y, m_rocks->at(index).isCracked());
                        *lastAddedEntity = &game.getRocks().at(game.getRocks().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_platforms, touchPoint)) != -1)
                    {
                        GroundPlatform::create(game.getPlatforms(), x, y, m_platforms->at(index).getGroundPlatformType());
                        *lastAddedEntity = &game.getPlatforms().at(game.getPlatforms().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_carrots, touchPoint)) != -1)
                    {
                        Carrot::create(game.getCarrots(), x, y);
                        *lastAddedEntity = &game.getCarrots().at(game.getCarrots().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_goldenCarrots, touchPoint)) != -1)
                    {
                        GoldenCarrot::create(game.getGoldenCarrots(), x, y);
                        *lastAddedEntity = &game.getGoldenCarrots().at(game.getGoldenCarrots().size() - 1);
                    }
                    else if ((index = EntityUtils::isTouchingEntityForPlacement(*m_endSigns, touchPoint)) != -1)
                    {
                        EndSign::create(game.getEndSigns(), x, y);
                        *lastAddedEntity = &game.getEndSigns().at(game.getEndSigns().size() - 1);
                    }
                    else
                    {
                        *lastAddedEntity = nullptr;
                    }
                    
                    return LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED;
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

std::vector<Tree>& LevelEditorEntitiesPanel::getTrees()
{
    return *m_trees;
}

std::vector<CaveSkeleton>& LevelEditorEntitiesPanel::getCaveSkeletons()
{
    return *m_caveSkeletons;
}

std::vector<Ground>& LevelEditorEntitiesPanel::getGrounds()
{
    return *m_grounds;
}

std::vector<LogVerticalTall>& LevelEditorEntitiesPanel::getLogVerticalTalls()
{
    return *m_logVerticalTalls;
}

std::vector<LogVerticalShort>& LevelEditorEntitiesPanel::getLogVerticalShorts()
{
    return *m_logVerticalShorts;
}

std::vector<Thorns>& LevelEditorEntitiesPanel::getThorns()
{
    return *m_thorns;
}

std::vector<Stump>& LevelEditorEntitiesPanel::getStumps()
{
    return *m_stumps;
}

std::vector<SideSpike>& LevelEditorEntitiesPanel::getSideSpikes()
{
    return *m_sideSpikes;
}

std::vector<UpwardSpike>& LevelEditorEntitiesPanel::getUpwardSpikes()
{
    return *m_upwardSpikes;
}

std::vector<JumpSpring>& LevelEditorEntitiesPanel::getJumpSprings()
{
    return *m_jumpSprings;
}

std::vector<Rock>& LevelEditorEntitiesPanel::getRocks()
{
    return *m_rocks;
}

std::vector<GroundPlatform>& LevelEditorEntitiesPanel::getPlatforms()
{
    return *m_platforms;
}

std::vector<EndSign>& LevelEditorEntitiesPanel::getEndSigns()
{
    return *m_endSigns;
}

std::vector<Carrot>& LevelEditorEntitiesPanel::getCarrots()
{
    return *m_carrots;
}

std::vector<GoldenCarrot>& LevelEditorEntitiesPanel::getGoldenCarrots()
{
    return *m_goldenCarrots;
}

float LevelEditorEntitiesPanel::getEntitiesCameraPos()
{
    return m_fEntitiesCameraPos;
}

bool LevelEditorEntitiesPanel::isOpen()
{
    return m_isOpen;
}