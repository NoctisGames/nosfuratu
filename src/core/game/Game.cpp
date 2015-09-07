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
    m_jon = std::unique_ptr<Jon>(new Jon(CAM_WIDTH / 5, 8.86776145203123f + 1.2f, 2.4f, 2.4f));
    
    m_platforms = std::unique_ptr<std::vector<Platform>>(new std::vector<Platform>);
    
    for(int i = 0; i < 200; i += 5)
    {
        m_platforms->push_back(Platform(CAM_WIDTH * (i + 1), 8.86776145203123f + 4, 3.70612244897958f, 1.6f));
        m_platforms->push_back(Platform(CAM_WIDTH * (i + 2), 8.86776145203123f + 4, 3.70612244897958f, 1.6f));
        m_platforms->push_back(Platform(CAM_WIDTH * (i + 3), 8.86776145203123f + 4, 3.70612244897958f, 1.6f));
        m_platforms->push_back(Platform(CAM_WIDTH * (i + 4), 8.86776145203123f + 4, 3.70612244897958f, 1.6f));
        m_platforms->push_back(Platform(CAM_WIDTH * (i + 5), 8.86776145203123f + 8, 3.70612244897958f, 1.6f));
    }
    
    m_carrots = std::unique_ptr<std::vector<Carrot>>(new std::vector<Carrot>);
    
    for(int i = 0; i < 100; i += 10)
    {
        m_carrots->push_back(Carrot(CAM_WIDTH * (i + 1), 8.86776145203123f + 7, 1.25925925925926, 1));
        m_carrots->push_back(Carrot(CAM_WIDTH * (i + 2), 8.86776145203123f + 2, 1.25925925925926, 1));
    }
    
    m_grounds = std::unique_ptr<std::vector<Ground>>(new std::vector<Ground>);
    
    for(int i = 0; i < 12; i += 4)
    {
        m_grounds->push_back(Ground(11.9416909620992f / 2 * (i + 1), 10.19792566983582f / 2, 11.9416909620992f, 10.19792566983582f, 0));
        m_grounds->push_back(Ground(11.9416909620992f / 2 * (i + 2), 10.19792566983582f / 2, 11.9416909620992f, 10.19792566983582f, 1));
        m_grounds->push_back(Ground(11.9416909620992f / 2 * (i + 3), 10.19792566983582f / 2, 11.9416909620992f, 10.19792566983582f, 2));
        m_grounds->push_back(Ground(11.9416909620992f / 2 * (i + 4), 10.19792566983582f / 2, 11.9416909620992f, 10.19792566983582f, 3));
    }
    
    for(int i = 12; i < 20; i += 4)
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

std::vector<Platform>& Game::getPlatforms()
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