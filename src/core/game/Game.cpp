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
#include "ResourceConstants.h"
#include "GroundSize.h"

Game::Game() : m_fDeltaTime(0), m_resetGame(false)
{
    m_backgroundSkies = std::unique_ptr<std::vector<BackgroundSky>>(new std::vector<BackgroundSky>);
    m_backgroundTrees = std::unique_ptr<std::vector<BackgroundTrees>>(new std::vector<BackgroundTrees>);
    m_backgroundCaves = std::unique_ptr<std::vector<BackgroundCave>>(new std::vector<BackgroundCave>);
    
    m_trees = std::unique_ptr<std::vector<Tree>>(new std::vector<Tree>);
    m_grounds = std::unique_ptr<std::vector<Ground>>(new std::vector<Ground>);
    m_logVerticalTalls = std::unique_ptr<std::vector<LogVerticalTall>>(new std::vector<LogVerticalTall>);
    m_platforms = std::unique_ptr<std::vector<GroundPlatform>>(new std::vector<GroundPlatform>);
    m_endSigns = std::unique_ptr<std::vector<EndSign>>(new std::vector<EndSign>);
    m_carrots = std::unique_ptr<std::vector<Carrot>>(new std::vector<Carrot>);
    m_goldenCarrots = std::unique_ptr<std::vector<GoldenCarrot>>(new std::vector<GoldenCarrot>);
    
    m_jon = std::unique_ptr<Jon>(new Jon(JON_STARTING_X, 0, 2.2f, 2.2f, EntityAnchor::ANCHOR_GROUND));
    
    /// Create Level
    // TODO, create level by reading in an external level document
    
    m_backgroundSkies->push_back(BackgroundSky(CAM_WIDTH / 2));
    m_backgroundTrees->push_back(BackgroundTrees(CAM_WIDTH / 2));
    m_backgroundCaves->push_back(BackgroundCave(CAM_WIDTH / 2));
    
    for (int i = 0; i < 3; i += 3)
    {
        m_trees->push_back(Tree::createTree(i * CAM_WIDTH, TreeType::TREE_ONE));
        m_trees->push_back(Tree::createTree((i + 1) * CAM_WIDTH, TreeType::TREE_TWO));
        m_trees->push_back(Tree::createTree((i + 2) * CAM_WIDTH, TreeType::TREE_THREE));
    }
    
    Ground::createGrassWithoutCave(*m_grounds, 0, GroundSize::GROUND_SIZE_LARGE, 1);
    Ground::createGrassWithCave(*m_grounds, 47.90643274853801f + 47.90643274853801f / 2 - 6, GroundSize::GROUND_SIZE_LARGE, 1);
    Ground::createCave(*m_grounds, 47.90643274853801f + 47.90643274853801f / 2 - 6, GroundSize::GROUND_SIZE_LARGE, 2);
    Ground::createCaveRaised(*m_grounds, 47.90643274853801f * 2 + 47.90643274853801f / 2 - 6, GroundSize::GROUND_SIZE_MEDIUM, 1);
    Ground::createGrassWithoutCave(*m_grounds, 47.90643274853801f * 7 / 2, GroundSize::GROUND_SIZE_SMALL, 1);
    
    m_logVerticalTalls->push_back(LogVerticalTall(CAM_WIDTH));
    m_logVerticalTalls->push_back(LogVerticalTall(CAM_WIDTH * 2));
    m_logVerticalTalls->push_back(LogVerticalTall(CAM_WIDTH * 3 - 4));
    
    GroundPlatform::createGrass(*m_platforms, CAM_WIDTH * 3 / 2, ANCHOR_GROUND_Y + 2.8f, 1);
    GroundPlatform::createGrass(*m_platforms, CAM_WIDTH * 2, ANCHOR_GROUND_Y + 5.6f, 2);
    GroundPlatform::createGrass(*m_platforms, CAM_WIDTH * 3, ANCHOR_GROUND_Y + 8.4f, 2);
    GroundPlatform::createCave(*m_platforms, 47.90643274853801f * 2, 4, 1);
    GroundPlatform::createCave(*m_platforms, 47.90643274853801f * 4 - 36, 4.4f, 1);
    
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
    m_fDeltaTime = deltaTime;
    
    m_jon->update(deltaTime, *this);
    
    if (EntityUtils::isCollected(getJon(), getCarrots(), m_fDeltaTime))
    {
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_CARROT);
    }
    
    if (EntityUtils::isCollected(getJon(), getGoldenCarrots(), m_fDeltaTime))
    {
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_GOLDEN_CARROT);
    }
    
    if (m_jon->isDead())
    {
        m_resetGame = true;
    }
    
    if (m_jon->getPosition().getX() - m_jon->getWidth() > getFarRight())
    {
        m_resetGame = true;
    }
}

bool Game::isJonGrounded()
{
    return EntityUtils::isLanding(getJon(), getGrounds(), m_fDeltaTime) || EntityUtils::isLanding(getJon(), getPlatforms(), m_fDeltaTime) || EntityUtils::isLanding(getJon(), getLogVerticalTalls(), m_fDeltaTime);
}

bool Game::isJonBlockedHorizontally()
{
    return EntityUtils::isBlockedOnRight(getJon(), getGrounds(), m_fDeltaTime);
}

bool Game::isJonBlockedVertically()
{
    return EntityUtils::isBlockedAbove(getJon(), getGrounds(), m_fDeltaTime);
}

bool Game::isSpinningBackFistDelivered()
{
    return EntityUtils::isHit(getJon(), getLogVerticalTalls());
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

std::vector<Ground>& Game::getGrounds()
{
    return *m_grounds;
}

std::vector<LogVerticalTall>& Game::getLogVerticalTalls()
{
    return *m_logVerticalTalls;
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

Jon& Game::getJon()
{
    return *m_jon;
}

float Game::getFarRight()
{
    return getEndSigns().at(0).getPosition().getX() + getEndSigns().at(0).getWidth();
}

bool Game::resetGame()
{
    return m_resetGame;
}