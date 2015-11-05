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

#define backgroundSkiesKey "backgroundSkies"
#define backgroundTreesKey "backgroundTrees"
#define backgroundCavesKey "backgroundCaves"
#define treesKey "trees"
#define caveSkeletonsKey "caveSkeletons"
#define groundsKey "grounds"
#define holesKey "holes"
#define caveExitsKey "caveExits"
#define logVerticalTallsKey "logVerticalTalls"
#define logVerticalShortsKey "logVerticalShorts"
#define thornsKey "thorns"
#define stumpsKey "stumps"
#define sideSpikesKey "sideSpikes"
#define upwardSpikesKey "upwardSpikes"
#define jumpSpringsKey "jumpSprings"
#define rocksKey "rocks"
#define platformsKey "platforms"
#define endSignsKey "endSigns"
#define carrotsKey "carrots"
#define goldenCarrotsKey "goldenCarrots"
#define jonsKey "jons"

Game::Game() : m_fStateTime(0.0f), m_iNumTotalCarrots(0), m_iNumTotalGoldenCarrots(0), m_isLoaded(false)
{
    // Empty
}

void Game::copy(Game* game)
{
    reset();
    
    copyPhysicalEntities(game->getBackgroundSkies(), m_backgroundSkies);
    copyPhysicalEntities(game->getBackgroundTrees(), m_backgroundTrees);
    copyPhysicalEntities(game->getBackgroundCaves(), m_backgroundCaves);
    copyPhysicalEntities(game->getTrees(), m_trees);
    copyPhysicalEntities(game->getCaveSkeletons(), m_caveSkeletons);
    copyPhysicalEntities(game->getGrounds(), m_grounds);
    copyPhysicalEntities(game->getHoles(), m_holes);
    copyPhysicalEntities(game->getCaveExits(), m_caveExits);
    copyPhysicalEntities(game->getLogVerticalTalls(), m_logVerticalTalls);
    copyPhysicalEntities(game->getLogVerticalShorts(), m_logVerticalShorts);
    copyPhysicalEntities(game->getThorns(), m_thorns);
    copyPhysicalEntities(game->getStumps(), m_stumps);
    copyPhysicalEntities(game->getSideSpikes(), m_sideSpikes);
    copyPhysicalEntities(game->getUpwardSpikes(), m_upwardSpikes);
    copyPhysicalEntities(game->getJumpSprings(), m_jumpSprings);
    copyPhysicalEntities(game->getRocks(), m_rocks);
    copyPhysicalEntities(game->getPlatforms(), m_platforms);
    copyPhysicalEntities(game->getEndSigns(), m_endSigns);
    copyPhysicalEntities(game->getCarrots(), m_carrots);
    copyPhysicalEntities(game->getGoldenCarrots(), m_goldenCarrots);
    copyPhysicalEntities(game->getJons(), m_jons);
    
    m_iNumTotalCarrots = (int) m_carrots.size();
    m_iNumTotalGoldenCarrots = (int) m_goldenCarrots.size();
    
    m_isLoaded = true;
}

