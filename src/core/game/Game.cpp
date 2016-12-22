//
//  Game.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "Game.h"
#include "GameConstants.h"
#include "EntityUtils.h"
#include "OverlapTester.h"
#include "Assets.h"

#define worldKey "world"
#define levelKey "level"

#define midgroundsKey "midgrounds"
#define groundsKey "grounds"
#define pitsKey "pits"
#define exitGroundsKey "exitGrounds"
#define holesKey "holes"
#define foregroundObjectsKey "foregroundObjects"
#define midBossForegroundObjectsKey "midBossForegroundObjects"
#define endBossForegroundObjectsKey "endBossForegroundObjects"
#define countHissWithMinasKey "countHissWithMinas"
#define endBossSnakesKey "endBossSnakes"
#define enemiesKey "enemies"
#define collectiblesKey "collectibles"
#define jonsKey "jons"
#define extraForegroundObjectsKey "extraForegroundObjects"
#define foregroundCoverObjectsKey "foregroundCoverObjects"
#define markersKey "markers"

Game::Game() :
m_fStateTime(0.0f),
m_fFarRight(ZOOMED_OUT_CAM_WIDTH),
m_fFarRightBottom(GAME_HEIGHT / 2),
m_fCamFarRight(ZOOMED_OUT_CAM_WIDTH),
m_fCamFarRightBottom(GAME_HEIGHT / 2),
m_iBestLevelStatsFlag(0),
m_iNumCarrotsCollected(0),
m_iNumGoldenCarrotsCollected(0),
m_iWorld(1),
m_iLevel(1),
m_isLoaded(false)
{
    GRID_MANAGER->setGridCellSize(GRID_CELL_SIZE);
}

void Game::copy(Game* game)
{
    reset();
    
    m_iWorld = game->getWorld();
    m_iLevel = game->getLevel();
    
    copyPhysicalEntities(game->getMidgrounds(), m_midgrounds);
    copyPhysicalEntities(game->getGrounds(), m_grounds);
    copyPhysicalEntities(game->getPits(), m_pits);
    copyPhysicalEntities(game->getExitGrounds(), m_exitGrounds);
    copyPhysicalEntities(game->getHoles(), m_holes);
    copyPhysicalEntities(game->getForegroundObjects(), m_foregroundObjects);
    copyPhysicalEntities(game->getMidBossForegroundObjects(), m_midBossForegroundObjects);
    copyPhysicalEntities(game->getEndBossForegroundObjects(), m_endBossForegroundObjects);
    copyPhysicalEntities(game->getCountHissWithMinas(), m_countHissWithMinas);
    copyPhysicalEntities(game->getEndBossSnakes(), m_endBossSnakes);
    copyPhysicalEntities(game->getEnemies(), m_enemies);
    copyPhysicalEntities(game->getCollectibleItems(), m_collectibleItems);
    copyPhysicalEntities(game->getJons(), m_jons);
    copyPhysicalEntities(game->getExtraForegroundObjects(), m_extraForegroundObjects);
    copyPhysicalEntities(game->getForegroundCoverObjects(), m_foregroundCoverObjects);
    
    copyPhysicalEntities(game->getMarkers(), m_markers);
    
    onLoaded();
}

