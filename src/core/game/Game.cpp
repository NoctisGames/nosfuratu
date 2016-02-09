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

#define midgroundsKey "midgrounds"
#define groundsKey "grounds"
#define exitGroundsKey "exitGrounds"
#define jonsKey "jons"

Game::Game() : m_fStateTime(0.0f), m_iNumTotalCarrots(0), m_iNumTotalGoldenCarrots(0), m_isLoaded(false)
{
    for (int i = 0; i < 4; i++)
    {
        m_backgroundUppers.push_back(new Upper(i * CAM_WIDTH + CAM_WIDTH / 2));
        m_backgroundMids.push_back(new Mid(i * CAM_WIDTH + CAM_WIDTH / 2));
        m_backgroundLowers.push_back(new Lower(i * CAM_WIDTH + CAM_WIDTH / 2));
        m_backgroundLowers.push_back(new WaterBack(i * CAM_WIDTH + CAM_WIDTH / 2));
        m_backgroundMidgroundCovers.push_back(new WaterFront(i * CAM_WIDTH + CAM_WIDTH / 2));
    }
}

void Game::copy(Game* game)
{
    reset();
    
    copyPhysicalEntities(game->getMidgrounds(), m_midgrounds);
    copyPhysicalEntities(game->getGrounds(), m_grounds);
    copyPhysicalEntities(game->getExitGrounds(), m_exitGrounds);
    copyPhysicalEntities(game->getJons(), m_jons);
    
    setGameToEntities(m_jons, this);

    m_iNumTotalCarrots = 0;
    m_iNumTotalGoldenCarrots = 0;
//    m_iNumTotalCarrots = (int) m_carrots.size();
//    m_iNumTotalGoldenCarrots = (int) m_goldenCarrots.size();
    
    m_isLoaded = true;
}

void Game::load(const char* json)
{
    reset();
    
    rapidjson::Document d;
    d.Parse<0>(json);
    
    loadArray(m_midgrounds, d, midgroundsKey);
    loadArray(m_grounds, d, groundsKey);
    loadArray(m_exitGrounds, d, exitGroundsKey);
    loadArray(m_jons, d, jonsKey);
    
    setGameToEntities(m_jons, this);
    
    m_iNumTotalCarrots = 0;
    m_iNumTotalGoldenCarrots = 0;
//    m_iNumTotalCarrots = (int) m_carrots.size();
//    m_iNumTotalGoldenCarrots = (int) m_goldenCarrots.size();
    
    m_isLoaded = true;
}

const char* Game::save()
{
    using namespace rapidjson;
    using namespace std;
    
    static StringBuffer s;
    Writer<StringBuffer> w(s);

	s.Clear();
    
    w.StartObject();
    
    saveArray(m_midgrounds, w, midgroundsKey);
    saveArray(m_grounds, w, groundsKey);
    saveArray(m_exitGrounds, w, exitGroundsKey);
    saveArray(m_jons, w, jonsKey);
    
    w.EndObject();
    
    return s.GetString();
}

void Game::reset()
{
    m_midgrounds.clear();
    m_grounds.clear();
    m_exitGrounds.clear();
    m_jons.clear();
    
    m_fStateTime = 0;
    m_iNumTotalCarrots = 0;
    m_iNumTotalGoldenCarrots = 0;
    m_isLoaded = false;
}

void Game::update(float deltaTime)
{
    m_fStateTime += deltaTime;
}

void Game::updateAndClean(float deltaTime)
{
    EntityUtils::updateAndClean(getMidgrounds(), deltaTime);
    EntityUtils::updateAndClean(getGrounds(), deltaTime);
    EntityUtils::updateAndClean(getExitGrounds(), deltaTime);
    
    getJon().update(deltaTime);
}

int Game::calcSum()
{
    int sum = 0;
    
    sum += m_midgrounds.size();
    sum += m_grounds.size();
    sum += m_exitGrounds.size();
    sum += m_jons.size();
    
    return sum;
}

bool Game::isJonGrounded(float deltaTime)
{
    return EntityUtils::isLanding(getJon(), getGrounds(), deltaTime) || EntityUtils::isLanding(getJon(), getExitGrounds(), deltaTime);
}

