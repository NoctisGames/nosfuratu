//
//  Game.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Game.h"
#include "GameConstants.h"
#include "EntityAnchor.h"
#include "EntityUtils.h"
#include "OverlapTester.h"
#include "Assets.h"
#include "GroundSize.h"

Game::Game() : m_resetGame(false)
{
    m_backgroundSkies = std::unique_ptr<std::vector<BackgroundSky>>(new std::vector<BackgroundSky>);
    m_backgroundTrees = std::unique_ptr<std::vector<BackgroundTrees>>(new std::vector<BackgroundTrees>);
    m_backgroundCaves = std::unique_ptr<std::vector<BackgroundCave>>(new std::vector<BackgroundCave>);
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
    m_jons = std::unique_ptr<std::vector<Jon>>(new std::vector<Jon>);
}

void Game::load()
{
    reset();
    
    /// Create Level
    // TODO, create level by reading in an external level document
    
    m_jons->push_back(Jon(JON_STARTING_X, 0, 2.2f, 2.2f, EntityAnchor::ANCHOR_GROUND));
    
    m_backgroundSkies->push_back(BackgroundSky(CAM_WIDTH / 2));
    m_backgroundTrees->push_back(BackgroundTrees(CAM_WIDTH / 2));
    m_backgroundCaves->push_back(BackgroundCave(CAM_WIDTH / 2));
    
    for (int i = 0; i < 3; i += 3)
    {
        m_trees->push_back(Tree::createTree(i * CAM_WIDTH, TreeType::TREE_ONE));
        m_trees->push_back(Tree::createTree((i + 1) * CAM_WIDTH, TreeType::TREE_TWO));
        m_trees->push_back(Tree::createTree((i + 2) * CAM_WIDTH, TreeType::TREE_THREE));
    }
    
    for (int i = 5; i < 8; i += 3)
    {
        m_trees->push_back(Tree::createTree(i * CAM_WIDTH, TreeType::TREE_ONE));
        m_trees->push_back(Tree::createTree((i + 1) * CAM_WIDTH, TreeType::TREE_TWO));
        m_trees->push_back(Tree::createTree((i + 2) * CAM_WIDTH - 4, TreeType::TREE_THREE));
    }
    
    Ground::createGrassWithoutCave(*m_grounds, 0, GroundSize::GROUND_SIZE_LARGE, 1);
    Ground::createGrassWithCave(*m_grounds, 47.90643274853801f + 47.90643274853801f / 2 - 6, GroundSize::GROUND_SIZE_LARGE, 1);
    Ground::createCave(*m_grounds, 47.90643274853801f + 47.90643274853801f / 2 - 6, GroundSize::GROUND_SIZE_LARGE, 2);
    Ground::createCaveRaised(*m_grounds, 47.90643274853801f * 2 + 47.90643274853801f / 2 - 6, GroundSize::GROUND_SIZE_MEDIUM, 1);
    Ground::createGrassWithoutCave(*m_grounds, 47.90643274853801f * 7 / 2, GroundSize::GROUND_SIZE_SMALL, 1);
    
    m_logVerticalTalls->push_back(LogVerticalTall(CAM_WIDTH));
    m_logVerticalTalls->push_back(LogVerticalTall(CAM_WIDTH * 2));
    m_logVerticalTalls->push_back(LogVerticalTall(CAM_WIDTH * 3 - 4));
    
    m_logVerticalShorts->push_back(LogVerticalShort(CAM_WIDTH * 4 + 5));
    m_logVerticalShorts->push_back(LogVerticalShort(CAM_WIDTH * 3));
    
    m_thorns->push_back(Thorns(CAM_WIDTH * 5));
    
    m_stumps->push_back(Stump(47.90643274853801f * 2 + 13));
    
    m_sideSpikes->push_back(SideSpike::create(0, 3.6f));
    EntityUtils::attach(m_sideSpikes->at(0), m_grounds->at(10), true);
    
    m_upwardSpikes->push_back(UpwardSpike::create(CAM_WIDTH * 5 - 4, UpwardSpikeType::UPWARD_SPIKE_WOOD_GRASS));
    m_upwardSpikes->push_back(UpwardSpike::create(CAM_WIDTH * 5 - 2, UpwardSpikeType::UPWARD_SPIKE_METAL_GRASS));
    m_upwardSpikes->push_back(UpwardSpike::create(CAM_WIDTH * 6 + 8.2f, UpwardSpikeType::UPWARD_SPIKE_METAL_CAVE));
    
    m_jumpSprings->push_back(JumpSpring::create(CAM_WIDTH * 3 - 5.5f, JumpSpringType::JUMP_SPRING_IN_GRASS));
    m_jumpSprings->push_back(JumpSpring::create(CAM_WIDTH * 9 + 3, JumpSpringType::JUMP_SPRING_IN_CAVE));
    
    m_rocks->push_back(Rock(47.90643274853801f * 2 - 10));
    m_rocks->push_back(Rock(47.90643274853801f * 2 - 6, true));
    
    GroundPlatform::createGrass(*m_platforms, CAM_WIDTH * 3 / 2, ANCHOR_GROUND_Y + 2.8f, 1);
    GroundPlatform::createGrass(*m_platforms, CAM_WIDTH * 2, ANCHOR_GROUND_Y + 5.6f, 2);
    GroundPlatform::createGrass(*m_platforms, CAM_WIDTH * 3, ANCHOR_GROUND_Y + 8.4f, 2);
    GroundPlatform::createCave(*m_platforms, 47.90643274853801f * 2, 4, 1);
    GroundPlatform::createCave(*m_platforms, 47.90643274853801f * 4 - 36, 4.4f, 1);
    
    m_upwardSpikes->push_back(UpwardSpike::create(CAM_WIDTH * 3 / 2 + 2, UpwardSpikeType::UPWARD_SPIKE_WOOD_GRASS));
    EntityUtils::placeOn(m_upwardSpikes->at(3), m_platforms->at(1));
    
    m_jumpSprings->push_back(JumpSpring::create(0, JumpSpringType::JUMP_SPRING_IN_GRASS));
    EntityUtils::placeOn(m_jumpSprings->at(2), m_platforms->at(8));
    EntityUtils::attach(m_jumpSprings->at(2), m_platforms->at(8), false);
    
    m_endSigns->push_back(EndSign(0));
    
    EntityUtils::attach(m_endSigns.get()->at(0), m_grounds.get()->at(m_grounds->size() - 1), true);
    
    for (int i = 0; i < 3; i += 3)
    {
        m_carrots->push_back(Carrot(i * CAM_WIDTH + 14, ANCHOR_GROUND_Y + 1.2f));
        m_carrots->push_back(Carrot((i + 1) * CAM_WIDTH + 8, ANCHOR_GROUND_Y + 1.2f));
        m_carrots->push_back(Carrot((i + 2) * CAM_WIDTH + 8, ANCHOR_GROUND_Y + 1.2f));
    }
    
    m_carrots->push_back(Carrot(47.90643274853801f * 2, ANCHOR_GROUND_Y + 1.2f));
    m_carrots->push_back(Carrot(47.90643274853801f * 2 + 3, ANCHOR_GROUND_Y + 1.2f));
    m_carrots->push_back(Carrot(47.90643274853801f * 2 + 6, ANCHOR_GROUND_Y + 1.2f));
    
    m_carrots->push_back(Carrot(47.90643274853801f * 4 - 20, ANCHOR_GROUND_Y + 1.2f));
    m_carrots->push_back(Carrot(47.90643274853801f * 4 - 20 + 3, ANCHOR_GROUND_Y + 3.2f));
    m_carrots->push_back(Carrot(47.90643274853801f * 4 - 20 + 6, ANCHOR_GROUND_Y + 6.2f));
    
    m_goldenCarrots->push_back(GoldenCarrot(CAM_WIDTH * 3 + 7, ANCHOR_GROUND_Y + 14));
    m_goldenCarrots->push_back(GoldenCarrot(47.90643274853801f * 4 - 37, 2.6f));
}