void Game::load(const char* json)
{
    reset();
    
    rapidjson::Document d;
    d.Parse<0>(json);
    
    if (d.HasMember(worldKey) && d.HasMember(levelKey))
    {
        rapidjson::Value& world = d[worldKey];
        assert(world.IsInt());
        m_iWorld = d[worldKey].GetInt();
        
        rapidjson::Value& level = d[levelKey];
        assert(level.IsInt());
        m_iLevel = d[levelKey].GetInt();
    }
    
    loadArray(m_midgrounds, d, midgroundsKey);
    loadArray(m_grounds, d, groundsKey);
    loadArray(m_pits, d, pitsKey);
    loadArray(m_exitGrounds, d, exitGroundsKey);
    loadArray(m_holes, d, holesKey);
    loadArray(m_foregroundObjects, d, foregroundObjectsKey);
    loadArray(m_midBossForegroundObjects, d, midBossForegroundObjectsKey);
    loadArray(m_endBossForegroundObjects, d, endBossForegroundObjectsKey);
    loadArray(m_countHissWithMinas, d, countHissWithMinasKey);
    loadArray(m_endBossSnakes, d, endBossSnakesKey);
    loadArray(m_enemies, d, enemiesKey);
    loadArray(m_collectibleItems, d, collectiblesKey);
    loadArray(m_jons, d, jonsKey);
    loadArray(m_extraForegroundObjects, d, extraForegroundObjectsKey);
    loadArray(m_foregroundCoverObjects, d, foregroundCoverObjectsKey);
    
    loadArray(m_markers, d, markersKey);
    
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
    
    w.String(worldKey);
    w.Int(m_iWorld);
    w.String(levelKey);
    w.Int(m_iLevel);
    
    saveArray(m_midgrounds, w, midgroundsKey);
    saveArray(m_grounds, w, groundsKey);
    saveArray(m_pits, w, pitsKey);
    saveArray(m_exitGrounds, w, exitGroundsKey);
    saveArray(m_holes, w, holesKey);
    saveArray(m_foregroundObjects, w, foregroundObjectsKey);
    saveArray(m_midBossForegroundObjects, w, midBossForegroundObjectsKey);
    saveArray(m_endBossForegroundObjects, w, endBossForegroundObjectsKey);
    saveArray(m_countHissWithMinas, w, countHissWithMinasKey);
    saveArray(m_endBossSnakes, w, endBossSnakesKey);
    saveArray(m_enemies, w, enemiesKey);
    saveArray(m_collectibleItems, w, collectiblesKey);
    saveArray(m_jons, w, jonsKey);
    saveArray(m_extraForegroundObjects, w, extraForegroundObjectsKey);
    saveArray(m_foregroundCoverObjects, w, foregroundCoverObjectsKey);
    
    saveArray(m_markers, w, markersKey);
    
    w.EndObject();
    
    return s.GetString();
}

void Game::reset()
{
    EntityUtils::cleanUpVectorOfPointers(m_backgroundUppers);
    EntityUtils::cleanUpVectorOfPointers(m_backgroundMids);
    EntityUtils::cleanUpVectorOfPointers(m_backgroundLowers);
    EntityUtils::cleanUpVectorOfPointers(m_backgroundLowers);
    EntityUtils::cleanUpVectorOfPointers(m_backgroundMidgroundCovers);
    
    EntityUtils::cleanUpVectorOfPointers(m_midgrounds);
    EntityUtils::cleanUpVectorOfPointers(m_grounds);
    EntityUtils::cleanUpVectorOfPointers(m_pits);
    EntityUtils::cleanUpVectorOfPointers(m_exitGrounds);
    EntityUtils::cleanUpVectorOfPointers(m_holes);
    EntityUtils::cleanUpVectorOfPointers(m_foregroundObjects);
    EntityUtils::cleanUpVectorOfPointers(m_midBossForegroundObjects);
    EntityUtils::cleanUpVectorOfPointers(m_endBossForegroundObjects);
    EntityUtils::cleanUpVectorOfPointers(m_countHissWithMinas);
    EntityUtils::cleanUpVectorOfPointers(m_endBossSnakes);
    EntityUtils::cleanUpVectorOfPointers(m_enemies);
    EntityUtils::cleanUpVectorOfPointers(m_collectibleItems);
    EntityUtils::cleanUpVectorOfPointers(m_jons);
    EntityUtils::cleanUpVectorOfPointers(m_extraForegroundObjects);
    EntityUtils::cleanUpVectorOfPointers(m_foregroundCoverObjects);
    
    EntityUtils::cleanUpVectorOfPointers(m_markers);
    
    m_fStateTime = 0;
    m_iNumCarrotsCollected = 0;
    m_iNumGoldenCarrotsCollected = 0;
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
    EntityUtils::updateAndClean(getPits(), deltaTime);
    EntityUtils::updateAndClean(getExitGrounds(), deltaTime);
    EntityUtils::updateAndClean(getHoles(), deltaTime);
    EntityUtils::updateAndClean(getForegroundObjects(), deltaTime);
    EntityUtils::updateAndClean(getMidBossForegroundObjects(), deltaTime);
    EntityUtils::updateAndClean(getEndBossForegroundObjects(), deltaTime);
    EntityUtils::updateAndClean(getCountHissWithMinas(), deltaTime);
    EntityUtils::updateAndClean(getEndBossSnakes(), deltaTime);
    EntityUtils::updateAndClean(getEnemies(), deltaTime);
    EntityUtils::updateAndClean(getCollectibleItems(), deltaTime);
    EntityUtils::updateAndClean(getExtraForegroundObjects(), deltaTime);
    EntityUtils::updateAndClean(getForegroundCoverObjects(), deltaTime);
    
    EntityUtils::updateAndClean(getMarkers(), deltaTime);
    
	if (getJons().size() > 0)
	{
		getJon().update(deltaTime);
	}
}

