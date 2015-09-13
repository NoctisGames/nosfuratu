//
//  Game.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Game.h"
#include "GameConstants.h"

Game::Game() : m_resetGame(false)
{
    m_jon = std::unique_ptr<Jon>(new Jon(CAM_WIDTH / 5, 10.06776145203123f, 2.4f, 2.4f));
    
    m_trees = std::unique_ptr<std::vector<Tree>>(new std::vector<Tree>);
    
    for (int i = 0; i < 18; i += 9)
    {
        m_trees->push_back(Tree(11.9416909620992f / 2 * (i + 3), 13.95505617977516f, 10.4489795918368f, 9.75453759723411f, 0));
        m_trees->push_back(Tree(11.9416909620992f / 2 * (i + 6), 12.92826274848742f, 5.85422740524784f, 7.700950734658649f, 1));
        m_trees->push_back(Tree(11.9416909620992f / 2 * (i + 9), 13.61668107173715f, 7.39358600583088f, 9.0777873811581f, 2));
    }
    
    m_platforms = std::unique_ptr<std::vector<GamePlatform>>(new std::vector<GamePlatform>);
    
    for (int i = 0; i < 20; i += 5)
    {
        m_platforms->push_back(GamePlatform(CAM_WIDTH * (i + 1), 8.86776145203123f + 4, 3.70612244897958f, 1.6f));
        m_platforms->push_back(GamePlatform(CAM_WIDTH * (i + 2), 8.86776145203123f + 4, 3.70612244897958f, 1.6f));
        m_platforms->push_back(GamePlatform(CAM_WIDTH * (i + 3), 8.86776145203123f + 4, 3.70612244897958f, 1.6f));
        m_platforms->push_back(GamePlatform(CAM_WIDTH * (i + 4), 8.86776145203123f + 4, 3.70612244897958f, 1.6f));
        m_platforms->push_back(GamePlatform(CAM_WIDTH * (i + 5), 8.86776145203123f + 8, 3.70612244897958f, 1.6f));
    }
    
    for (int i = 6; i < 20; i += 5)
    {
        m_platforms->push_back(GamePlatform(12 * (i + 1), 2, 3.70612244897958f, 1.6f));
        m_platforms->push_back(GamePlatform(12 * (i + 2), 2, 3.70612244897958f, 1.6f));
        m_platforms->push_back(GamePlatform(12 * (i + 3), 2, 3.70612244897958f, 1.6f));
        m_platforms->push_back(GamePlatform(12 * (i + 4), 2, 3.70612244897958f, 1.6f));
        m_platforms->push_back(GamePlatform(12 * (i + 5), 6, 3.70612244897958f, 1.6f));
    }
    
    m_carrots = std::unique_ptr<std::vector<Carrot>>(new std::vector<Carrot>);
    
    for (int i = 0; i < 20; i += 10)
    {
        m_carrots->push_back(Carrot(CAM_WIDTH * (i + 1), 8.86776145203123f + 7, 1.25925925925926, 1));
        m_carrots->push_back(Carrot(CAM_WIDTH * (i + 2), 8.86776145203123f + 2, 1.25925925925926, 1));
    }
    
    m_grounds = std::unique_ptr<std::vector<Ground>>(new std::vector<Ground>);
    
    for (int i = 0; i < 12; i += 4)
    {
        m_grounds->push_back(Ground(11.9416909620992f / 2 * (i + 1), 10.19792566983582f / 2, 11.9416909620992f, 10.19792566983582f, 0));
        m_grounds->push_back(Ground(11.9416909620992f / 2 * (i + 2), 10.19792566983582f / 2, 11.9416909620992f, 10.19792566983582f, 1));
        m_grounds->push_back(Ground(11.9416909620992f / 2 * (i + 3), 10.19792566983582f / 2, 11.9416909620992f, 10.19792566983582f, 2));
        m_grounds->push_back(Ground(11.9416909620992f / 2 * (i + 4), 10.19792566983582f / 2, 11.9416909620992f, 10.19792566983582f, 3));
    }
    
    for (int i = 12; i < 20; i += 4)
    {
        m_grounds->push_back(Ground(11.9416909620992f / 2 * (i + 1), 10.19792566983582f / 2, 11.9416909620992f, 10.19792566983582f, 0));
        m_grounds->push_back(Ground(11.9416909620992f / 2 * (i + 2), 10.19792566983582f / 2, 11.9416909620992f, 10.19792566983582f, 1));
        m_grounds->push_back(Ground(11.9416909620992f / 2 * (i + 3), 10.19792566983582f / 2, 11.9416909620992f, 10.19792566983582f, 2));
        m_grounds->push_back(Ground(11.9416909620992f / 2 * (i + 4), 10.19792566983582f / 2, 11.9416909620992f, 10.19792566983582f, 3));
    }
}

void Game::update(float deltaTime)
{
    m_jon->update(deltaTime, *this);
    
    if (m_jon->isDead())
    {
        m_resetGame = true;
    }
}

Jon& Game::getJon()
{
    return *m_jon;
}

std::vector<Tree>& Game::getTrees()
{
    return *m_trees;
}

std::vector<GamePlatform>& Game::getPlatforms()
{
    return *m_platforms;
}

std::vector<Carrot>& Game::getCarrots()
{
    return *m_carrots;
}

std::vector<Ground>& Game::getGrounds()
{
    return *m_grounds;
}

bool Game::resetGame()
{
    return m_resetGame;
}