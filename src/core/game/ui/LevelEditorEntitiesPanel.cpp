//
//  LevelEditorEntitiesPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "LevelEditorEntitiesPanel.h"

#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "Game.h"
#include "EntityUtils.h"

LevelEditorEntitiesPanel::LevelEditorEntitiesPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_isOpen(false), m_fEntitiesCameraPos(0), m_fEntitiesHeight(0)
{
    m_openButton = std::unique_ptr<Rectangle>(new Rectangle(CAM_WIDTH - 1, height * 0.49081920903955f, 1, 1));
    m_closeButton = std::unique_ptr<Rectangle>(new Rectangle(CAM_WIDTH - width, height * 0.49081920903955f, 1, 1));
    m_touchPointDown = std::unique_ptr<Vector2D>(new Vector2D());
    m_touchPointDown2 = std::unique_ptr<Vector2D>(new Vector2D());
}

void LevelEditorEntitiesPanel::initForLevel(int world, int level)
{
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
    
    if (world == 1)
    {
        m_midgrounds.push_back(Midground::create(0, 0, MidgroundType_TreeOne));
        m_midgrounds.push_back(Midground::create(0, 0, MidgroundType_TreeTwo));
        
        if (level >= 10)
        {
            m_midgrounds.push_back(Midground::create(0, 0, MidgroundType_DeepCaveColumnSmall));
            m_midgrounds.push_back(Midground::create(0, 0, MidgroundType_DeepCaveColumnMedium));
            m_midgrounds.push_back(Midground::create(0, 0, MidgroundType_DeepCaveColumnBig));
            
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveExtraDeepEndLeft));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveExtraDeepSmall));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveExtraDeepMedium));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveExtraDeepLarge));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveExtraDeepEndRight));
            
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveDeepEndLeft));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveDeepSmall));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveDeepMedium));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveDeepLarge));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveDeepEndRight));
            
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveEndLeft));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveSmall));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveMedium));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveLarge));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveEndRight));
            
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveRaisedEndLeft));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveRaisedSmall));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveRaisedMedium));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveRaisedLarge));
            m_grounds.push_back(Ground::create(0, 0, GroundType_CaveRaisedEndRight));
            
            m_grounds.push_back(Ground::create(0, 0, GroundType_GrassWithCaveEndLeft));
            m_grounds.push_back(Ground::create(0, 0, GroundType_GrassWithCaveSmall));
            m_grounds.push_back(Ground::create(0, 0, GroundType_GrassWithCaveMedium));
            m_grounds.push_back(Ground::create(0, 0, GroundType_GrassWithCaveLarge));
            m_grounds.push_back(Ground::create(0, 0, GroundType_GrassWithCaveEndRight));
        }
        
        m_grounds.push_back(Ground::create(0, 0, GroundType_GrassWithoutCaveEndLeft));
        m_grounds.push_back(Ground::create(0, 0, GroundType_GrassWithoutCaveSmall));
        m_grounds.push_back(Ground::create(0, 0, GroundType_GrassWithoutCaveMedium));
        m_grounds.push_back(Ground::create(0, 0, GroundType_GrassWithoutCaveLarge));
        m_grounds.push_back(Ground::create(0, 0, GroundType_GrassWithoutCaveEndRight));
        
        if (level < 10)
        {
            m_pits.push_back(Ground::create(0, 0, GroundType_GrassPitSmall));
            m_pits.push_back(Ground::create(0, 0, GroundType_GrassPitMedium));
            m_pits.push_back(Ground::create(0, 0, GroundType_GrassPitLarge));
            m_pits.push_back(Ground::create(0, 0, GroundType_GrassPitExtraLarge));
        }
        
        if (level >= 10)
        {
            m_exitGrounds.push_back(ExitGround::create(0, 0, ExitGroundType_GrassWithCaveSmallExitMid));
            m_exitGrounds.push_back(ExitGround::create(0, 0, ExitGroundType_GrassWithCaveSmallExitEnd));
            m_exitGrounds.push_back(ExitGround::create(0, 0, ExitGroundType_CaveSmallExit));
            
            m_exitGrounds.push_back(ExitGround::create(0, 0, ExitGroundType_CaveDeepSmallWaterfall));
            
            m_holes.push_back(Hole::create(0, 0, HoleType_GrassTileLeft));
            m_holes.push_back(Hole::create(0, 0, HoleType_GrassTileCenter));
            m_holes.push_back(Hole::create(0, 0, HoleType_GrassTileRight));
            m_holes.push_back(Hole::create(0, 0, HoleType_Grass));
            m_holes.push_back(Hole::create(0, 0, HoleType_Cave));
        }
        
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_GrassPlatformLeft));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_GrassPlatformCenter));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_GrassPlatformRight));
		m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_CavePlatformLeft));
		m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_CavePlatformCenter));
		m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_CavePlatformRight));
        
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_RockLarge));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_RockMedium));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_RockSmall));
        
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_StumpBig));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_StumpSmall));
        
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_EndSign));
        
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_ThornsLeft));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_ThornsCenterBig));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_ThornsRight));
        
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_JumpSpringLightFlush));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_JumpSpringLight));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_JumpSpringMedium));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_JumpSpringHeavy));
        
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeGrassSingle));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeGrassFour));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeGrassEight));
        
        if (level >= 10)
        {
            m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeCaveSingle));
            m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeCaveFour));
            m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeCaveEight));
            
            m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeCaveCeilingSingle));
            m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeCaveCeilingFour));
            m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeCaveCeilingEight));
        }
        
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeWallSingle));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeWallFour));
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeWallEight));
        
        m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikeStar));
        
        if (level == 10)
        {
            m_midBossForegroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_GiantShakingTree));
            m_midBossForegroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_GiantPerchTree));
        }
        else if (level > 10)
        {
            m_foregroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_GiantTree));
        }
        
        if (level == 21)
        {
            m_endBossForegroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikedBall));
            m_endBossForegroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikedBallChain));
            m_endBossForegroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikedBallRollingLeft));
            m_endBossForegroundObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_SpikedBallRollingRight));
        }
        
        m_countHissWithMinas.push_back(CountHissWithMina::create(0, 0, -1));
        
		if (level == 21)
		{
			m_endBossSnakes.push_back(new EndBossSnake(0, 0));
		}
        
        m_enemies.push_back(new BigMushroomGround(0, 0));
        
        if (level >= 10)
        {
            m_enemies.push_back(new BigMushroomCeiling(0, 0));
        }
        
        m_enemies.push_back(Enemy::create(0, 0, EnemyType_MovingSnakeGruntV1));
        m_enemies.push_back(Enemy::create(0, 0, EnemyType_MovingSnakeGruntV2));
        m_enemies.push_back(Enemy::create(0, 0, EnemyType_MovingSnakeGruntV3));
        m_enemies.push_back(Enemy::create(0, 0, EnemyType_MovingSnakeGruntV4));
        m_enemies.push_back(Enemy::create(0, 0, EnemyType_MovingSnakeGruntV5));
        m_enemies.push_back(new Sparrow(0, 0));
        m_enemies.push_back(new Toad(0, 0));
        
        if (level >= 10)
        {
            m_enemies.push_back(new Fox(0, 0));
        }

		if (level == 10 || level == 21)
		{
			m_enemies.clear();
		}
        
        m_collectibleItems.push_back(new Carrot(0, 0));
        m_collectibleItems.push_back(new GoldenCarrot(0, 0));
        
        m_foregroundCoverObjects.push_back(ForegroundObject::create(0, 0, ForegroundObjectType_VerticalSaw));
        
        m_extraForegroundObjects.push_back(ExtraForegroundObject::create(0, 0, ForegroundObjectType_SpikeTower));
    }
    
    m_jons.push_back(new Jon(0, 0));
    
    float eWidth = m_fWidth * 0.6f;
    float eHeight = m_fHeight / 6;
    float eX = CAM_WIDTH - m_fWidth / 2 + 0.4f;
    float eY = eHeight / 2;
    
    int i = EntityUtils::boxInAll(m_midgrounds, eX, eY, eWidth, eHeight, 0);
    i = EntityUtils::boxInAll(m_grounds, eX, eY, eWidth, eHeight, i);
    i = EntityUtils::boxInAll(m_pits, eX, eY, eWidth, eHeight, i);
    i = EntityUtils::boxInAll(m_exitGrounds, eX, eY, eWidth, eHeight, i);
    i = EntityUtils::boxInAll(m_holes, eX, eY, eWidth, eHeight, i);
    i = EntityUtils::boxInAll(m_foregroundObjects, eX, eY, eWidth, eHeight, i);
    i = EntityUtils::boxInAll(m_midBossForegroundObjects, eX, eY, eWidth, eHeight, i);
    i = EntityUtils::boxInAll(m_endBossForegroundObjects, eX, eY, eWidth, eHeight, i);
	i = EntityUtils::boxInAll(m_countHissWithMinas, eX, eY, eWidth, eHeight, i);
    i = EntityUtils::boxInAll(m_endBossSnakes, eX, eY, eWidth, eHeight, i);
    i = EntityUtils::boxInAll(m_enemies, eX, eY, eWidth, eHeight, i);
    i = EntityUtils::boxInAll(m_collectibleItems, eX, eY, eWidth, eHeight, i);
    i = EntityUtils::boxInAll(m_jons, eX, eY, eWidth, eHeight, i);
    i = EntityUtils::boxInAll(m_extraForegroundObjects, eX, eY, eWidth, eHeight, i);
    i = EntityUtils::boxInAll(m_foregroundCoverObjects, eX, eY, eWidth, eHeight, i);
    
    m_fEntitiesHeight = fmaxf((i * eHeight), m_fHeight);
    
    if (m_fEntitiesCameraPos > (m_fEntitiesHeight - getHeight()))
    {
        m_fEntitiesCameraPos = m_fEntitiesHeight - getHeight();
    }
}