void Game::updateBackgrounds(Vector2D& cameraPosition, float deltaTime)
{
    EntityUtils::updateBackgrounds(getBackgroundUppers(), cameraPosition, deltaTime);
    EntityUtils::updateBackgrounds(getBackgroundMids(), cameraPosition, deltaTime);
    EntityUtils::updateBackgrounds(getBackgroundLowerBacks(), cameraPosition, deltaTime);
    EntityUtils::updateBackgrounds(getBackgroundLowers(), cameraPosition, deltaTime);
    EntityUtils::updateBackgrounds(getBackgroundMidgroundCovers(), cameraPosition, deltaTime);
}

int Game::calcSum()
{
    int sum = 0;
    
    sum += m_midgrounds.size();
    sum += m_grounds.size();
    sum += m_pits.size();
    sum += m_exitGrounds.size();
    sum += m_holes.size();
    sum += m_foregroundObjects.size();
    sum += m_midBossForegroundObjects.size();
    sum += m_endBossForegroundObjects.size();
    sum += m_countHissWithMinas.size();
    sum += m_endBossSnakes.size();
    sum += m_enemies.size();
    sum += m_collectibleItems.size();
    sum += m_jons.size();
    sum += m_extraForegroundObjects.size();
    sum += m_foregroundCoverObjects.size();
    
    sum += m_markers.size();
    
    return sum;
}

bool Game::isEntityGrounded(PhysicalEntity* entity, float deltaTime)
{
    if (EntityUtils::isFallingThroughHole(entity, getHoles(), deltaTime))
    {
        return EntityUtils::isLanding(entity, getForegroundObjects(), deltaTime)
        || EntityUtils::isLanding(entity, getForegroundCoverObjects(), deltaTime);
    }

	if (EntityUtils::isFallingThroughPit(entity, getPits(), deltaTime))
	{
		return EntityUtils::isLanding(entity, getForegroundObjects(), deltaTime)
        || EntityUtils::isLanding(entity, getExtraForegroundObjects(), deltaTime)
        || EntityUtils::isLanding(entity, getMidBossForegroundObjects(), deltaTime)
        || EntityUtils::isLanding(entity, getEndBossForegroundObjects(), deltaTime)
        || EntityUtils::isLanding(entity, getForegroundCoverObjects(), deltaTime)
        || EntityUtils::isLanding(entity, getEnemies(), deltaTime);
	}
    
    return EntityUtils::isLanding(entity, getForegroundObjects(), deltaTime)
    || EntityUtils::isLanding(entity, getExtraForegroundObjects(), deltaTime)
    || EntityUtils::isLanding(entity, getMidBossForegroundObjects(), deltaTime)
    || EntityUtils::isLanding(entity, getEndBossForegroundObjects(), deltaTime)
    || EntityUtils::isLanding(entity, getForegroundCoverObjects(), deltaTime)
    || EntityUtils::isLanding(entity, getEnemies(), deltaTime)
    || EntityUtils::isLanding(entity, getEndBossSnakes(), deltaTime)
    || EntityUtils::isLanding(entity, getExitGrounds(), deltaTime)
    || EntityUtils::isLanding(entity, getGrounds(), deltaTime);
}

bool Game::isJonBlockedOnRight(float deltaTime)
{
    if (EntityUtils::isFallingThroughHole(getJonP(), getHoles(), deltaTime))
    {
        return EntityUtils::isBlockedOnRight(getJonP(), getForegroundObjects(), deltaTime)
        || EntityUtils::isBlockedOnRight(getJonP(), getForegroundCoverObjects(), deltaTime);
    }
    
    if (EntityUtils::isFallingThroughPit(getJonP(), getPits(), deltaTime))
    {
        return EntityUtils::isBlockedOnRight(getJonP(), getPits(), deltaTime)
        || EntityUtils::isBlockedOnRight(getJonP(), getForegroundObjects(), deltaTime)
        || EntityUtils::isBlockedOnRight(getJonP(), getExtraForegroundObjects(), deltaTime)
        || EntityUtils::isBlockedOnRight(getJonP(), getMidBossForegroundObjects(), deltaTime)
        || EntityUtils::isBlockedOnRight(getJonP(), getEndBossForegroundObjects(), deltaTime)
        || EntityUtils::isBlockedOnRight(getJonP(), getForegroundCoverObjects(), deltaTime);
    }
    
    return EntityUtils::isBlockedOnRight(getJonP(), getGrounds(), deltaTime)
    || EntityUtils::isBlockedOnRight(getJonP(), getForegroundObjects(), deltaTime)
    || EntityUtils::isBlockedOnRight(getJonP(), getExtraForegroundObjects(), deltaTime)
    || EntityUtils::isBlockedOnRight(getJonP(), getMidBossForegroundObjects(), deltaTime)
    || EntityUtils::isBlockedOnRight(getJonP(), getEndBossForegroundObjects(), deltaTime)
    || EntityUtils::isBlockedOnRight(getJonP(), getForegroundCoverObjects(), deltaTime);
}

