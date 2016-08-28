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
	EntityUtils::cleanUpVectorOfPointers(m_countHissWithMinas);
    EntityUtils::cleanUpVectorOfPointers(m_enemies);
    EntityUtils::cleanUpVectorOfPointers(m_collectibleItems);
    EntityUtils::cleanUpVectorOfPointers(m_jons);
    EntityUtils::cleanUpVectorOfPointers(m_extraForegroundObjects);
    
    if (world == 1)
    {
        m_midgrounds.push_back(new TreeOne(0));
        m_midgrounds.push_back(new TreeTwo(0));
        m_midgrounds.push_back(new TreeThree(0));
        
        if (level >= 10)
        {
            m_midgrounds.push_back(new DeepCaveColumnSmall(0));
            m_midgrounds.push_back(new DeepCaveColumnMedium(0));
            m_midgrounds.push_back(new DeepCaveColumnBig(0));
            
            m_grounds.push_back(new CaveExtraDeepEndLeft(0));
            m_grounds.push_back(new CaveExtraDeepSmall(0));
            m_grounds.push_back(new CaveExtraDeepMedium(0));
            m_grounds.push_back(new CaveExtraDeepLarge(0));
            m_grounds.push_back(new CaveExtraDeepEndRight(0));
            
            m_grounds.push_back(new CaveDeepEndLeft(0));
            m_grounds.push_back(new CaveDeepSmall(0));
            m_grounds.push_back(new CaveDeepMedium(0));
            m_grounds.push_back(new CaveDeepLarge(0));
            m_grounds.push_back(new CaveDeepEndRight(0));
            
            m_grounds.push_back(new CaveEndLeft(0));
            m_grounds.push_back(new CaveSmall(0));
            m_grounds.push_back(new CaveMedium(0));
            m_grounds.push_back(new CaveLarge(0));
            m_grounds.push_back(new CaveEndRight(0));
            
            m_grounds.push_back(new CaveRaisedEndLeft(0));
            m_grounds.push_back(new CaveRaisedSmall(0));
            m_grounds.push_back(new CaveRaisedMedium(0));
            m_grounds.push_back(new CaveRaisedLarge(0));
            m_grounds.push_back(new CaveRaisedEndRight(0));
            
            m_grounds.push_back(new GrassWithCaveEndLeft(0));
            m_grounds.push_back(new GrassWithCaveSmall(0));
            m_grounds.push_back(new GrassWithCaveMedium(0));
            m_grounds.push_back(new GrassWithCaveLarge(0));
            m_grounds.push_back(new GrassWithCaveEndRight(0));
        }
        
        m_grounds.push_back(new GrassWithoutCaveEndLeft(0));
        m_grounds.push_back(new GrassWithoutCaveSmall(0));
        m_grounds.push_back(new GrassWithoutCaveMedium(0));
        m_grounds.push_back(new GrassWithoutCaveLarge(0));
        m_grounds.push_back(new GrassWithoutCaveEndRight(0));
        
        if (level < 10)
        {
            m_pits.push_back(new GrassPitSmall(0));
            m_pits.push_back(new GrassPitMedium(0));
            m_pits.push_back(new GrassPitLarge(0));
            m_pits.push_back(new GrassPitExtraLarge(0));
        }
        
        if (level >= 10)
        {
            m_exitGrounds.push_back(new GrassWithCaveSmallExitMid(0));
            m_exitGrounds.push_back(new GrassWithCaveSmallExitEnd(0));
            m_exitGrounds.push_back(new CaveSmallExit(0));
            
            m_exitGrounds.push_back(new CaveDeepSmallWaterfall(0));
            
            m_holes.push_back(new HoleGrassTileLeft(0));
            m_holes.push_back(new HoleGrassTileCenter(0));
            m_holes.push_back(new HoleGrassTileRight(0));
            m_holes.push_back(new HoleGrass(0));
            m_holes.push_back(new HoleCave(0));
        }
        
        m_foregroundObjects.push_back(new GrassPlatformLeft(0, 0));
        m_foregroundObjects.push_back(new GrassPlatformCenter(0, 0));
        m_foregroundObjects.push_back(new GrassPlatformRight(0, 0));
        
        if (level >= 10)
        {
            m_foregroundObjects.push_back(new CavePlatformLeft(0, 0));
            m_foregroundObjects.push_back(new CavePlatformCenter(0, 0));
            m_foregroundObjects.push_back(new CavePlatformRight(0, 0));
        }
        
        m_foregroundObjects.push_back(new RockLarge(0, 0));
        m_foregroundObjects.push_back(new RockMedium(0, 0));
        m_foregroundObjects.push_back(new RockSmall(0, 0));
        m_foregroundObjects.push_back(new RockSmallCracked(0, 0));
        
        m_foregroundObjects.push_back(new StumpBig(0, 0));
        m_foregroundObjects.push_back(new StumpSmall(0, 0));
        
        m_foregroundObjects.push_back(new EndSign(0, 0));
        
        m_foregroundObjects.push_back(new ThornsLeft(0, 0));
        m_foregroundObjects.push_back(new ThornsCenterSmall(0, 0));
        m_foregroundObjects.push_back(new ThornsCenterBig(0, 0));
        m_foregroundObjects.push_back(new ThornsRight(0, 0));
        
        m_foregroundObjects.push_back(new LogVerticalTall(0, 0));
        m_foregroundObjects.push_back(new LogVerticalShort(0, 0));
        
        m_foregroundObjects.push_back(new JumpSpringLightFlush(0, 0));
        m_foregroundObjects.push_back(new JumpSpringLight(0, 0));
        m_foregroundObjects.push_back(new JumpSpringMedium(0, 0));
        m_foregroundObjects.push_back(new JumpSpringHeavy(0, 0));
        
        m_foregroundObjects.push_back(new SpikeGrassSingle(0, 0));
        m_foregroundObjects.push_back(new SpikeGrassFour(0, 0));
        m_foregroundObjects.push_back(new SpikeGrassEight(0, 0));
        
        if (level >= 10)
        {
            m_foregroundObjects.push_back(new SpikeCaveSingle(0, 0));
            m_foregroundObjects.push_back(new SpikeCaveFour(0, 0));
            m_foregroundObjects.push_back(new SpikeCaveEight(0, 0));
            
            m_foregroundObjects.push_back(new SpikeCaveCeilingSingle(0, 0));
            m_foregroundObjects.push_back(new SpikeCaveCeilingFour(0, 0));
            m_foregroundObjects.push_back(new SpikeCaveCeilingEight(0, 0));
        }
        
        m_foregroundObjects.push_back(new SpikeWallSingle(0, 0));
        m_foregroundObjects.push_back(new SpikeWallFour(0, 0));
        m_foregroundObjects.push_back(new SpikeWallEight(0, 0));
        
        m_foregroundObjects.push_back(new SpikeStar(0, 0));
        
        m_foregroundObjects.push_back(new VerticalSaw(0));
        
        if (level == 10)
        {
            m_midBossForegroundObjects.push_back(new GiantShakingTree(0, 0));
            m_midBossForegroundObjects.push_back(new GiantPerchTree(0, 0));
        }
        else if (level > 10)
        {
            m_foregroundObjects.push_back(new GiantTree(0, 0));
        }
        
        m_countHissWithMinas.push_back(new CountHissWithMina(0, 0));
        
        m_enemies.push_back(new BigMushroomGround(0, 0));
        
        if (level >= 10)
        {
            m_enemies.push_back(new BigMushroomCeiling(0, 0));
        }
        
        m_enemies.push_back(new MovingSnakeGruntV1(0, 0));
        m_enemies.push_back(new MovingSnakeGruntV2(0, 0));
        m_enemies.push_back(new MovingSnakeGruntV3(0, 0));
        m_enemies.push_back(new MovingSnakeGruntV4(0, 0));
        m_enemies.push_back(new MovingSnakeGruntV5(0, 0));
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
        
        m_extraForegroundObjects.push_back(new SpikeTower(0));
    }
    
    m_jons.push_back(new Jon(0, 0));
    
    float eWidth = m_fWidth * 0.6f;
    float eHeight = m_fHeight / 6;
    float eX = CAM_WIDTH - m_fWidth / 2 + 0.4f;
    float eY = eHeight / 2;
    
    int i = boxInAll(m_midgrounds, eX, eY, eWidth, eHeight, 0);
    i = boxInAll(m_grounds, eX, eY, eWidth, eHeight, i);
    i = boxInAll(m_pits, eX, eY, eWidth, eHeight, i);
    i = boxInAll(m_exitGrounds, eX, eY, eWidth, eHeight, i);
    i = boxInAll(m_holes, eX, eY, eWidth, eHeight, i);
    i = boxInAll(m_foregroundObjects, eX, eY, eWidth, eHeight, i);
    i = boxInAll(m_midBossForegroundObjects, eX, eY, eWidth, eHeight, i);
	i = boxInAll(m_countHissWithMinas, eX, eY, eWidth, eHeight, i);
    i = boxInAll(m_enemies, eX, eY, eWidth, eHeight, i);
    i = boxInAll(m_collectibleItems, eX, eY, eWidth, eHeight, i);
    i = boxInAll(m_jons, eX, eY, eWidth, eHeight, i);
    i = boxInAll(m_extraForegroundObjects, eX, eY, eWidth, eHeight, i);
    
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
                        || isTouchingEntityForPlacement(m_countHissWithMinas, game.getCountHissWithMinas(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_enemies, game.getEnemies(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_collectibleItems, game.getCollectibleItems(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_jons, game.getJons(), gridX, gridY, lastAddedEntity, touchPoint)
                        || isTouchingEntityForPlacement(m_extraForegroundObjects, game.getExtraForegroundObjects(), gridX, gridY, lastAddedEntity, touchPoint))
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

std::vector<CountHissWithMina *>& LevelEditorEntitiesPanel::getCountHissWithMinas()
{
	return m_countHissWithMinas;
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

float LevelEditorEntitiesPanel::getEntitiesCameraPos()
{
    return m_fEntitiesCameraPos;
}

bool LevelEditorEntitiesPanel::isOpen()
{
    return m_isOpen;
}