int LevelEditorEntitiesPanel::handleTouch(TouchEvent& te, Vector2D& touchPoint, Game& game, Vector2D& camPos, PhysicalEntity** lastAddedEntity)
{
    if (m_isOpen && touchPoint.getX() > (CAM_WIDTH - getWidth()))
    {
        switch (te.getTouchType())
        {
            case DOWN:
            {
                if(touchPoint.getX() > m_closeButton->getLowerLeft().getX() + m_closeButton->getWidth())
                {
                    m_touchPointDown->set(touchPoint.getX(), touchPoint.getY());
                    m_touchPointDown2->set(touchPoint.getX(), touchPoint.getY());
                    
                    return LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED;
                }
            }
                break;
            case DRAGGED:
                m_fEntitiesCameraPos += (m_touchPointDown->getY() - touchPoint.getY()) * 3;
                if (m_fEntitiesCameraPos < 0)
                {
                    m_fEntitiesCameraPos = 0;
                }
                else if (m_fEntitiesCameraPos > (m_fEntitiesHeight - getHeight()))
                {
                    m_fEntitiesCameraPos = m_fEntitiesHeight - getHeight();
                }
                
                m_touchPointDown->set(touchPoint.getX(), touchPoint.getY());
                
                return LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED;
            case UP:
            {
                if (OverlapTester::isPointInRectangle(touchPoint, *m_closeButton))
                {
                    m_position->setX(CAM_WIDTH + (3.76608187134503f - 1.894736842105263f) / 2);
                    
                    m_isOpen = false;
                    
                    return LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED;
                }
                else if (touchPoint.getY() < m_touchPointDown2->getY() + 0.25f && touchPoint.getY() > m_touchPointDown2->getY() - 0.25f)
                {
                    touchPoint.add(0, m_fEntitiesCameraPos);
                    
                    int gridX = (camPos.getX() + ZOOMED_OUT_CAM_WIDTH / 2) / GRID_CELL_SIZE;
                    int gridY = GAME_HEIGHT / 2 / GRID_CELL_SIZE;
                    
                    if (isTouchingEntityForPlacement(m_midgrounds, game.getMidgrounds(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_grounds, game.getGrounds(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_pits, game.getPits(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_exitGrounds, game.getExitGrounds(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_holes, game.getHoles(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_foregroundObjects, game.getForegroundObjects(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_midBossForegroundObjects, game.getMidBossForegroundObjects(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_endBossForegroundObjects, game.getEndBossForegroundObjects(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_countHissWithMinas, game.getCountHissWithMinas(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_endBossSnakes, game.getEndBossSnakes(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_enemies, game.getEnemies(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_collectibleItems, game.getCollectibleItems(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_jons, game.getJons(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_extraForegroundObjects, game.getExtraForegroundObjects(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_foregroundCoverObjects, game.getForegroundCoverObjects(), gridX, gridY, lastAddedEntity, touchPoint))
                    {
                        return LEVEL_EDITOR_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                }
            }
        }
    }
    else
    {
		if (OverlapTester::isPointInRectangle(touchPoint, *m_openButton))
		{
			if (te.getTouchType() == UP)
			{
				m_position->setX(CAM_WIDTH - getWidth() / 2);

				m_isOpen = true;
			}

			return LEVEL_EDITOR_ENTITIES_PANEL_RC_HANDLED;
		}
    }
    
    return LEVEL_EDITOR_ENTITIES_PANEL_RC_UNHANDLED;
}

std::vector<Midground *>& LevelEditorEntitiesPanel::getMidgrounds()
{
    return m_midgrounds;
}

std::vector<Ground *>& LevelEditorEntitiesPanel::getGrounds()
{
    return m_grounds;
}

std::vector<Ground *>& LevelEditorEntitiesPanel::getPits()
{
    return m_pits;
}

std::vector<ExitGround *>& LevelEditorEntitiesPanel::getExitGrounds()
{
    return m_exitGrounds;
}

std::vector<Hole *>& LevelEditorEntitiesPanel::getHoles()
{
    return m_holes;
}

std::vector<ForegroundObject *>& LevelEditorEntitiesPanel::getForegroundObjects()
{
    return m_foregroundObjects;
}

std::vector<ForegroundObject *>& LevelEditorEntitiesPanel::getMidBossForegroundObjects()
{
    return m_midBossForegroundObjects;
}

std::vector<ForegroundObject *>& LevelEditorEntitiesPanel::getEndBossForegroundObjects()
{
    return m_endBossForegroundObjects;
}

std::vector<CountHissWithMina *>& LevelEditorEntitiesPanel::getCountHissWithMinas()
{
	return m_countHissWithMinas;
}

std::vector<EndBossSnake *>& LevelEditorEntitiesPanel::getEndBossSnakes()
{
    return m_endBossSnakes;
}

std::vector<Enemy *>& LevelEditorEntitiesPanel::getEnemies()
{
    return m_enemies;
}

std::vector<CollectibleItem *>& LevelEditorEntitiesPanel::getCollectibleItems()
{
    return m_collectibleItems;
}

std::vector<Jon *>& LevelEditorEntitiesPanel::getJons()
{
    return m_jons;
}

std::vector<ExtraForegroundObject *>& LevelEditorEntitiesPanel::getExtraForegroundObjects()
{
    return m_extraForegroundObjects;
}

std::vector<ForegroundObject *>& LevelEditorEntitiesPanel::getForegroundCoverObjects()
{
    return m_foregroundCoverObjects;
}

float LevelEditorEntitiesPanel::getEntitiesCameraPos()
{
    return m_fEntitiesCameraPos;
}

bool LevelEditorEntitiesPanel::isOpen()
{
    return m_isOpen;
}

RTTI_IMPL(LevelEditorEntitiesPanel, PhysicalEntity);