bool Game::isJonBlockedOnLeft(float deltaTime)
{
	if (getJon().getVelocity().getX() < 0)
	{
        if (EntityUtils::isFallingThroughHole(getJonP(), getHoles(), deltaTime))
        {
            return EntityUtils::isBlockedOnLeft(getJonP(), getForegroundObjects(), deltaTime)
            || EntityUtils::isBlockedOnLeft(getJonP(), getForegroundCoverObjects(), deltaTime);
        }
        
        if (EntityUtils::isFallingThroughPit(getJonP(), getPits(), deltaTime))
        {
            return EntityUtils::isBlockedOnLeft(getJonP(), getPits(), deltaTime)
            || EntityUtils::isBlockedOnLeft(getJonP(), getForegroundObjects(), deltaTime)
            || EntityUtils::isBlockedOnLeft(getJonP(), getExtraForegroundObjects(), deltaTime)
            || EntityUtils::isBlockedOnLeft(getJonP(), getMidBossForegroundObjects(), deltaTime)
            || EntityUtils::isBlockedOnLeft(getJonP(), getEndBossForegroundObjects(), deltaTime)
            || EntityUtils::isBlockedOnLeft(getJonP(), getForegroundCoverObjects(), deltaTime);
        }
        
        return EntityUtils::isBlockedOnLeft(getJonP(), getGrounds(), deltaTime)
        || EntityUtils::isBlockedOnLeft(getJonP(), getForegroundObjects(), deltaTime)
        || EntityUtils::isBlockedOnLeft(getJonP(), getExtraForegroundObjects(), deltaTime)
        || EntityUtils::isBlockedOnLeft(getJonP(), getMidBossForegroundObjects(), deltaTime)
        || EntityUtils::isBlockedOnLeft(getJonP(), getEndBossForegroundObjects(), deltaTime)
        || EntityUtils::isBlockedOnLeft(getJonP(), getForegroundCoverObjects(), deltaTime);
	}
    
    return false;
}

bool Game::isJonBlockedVertically(float deltaTime)
{
    if (EntityUtils::isFallingThroughPit(getJonP(), getPits(), deltaTime))
    {
        return false;
    }
    
    return EntityUtils::isBlockedAbove(getJon(), getGrounds(), deltaTime)
    || EntityUtils::isBlockedAbove(getJon(), getExitGrounds(), deltaTime)
    || EntityUtils::isBlockedAbove(getJon(), getForegroundObjects(), deltaTime)
    || EntityUtils::isBlockedAbove(getJon(), getExtraForegroundObjects(), deltaTime)
    || EntityUtils::isBlockedAbove(getJon(), getMidBossForegroundObjects(), deltaTime)
    || EntityUtils::isBlockedAbove(getJon(), getEndBossForegroundObjects(), deltaTime)
    || EntityUtils::isBlockedAbove(getJon(), getForegroundCoverObjects(), deltaTime)
    || EntityUtils::isBlockedAbove(getJon(), getEnemies(), deltaTime);
}

bool Game::isSpinningBackFistDelivered(float deltaTime)
{
    return EntityUtils::isHorizontallyHitting(getJon(), getEnemies(), deltaTime)
    || EntityUtils::isHorizontallyHitting(getJon(), getForegroundObjects(), deltaTime)
    || EntityUtils::isHorizontallyHitting(getJon(), getExtraForegroundObjects(), deltaTime)
    || EntityUtils::isHorizontallyHitting(getJon(), getMidBossForegroundObjects(), deltaTime)
    || EntityUtils::isHorizontallyHitting(getJon(), getEndBossForegroundObjects(), deltaTime);
}

