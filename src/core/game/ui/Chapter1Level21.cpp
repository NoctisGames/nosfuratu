//
//  Chapter1Level21.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/16/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameScreenLevels.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"

void Chapter1Level21::enter(GameScreen* gs)
{
    Level::enter(gs);
    
    for (std::vector<Hole*>::iterator i = m_game->getHoles().begin(); i != m_game->getHoles().end(); i++)
    {
        if ((*i)->getType() == HoleType_Grass)
        {
            m_hole = (*i);
            break;
        }
    }

	SpikedBall* spikedBall1 = nullptr;
	SpikedBall* spikedBall2 = nullptr;
	SpikedBall* spikedBall3 = nullptr;
	SpikedBallChain * spikedBallChain1 = nullptr;
	SpikedBallChain* spikedBallChain2 = nullptr;
	SpikedBallChain* spikedBallChain3 = nullptr;

	int spikedBallIndex = 0;
	int spikedBallChainIndex = 0;
	for (std::vector<ForegroundObject*>::iterator i = m_game->getEndBossForegroundObjects().begin(); i != m_game->getEndBossForegroundObjects().end(); i++)
	{
		if ((*i)->getType() == ForegroundObjectType_SpikedBall)
		{
			spikedBallIndex++;
			switch (spikedBallIndex)
			{
				case 1:
					spikedBall1 = dynamic_cast<SpikedBall*>((*i));
					break;
				case 2:
					spikedBall2 = dynamic_cast<SpikedBall*>((*i));
					break;
				case 3:
					spikedBall3 = dynamic_cast<SpikedBall*>((*i));
					break;
			}
		}
		else if ((*i)->getType() == ForegroundObjectType_SpikedBallChain)
		{
			spikedBallChainIndex++;
			switch (spikedBallChainIndex)
			{
			case 1:
				spikedBallChain1 = dynamic_cast<SpikedBallChain*>((*i));
				break;
			case 2:
				spikedBallChain2 = dynamic_cast<SpikedBallChain*>((*i));
				break;
			case 3:
				spikedBallChain3 = dynamic_cast<SpikedBallChain*>((*i));
				break;
			}
		}
	}

	if (spikedBallChain1 && spikedBall1)
	{
		spikedBallChain1->setSpikedBall(spikedBall1);
	}

	if (spikedBallChain2 && spikedBall2)
	{
		spikedBallChain2->setSpikedBall(spikedBall2);
	}

	if (spikedBallChain3 && spikedBall3)
	{
		spikedBallChain3->setSpikedBall(spikedBall3);
	}

	GiantTree* giantTree1 = nullptr;
	GiantTree* giantTree2 = nullptr;
	int giantTreeIndex = 0;
	for (std::vector<ForegroundObject*>::iterator i = m_game->getForegroundObjects().begin(); i != m_game->getForegroundObjects().end(); i++)
	{
		if ((*i)->getType() == ForegroundObjectType_GiantTree)
		{
			giantTreeIndex++;
			switch (giantTreeIndex)
			{
			case 1:
				giantTree1 = dynamic_cast<GiantTree*>((*i));
				break;
			case 2:
				giantTree2 = dynamic_cast<GiantTree*>((*i));
				break;
			}
		}
	}
	
	if (giantTree1)
	{
		m_fMarker1X = giantTree1->getPosition().getX();
	}

	if (giantTree2)
	{
		m_fMarker2X = giantTree2->getPosition().getX();
	}
    
    if (m_game->getEndBossSnakes().size() > 0)
    {
        m_endBossSnake = m_game->getEndBossSnakeP();
    }
    
    m_game->getCountHissWithMina().faceLeft();

	Jon& jon = m_game->getJon(); 
	if (m_hasTriggeredSnakeDeathCheckPoint)
	{
		m_game->setStateTime(m_fCheckPointStateTime);
		m_game->setNumCarrotsCollected(m_iNumCarrotsCollectedAtCheckpoint);
		m_game->setNumGoldenCarrotsCollected(m_iNumGoldenCarrotsCollectedAtCheckpoint);

		if (spikedBallChain3)
		{
			spikedBallChain3->trigger();
		}

		jon.getPosition().set(m_fCheckPointX, m_fCheckPointY);
		jon.becomeVampire();
		jon.flash();

		if (jon.isIdle())
		{
			jon.setIdle(false);
		}

		if (!FlagUtil::isFlagSet(m_iBestLevelStatsFlag, FLAG_LEVEL_COMPLETE))
		{
			m_batPanel->config(m_game.get(), BatGoalType_Dash);
		}

		m_endBossSnake->checkPointKill();

		for (std::vector<CollectibleItem*>::iterator i = m_game->getCollectibleItems().begin(); i != m_game->getCollectibleItems().end(); )
		{
			if ((*i)->getPosition().getX() < jon.getPosition().getX())
			{
				(*i)->onDeletion();

				delete *i;
				i = m_game->getCollectibleItems().erase(i);
			}
			else
			{
				i++;
			}
		}
	}
	else if (m_hasTriggeredCheckPoint)
	{
		jon.getPosition().set(m_fCheckPointX, m_fCheckPointY);

		m_game->setStateTime(m_fCheckPointStateTime);

		jon.setUserActionPrevented(false);
		jon.becomeVampire();

		if (m_endBossSnake)
		{
			m_endBossSnake->beginPursuit();
		}
	}
}

void Chapter1Level21::exit(GameScreen* gs)
{
	m_endBossSnake = nullptr;
    m_hole = nullptr;

	m_fGameStateTime = 0;
	m_fCheckPointX = 0;
	m_fCheckPointY = 0;
	m_fMarker1X = 0;
	m_fMarker2X = 0;
	m_fMusicVolume = 0.5f;
	m_fCheckPointStateTime = 0;
	m_iNumCarrotsCollectedAtCheckpoint = 0;
	m_iNumGoldenCarrotsCollectedAtCheckpoint = 0;
	m_isChaseCamActivated = false;
	m_hasTriggeredMusicLoopIntro = false;
	m_hasTriggeredSnakeAwaken = false;
	m_hasTriggeredMusicLoop = false;
	m_hasTriggeredCheckPoint = false;
	m_hasTriggeredSnakeDeathCheckPoint = false;
    
    Level::exit(gs);
}