void Game::load(const char* json)
{
    reset();
    
    rapidjson::Document d;
    d.Parse<0>(json);
    
    loadArray(m_backgroundSkies, d, backgroundSkiesKey);
    loadArray(m_backgroundTrees, d, backgroundTreesKey);
    loadArray(m_backgroundCaves, d, backgroundCavesKey);
    loadArray(m_trees, d, treesKey);
    loadArray(m_caveSkeletons, d, caveSkeletonsKey);
    loadArray(m_grounds, d, groundsKey);
    loadArray(m_holes, d, holesKey);
    loadArray(m_caveExits, d, caveExitsKey);
    loadArray(m_logVerticalTalls, d, logVerticalTallsKey);
    loadArray(m_logVerticalShorts, d, logVerticalShortsKey);
    loadArray(m_thorns, d, thornsKey);
    loadArray(m_stumps, d, stumpsKey);
    loadArray(m_sideSpikes, d, sideSpikesKey);
    loadArray(m_upwardSpikes, d, upwardSpikesKey);
    loadArray(m_jumpSprings, d, jumpSpringsKey);
    loadArray(m_rocks, d, rocksKey);
    loadArray(m_platforms, d, platformsKey);
    loadArray(m_endSigns, d, endSignsKey);
    loadArray(m_carrots, d, carrotsKey);
    loadArray(m_goldenCarrots, d, goldenCarrotsKey);
    loadArray(m_jons, d, jonsKey);
    
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
    
    saveArray(m_backgroundSkies, w, backgroundSkiesKey);
    saveArray(m_backgroundTrees, w, backgroundTreesKey);
    saveArray(m_backgroundCaves, w, backgroundCavesKey);
    saveArray(m_trees, w, treesKey);    
    saveArray(m_caveSkeletons, w, caveSkeletonsKey);
    saveArray(m_grounds, w, groundsKey);
    saveArray(m_holes, w, holesKey);
    saveArray(m_caveExits, w, caveExitsKey);
    saveArray(m_logVerticalTalls, w, logVerticalTallsKey);
    saveArray(m_logVerticalShorts, w, logVerticalShortsKey);
    saveArray(m_thorns, w, thornsKey);
    saveArray(m_stumps, w, stumpsKey);
    saveArray(m_sideSpikes, w, sideSpikesKey);
    saveArray(m_upwardSpikes, w, upwardSpikesKey);
    saveArray(m_jumpSprings, w, jumpSpringsKey);
    saveArray(m_rocks, w, rocksKey);
    saveArray(m_platforms, w, platformsKey);
    saveArray(m_endSigns, w, endSignsKey);
    saveArray(m_carrots, w, carrotsKey);
    saveArray(m_goldenCarrots, w, goldenCarrotsKey);
    saveArray(m_jons, w, jonsKey);
    
    w.EndObject();
    
    return s.GetString();
}

void Game::reset()
{
    m_backgroundSkies.clear();
    m_backgroundTrees.clear();
    m_backgroundCaves.clear();
    m_trees.clear();
    m_caveSkeletons.clear();
    m_grounds.clear();
    m_holes.clear();
    m_caveExits.clear();
    m_logVerticalTalls.clear();
    m_logVerticalShorts.clear();
    m_thorns.clear();
    m_stumps.clear();
    m_sideSpikes.clear();
    m_upwardSpikes.clear();
    m_jumpSprings.clear();
    m_rocks.clear();
    m_platforms.clear();
    m_endSigns.clear();
    m_carrots.clear();
    m_goldenCarrots.clear();
    m_jons.clear();
    
    m_fStateTime = 0;
    m_iNumTotalCarrots = 0;
    m_iNumTotalGoldenCarrots = 0;
    m_isLoaded = false;
}

void Game::updateAndClean(float deltaTime, bool isJonAllowedToMove)
{
    m_fStateTime += deltaTime;
    
    if (getJons().size() > 0)
    {
        getJon().update(deltaTime, *this, isJonAllowedToMove);
    }
    
    EntityUtils::clean(getJons());
    
    EntityUtils::updateAndClean(getTrees(), deltaTime);
    EntityUtils::updateAndClean(getCaveSkeletons(), deltaTime);
    EntityUtils::updateAndClean(getGrounds(), deltaTime);
    EntityUtils::updateAndClean(getHoles(), deltaTime);
    EntityUtils::updateAndClean(getCaveExits(), deltaTime);
    EntityUtils::updateAndClean(getLogVerticalTalls(), deltaTime);
    EntityUtils::updateAndClean(getLogVerticalShorts(), deltaTime);
    EntityUtils::updateAndClean(getThorns(), deltaTime);
    EntityUtils::updateAndClean(getStumps(), deltaTime);
    EntityUtils::updateAndClean(getSideSpikes(), deltaTime);
    EntityUtils::updateAndClean(getUpwardSpikes(), deltaTime);
    EntityUtils::updateAndClean(getJumpSprings(), deltaTime);
    EntityUtils::updateAndClean(getRocks(), deltaTime);
    EntityUtils::updateAndClean(getPlatforms(), deltaTime);
    EntityUtils::updateAndClean(getEndSigns(), deltaTime);
    EntityUtils::updateAndClean(getCarrots(), deltaTime);
    EntityUtils::updateAndClean(getGoldenCarrots(), deltaTime);
}