bool Game::isBurrowEffective(float deltaTime)
{
	float originalY = getJon().getPosition().getY();

    bool ret = EntityUtils::isLanding(getJonP(), getGrounds(), deltaTime)
		&& EntityUtils::isBurrowingThroughHole(getJon(), getHoles());

	getJon().getPosition().setY(originalY);

	return ret;
}

bool Game::isUpwardThrustEffective(float deltaTime)
{
    return EntityUtils::isHorizontallyHitting(getJon(), getEnemies(), deltaTime)
    || EntityUtils::isHittingFromBelow(getJon(), getEnemies(), deltaTime)
    || EntityUtils::isHittingFromBelow(getJon(), getForegroundObjects(), deltaTime)
    || EntityUtils::isHittingFromBelow(getJon(), getExtraForegroundObjects(), deltaTime)
    || EntityUtils::isHittingFromBelow(getJon(), getMidBossForegroundObjects(), deltaTime)
    || EntityUtils::isHittingFromBelow(getJon(), getEndBossForegroundObjects(), deltaTime);
}

bool Game::isDashEffective(float deltaTime)
{
    return EntityUtils::isHorizontallyHitting(getJon(), getEnemies(), deltaTime);
}

std::vector<Background *>& Game::getBackgroundUppers()
{
    return m_backgroundUppers;
}

std::vector<Background *>& Game::getBackgroundMids()
{
    return m_backgroundMids;
}