void Game::update(float deltaTime)
{
    getJon().update(deltaTime, *this);
    
    EntityUtils::updateBackgrounds(getBackgroundSkies(), *this);
    EntityUtils::updateBackgrounds(getBackgroundTrees(), *this);
    EntityUtils::updateBackgrounds(getBackgroundCaves(), *this);
    
    if (EntityUtils::isCollected(getJon(), getCarrots(), deltaTime))
    {
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_CARROT);
    }
    
    if (EntityUtils::isCollected(getJon(), getGoldenCarrots(), deltaTime))
    {
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_GOLDEN_CARROT);
    }
    
    EntityUtils::update(getGoldenCarrots(), deltaTime);
    
    EntityUtils::update(getJumpSprings(), deltaTime);
    
    EntityUtils::updateAndClean(getRocks(), deltaTime);
    
    if (getJon().isDead() || getJon().getPosition().getX() - getJon().getWidth() > getFarRight())
    {
        m_resetGame = true;
    }
}

bool Game::isJonGrounded(float deltaTime)
{
    return EntityUtils::isLanding(getJon(), getGrounds(), deltaTime) || EntityUtils::isLanding(getJon(), getPlatforms(), deltaTime) || EntityUtils::isLanding(getJon(), getLogVerticalTalls(), deltaTime) || EntityUtils::isLanding(getJon(), getLogVerticalShorts(), deltaTime) || EntityUtils::isLanding(getJon(), getStumps(), deltaTime) || EntityUtils::isLanding(getJon(), getRocks(), deltaTime);
}