void Chapter1Level21::beginOpeningSequence(GameScreen* gs)
{
	Jon& jon = m_game->getJon();
	jon.setAllowedToMove(true);
	jon.getVelocity().setX(RABBIT_DEFAULT_MAX_SPEED);

	updateCamera(gs, 0, false, true);

	EntityUtils::updateBackgrounds(m_game->getBackgroundUppers(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundMids(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundLowers(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundMidgroundCovers(), gs->m_renderer->getCameraPosition(), 0);

	m_hasShownOpeningSequence = true;
}

void Chapter1Level21::handleOpeningSequence(GameScreen* gs)
{
	m_hasOpeningSequenceCompleted = true;

	EntityUtils::updateBackgrounds(m_game->getBackgroundUppers(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundMids(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundLowers(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundMidgroundCovers(), gs->m_renderer->getCameraPosition(), 0);
}

void Chapter1Level21::update(GameScreen* gs)
{
    Level::update(gs);
    
    if (m_game->getJons().size() == 0
        || !m_endBossSnake
        || !m_hole)
    {
        return;
    }
    
    Jon& jon = m_game->getJon();
    
    if (m_endBossSnake->getState() == EndBossSnakeState_Sleeping
		&& jon.getPosition().getX() < (m_hole->getPosition().getX() + 4))
    {
		jon.setUserActionPrevented(true);

        if (jon.getPosition().getX() > m_hole->getPosition().getX())
        {
			jon.getAcceleration().set(0, 0);
			jon.getVelocity().set(0, 0);
			jon.setIdle(true);

			jon.getPosition().setX(m_hole->getPosition().getX());
			jon.updateBounds();

			if (!m_hasTriggeredMusicLoopIntro)
			{
				if (Assets::getInstance()->isMusicEnabled())
				{
					Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_LOOP_INTRO);
				}

				m_hasTriggeredMusicLoopIntro = true;

				return;
			}
        }

		if (m_hasTriggeredMusicLoopIntro
			&& !m_hasTriggeredSnakeAwaken)
		{
			m_fGameStateTime += gs->m_fDeltaTime;

			if (m_fGameStateTime > 3)
			{
				m_endBossSnake->awaken();

				m_hasTriggeredSnakeAwaken = true;
			}
		}
    }
	else if (m_endBossSnake->getState() == EndBossSnakeState_Awakening
		|| m_endBossSnake->getState() == EndBossSnakeState_OpeningMouthLeft
		|| m_endBossSnake->getState() == EndBossSnakeState_OpenMouthLeft)
	{
		m_fGameStateTime += gs->m_fDeltaTime;
	}
	else if (m_endBossSnake->getState() == EndBossSnakeState_ChargingLeft)
	{
		m_isChaseCamActivated = false;

		if (!m_hasTriggeredMusicLoop)
		{
			m_fGameStateTime += gs->m_fDeltaTime;

			if (m_fGameStateTime > 7.15f)
			{
				Assets::getInstance()->setMusicId(MUSIC_PLAY_END_BOSS_LOOP);

				m_fMusicVolume = 0.5f;

				m_hasTriggeredMusicLoop = true;
			}
		}

		if (m_endBossSnake->getDamage() == 2
			&& jon.getPosition().getX() > m_fMarker2X
			&& jon.getPhysicalState() == PHYSICAL_GROUNDED)
		{
			m_endBossSnake->beginPursuit();
		}
		else if (m_endBossSnake->getDamage() == 1
			&& jon.getPosition().getX() > m_fMarker1X
			&& jon.getPhysicalState() == PHYSICAL_GROUNDED)
		{
			m_endBossSnake->beginPursuit();
		}

		if (!m_hasTriggeredCheckPoint)
		{
			if (jon.isVampire()
				&& jon.getPhysicalState() == PHYSICAL_GROUNDED)
			{
				jon.setUserActionPrevented(false);

				m_fCheckPointX = jon.getPosition().getX();
				m_fCheckPointY = jon.getPosition().getY();
				m_fCheckPointStateTime = m_game->getStateTime();

				m_hasTriggeredCheckPoint = true;

				m_endBossSnake->beginPursuit();
			}
			else if (jon.getNumJumps() == 2
				&& jon.isFalling()
				&& !jon.isTransformingIntoVampire()
				&& !jon.isVampire())
			{
				jon.setUserActionPrevented(false);
				jon.triggerTransform();
				jon.setUserActionPrevented(true);

				m_hole->triggerBurrow();
			}
			else if (jon.getNumJumps() == 1
				&& jon.isFalling()
				&& jon.getMainBounds().getBottom() < (m_endBossSnake->getMainBounds().getTop() + 1)
				&& !jon.isTransformingIntoVampire()
				&& !jon.isVampire())
			{
				jon.setUserActionPrevented(false);
				jon.triggerJump();
				jon.setUserActionPrevented(true);
			}
			else if (jon.getNumJumps() == 0
				&& !jon.isTransformingIntoVampire()
				&& !jon.isVampire())
			{
				jon.setIdle(false);
				jon.setUserActionPrevented(false);
				jon.triggerJump();
				jon.setUserActionPrevented(true);
			}
		}
	}
	else if (m_endBossSnake->getState() == EndBossSnakeState_Pursuing
             || m_endBossSnake->getState() == EndBossSnakeState_OpeningMouthRight
             || m_endBossSnake->getState() == EndBossSnakeState_OpenMouthRight
             || m_endBossSnake->getState() == EndBossSnakeState_Damaged
             || m_endBossSnake->getState() == EndBossSnakeState_ChargingRight
             || m_endBossSnake->getState() == EndBossSnakeState_Dying
             || m_endBossSnake->getState() == EndBossSnakeState_DeadSpiritReleasing)
	{
		m_isChaseCamActivated = true;

		if (m_endBossSnake->getPosition().getX() > jon.getPosition().getX())
		{
			m_isChaseCamActivated = false;
		}

		if (m_endBossSnake->getState() == EndBossSnakeState_Dying)
		{
			jon.getAcceleration().setX(0);
			jon.getVelocity().setX(0);
			jon.setIdle(true);
			jon.setUserActionPrevented(true);
		}

		if (m_endBossSnake->getState() == EndBossSnakeState_Dying
			&& m_fMusicVolume > 0)
		{
			m_fMusicVolume -= gs->m_fDeltaTime;
			if (m_fMusicVolume < 0)
			{
				m_fMusicVolume = 0;
			}

			short musicId = MUSIC_SET_VOLUME * 1000 + (short)(m_fMusicVolume * 100);
			Assets::getInstance()->setMusicId(musicId);
		}
	}
	else if (m_endBossSnake->getState() == EndBossSnakeState_Dead)
	{
		m_isChaseCamActivated = false;

		if (!m_hasTriggeredSnakeDeathCheckPoint)
		{
			m_fCheckPointX = jon.getPosition().getX();
			m_fCheckPointY = jon.getPosition().getY();

			m_fCheckPointStateTime = m_game->getStateTime();
			m_iNumCarrotsCollectedAtCheckpoint = m_game->getNumCarrotsCollected();
			m_iNumGoldenCarrotsCollectedAtCheckpoint = m_game->getNumGoldenCarrotsCollected();

			jon.flash();

			if (jon.isIdle())
			{
				jon.setIdle(false);
			}

			if (!FlagUtil::isFlagSet(m_iBestLevelStatsFlag, FLAG_LEVEL_COMPLETE))
			{
				m_batPanel->config(m_game.get(), BatGoalType_Dash);
			}

			m_hasTriggeredSnakeDeathCheckPoint = true;
		}
	}
}

void Chapter1Level21::updateCamera(GameScreen* gs, float paddingX, bool ignoreY, bool instant)
{
    if (m_isChaseCamActivated)
    {
		gs->m_renderer->updateCameraToFollowJon(*m_game, m_batPanel.get(), gs->m_fDeltaTime, paddingX, true, ignoreY, instant);
    }
    else
    {
        Level::updateCamera(gs, paddingX, ignoreY, instant);
    }
}

bool Chapter1Level21::isInSlowMotionMode()
{
    return false;
}

Chapter1Level21::Chapter1Level21(const char* json) : Level(json),
m_endBossSnake(nullptr),
m_hole(nullptr),
m_fGameStateTime(0),
m_fCheckPointStateTime(0),
m_fCheckPointX(0),
m_fCheckPointY(0),
m_fMarker1X(0),
m_fMarker2X(0),
m_fMusicVolume(0.5f),
m_iNumCarrotsCollectedAtCheckpoint(0),
m_iNumGoldenCarrotsCollectedAtCheckpoint(0),
m_isChaseCamActivated(false),
m_hasTriggeredMusicLoopIntro(false),
m_hasTriggeredSnakeAwaken(false),
m_hasTriggeredMusicLoop(false),
m_hasTriggeredCheckPoint(false),
m_hasTriggeredSnakeDeathCheckPoint(false)
{
    // Empty
}

/// Chapter 1 Level 21 ///

#include <sstream>

Chapter1Level21 * Chapter1Level21::getInstance()
{
	std::stringstream ss;
	ss << "{\"world\":1,\"level\":21,\"midgrounds\":[{\"gridX\":564,\"gridY\":0,\"type\":5},{\"gridX\":612,\"gridY\":0,\"type\":4},{\"gridX\":652,\"gridY\":0,\"type\":5},{\"gridX\":748,\"gridY\":0,\"type\":5},{\"gridX\":704,\"gridY\":0,\"type\":4},{\"gridX\":800,\"gridY\":0,\"type\":4},{\"gridX\":848,\"gridY\":0,\"type\":5},{\"gridX\":900,\"gridY\":0,\"type\":4},{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":56,\"gridY\":96,\"type\":1},{\"gridX\":96,\"gridY\":96,\"type\":0},{\"gridX\":136,\"gridY\":96,\"type\":1},{\"gridX\":248,\"gridY\":96,\"type\":2},{\"gridX\":308,\"gridY\":96,\"type\":1},{\"gridX\":352,\"gridY\":96,\"type\":1},{\"gridX\":396,\"gridY\":96,\"type\":2},{\"gridX\":456,\"gridY\":96,\"type\":1},{\"gridX\":500,\"gridY\":96,\"type\":1},{\"gridX\":544,\"gridY\":96,\"type\":2},{\"gridX\":604,\"gridY\":96,\"type\":1},{\"gridX\":648,\"gridY\":96,\"type\":1},{\"gridX\":692,\"gridY\":96,\"type\":2},{\"gridX\":752,\"gridY\":96,\"type\":1},{\"gridX\":796,\"gridY\":96,\"type\":1},{\"gridX\":840,\"gridY\":96,\"type\":2},{\"gridX\":904,\"gridY\":96,\"type\":1},{\"gridX\":948,\"gridY\":96,\"type\":1},{\"gridX\":992,\"gridY\":96,\"type\":2},{\"gridX\":1052,\"gridY\":96,\"type\":1},{\"gridX\":1100,\"gridY\":96,\"type\":1},{\"gridX\":1148,\"gridY\":96,\"type\":0},{\"gridX\":1364,\"gridY\":96,\"type\":2},{\"gridX\":1440,\"gridY\":96,\"type\":1},{\"gridX\":1496,\"gridY\":96,\"type\":1},{\"gridX\":1544,\"gridY\":96,\"type\":2},{\"gridX\":1608,\"gridY\":96,\"type\":1},{\"gridX\":1656,\"gridY\":96,\"type\":1},{\"gridX\":1704,\"gridY\":96,\"type\":2},{\"gridX\":1768,\"gridY\":96,\"type\":1},{\"gridX\":1816,\"gridY\":96,\"type\":1},{\"gridX\":1864,\"gridY\":96,\"type\":2},{\"gridX\":1928,\"gridY\":96,\"type\":1},{\"gridX\":1976,\"gridY\":96,\"type\":1},{\"gridX\":2024,\"gridY\":96,\"type\":2},{\"gridX\":2088,\"gridY\":96,\"type\":1},{\"gridX\":2136,\"gridY\":96,\"type\":1},{\"gridX\":2184,\"gridY\":96,\"type\":2},{\"gridX\":2248,\"gridY\":96,\"type\":1},{\"gridX\":2296,\"gridY\":96,\"type\":1},{\"gridX\":2348,\"gridY\":96,\"type\":2},{\"gridX\":2412,\"gridY\":96,\"type\":1},{\"gridX\":2668,\"gridY\":96,\"type\":1},{\"gridX\":2712,\"gridY\":96,\"type\":0},{\"gridX\":2748,\"gridY\":96,\"type\":0},{\"gridX\":144,\"gridY\":0,\"type\":5},{\"gridX\":204,\"gridY\":0,\"type\":4},{\"gridX\":232,\"gridY\":0,\"type\":4},{\"gridX\":276,\"gridY\":0,\"type\":5},{\"gridX\":316,\"gridY\":0,\"type\":4},{\"gridX\":376,\"gridY\":0,\"type\":5},{\"gridX\":1348,\"gridY\":0,\"type\":5},{\"gridX\":1400,\"gridY\":0,\"type\":4},{\"gridX\":1444,\"gridY\":0,\"type\":4},{\"gridX\":1488,\"gridY\":0,\"type\":4},{\"gridX\":1528,\"gridY\":0,\"type\":4},{\"gridX\":1572,\"gridY\":0,\"type\":4},{\"gridX\":1588,\"gridY\":0,\"type\":5},{\"gridX\":2788,\"gridY\":96,\"type\":2},{\"gridX\":2852,\"gridY\":96,\"type\":1},{\"gridX\":2900,\"gridY\":96,\"type\":1},{\"gridX\":2948,\"gridY\":96,\"type\":2},{\"gridX\":3012,\"gridY\":96,\"type\":1},{\"gridX\":3060,\"gridY\":96,\"type\":1},{\"gridX\":3108,\"gridY\":96,\"type\":2},{\"gridX\":3172,\"gridY\":96,\"type\":1},{\"gridX\":3220,\"gridY\":96,\"type\":1},{\"gridX\":3268,\"gridY\":96,\"type\":2},{\"gridX\":3332,\"gridY\":96,\"type\":1},{\"gridX\":3380,\"gridY\":96,\"type\":1},{\"gridX\":3428,\"gridY\":96,\"type\":2},{\"gridX\":3492,\"gridY\":96,\"type\":2},{\"gridX\":3556,\"gridY\":96,\"type\":1},{\"gridX\":3604,\"gridY\":96,\"type\":1},{\"gridX\":3652,\"gridY\":96,\"type\":2},{\"gridX\":3716,\"gridY\":96,\"type\":1},{\"gridX\":3764,\"gridY\":96,\"type\":2},{\"gridX\":3828,\"gridY\":96,\"type\":1},{\"gridX\":3876,\"gridY\":96,\"type\":2},{\"gridX\":3940,\"gridY\":96,\"type\":1},{\"gridX\":3988,\"gridY\":96,\"type\":2},{\"gridX\":4052,\"gridY\":96,\"type\":1},{\"gridX\":4096,\"gridY\":96,\"type\":1},{\"gridX\":4140,\"gridY\":96,\"type\":2},{\"gridX\":4256,\"gridY\":96,\"type\":1},{\"gridX\":4204,\"gridY\":96,\"type\":0},{\"gridX\":4300,\"gridY\":96,\"type\":2},{\"gridX\":4360,\"gridY\":96,\"type\":0},{\"gridX\":4392,\"gridY\":96,\"type\":2},{\"gridX\":4452,\"gridY\":96,\"type\":1},{\"gridX\":4496,\"gridY\":96,\"type\":0},{\"gridX\":4532,\"gridY\":96,\"type\":0},{\"gridX\":4564,\"gridY\":96,\"type\":2},{\"gridX\":4624,\"gridY\":96,\"type\":0},{\"gridX\":1772,\"gridY\":0,\"type\":4},{\"gridX\":1840,\"gridY\":0,\"type\":4},{\"gridX\":1788,\"gridY\":0,\"type\":5},{\"gridX\":1872,\"gridY\":0,\"type\":4},{\"gridX\":1932,\"gridY\":0,\"type\":4},{\"gridX\":1948,\"gridY\":0,\"type\":5},{\"gridX\":1972,\"gridY\":0,\"type\":5},{\"gridX\":2018,\"gridY\":0,\"type\":4},{\"gridX\":2100,\"gridY\":0,\"type\":5},{\"gridX\":2560,\"gridY\":0,\"type\":5},{\"gridX\":2580,\"gridY\":0,\"type\":5},{\"gridX\":2652,\"gridY\":0,\"type\":5},{\"gridX\":2684,\"gridY\":0,\"type\":4},{\"gridX\":2712,\"gridY\":0,\"type\":5},{\"gridX\":2756,\"gridY\":0,\"type\":5},{\"gridX\":2800,\"gridY\":0,\"type\":4},{\"gridX\":2820,\"gridY\":0,\"type\":4},{\"gridX\":2884,\"gridY\":0,\"type\":5},{\"gridX\":3160,\"gridY\":0,\"type\":5},{\"gridX\":3140,\"gridY\":0,\"type\":4},{\"gridX\":3256,\"gridY\":0,\"type\":5},{\"gridX\":3276,\"gridY\":0,\"type\":4},{\"gridX\":3360,\"gridY\":0,\"type\":4},{\"gridX\":3404,\"gridY\":0,\"type\":4},{\"gridX\":3456,\"gridY\":0,\"type\":5},{\"gridX\":3548,\"gridY\":0,\"type\":5},{\"gridX\":3572,\"gridY\":0,\"type\":4},{\"gridX\":3592,\"gridY\":0,\"type\":4},{\"gridX\":3604,\"gridY\":0,\"type\":5},{\"gridX\":3668,\"gridY\":0,\"type\":5},{\"gridX\":3704,\"gridY\":0,\"type\":5},{\"gridX\":3652,\"gridY\":0,\"type\":4},{\"gridX\":3796,\"gridY\":0,\"type\":5},{\"gridX\":3820,\"gridY\":0,\"type\":4},{\"gridX\":3856,\"gridY\":0,\"type\":5},{\"gridX\":3924,\"gridY\":0,\"type\":4},{\"gridX\":3964,\"gridY\":0,\"type\":5},{\"gridX\":3996,\"gridY\":0,\"type\":4},{\"gridX\":3948,\"gridY\":0,\"type\":4},{\"gridX\":1188,\"gridY\":96,\"type\":2},{\"gridX\":1252,\"gridY\":96,\"type\":0},{\"gridX\":2456,\"gridY\":96,\"type\":2},{\"gridX\":2520,\"gridY\":96,\"type\":1},{\"gridX\":2564,\"gridY\":96,\"type\":0}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":0},{\"gridX\":4232,\"gridY\":0,\"type\":1},{\"gridX\":8,\"gridY\":0,\"type\":3},{\"gridX\":136,\"gridY\":0,\"type\":3},{\"gridX\":264,\"gridY\":0,\"type\":3},{\"gridX\":520,\"gridY\":0,\"type\":3},{\"gridX\":648,\"gridY\":0,\"type\":3},{\"gridX\":776,\"gridY\":0,\"type\":3},{\"gridX\":904,\"gridY\":0,\"type\":3}";
	ss << ",{\"gridX\":1032,\"gridY\":0,\"type\":3},{\"gridX\":1288,\"gridY\":0,\"type\":3},{\"gridX\":1416,\"gridY\":0,\"type\":3},{\"gridX\":1544,\"gridY\":0,\"type\":3},{\"gridX\":1672,\"gridY\":0,\"type\":3},{\"gridX\":392,\"gridY\":0,\"type\":3},{\"gridX\":1800,\"gridY\":0,\"type\":3},{\"gridX\":1928,\"gridY\":0,\"type\":3},{\"gridX\":2184,\"gridY\":0,\"type\":3},{\"gridX\":2312,\"gridY\":0,\"type\":3},{\"gridX\":2568,\"gridY\":0,\"type\":3},{\"gridX\":2696,\"gridY\":0,\"type\":3},{\"gridX\":2824,\"gridY\":0,\"type\":3},{\"gridX\":2952,\"gridY\":0,\"type\":3},{\"gridX\":3080,\"gridY\":0,\"type\":3},{\"gridX\":3208,\"gridY\":0,\"type\":3},{\"gridX\":3336,\"gridY\":0,\"type\":3},{\"gridX\":3464,\"gridY\":0,\"type\":3},{\"gridX\":3592,\"gridY\":0,\"type\":3},{\"gridX\":3720,\"gridY\":0,\"type\":3},{\"gridX\":3848,\"gridY\":0,\"type\":3},{\"gridX\":3976,\"gridY\":0,\"type\":3},{\"gridX\":4104,\"gridY\":0,\"type\":3},{\"gridX\":2056,\"gridY\":0,\"type\":3},{\"gridX\":1160,\"gridY\":0,\"type\":3},{\"gridX\":2440,\"gridY\":0,\"type\":3},{\"gridX\":532,\"gridY\":50,\"type\":10},{\"gridX\":140,\"gridY\":50,\"type\":10},{\"gridX\":1736,\"gridY\":50,\"type\":10},{\"gridX\":3040,\"gridY\":50,\"type\":10},{\"gridX\":3232,\"gridY\":50,\"type\":10},{\"gridX\":3528,\"gridY\":50,\"type\":10},{\"gridX\":3780,\"gridY\":50,\"type\":10},{\"gridX\":1344,\"gridY\":50,\"type\":10},{\"gridX\":964,\"gridY\":50,\"type\":10},{\"gridX\":2560,\"gridY\":50,\"type\":10},{\"gridX\":1220,\"gridY\":50,\"type\":10},{\"gridX\":2648,\"gridY\":50,\"type\":10},{\"gridX\":2568,\"gridY\":50,\"type\":11},{\"gridX\":3048,\"gridY\":50,\"type\":12},{\"gridX\":3112,\"gridY\":50,\"type\":12},{\"gridX\":3536,\"gridY\":50,\"type\":12},{\"gridX\":3600,\"gridY\":50,\"type\":12},{\"gridX\":3664,\"gridY\":50,\"type\":12},{\"gridX\":1100,\"gridY\":50,\"type\":12},{\"gridX\":1228,\"gridY\":50,\"type\":12},{\"gridX\":2000,\"gridY\":50,\"type\":13},{\"gridX\":668,\"gridY\":50,\"type\":13},{\"gridX\":796,\"gridY\":50,\"type\":13},{\"gridX\":1352,\"gridY\":50,\"type\":13},{\"gridX\":148,\"gridY\":50,\"type\":13},{\"gridX\":3788,\"gridY\":50,\"type\":13},{\"gridX\":540,\"gridY\":50,\"type\":13},{\"gridX\":1872,\"gridY\":50,\"type\":13},{\"gridX\":2656,\"gridY\":50,\"type\":13},{\"gridX\":2784,\"gridY\":50,\"type\":13},{\"gridX\":1480,\"gridY\":50,\"type\":13},{\"gridX\":276,\"gridY\":50,\"type\":13},{\"gridX\":3916,\"gridY\":50,\"type\":13},{\"gridX\":1744,\"gridY\":50,\"type\":13},{\"gridX\":3240,\"gridY\":50,\"type\":13},{\"gridX\":3368,\"gridY\":50,\"type\":13},{\"gridX\":972,\"gridY\":50,\"type\":13},{\"gridX\":3176,\"gridY\":50,\"type\":14},{\"gridX\":3728,\"gridY\":50,\"type\":14},{\"gridX\":1292,\"gridY\":50,\"type\":14},{\"gridX\":1164,\"gridY\":48,\"type\":14},{\"gridX\":3228,\"gridY\":48,\"type\":15},{\"gridX\":136,\"gridY\":48,\"type\":15},{\"gridX\":3036,\"gridY\":48,\"type\":15},{\"gridX\":528,\"gridY\":48,\"type\":15},{\"gridX\":1340,\"gridY\":48,\"type\":15},{\"gridX\":1732,\"gridY\":48,\"type\":15},{\"gridX\":3776,\"gridY\":48,\"type\":15},{\"gridX\":2644,\"gridY\":48,\"type\":15},{\"gridX\":3596,\"gridY\":48,\"type\":15},{\"gridX\":2556,\"gridY\":48,\"type\":15},{\"gridX\":2564,\"gridY\":48,\"type\":16},{\"gridX\":3044,\"gridY\":48,\"type\":17},{\"gridX\":3604,\"gridY\":48,\"type\":17},{\"gridX\":536,\"gridY\":48,\"type\":18},{\"gridX\":1348,\"gridY\":48,\"type\":18},{\"gridX\":1476,\"gridY\":48,\"type\":18},{\"gridX\":144,\"gridY\":48,\"type\":18},{\"gridX\":664,\"gridY\":48,\"type\":18},{\"gridX\":2652,\"gridY\":48,\"type\":18},{\"gridX\":2780,\"gridY\":48,\"type\":18},{\"gridX\":272,\"gridY\":48,\"type\":18},{\"gridX\":1740,\"gridY\":48,\"type\":18},{\"gridX\":1868,\"gridY\":48,\"type\":18},{\"gridX\":1996,\"gridY\":48,\"type\":18},{\"gridX\":792,\"gridY\":48,\"type\":18},{\"gridX\":3236,\"gridY\":48,\"type\":18},{\"gridX\":3364,\"gridY\":48,\"type\":18},{\"gridX\":3784,\"gridY\":48,\"type\":18},{\"gridX\":3912,\"gridY\":48,\"type\":18},{\"gridX\":1604,\"gridY\":48,\"type\":19},{\"gridX\":2596,\"gridY\":48,\"type\":19},{\"gridX\":2908,\"gridY\":48,\"type\":19},{\"gridX\":2124,\"gridY\":48,\"type\":19},{\"gridX\":920,\"gridY\":48,\"type\":19},{\"gridX\":400,\"gridY\":48,\"type\":19},{\"gridX\":3108,\"gridY\":48,\"type\":19},{\"gridX\":3668,\"gridY\":48,\"type\":19},{\"gridX\":4040,\"gridY\":48,\"type\":19},{\"gridX\":3492,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":2952,\"gridY\":90,\"type\":23},{\"gridX\":2568,\"gridY\":90,\"type\":23},{\"gridX\":520,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":3208,\"gridY\":90,\"type\":23},{\"gridX\":3336,\"gridY\":90,\"type\":23},{\"gridX\":3464,\"gridY\":90,\"type\":23},{\"gridX\":392,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":264,\"gridY\":90,\"type\":23},{\"gridX\":648,\"gridY\":90,\"type\":23},{\"gridX\":2696,\"gridY\":90,\"type\":23},{\"gridX\":2824,\"gridY\":90,\"type\":23},{\"gridX\":1800,\"gridY\":90,\"type\":23},{\"gridX\":1416,\"gridY\":90,\"type\":23},{\"gridX\":1672,\"gridY\":90,\"type\":23},{\"gridX\":776,\"gridY\":90,\"type\":23},{\"gridX\":2184,\"gridY\":90,\"type\":23},{\"gridX\":3080,\"gridY\":90,\"type\":23},{\"gridX\":1032,\"gridY\":90,\"type\":23},{\"gridX\":3592,\"gridY\":90,\"type\":23},{\"gridX\":1544,\"gridY\":90,\"type\":23},{\"gridX\":3720,\"gridY\":90,\"type\":23},{\"gridX\":3848,\"gridY\":90,\"type\":23},{\"gridX\":3976,\"gridY\":90,\"type\":23},{\"gridX\":904,\"gridY\":90,\"type\":23},{\"gridX\":4104,\"gridY\":90,\"type\":23},{\"gridX\":1160,\"gridY\":90,\"type\":23},{\"gridX\":2312,\"gridY\":88,\"type\":23},{\"gridX\":2056,\"gridY\":90,\"type\":23},{\"gridX\":1928,\"gridY\":90,\"type\":23},{\"gridX\":1288,\"gridY\":90,\"type\":23},{\"gridX\":2440,\"gridY\":88,\"type\":23},{\"gridX\":4256,\"gridY\":0,\"type\":25},{\"gridX\":4264,\"gridY\":0,\"type\":28},{\"gridX\":4392,\"gridY\":0,\"type\":28},{\"gridX\":4520,\"gridY\":0,\"type\":28},{\"gridX\":4648,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[{\"gridX\":4232,\"gridY\":88,\"type\":0}],\"holes\":[{\"gridX\":120,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":184,\"gridY\":128,\"type\":0},{\"gridX\":188,\"gridY\":128,\"type\":1},{\"gridX\":202,\"gridY\":128,\"type\":1},{\"gridX\":244,\"gridY\":128,\"type\":2},{\"gridX\":532,\"gridY\":75,\"type\":27},{\"gridX\":532,\"gridY\":84,\"type\":30},{\"gridX\":230,\"gridY\":128,\"type\":1},{\"gridX\":216,\"gridY\":128,\"type\":1},{\"gridX\":488,\"gridY\":48,\"type\":34},{\"gridX\":504,\"gridY\":60,\"type\":34},{\"gridX\":516,\"gridY\":52,\"type\":34},{\"gridX\":424,\"gridY\":56,\"type\":34},{\"gridX\":412,\"gridY\":48,\"type\":34},{\"gridX\":436,\"gridY\":48,\"type\":34},{\"gridX\":140,\"gridY\":84,\"type\":30},{\"gridX\":140,\"gridY\":75,\"type\":27},{\"gridX\":176,\"gridY\":84,\"type\":30},{\"gridX\":176,\"gridY\":75,\"type\":27},{\"gridX\":212,\"gridY\":84,\"type\":30},{\"gridX\":212,\"gridY\":75,\"type\":27},{\"gridX\":1320,\"gridY\":96,\"type\":36},{\"gridX\":1344,\"gridY\":75,\"type\":27},{\"gridX\":1344,\"gridY\":84,\"type\":30},{\"gridX\":2160,\"gridY\":48,\"type\":3},{\"gridX\":2164,\"gridY\":48,\"type\":4},{\"gridX\":2178,\"gridY\":48,\"type\":4},{\"gridX\":2192,\"gridY\":48,\"type\":5},{\"gridX\":2260,\"gridY\":48,\"type\":3},{\"gridX\":2264,\"gridY\":48,\"type\":4},{\"gridX\":1000,\"gridY\":70,\"type\":3},{\"gridX\":1004,\"gridY\":70,\"type\":4},{\"gridX\":1018,\"gridY\":70,\"type\":5},{\"gridX\":1080,\"gridY\":70,\"type\":3},{\"gridX\":1084,\"gridY\":70,\"type\":4},{\"gridX\":1098,\"gridY\":70,\"type\":5},{\"gridX\":1616,\"gridY\":48,\"type\":34},{\"gridX\":1640,\"gridY\":48,\"type\":34},{\"gridX\":1628,\"gridY\":56,\"type\":34},{\"gridX\":1692,\"gridY\":48,\"type\":34},{\"gridX\":1704,\"gridY\":64,\"type\":34},{\"gridX\":1716,\"gridY\":56,\"type\":34},{\"gridX\":2278,\"gridY\":48,\"type\":4},{\"gridX\":2292,\"gridY\":48,\"type\":5},{\"gridX\":1336,\"gridY\":59,\"type\":32},{\"gridX\":1736,\"gridY\":84,\"type\":30},{\"gridX\":1736,\"gridY\":75,\"type\":27},{\"gridX\":2360,\"gridY\":48,\"type\":3},{\"gridX\":2364,\"gridY\":48,\"type\":4},{\"gridX\":2378,\"gridY\":48,\"type\":4},{\"gridX\":2392,\"gridY\":48,\"type\":5},{\"gridX\":2628,\"gridY\":96,\"type\":36},{\"gridX\":2648,\"gridY\":75,\"type\":27},{\"gridX\":2648,\"gridY\":84,\"type\":30},{\"gridX\":2920,\"gridY\":48,\"type\":34},{\"gridX\":2944,\"gridY\":48,\"type\":34},{\"gridX\":2932,\"gridY\":56,\"type\":34},{\"gridX\":2996,\"gridY\":48,\"type\":34},{\"gridX\":3012,\"gridY\":60,\"type\":34},{\"gridX\":3024,\"gridY\":52,\"type\":34},{\"gridX\":3040,\"gridY\":75,\"type\":27},{\"gridX\":3040,\"gridY\":84,\"type\":30},{\"gridX\":3232,\"gridY\":84,\"type\":30},{\"gridX\":3232,\"gridY\":75,\"type\":27},{\"gridX\":3592,\"gridY\":58,\"type\":32},{\"gridX\":3600,\"gridY\":84,\"type\":30},{\"gridX\":3600,\"gridY\":75,\"type\":27},{\"gridX\":4450,\"gridY\":96,\"type\":12},{\"gridX\":4240,\"gridY\":7,\"type\":21},{\"gridX\":916,\"gridY\":24,\"type\":3},{\"gridX\":934,\"gridY\":24,\"type\":5},{\"gridX\":920,\"gridY\":24,\"type\":4},{\"gridX\":2460,\"gridY\":48,\"type\":3},{\"gridX\":2464,\"gridY\":48,\"type\":4},{\"gridX\":2478,\"gridY\":48,\"type\":5}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[{\"gridX\":536,\"gridY\":7,\"type\":45},{\"gridX\":449,\"gridY\":66,\"type\":44},{\"gridX\":1740,\"gridY\":7,\"type\":45},{\"gridX\":1653,\"gridY\":66,\"type\":44},{\"gridX\":3160,\"gridY\":55,\"type\":43},{\"gridX\":3044,\"gridY\":7,\"type\":45},{\"gridX\":2957,\"gridY\":66,\"type\":44},{\"gridX\":3564,\"gridY\":55,\"type\":42},{\"gridX\":3708,\"gridY\":55,\"type\":43}],\"countHissWithMinas\":[{\"gridX\":180,\"gridY\":133}],\"endBossSnakes\":[{\"gridX\":196,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":940,\"gridY\":40,\"type\":0},{\"gridX\":944,\"gridY\":48,\"type\":0},{\"gridX\":512,\"gridY\":16,\"type\":0},{\"gridX\":520,\"gridY\":16,\"type\":0},{\"gridX\":504,\"gridY\":16,\"type\":0},{\"gridX\":176,\"gridY\":16,\"type\":0},{\"gridX\":184,\"gridY\":16,\"type\":0},{\"gridX\":192,\"gridY\":16,\"type\":0},{\"gridX\":352,\"gridY\":16,\"type\":0},{\"gridX\":336,\"gridY\":16,\"type\":0},{\"gridX\":344,\"gridY\":16,\"type\":0},{\"gridX\":676,\"gridY\":16,\"type\":0},{\"gridX\":832,\"gridY\":16,\"type\":0},{\"gridX\":684,\"gridY\":16,\"type\":0},{\"gridX\":692,\"gridY\":16,\"type\":0},{\"gridX\":824,\"gridY\":16,\"type\":0},{\"gridX\":816,\"gridY\":16,\"type\":0},{\"gridX\":1000,\"gridY\":80,\"type\":0},{\"gridX\":1008,\"gridY\":80,\"type\":0},{\"gridX\":1016,\"gridY\":80,\"type\":0},{\"gridX\":1044,\"gridY\":64,\"type\":0},{\"gridX\":1052,\"gridY\":64,\"type\":0},{\"gridX\":1060,\"gridY\":64,\"type\":0},{\"gridX\":1080,\"gridY\":80,\"type\":0},{\"gridX\":1088,\"gridY\":80,\"type\":0},{\"gridX\":1096,\"gridY\":80,\"type\":0},{\"gridX\":1124,\"gridY\":64,\"type\":0},{\"gridX\":1132,\"gridY\":64,\"type\":0},{\"gridX\":1140,\"gridY\":64,\"type\":0},{\"gridX\":1328,\"gridY\":76,\"type\":1},{\"gridX\":2596,\"gridY\":80,\"type\":1},{\"gridX\":1368,\"gridY\":16,\"type\":0},{\"gridX\":1376,\"gridY\":16,\"type\":0},{\"gridX\":1384,\"gridY\":16,\"type\":0},{\"gridX\":1460,\"gridY\":16,\"type\":0},{\"gridX\":1476,\"gridY\":16,\"type\":0},{\"gridX\":1468,\"gridY\":16,\"type\":0},{\"gridX\":1544,\"gridY\":16,\"type\":0},{\"gridX\":1552,\"gridY\":16,\"type\":0},{\"gridX\":1560,\"gridY\":16,\"type\":0},{\"gridX\":1708,\"gridY\":16,\"type\":0},{\"gridX\":1716,\"gridY\":16,\"type\":0},{\"gridX\":1724,\"gridY\":16,\"type\":0},{\"gridX\":2164,\"gridY\":60,\"type\":0},{\"gridX\":2176,\"gridY\":60,\"type\":0},{\"gridX\":2188,\"gridY\":60,\"type\":0},{\"gridX\":2264,\"gridY\":60,\"type\":0},{\"gridX\":2276,\"gridY\":60,\"type\":0},{\"gridX\":2288,\"gridY\":60,\"type\":0},{\"gridX\":2364,\"gridY\":60,\"type\":0},{\"gridX\":2376,\"gridY\":60,\"type\":0},{\"gridX\":2388,\"gridY\":60,\"type\":0},{\"gridX\":948,\"gridY\":56,\"type\":0},{\"gridX\":1820,\"gridY\":16,\"type\":0},{\"gridX\":1812,\"gridY\":16,\"type\":0},{\"gridX\":1828,\"gridY\":16,\"type\":0},{\"gridX\":1904,\"gridY\":16,\"type\":0},{\"gridX\":1912,\"gridY\":16,\"type\":0},{\"gridX\":1920,\"gridY\":16,\"type\":0},{\"gridX\":2052,\"gridY\":16,\"type\":0},{\"gridX\":2060,\"gridY\":16,\"type\":0},{\"gridX\":2068,\"gridY\":16,\"type\":0},{\"gridX\":2216,\"gridY\":44,\"type\":0},{\"gridX\":2224,\"gridY\":40,\"type\":0},{\"gridX\":2232,\"gridY\":44,\"type\":0},{\"gridX\":2316,\"gridY\":44,\"type\":0},{\"gridX\":2324,\"gridY\":40,\"type\":0},{\"gridX\":2332,\"gridY\":44,\"type\":0},{\"gridX\":2732,\"gridY\":16,\"type\":0},{\"gridX\":2740,\"gridY\":16,\"type\":0},{\"gridX\":2748,\"gridY\":16,\"type\":0},{\"gridX\":3028,\"gridY\":16,\"type\":0},{\"gridX\":3020,\"gridY\":16,\"type\":0},{\"gridX\":3012,\"gridY\":16,\"type\":0},{\"gridX\":2972,\"gridY\":40,\"type\":1},{\"gridX\":3316,\"gridY\":16,\"type\":0},{\"gridX\":3324,\"gridY\":16,\"type\":0},{\"gridX\":3332,\"gridY\":16,\"type\":0},{\"gridX\":3644,\"gridY\":16,\"type\":0},{\"gridX\":3636,\"gridY\":16,\"type\":0},{\"gridX\":3628,\"gridY\":16,\"type\":0},{\"gridX\":3876,\"gridY\":16,\"type\":0},{\"gridX\":3884,\"gridY\":16,\"type\":0},{\"gridX\":3892,\"gridY\":16,\"type\":0},{\"gridX\":4404,\"gridY\":104,\"type\":0},{\"gridX\":4420,\"gridY\":104,\"type\":0},{\"gridX\":4412,\"gridY\":104,\"type\":0},{\"gridX\":4124,\"gridY\":16,\"type\":0},{\"gridX\":4132,\"gridY\":16,\"type\":0},{\"gridX\":4140,\"gridY\":16,\"type\":0},{\"gridX\":4428,\"gridY\":104,\"type\":0},{\"gridX\":1180,\"gridY\":44,\"type\":0},{\"gridX\":1192,\"gridY\":36,\"type\":0},{\"gridX\":1204,\"gridY\":44,\"type\":0},{\"gridX\":2416,\"gridY\":44,\"type\":0},{\"gridX\":2424,\"gridY\":40,\"type\":0},{\"gridX\":2432,\"gridY\":44,\"type\":0},{\"gridX\":2460,\"gridY\":60,\"type\":0},{\"gridX\":2468,\"gridY\":60,\"type\":0},{\"gridX\":2476,\"gridY\":60,\"type\":0}],\"jons\":[{\"gridX\":16,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[{\"gridX\":4705,\"gridY\":0,\"type\":1}]}";
	std::string text = ss.str();

    static Chapter1Level21 *instance = new Chapter1Level21(text.c_str());
    
    return instance;
}