std::vector<Background *>& Game::getBackgroundLowerBacks()
{
    return m_backgroundLowerBacks;
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

std::vector<Ground *>& Game::getPits()
{
    return m_pits;
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

std::vector<ForegroundObject *>& Game::getMidBossForegroundObjects()
{
    return m_midBossForegroundObjects;
}

std::vector<ForegroundObject *>& Game::getEndBossForegroundObjects()
{
    return m_endBossForegroundObjects;
}

std::vector<CountHissWithMina *>& Game::getCountHissWithMinas()
{
    return m_countHissWithMinas;
}

CountHissWithMina& Game::getCountHissWithMina()
{
    return *getCountHissWithMinas().at(0);
}

std::vector<EndBossSnake *>& Game::getEndBossSnakes()
{
    return m_endBossSnakes;
}

EndBossSnake& Game::getEndBossSnake()
{
    return *getEndBossSnakes().at(0);
}

EndBossSnake* Game::getEndBossSnakeP()
{
    return getEndBossSnakes().at(0);
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
    return *getJons().at(getJons().size() - 1);
}

Jon* Game::getJonP()
{
    return getJons().at(getJons().size() - 1);
}

std::vector<ExtraForegroundObject *>& Game::getExtraForegroundObjects()
{
    return m_extraForegroundObjects;
}

std::vector<ForegroundCoverObject *>& Game::getForegroundCoverObjects()
{
    return m_foregroundCoverObjects;
}

std::vector<GameMarker *>& Game::getMarkers()
{
    return m_markers;
}

void Game::setBestLevelStatsFlag(int bestLevelStatsFlag)
{
    m_iBestLevelStatsFlag = bestLevelStatsFlag;
    
    configureGoldenCarrots();
}

int Game::getBestLevelStatsFlag()
{
    return m_iBestLevelStatsFlag;
}

void Game::setCameraBounds(Rectangle* cameraBounds)
{
    m_cameraBounds = cameraBounds;
}

Rectangle* Game::getCameraBounds()
{
    return m_cameraBounds;
}

float Game::getFarRight()
{
    return m_fFarRight;
}

float Game::getFarRightBottom()
{
    return m_fFarRightBottom;
}

float Game::getCamFarRight()
{
    return m_fCamFarRight;
}

float Game::getCamFarRightBottom()
{
    return m_fCamFarRightBottom;
}

float Game::getStateTime()
{
    return m_fStateTime;
}

void Game::setNumCarrotsCollected(int numCarrotsCollected)
{
    m_iNumCarrotsCollected = numCarrotsCollected;
}

int Game::getNumCarrotsCollected()
{
    return m_iNumCarrotsCollected;
}

void Game::setNumGoldenCarrotsCollected(int numGoldenCarrotsCollected)
{
    m_iNumGoldenCarrotsCollected = numGoldenCarrotsCollected;
}

int Game::getNumGoldenCarrotsCollected()
{
    return m_iNumGoldenCarrotsCollected;
}

int Game::getWorld()
{
    return m_iWorld;
}

int Game::getLevel()
{
    return m_iLevel;
}

bool Game::isLoaded()
{
    return m_isLoaded;
}

bool Game::hasEndSign()
{
	for (std::vector<ForegroundObject *>::iterator i = m_foregroundObjects.begin(); i != m_foregroundObjects.end(); i++)
	{
		if ((*i)->getType() == ForegroundObjectType_EndSign)
		{
			return true;
		}
	}

	return false;
}

void Game::calcFarRight()
{
    bool isUsingEndBossCam = m_iLevel == 21;
    
    if (isUsingEndBossCam)
    {
        // End Boss Level
        CountHissWithMina* countHissWithMina = m_countHissWithMinas.at(0);
        m_fCamFarRight = countHissWithMina->getMainBounds().getRight() - CAM_WIDTH;
        m_fCamFarRightBottom = countHissWithMina->getMainBounds().getBottom() - 0.5625f;
    }
    
    for (std::vector<ForegroundObject *>::iterator i = m_foregroundObjects.begin(); i != m_foregroundObjects.end(); i++)
    {
        if ((*i)->getType() == ForegroundObjectType_EndSign)
        {
            m_fFarRight = (*i)->getMainBounds().getLeft();
            m_fFarRightBottom = (*i)->getMainBounds().getBottom() - 0.5625f;
            
            if (!isUsingEndBossCam)
            {
                m_fCamFarRight = m_fFarRight;
                m_fCamFarRightBottom = m_fFarRightBottom;
            }
            
            return;
        }
    }
}

void Game::setStateTime(float stateTime)
{
    m_fStateTime = stateTime;
}

#pragma mark private

void Game::onLoaded()
{
    if (m_iWorld == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            m_backgroundUppers.push_back(Background::create(i * CAM_WIDTH + CAM_WIDTH / 2, BackgroundType_Upper));
            
            m_backgroundMids.push_back(Background::create(i * CAM_WIDTH + CAM_WIDTH / 2, BackgroundType_Mid_Hills));
            m_backgroundMids.push_back(Background::create(i * CAM_WIDTH + CAM_WIDTH / 2, BackgroundType_Mid_Trees));
            
            m_backgroundLowerBacks.push_back(Background::create(i * CAM_WIDTH + CAM_WIDTH / 2, BackgroundType_Lower_Innermost));
            m_backgroundLowerBacks.push_back(Background::create(i * CAM_WIDTH + CAM_WIDTH / 2, BackgroundType_Lower_Inner));
            
            m_backgroundLowers.push_back(Background::create(i * CAM_WIDTH + CAM_WIDTH / 2, BackgroundType_Lower_Top));
            m_backgroundLowers.push_back(Background::create(i * CAM_WIDTH + CAM_WIDTH / 2, BackgroundType_Lower_Bottom));
            m_backgroundLowers.push_back(Background::create(i * CAM_WIDTH + CAM_WIDTH / 2, BackgroundType_WaterBack));
            
            m_backgroundMidgroundCovers.push_back(Background::create(i * CAM_WIDTH + CAM_WIDTH / 2, BackgroundType_WaterFront));
        }
    }
    
    EntityUtils::setGameToEntities(m_jons, this);
    EntityUtils::setGameToEntities(m_collectibleItems, this);
    EntityUtils::setGameToEntities(m_enemies, this);
    EntityUtils::setGameToEntities(m_foregroundObjects, this);
    EntityUtils::setGameToEntities(m_midBossForegroundObjects, this);
    EntityUtils::setGameToEntities(m_endBossForegroundObjects, this);
    EntityUtils::setGameToEntities(m_extraForegroundObjects, this);
    EntityUtils::setGameToEntities(m_endBossSnakes, this);
    
    configureGoldenCarrots();
    
    calcFarRight();
    
    m_isLoaded = true;
}

void Game::configureGoldenCarrots()
{
    int index = 0;
    for (std::vector<CollectibleItem *>::iterator i = getCollectibleItems().begin(); i != getCollectibleItems().end(); i++)
    {
        if ((*i)->getType() == CollectibleItemType_GoldenCarrot)
        {
            GoldenCarrot* gc = reinterpret_cast<GoldenCarrot *>((*i));
            gc->init(index++, m_iBestLevelStatsFlag);
        }
    }
}
