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

Game::Game() : m_fStateTime(0.0f), m_isLoaded(false)
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

void Game::copy(Game* game)
{
    reset();
    
    copyPhysicalEntities(game->getBackgroundSkies(), *m_backgroundSkies);
    copyPhysicalEntities(game->getBackgroundTrees(), *m_backgroundTrees);
    copyPhysicalEntities(game->getBackgroundCaves(), *m_backgroundCaves);
    copyTrees(game->getTrees(), *m_trees);
    copyCaveSkeletons(game->getCaveSkeletons(), *m_caveSkeletons);
    copyGrounds(game->getGrounds(), *m_grounds);
    copyPhysicalEntities(game->getLogVerticalTalls(), *m_logVerticalTalls);
    copyPhysicalEntities(game->getLogVerticalShorts(), *m_logVerticalShorts);
    copyPhysicalEntities(game->getThorns(), *m_thorns);
    copyPhysicalEntities(game->getStumps(), *m_stumps);
    copyPhysicalEntities(game->getSideSpikes(), *m_sideSpikes);
    copyUpwardSpikes(game->getUpwardSpikes(), *m_upwardSpikes);
    copyJumpSprings(game->getJumpSprings(), *m_jumpSprings);
    copyRocks(game->getRocks(), *m_rocks);
    copyPlatforms(game->getPlatforms(), *m_platforms);
    copyPhysicalEntities(game->getEndSigns(), *m_endSigns);
    copyPhysicalEntities(game->getCarrots(), *m_carrots);
    copyPhysicalEntities(game->getGoldenCarrots(), *m_goldenCarrots);
    copyPhysicalEntities(game->getJons(), *m_jons);
    
    m_fStateTime = 0;
    m_isLoaded = true;
}

void Game::load(const char* json)
{
    reset();
    
    rapidjson::Document d;
    d.Parse<0>(json);
    
    loadArray(*m_backgroundSkies, d, backgroundSkiesKey);
    loadArray(*m_backgroundTrees, d, backgroundTreesKey);
    loadArray(*m_backgroundCaves, d, backgroundCavesKey);
    loadArray(*m_trees, d, treesKey);
    loadArray(*m_caveSkeletons, d, caveSkeletonsKey);
    loadArray(*m_grounds, d, groundsKey);
    loadArray(*m_logVerticalTalls, d, logVerticalTallsKey);
    loadArray(*m_logVerticalShorts, d, logVerticalShortsKey);
    loadArray(*m_thorns, d, thornsKey);
    loadArray(*m_stumps, d, stumpsKey);
    loadArray(*m_sideSpikes, d, sideSpikesKey);
    loadArray(*m_upwardSpikes, d, upwardSpikesKey);
    loadArray(*m_jumpSprings, d, jumpSpringsKey);
    loadArray(*m_rocks, d, rocksKey);
    loadArray(*m_platforms, d, platformsKey);
    loadArray(*m_endSigns, d, endSignsKey);
    loadArray(*m_carrots, d, carrotsKey);
    loadArray(*m_goldenCarrots, d, goldenCarrotsKey);
    loadArray(*m_jons, d, jonsKey);
    
    m_fStateTime = 0;
    m_isLoaded = true;
}

const char* Game::save()
{
    using namespace rapidjson;
    using namespace std;
    
    StringBuffer s;
    Writer<StringBuffer> w(s);
    
    w.StartObject();
    
    saveArray(*m_backgroundSkies, w, backgroundSkiesKey);
    saveArray(*m_backgroundTrees, w, backgroundTreesKey);
    saveArray(*m_backgroundCaves, w, backgroundCavesKey);
    saveArray(*m_trees, w, treesKey);    
    saveArray(*m_caveSkeletons, w, caveSkeletonsKey);
    saveArray(*m_grounds, w, groundsKey);
    saveArray(*m_logVerticalTalls, w, logVerticalTallsKey);
    saveArray(*m_logVerticalShorts, w, logVerticalShortsKey);
    saveArray(*m_thorns, w, thornsKey);
    saveArray(*m_stumps, w, stumpsKey);
    saveArray(*m_sideSpikes, w, sideSpikesKey);
    saveArray(*m_upwardSpikes, w, upwardSpikesKey);
    saveArray(*m_jumpSprings, w, jumpSpringsKey);
    saveArray(*m_rocks, w, rocksKey);
    saveArray(*m_platforms, w, platformsKey);
    saveArray(*m_endSigns, w, endSignsKey);
    saveArray(*m_carrots, w, carrotsKey);
    saveArray(*m_goldenCarrots, w, goldenCarrotsKey);
    saveArray(*m_jons, w, jonsKey);
    
    w.EndObject();
    
    const char * retval = s.GetString();
    size_t len = strlen(retval);
    if (len > 8192)
    {
        retval = nullptr;
    }
    
    return retval;
}

void Game::reset()
{
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
    
    m_isLoaded = false;
}

void Game::updateAndClean(float deltaTime)
{
    m_fStateTime += deltaTime;
    
    EntityUtils::updateAndClean(getTrees(), deltaTime);
    EntityUtils::updateAndClean(getCaveSkeletons(), deltaTime);
    EntityUtils::updateAndClean(getGrounds(), deltaTime);
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
    
    sum += m_backgroundSkies->size();
    sum += m_backgroundTrees->size();
    sum += m_backgroundCaves->size();
    sum += m_trees->size();
    sum += m_caveSkeletons->size();
    sum += m_grounds->size();
    sum += m_logVerticalTalls->size();
    sum += m_logVerticalShorts->size();
    sum += m_thorns->size();
    sum += m_stumps->size();
    sum += m_sideSpikes->size();
    sum += m_upwardSpikes->size();
    sum += m_jumpSprings->size();
    sum += m_rocks->size();
    sum += m_platforms->size();
    sum += m_endSigns->size();
    sum += m_carrots->size();
    sum += m_goldenCarrots->size();
    sum += m_jons->size();
    
    return sum;
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
    if (getEndSigns().size() == 0)
    {
        return ZOOMED_OUT_CAM_WIDTH;
    }
    
    return getEndSigns().at(0).getPosition().getX() + getEndSigns().at(0).getWidth();
}

float Game::getStateTime()
{
    return m_fStateTime;
}

bool Game::isLoaded()
{
    return m_isLoaded;
}