bool Game::isJonBlockedHorizontally(float deltaTime)
{
    return EntityUtils::isBlockedOnRight(getJon(), getGrounds(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getExitGrounds(), deltaTime);
}

bool Game::isJonBlockedVertically(float deltaTime)
{
    return EntityUtils::isBlockedAbove(getJon(), getGrounds(), deltaTime) || EntityUtils::isBlockedAbove(getJon(), getExitGrounds(), deltaTime);
}

bool Game::isJonHit()
{
    return false;
}

bool Game::isJonLandingOnSpring(float deltaTime)
{
    return false;
}

bool Game::isJonLandingOnEnemy(float deltaTime)
{
    return false;
}

bool Game::isSpinningBackFistDelivered(float deltaTime)
{
    return false;
}

bool Game::isBurrowEffective()
{
    return false;
}

bool Game::isUpwardThrustEffectiveAgainstEnemy()
{
    return false;
}

std::vector<Background *>& Game::getBackgroundUppers()
{
    return m_backgroundUppers;
}

std::vector<Background *>& Game::getBackgroundMids()
{
    return m_backgroundMids;
}

std::vector<Background *>& Game::getBackgroundLowers()
{
    return m_backgroundLowers;
}

std::vector<Midground *>& Game::getMidgrounds()
{
    return m_midgrounds;
}

std::vector<Background *>& Game::getBackgroundMidgroundCovers()
{
    return m_backgroundMidgroundCovers;
}

std::vector<Ground *>& Game::getGrounds()
{
    return m_grounds;
}

std::vector<ExitGround *>& Game::getExitGrounds()
{
    return m_exitGrounds;
}

std::vector<Jon *>& Game::getJons()
{
    return m_jons;
}

Jon& Game::getJon()
{
    return *getJons().at(getJons().size() - 1);
}

float Game::getFarRight()
{
    return ZOOMED_OUT_CAM_WIDTH;
//    if (getEndSigns().size() == 0)
//    {
//        return ZOOMED_OUT_CAM_WIDTH;
//    }
//    
//    return getEndSigns().at(0)->getPosition().getX() + getEndSigns().at(0)->getWidth();
}

float Game::getFarRightBottom()
{
    return 0.0f;
//    if (getEndSigns().size() == 0)
//    {
//        return 8.750433275563259f;
//    }
//    
//    return getEndSigns().at(0)->getPosition().getY() - getJon().getHeight() / 2;
}

float Game::getStateTime()
{
    return m_fStateTime;
}

int Game::getNumTotalCarrots()
{
    return m_iNumTotalCarrots;
}

int Game::getNumTotalGoldenCarrots()
{
    return m_iNumTotalGoldenCarrots;
}

bool Game::isLoaded()
{
    return m_isLoaded;
}

//bool Game::isBurstingThroughCaveToSurface(PhysicalEntity& entity, std::vector<std::unique_ptr<CaveExit>>& items, float deltaTime)
//{
//    float entityVelocityY = entity.getVelocity().getY();
//    float entityLeft = entity.getBounds().getLowerLeft().getX();
//    float entityBottom = entity.getBounds().getLowerLeft().getY();
//    
//    if (entityVelocityY > 13.1f)
//    {
//        for (std::vector<std::unique_ptr<CaveExit>>::iterator i = items.begin(); i != items.end(); i++)
//        {
//            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i)->getHoleBounds()))
//            {
//                float itemLeft = (*i)->getHoleBounds().getLowerLeft().getX();
//                float itemTop = (*i)->getHoleBounds().getTop();
//                
//                if (itemLeft < entityLeft && entityBottom < itemTop)
//                {
//                    (*i)->triggerEruption();
//                    
//                    return true;
//                }
//            }
//        }
//    }
//    
//    return false;
//}
//
//bool Game::isFallingThroughCaveExit(PhysicalEntity& entity, std::vector<std::unique_ptr<CaveExit>>& items, float deltaTime)
//{
//    float entityVelocityY = entity.getVelocity().getY();
//    float entityLowerLeftX = entity.getBounds().getLowerLeft().getX();
//    float entityRight = entity.getBounds().getRight();
//    
//    if (entityVelocityY <= 0)
//    {
//        for (std::vector<std::unique_ptr<CaveExit>>::iterator i = items.begin(); i != items.end(); i++)
//        {
//            if (!(*i)->hasCover())
//            {
//                float itemLowerLeftX = (*i)->getHoleBounds().getLowerLeft().getX();
//                float itemRight = (*i)->getHoleBounds().getRight();
//                
//                if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i)->getHoleBounds()))
//                {
//                    if (entityLowerLeftX >= itemLowerLeftX && entityRight <= itemRight)
//                    {
//                        return true;
//                    }
//                }
//            }
//        }
//    }
//    
//    return false;
//}