bool Game::isJonBlockedHorizontally(float deltaTime)
{
    return EntityUtils::isBlockedOnRight(getJon(), getGrounds(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getLogVerticalTalls(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getLogVerticalShorts(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getStumps(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getRocks(), deltaTime);
}

bool Game::isJonBlockedVertically(float deltaTime)
{
    return EntityUtils::isBlockedAbove(getJon(), getGrounds(), deltaTime);
}

bool Game::isJonHit(float deltaTime)
{
    return EntityUtils::isHit(getJon(), getThorns()) || EntityUtils::isHit(getJon(), getSideSpikes()) || EntityUtils::isFallingIntoDeath(getJon(), getUpwardSpikes());
}

bool Game::isJonLandingOnSpring(float deltaTime)
{
    return EntityUtils::isLandingOnSpring(getJon(), getJumpSprings(), deltaTime);
}

bool Game::isSpinningBackFistDelivered(float deltaTime)
{
    return EntityUtils::isHitting(getJon(), getLogVerticalTalls()) || EntityUtils::isHitting(getJon(), getLogVerticalShorts()) || EntityUtils::isHitting(getJon(), getRocks());
}

std::vector<BackgroundSky>& Game::getBackgroundSkies()
{
    return *m_backgroundSkies;
}

std::vector<BackgroundTrees>& Game::getBackgroundTrees()
{
    return *m_backgroundTrees;
}

std::vector<BackgroundCave>& Game::getBackgroundCaves()
{
    return *m_backgroundCaves;
}

std::vector<Tree>& Game::getTrees()
{
    return *m_trees;
}

std::vector<CaveSkeleton>& Game::getCaveSkeletons()
{
    return *m_caveSkeletons;
}

std::vector<Ground>& Game::getGrounds()
{
    return *m_grounds;
}

std::vector<LogVerticalTall>& Game::getLogVerticalTalls()
{
    return *m_logVerticalTalls;
}

std::vector<LogVerticalShort>& Game::getLogVerticalShorts()
{
    return *m_logVerticalShorts;
}

std::vector<Thorns>& Game::getThorns()
{
    return *m_thorns;
}

std::vector<Stump>& Game::getStumps()
{
    return *m_stumps;
}

std::vector<SideSpike>& Game::getSideSpikes()
{
    return *m_sideSpikes;
}

std::vector<UpwardSpike>& Game::getUpwardSpikes()
{
    return *m_upwardSpikes;
}

std::vector<JumpSpring>& Game::getJumpSprings()
{
    return *m_jumpSprings;
}

std::vector<Rock>& Game::getRocks()
{
    return *m_rocks;
}

std::vector<GroundPlatform>& Game::getPlatforms()
{
    return *m_platforms;
}

std::vector<EndSign>& Game::getEndSigns()
{
    return *m_endSigns;
}

std::vector<Carrot>& Game::getCarrots()
{
    return *m_carrots;
}

std::vector<GoldenCarrot>& Game::getGoldenCarrots()
{
    return *m_goldenCarrots;
}

std::vector<Jon>& Game::getJons()
{
    return *m_jons;
}

Jon& Game::getJon()
{
    return getJons().at(0);
}

float Game::getFarRight()
{
    return getEndSigns().at(0).getPosition().getX() + getEndSigns().at(0).getWidth();
}

bool Game::resetGame()
{
    return m_resetGame;
}

#pragma mark private

void Game::reset()
{
    m_resetGame = false;
    
    m_backgroundSkies->clear();
    m_backgroundTrees->clear();
    m_backgroundCaves->clear();
    m_trees->clear();
    m_caveSkeletons->clear();
    m_grounds->clear();
    m_logVerticalTalls->clear();
    m_logVerticalShorts->clear();
    m_thorns->clear();
    m_stumps->clear();
    m_sideSpikes->clear();
    m_upwardSpikes->clear();
    m_jumpSprings->clear();
    m_rocks->clear();
    m_platforms->clear();
    m_endSigns->clear();
    m_carrots->clear();
    m_goldenCarrots->clear();
    m_jons->clear();
}