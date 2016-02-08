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

#define groundsKey "grounds"
#define jonsKey "jons"

Game::Game() : m_fStateTime(0.0f), m_iNumTotalCarrots(0), m_iNumTotalGoldenCarrots(0), m_isLoaded(false)
{
    for (int i = 0; i < 4; i++)
    {
        m_backgroundUppers.push_back(new BackgroundUpper(i * CAM_WIDTH + CAM_WIDTH / 2));
        m_backgroundMids.push_back(new BackgroundMid(i * CAM_WIDTH + CAM_WIDTH / 2));
        m_backgroundLowers.push_back(new BackgroundLower(i * CAM_WIDTH + CAM_WIDTH / 2));
    }
}

void Game::copy(Game* game)
{
    reset();
    
    copyPhysicalEntities(game->getGrounds(), m_grounds);
    copyPhysicalEntities(game->getJons(), m_jons);
    
    setGameToEntities(m_jons, this);
    
    m_iNumTotalCarrots = (int) m_carrots.size();
    m_iNumTotalGoldenCarrots = (int) m_goldenCarrots.size();
    
    m_isLoaded = true;
}

void Game::load(const char* json)
{
    reset();
    
    rapidjson::Document d;
    d.Parse<0>(json);
    
    loadArray(m_grounds, d, groundsKey);
    loadArray(m_jons, d, jonsKey);
    
    setGameToEntities(m_jons, this);
    
    m_iNumTotalCarrots = (int) m_carrots.size();
    m_iNumTotalGoldenCarrots = (int) m_goldenCarrots.size();
    
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
    
    saveArray(m_grounds, w, groundsKey);
    saveArray(m_jons, w, jonsKey);
    
    w.EndObject();
    
    return s.GetString();
}

void Game::reset()
{
    m_grounds.clear();
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
    EntityUtils::updateAndClean(getGrounds(), deltaTime);
    
    getJon().update(deltaTime);
}

int Game::calcSum()
{
    int sum = 0;
    
    sum += m_grounds.size();
    sum += m_jons.size();
    
    return sum;
}

bool Game::isJonGrounded(float deltaTime)
{
    return !EntityUtils::isFallingThroughHole(getJon(), getHoles(), deltaTime) && !isFallingThroughCaveExit(getJon(), getCaveExits(), deltaTime) && (EntityUtils::isLanding(getJon(), getGrounds(), deltaTime) || EntityUtils::isLanding(getJon(), getPlatforms(), deltaTime) || EntityUtils::isLanding(getJon(), getLogVerticalTalls(), deltaTime) || EntityUtils::isLanding(getJon(), getLogVerticalShorts(), deltaTime) || EntityUtils::isLanding(getJon(), getStumps(), deltaTime) || EntityUtils::isLanding(getJon(), getRocks(), deltaTime) || EntityUtils::isLanding(getJon(), getCaveExits(), deltaTime) || EntityUtils::isLanding(getJon(), getJumpSprings(), deltaTime));
}

