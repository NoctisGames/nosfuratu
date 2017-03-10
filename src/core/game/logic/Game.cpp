//
//  Game.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "Game.h"

#include "Vector2D.h"
#include "PhysicalEntity.h"
#include "Background.h"
#include "Midground.h"
#include "Ground.h"
#include "ExitGround.h"
#include "Hole.h"
#include "ForegroundObject.h"
#include "CountHissWithMina.h"
#include "EndBossSnake.h"
#include "Enemy.h"
#include "CollectibleItem.h"
#include "Jon.h"
#include "ForegroundCoverObject.h"
#include "GameMarker.h"
#include "NGRect.h"

#include "GameConstants.h"
#include "EntityUtils.h"
#include "OverlapTester.h"
#include "Assets.h"
#include "GridManager.h"
#include "NGSTDUtil.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

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
m_iNumVialsCollected(0),
m_iWorld(1),
m_iLevel(1),
m_isLevelEditor(false)
{
    GRID_MANAGER->setGridCellSize(GRID_CELL_SIZE);
}

Game::~Game()
{
    reset();
}

void Game::copy(Game* game)
{
    reset();
    
    m_iWorld = game->getWorld();
    m_iLevel = game->getLevel();
    m_isLevelEditor = game->isLevelEditor();
    
    EntityUtils::copyPhysicalEntities(game->getMidgrounds(), m_midgrounds);
    EntityUtils::copyPhysicalEntities(game->getGrounds(), m_grounds);
    EntityUtils::copyPhysicalEntities(game->getPits(), m_pits);
    EntityUtils::copyPhysicalEntities(game->getExitGrounds(), m_exitGrounds);
    EntityUtils::copyPhysicalEntities(game->getHoles(), m_holes);
    EntityUtils::copyPhysicalEntities(game->getForegroundObjects(), m_foregroundObjects);
    EntityUtils::copyPhysicalEntities(game->getMidBossForegroundObjects(), m_midBossForegroundObjects);
    EntityUtils::copyPhysicalEntities(game->getEndBossForegroundObjects(), m_endBossForegroundObjects);
    EntityUtils::copyPhysicalEntities(game->getCountHissWithMinas(), m_countHissWithMinas);
    EntityUtils::copyPhysicalEntities(game->getEndBossSnakes(), m_endBossSnakes);
    EntityUtils::copyPhysicalEntities(game->getEnemies(), m_enemies);
    EntityUtils::copyPhysicalEntities(game->getCollectibleItems(), m_collectibleItems);
    EntityUtils::copyPhysicalEntities(game->getJons(), m_jons);
    EntityUtils::copyPhysicalEntities(game->getExtraForegroundObjects(), m_extraForegroundObjects);
    EntityUtils::copyPhysicalEntities(game->getForegroundCoverObjects(), m_foregroundCoverObjects);
    
    EntityUtils::copyPhysicalEntities(game->getMarkers(), m_markers);
    
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
    
    EntityUtils::loadArray(m_midgrounds, d, midgroundsKey);
    EntityUtils::loadArray(m_grounds, d, groundsKey);
    EntityUtils::loadArray(m_pits, d, pitsKey);
    EntityUtils::loadArray(m_exitGrounds, d, exitGroundsKey);
    EntityUtils::loadArray(m_holes, d, holesKey);
    EntityUtils::loadArray(m_foregroundObjects, d, foregroundObjectsKey);
    EntityUtils::loadArray(m_midBossForegroundObjects, d, midBossForegroundObjectsKey);
    EntityUtils::loadArray(m_endBossForegroundObjects, d, endBossForegroundObjectsKey);
    EntityUtils::loadArray(m_countHissWithMinas, d, countHissWithMinasKey);
    EntityUtils::loadArray(m_endBossSnakes, d, endBossSnakesKey);
    EntityUtils::loadArray(m_enemies, d, enemiesKey);
    EntityUtils::loadArray(m_collectibleItems, d, collectiblesKey);
    EntityUtils::loadArray(m_jons, d, jonsKey);
    EntityUtils::loadArray(m_extraForegroundObjects, d, extraForegroundObjectsKey);
    EntityUtils::loadArray(m_foregroundCoverObjects, d, foregroundCoverObjectsKey);
    
    EntityUtils::loadArray(m_markers, d, markersKey);
    
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
    
    EntityUtils::saveArray(m_midgrounds, w, midgroundsKey);
    EntityUtils::saveArray(m_grounds, w, groundsKey);
    EntityUtils::saveArray(m_pits, w, pitsKey);
    EntityUtils::saveArray(m_exitGrounds, w, exitGroundsKey);
    EntityUtils::saveArray(m_holes, w, holesKey);
    EntityUtils::saveArray(m_foregroundObjects, w, foregroundObjectsKey);
    EntityUtils::saveArray(m_midBossForegroundObjects, w, midBossForegroundObjectsKey);
    EntityUtils::saveArray(m_endBossForegroundObjects, w, endBossForegroundObjectsKey);
    EntityUtils::saveArray(m_countHissWithMinas, w, countHissWithMinasKey);
    EntityUtils::saveArray(m_endBossSnakes, w, endBossSnakesKey);
    EntityUtils::saveArray(m_enemies, w, enemiesKey);
    EntityUtils::saveArray(m_collectibleItems, w, collectiblesKey);
    EntityUtils::saveArray(m_jons, w, jonsKey);
    EntityUtils::saveArray(m_extraForegroundObjects, w, extraForegroundObjectsKey);
    EntityUtils::saveArray(m_foregroundCoverObjects, w, foregroundCoverObjectsKey);
    
    EntityUtils::saveArray(m_markers, w, markersKey);
    
    w.EndObject();
    
    return s.GetString();
}