int Game::calcSum()
{
    int sum = 0;
    
    sum += m_backgroundSkies.size();
    sum += m_backgroundTrees.size();
    sum += m_backgroundCaves.size();
    sum += m_trees.size();
    sum += m_caveSkeletons.size();
    sum += m_grounds.size();
    sum += m_holes.size();
    sum += m_caveExits.size();
    sum += m_logVerticalTalls.size();
    sum += m_logVerticalShorts.size();
    sum += m_thorns.size();
    sum += m_stumps.size();
    sum += m_sideSpikes.size();
    sum += m_upwardSpikes.size();
    sum += m_jumpSprings.size();
    sum += m_rocks.size();
    sum += m_platforms.size();
    sum += m_endSigns.size();
    sum += m_carrots.size();
    sum += m_goldenCarrots.size();
    sum += m_jons.size();
    
    return sum;
}

bool Game::isJonGrounded(float deltaTime)
{
    return !EntityUtils::isFallingThroughHole(getJon(), getHoles(), deltaTime) && (EntityUtils::isLanding(getJon(), getGrounds(), deltaTime) || EntityUtils::isLanding(getJon(), getPlatforms(), deltaTime) || EntityUtils::isLanding(getJon(), getLogVerticalTalls(), deltaTime) || EntityUtils::isLanding(getJon(), getLogVerticalShorts(), deltaTime) || EntityUtils::isLanding(getJon(), getStumps(), deltaTime) || EntityUtils::isLanding(getJon(), getRocks(), deltaTime));
}

bool Game::isJonBlockedHorizontally(float deltaTime)
{
    return EntityUtils::isBlockedOnRight(getJon(), getGrounds(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getLogVerticalTalls(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getLogVerticalShorts(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getStumps(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getRocks(), deltaTime) || EntityUtils::isBlockedOnRight(getJon(), getUpwardSpikes(), deltaTime);
}

bool Game::isJonBlockedVertically(float deltaTime)
{
    return EntityUtils::isBlockedAbove(getJon(), getGrounds(), deltaTime);
}

bool Game::isJonHit()
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

bool Game::isBurrowEffective()
{
    return EntityUtils::isBurrowingThroughHole(getJon(), getHoles());
}

std::vector<std::unique_ptr<BackgroundSky>>& Game::getBackgroundSkies()
{
    return m_backgroundSkies;
}

std::vector<std::unique_ptr<BackgroundTrees>>& Game::getBackgroundTrees()
{
    return m_backgroundTrees;
}

std::vector<std::unique_ptr<BackgroundCave>>& Game::getBackgroundCaves()
{
    return m_backgroundCaves;
}

std::vector<std::unique_ptr<Tree>>& Game::getTrees()
{
    return m_trees;
}

std::vector<std::unique_ptr<CaveSkeleton>>& Game::getCaveSkeletons()
{
    return m_caveSkeletons;
}

std::vector<std::unique_ptr<Ground>>& Game::getGrounds()
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

std::vector<std::unique_ptr<Jon>>& Game::getJons()
{
    return m_jons;
}

Jon& Game::getJon()
{
    return *getJons().at(getJons().size() - 1).get();
}

float Game::getFarRight()
{
    if (getEndSigns().size() == 0)
    {
        return ZOOMED_OUT_CAM_WIDTH;
    }
    
    return getEndSigns().at(0)->getPosition().getX() + getEndSigns().at(0)->getWidth();
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