bool Game::isJonBlockedHorizontally(float deltaTime)
{
    return EntityUtils::isBlockedOnRight(getJon(), getGrounds(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getLogVerticalTalls(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getLogVerticalShorts(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getStumps(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getRocks(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getUpwardSpikes(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getJumpSprings(), deltaTime);
}

bool Game::isJonBlockedVertically(float deltaTime)
{
    return (!isBurstingThroughCaveToSurface(getJon(), getCaveExits(), deltaTime) && EntityUtils::isBlockedAbove(getJon(), getCaveExits(), deltaTime)) || EntityUtils::isBlockedAbove(getJon(), getGrounds(), deltaTime);
}

bool Game::isJonHit()
{
    return EntityUtils::isHit(getJon(), getThorns()) || EntityUtils::isHit(getJon(), getSideSpikes()) || EntityUtils::isFallingIntoDeath(getJon(), getUpwardSpikes()) || EntityUtils::isKilledByEnemy(getJon(), getSnakeGruntEnemies()) || EntityUtils::isKilledByEnemy(getJon(), getSnakeHornedEnemies());
}

bool Game::isJonLandingOnSpring(float deltaTime)
{
    return EntityUtils::isLandingOnSpring(getJon(), getJumpSprings(), deltaTime);
}

bool Game::isJonLandingOnEnemy(float deltaTime)
{
    return EntityUtils::isLandingOnEnemy(getJon(), getSnakeGruntEnemies(), deltaTime);
}

bool Game::isSpinningBackFistDelivered(float deltaTime)
{
    return EntityUtils::isHitting(getJon(), getLogVerticalTalls()) || EntityUtils::isHitting(getJon(), getLogVerticalShorts()) || EntityUtils::isHitting(getJon(), getRocks()) || EntityUtils::isHorizontallyHittingAnEnemy(getJon(), getSnakeGruntEnemies()) || EntityUtils::isHorizontallyHittingAnEnemy(getJon(), getSnakeHornedEnemies());
}

bool Game::isBurrowEffective()
{
    return EntityUtils::isBurrowingThroughHole(getJon(), getHoles());
}

bool Game::isUpwardThrustEffectiveAgainstEnemy()
{
    return EntityUtils::isHittingEnemyFromBelow(getJon(), getSnakeGruntEnemies()) || EntityUtils::isHittingEnemyFromBelow(getJon(), getSnakeHornedEnemies());
}

std::vector<Background *>& Game::getBackgroundUppers()
{
    return m_backgroundUppers;
}

std::vector<Background *>& Game::getBackgroundLowers()
{
    return m_backgroundLowers;
}

std::vector<Background *>& Game::getBackgroundMids()
{
    return m_backgroundMids;
}

std::vector<std::unique_ptr<Tree>>& Game::getTrees()
{
    return m_trees;
}

std::vector<Ground *>& Game::getGrounds()
{
    return m_grounds;
}

std::vector<std::unique_ptr<Hole>>& Game::getHoles()
{
    return m_holes;
}

std::vector<std::unique_ptr<CaveExit>>& Game::getCaveExits()
{
    return m_caveExits;
}

std::vector<std::unique_ptr<LogVerticalTall>>& Game::getLogVerticalTalls()
{
    return m_logVerticalTalls;
}

std::vector<std::unique_ptr<LogVerticalShort>>& Game::getLogVerticalShorts()
{
    return m_logVerticalShorts;
}

std::vector<std::unique_ptr<Thorns>>& Game::getThorns()
{
    return m_thorns;
}

std::vector<std::unique_ptr<Stump>>& Game::getStumps()
{
    return m_stumps;
}

std::vector<std::unique_ptr<SideSpike>>& Game::getSideSpikes()
{
    return m_sideSpikes;
}

std::vector<std::unique_ptr<UpwardSpike>>& Game::getUpwardSpikes()
{
    return m_upwardSpikes;
}

std::vector<std::unique_ptr<JumpSpring>>& Game::getJumpSprings()
{
    return m_jumpSprings;
}

std::vector<std::unique_ptr<Rock>>& Game::getRocks()
{
    return m_rocks;
}

std::vector<std::unique_ptr<GroundPlatform>>& Game::getPlatforms()
{
    return m_platforms;
}

std::vector<std::unique_ptr<EndSign>>& Game::getEndSigns()
{
    return m_endSigns;
}

std::vector<std::unique_ptr<Carrot>>& Game::getCarrots()
{
    return m_carrots;
}

std::vector<std::unique_ptr<GoldenCarrot>>& Game::getGoldenCarrots()
{
    return m_goldenCarrots;
}

std::vector<std::unique_ptr<SnakeGrunt>>& Game::getSnakeGruntEnemies()
{
    return m_snakeGruntEnemies;
}

std::vector<std::unique_ptr<SnakeHorned>>& Game::getSnakeHornedEnemies()
{
    return m_snakeHornedEnemies;
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
    if (getEndSigns().size() == 0)
    {
        return ZOOMED_OUT_CAM_WIDTH;
    }
    
    return getEndSigns().at(0)->getPosition().getX() + getEndSigns().at(0)->getWidth();
}

float Game::getFarRightBottom()
{
    if (getEndSigns().size() == 0)
    {
        return 8.750433275563259f;
    }
    
    return getEndSigns().at(0)->getPosition().getY() - getJon().getHeight() / 2;
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

bool Game::isBurstingThroughCaveToSurface(PhysicalEntity& entity, std::vector<std::unique_ptr<CaveExit>>& items, float deltaTime)
{
    float entityVelocityY = entity.getVelocity().getY();
    float entityLeft = entity.getBounds().getLowerLeft().getX();
    float entityBottom = entity.getBounds().getLowerLeft().getY();
    
    if (entityVelocityY > 13.1f)
    {
        for (std::vector<std::unique_ptr<CaveExit>>::iterator i = items.begin(); i != items.end(); i++)
        {
            if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i)->getHoleBounds()))
            {
                float itemLeft = (*i)->getHoleBounds().getLowerLeft().getX();
                float itemTop = (*i)->getHoleBounds().getTop();
                
                if (itemLeft < entityLeft && entityBottom < itemTop)
                {
                    (*i)->triggerEruption();
                    
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool Game::isFallingThroughCaveExit(PhysicalEntity& entity, std::vector<std::unique_ptr<CaveExit>>& items, float deltaTime)
{
    float entityVelocityY = entity.getVelocity().getY();
    float entityLowerLeftX = entity.getBounds().getLowerLeft().getX();
    float entityRight = entity.getBounds().getRight();
    
    if (entityVelocityY <= 0)
    {
        for (std::vector<std::unique_ptr<CaveExit>>::iterator i = items.begin(); i != items.end(); i++)
        {
            if (!(*i)->hasCover())
            {
                float itemLowerLeftX = (*i)->getHoleBounds().getLowerLeft().getX();
                float itemRight = (*i)->getHoleBounds().getRight();
                
                if (OverlapTester::doRectanglesOverlap(entity.getBounds(), (*i)->getHoleBounds()))
                {
                    if (entityLowerLeftX >= itemLowerLeftX && entityRight <= itemRight)
                    {
                        return true;
                    }
                }
            }
        }
    }
    
    return false;
}