void Game::reset()
{
    NGSTDUtil::cleanUpVectorOfPointers(m_backgroundUppers);
    NGSTDUtil::cleanUpVectorOfPointers(m_backgroundMids);
    NGSTDUtil::cleanUpVectorOfPointers(m_backgroundLowers);
    NGSTDUtil::cleanUpVectorOfPointers(m_backgroundLowers);
    NGSTDUtil::cleanUpVectorOfPointers(m_backgroundMidgroundCovers);
    
    NGSTDUtil::cleanUpVectorOfPointers(m_midgrounds);
    NGSTDUtil::cleanUpVectorOfPointers(m_grounds);
    NGSTDUtil::cleanUpVectorOfPointers(m_pits);
    NGSTDUtil::cleanUpVectorOfPointers(m_exitGrounds);
    NGSTDUtil::cleanUpVectorOfPointers(m_holes);
    NGSTDUtil::cleanUpVectorOfPointers(m_foregroundObjects);
    NGSTDUtil::cleanUpVectorOfPointers(m_midBossForegroundObjects);
    NGSTDUtil::cleanUpVectorOfPointers(m_endBossForegroundObjects);
    NGSTDUtil::cleanUpVectorOfPointers(m_countHissWithMinas);
    NGSTDUtil::cleanUpVectorOfPointers(m_endBossSnakes);
    NGSTDUtil::cleanUpVectorOfPointers(m_enemies);
    NGSTDUtil::cleanUpVectorOfPointers(m_collectibleItems);
    NGSTDUtil::cleanUpVectorOfPointers(m_jons);
    NGSTDUtil::cleanUpVectorOfPointers(m_extraForegroundObjects);
    NGSTDUtil::cleanUpVectorOfPointers(m_foregroundCoverObjects);
    
    NGSTDUtil::cleanUpVectorOfPointers(m_markers);
    
    m_fStateTime = 0;
    m_iNumCarrotsCollected = 0;
    m_iNumGoldenCarrotsCollected = 0;
    m_iNumVialsCollected = 0;
}

void Game::update(float deltaTime)
{
	m_fStateTime += deltaTime;
}

void Game::updateAndClean(float deltaTime, bool onlyJonCollectiblesAndCountHiss)
{
    if (onlyJonCollectiblesAndCountHiss)
    {
        EntityUtils::updateAndClean(getCountHissWithMinas(), deltaTime);
        EntityUtils::updateAndClean(getCollectibleItems(), deltaTime);
        
        if (getJons().size() > 0)
        {
            getJon().update(deltaTime);
        }
        
        return;
    }
    
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
		return EntityUtils::isLanding(entity, getPits(), deltaTime)
        || EntityUtils::isLanding(entity, getForegroundObjects(), deltaTime)
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
        return EntityUtils::isBlockedAbove(getJon(), getPits(), deltaTime);
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

void Game::setCameraBounds(NGRect* cameraBounds)
{
    m_cameraBounds = cameraBounds;
}

NGRect* Game::getCameraBounds()
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

void Game::setNumVialsCollected(int numVialsCollected)
{
    m_iNumVialsCollected = numVialsCollected;
}

int Game::getNumVialsCollected()
{
    return m_iNumVialsCollected;
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
    return m_backgroundUppers.size() > 0;
}

bool Game::hasEndSign()
{
	for (std::vector<ForegroundObject *>::iterator i = m_foregroundObjects.begin(); i != m_foregroundObjects.end(); ++i)
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
    
    if (isUsingEndBossCam
        && m_countHissWithMinas.size() > 0)
    {
        // End Boss Level
        CountHissWithMina* countHissWithMina = m_countHissWithMinas.at(0);
        m_fCamFarRight = countHissWithMina->getMainBounds().getRight() - CAM_WIDTH;
        m_fCamFarRightBottom = countHissWithMina->getMainBounds().getBottom() - 0.5625f;
    }
    
    for (std::vector<ForegroundObject *>::iterator i = m_foregroundObjects.begin(); i != m_foregroundObjects.end(); ++i)
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

void Game::setIsLevelEditor(bool isLevelEditor)
{
    m_isLevelEditor = isLevelEditor;
}

bool Game::isLevelEditor()
{
    return m_isLevelEditor;
}

#pragma mark private

void Game::onLoaded()
{
    if (m_iWorld == 1)
    {
        int numBgs = m_isLevelEditor ? 4 : 1;
        for (int i = 0; i < numBgs; i++)
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
    
    EntityUtils::setGameToEntities(m_grounds, this);
    EntityUtils::setGameToEntities(m_pits, this);
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
}

void Game::configureGoldenCarrots()
{
    int index = 0;
    for (std::vector<CollectibleItem *>::iterator i = getCollectibleItems().begin(); i != getCollectibleItems().end(); ++i)
    {
        if ((*i)->getType() == CollectibleItemType_GoldenCarrot)
        {
            GoldenCarrot* gc = reinterpret_cast<GoldenCarrot *>((*i));
            gc->init(index++, m_iBestLevelStatsFlag);
        }
    }
}
