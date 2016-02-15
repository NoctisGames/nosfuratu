//
//  Game.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Game.h"
#include "GameConstants.h"
#include "EntityUtils.h"
#include "OverlapTester.h"
#include "Assets.h"

#define midgroundsKey "midgrounds"
#define groundsKey "grounds"
#define exitGroundsKey "exitGrounds"
#define holesKey "holes"
#define foregroundObjectsKey "foregroundObjects"
#define enemiesKey "enemies"
#define collectiblesKey "collectibles"
#define jonsKey "jons"

Game::Game(int level) : m_fStateTime(0.0f), m_fFarRight(ZOOMED_OUT_CAM_WIDTH), m_fFarRightBottom(GAME_HEIGHT / 2), m_iNumTotalCarrots(0), m_iNumTotalGoldenCarrots(0), m_iLevel(level), m_isLoaded(false)
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
    copyPhysicalEntities(game->getHoles(), m_holes);
    copyPhysicalEntities(game->getForegroundObjects(), m_foregroundObjects);
    copyPhysicalEntities(game->getEnemies(), m_enemies);
    copyPhysicalEntities(game->getCollectibleItems(), m_collectibleItems);
    copyPhysicalEntities(game->getJons(), m_jons);
    
    onLoaded();
}

void Game::load(const char* json)
{
    reset();
    
    rapidjson::Document d;
    d.Parse<0>(json);
    
    loadArray(m_midgrounds, d, midgroundsKey);
    loadArray(m_grounds, d, groundsKey);
    loadArray(m_exitGrounds, d, exitGroundsKey);
    loadArray(m_holes, d, holesKey);
    loadArray(m_foregroundObjects, d, foregroundObjectsKey);
    loadArray(m_enemies, d, enemiesKey);
    loadArray(m_collectibleItems, d, collectiblesKey);
    loadArray(m_jons, d, jonsKey);
    
    onLoaded();
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
    saveArray(m_holes, w, holesKey);
    saveArray(m_foregroundObjects, w, foregroundObjectsKey);
    saveArray(m_enemies, w, enemiesKey);
    saveArray(m_collectibleItems, w, collectiblesKey);
    saveArray(m_jons, w, jonsKey);
    
    w.EndObject();
    
    return s.GetString();
}

void Game::reset()
{
    m_midgrounds.clear();
    m_grounds.clear();
    m_exitGrounds.clear();
    m_holes.clear();
    m_foregroundObjects.clear();
    m_enemies.clear();
    m_collectibleItems.clear();
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
    EntityUtils::updateAndClean(getHoles(), deltaTime);
    EntityUtils::updateAndClean(getForegroundObjects(), deltaTime);
    EntityUtils::updateAndClean(getEnemies(), deltaTime);
    EntityUtils::updateAndClean(getCollectibleItems(), deltaTime);
    
	if (getJons().size() >= 1)
	{
		getJon().update(deltaTime);
	}
}

int Game::calcSum()
{
    int sum = 0;
    
    sum += m_midgrounds.size();
    sum += m_grounds.size();
    sum += m_exitGrounds.size();
    sum += m_holes.size();
    sum += m_foregroundObjects.size();
    sum += m_enemies.size();
    sum += m_collectibleItems.size();
    sum += m_jons.size();
    
    return sum;
}

bool Game::isJonGrounded(float deltaTime)
{
    if (EntityUtils::isFallingThroughHole(getJon(), getHoles(), deltaTime))
    {
        return false;
    }
    
    return EntityUtils::isLanding(getJon(), getGrounds(), deltaTime)
    || EntityUtils::isLanding(getJon(), getExitGrounds(), deltaTime)
    || EntityUtils::isLanding(getJon(), getForegroundObjects(), deltaTime)
    || EntityUtils::isLanding(getJon(), getEnemies(), deltaTime);
}

bool Game::isJonBlockedHorizontally(float deltaTime)
{
    return EntityUtils::isBlockedOnRight(getJon(), getGrounds(), deltaTime)
    || EntityUtils::isBlockedOnRight(getJon(), getExitGrounds(), deltaTime)
    || EntityUtils::isBlockedOnRight(getJon(), getForegroundObjects(), deltaTime);
}

bool Game::isJonBlockedVertically(float deltaTime)
{
    return EntityUtils::isBlockedAbove(getJon(), getGrounds(), deltaTime)
    || EntityUtils::isBlockedAbove(getJon(), getExitGrounds(), deltaTime)
    || EntityUtils::isBlockedAbove(getJon(), getForegroundObjects(), deltaTime)
    || EntityUtils::isBlockedAbove(getJon(), getEnemies(), deltaTime);
}

bool Game::isSpinningBackFistDelivered(float deltaTime)
{
    return EntityUtils::isHorizontallyHitting(getJon(), getEnemies(), deltaTime)
    || EntityUtils::isHorizontallyHitting(getJon(), getForegroundObjects(), deltaTime);
}

bool Game::isBurrowEffective()
{
    return EntityUtils::isBurrowingThroughHole(getJon(), getHoles());
}

bool Game::isUpwardThrustEffective(float deltaTime)
{
    return EntityUtils::isHittingFromBelow(getJon(), getEnemies(), deltaTime)
    || EntityUtils::isHittingFromBelow(getJon(), getForegroundObjects(), deltaTime);
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

std::vector<Hole *>& Game::getHoles()
{
    return m_holes;
}

std::vector<ForegroundObject *>& Game::getForegroundObjects()
{
    return m_foregroundObjects;
}

std::vector<Enemy *>& Game::getEnemies()
{
    return m_enemies;
}

std::vector<CollectibleItem *>& Game::getCollectibleItems()
{
    return m_collectibleItems;
}

std::vector<Jon *>& Game::getJons()
{
    return m_jons;
}

Jon& Game::getJon()
{
    return *getJons().at(0);
}

float Game::getFarRight()
{
    return m_fFarRight;
}

float Game::getFarRightBottom()
{
    return m_fFarRightBottom;
}

float Game::getStateTime()
{
    return m_fStateTime;
}

int Game::getNumTotalCarrots()
{
    return m_iNumTotalCarrots;
}

int Game::getNumRemainingCarrots()
{
    int numRemaining = 0;
    for (std::vector<CollectibleItem *>::iterator i = getCollectibleItems().begin(); i != getCollectibleItems().end(); i++)
    {
        if ((*i)->getType() == CollectibleItemType_Carrot)
        {
            numRemaining++;
        }
    }
    
    return numRemaining;
}

int Game::getNumTotalGoldenCarrots()
{
    return m_iNumTotalGoldenCarrots;
}

int Game::getNumRemainingGoldenCarrots()
{
    int numRemaining = 0;
    for (std::vector<CollectibleItem *>::iterator i = getCollectibleItems().begin(); i != getCollectibleItems().end(); i++)
    {
        if ((*i)->getType() == CollectibleItemType_GoldenCarrot)
        {
            numRemaining++;
        }
    }
    
    return numRemaining;
}

int Game::getLevel()
{
    return m_iLevel;
}

bool Game::isLoaded()
{
    return m_isLoaded;
}

void Game::calcFarRight()
{
    for (std::vector<Ground *>::iterator i = m_grounds.begin(); i != m_grounds.end(); i++)
    {
        float right = (*i)->getBounds().getRight();
        if (right > m_fFarRight)
        {
            m_fFarRight = right;
        }
    }
    
    for (std::vector<ForegroundObject *>::iterator i = m_foregroundObjects.begin(); i != m_foregroundObjects.end(); i++)
    {
        if (dynamic_cast<EndSign *>((*i)))
        {
            m_fFarRight = (*i)->getBounds().getRight();
            m_fFarRightBottom = (*i)->getBounds().getBottom();
            break;
        }
    }
}

#pragma mark private

void Game::onLoaded()
{
    setGameToEntities(m_jons, this);
    setGameToEntities(m_enemies, this);
    
    m_iNumTotalCarrots = getNumRemainingCarrots();
    m_iNumTotalGoldenCarrots = getNumRemainingGoldenCarrots();
    
    calcFarRight();
    
    m_isLoaded = true;
}
