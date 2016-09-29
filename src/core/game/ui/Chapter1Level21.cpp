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

		if (spikedBallIndex == 3
			&& spikedBallChainIndex == 3)
		{
			break;
		}
	}

	if (spikedBallChain1 && spikedBall1
		&& spikedBallChain2 && spikedBall2
		&& spikedBallChain3 && spikedBall3)
	{
		spikedBallChain1->setSpikedBall(spikedBall1);
		spikedBallChain2->setSpikedBall(spikedBall2);
		spikedBallChain3->setSpikedBall(spikedBall3);
	}

	GiantTree* gt1 = nullptr;
	GiantTree* gt2 = nullptr;
	GiantTree* gt3 = nullptr;
	int giantTreeIndex = 0;
	for (std::vector<ForegroundObject*>::iterator i = m_game->getForegroundObjects().begin(); i != m_game->getForegroundObjects().end(); i++)
	{
		if ((*i)->getType() == ForegroundObjectType_GiantTree)
		{
			giantTreeIndex++;
			switch (giantTreeIndex)
			{
			case 1:
				gt1 = dynamic_cast<GiantTree*>((*i));
				break;
			case 2:
				gt2 = dynamic_cast<GiantTree*>((*i));
				break;
			case 3:
				gt3 = dynamic_cast<GiantTree*>((*i));
				break;
			}
		}

		if (giantTreeIndex == 3)
		{
			break;
		}
	}
	
	if (gt1
		&& gt2
		&& gt3)
	{
		m_fMarker1X = fminf(gt1->getPosition().getX(), fminf(gt2->getPosition().getX(), gt3->getPosition().getX()));
		m_fMarker3X = fmaxf(gt1->getPosition().getX(), fmaxf(gt2->getPosition().getX(), gt3->getPosition().getX()));
		
		m_fMarker2X = gt1->getPosition().getX();
		if (m_fMarker2X < m_fMarker1X
			|| m_fMarker2X > m_fMarker3X)
		{
			m_fMarker2X = gt2->getPosition().getX();
			if (m_fMarker2X < m_fMarker1X
				|| m_fMarker2X > m_fMarker3X)
			{
				m_fMarker2X = gt3->getPosition().getX();
			}
		}
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

		m_endBossSnake->chargeLeft();
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
	m_fMarker3X = 0;
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

void Chapter1Level21::onEnter(GameScreen* gs)
{
    // Empty
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
			&& jon.getPosition().getX() > m_fMarker3X)
		{
			m_endBossSnake->beginPursuit();
		}
		else if (m_endBossSnake->getDamage() == 1
			&& jon.getPosition().getX() > m_fMarker2X)
		{
			m_endBossSnake->beginPursuit();
		}
		else if (m_endBossSnake->getDamage() == 0
			&& jon.getPosition().getX() > m_fMarker1X)
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
m_fMarker3X(0),
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

#include <sstream>

/// Chapter 1 Level 21 ///

Chapter1Level21 * Chapter1Level21::getInstance()
{
    std::stringstream ss;
    
    ss << "{\"world\":1,\"level\":21,\"midgrounds\":[{\"gridX\":692,\"gridY\":0,\"type\":5},{\"gridX\":740,\"gridY\":0,\"type\":4},{\"gridX\":780,\"gridY\":0,\"type\":5},{\"gridX\":876,\"gridY\":0,\"type\":5},{\"gridX\":832,\"gridY\":0,\"type\":4},{\"gridX\":928,\"gridY\":0,\"type\":4},{\"gridX\":976,\"gridY\":0,\"type\":5},{\"gridX\":1028,\"gridY\":0,\"type\":4},{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":56,\"gridY\":96,\"type\":1},{\"gridX\":96,\"gridY\":96,\"type\":0},{\"gridX\":136,\"gridY\":96,\"type\":1},{\"gridX\":436,\"gridY\":96,\"type\":1},{\"gridX\":480,\"gridY\":96,\"type\":1},{\"gridX\":524,\"gridY\":96,\"type\":2},{\"gridX\":584,\"gridY\":96,\"type\":1},{\"gridX\":628,\"gridY\":96,\"type\":1},{\"gridX\":672,\"gridY\":96,\"type\":2},{\"gridX\":732,\"gridY\":96,\"type\":1},{\"gridX\":776,\"gridY\":96,\"type\":1},{\"gridX\":820,\"gridY\":96,\"type\":2},{\"gridX\":880,\"gridY\":96,\"type\":1},{\"gridX\":924,\"gridY\":96,\"type\":1},{\"gridX\":968,\"gridY\":96,\"type\":2},{\"gridX\":1032,\"gridY\":96,\"type\":1},{\"gridX\":1076,\"gridY\":96,\"type\":1},{\"gridX\":1120,\"gridY\":96,\"type\":2},{\"gridX\":1180,\"gridY\":96,\"type\":1},{\"gridX\":1228,\"gridY\":96,\"type\":1},{\"gridX\":1276,\"gridY\":96,\"type\":0},{\"gridX\":1556,\"gridY\":96,\"type\":2},{\"gridX\":1436,\"gridY\":96,\"type\":1},{\"gridX\":1696,\"gridY\":96,\"type\":1},{\"gridX\":1736,\"gridY\":96,\"type\":2},{\"gridX\":1800,\"gridY\":96,\"type\":1},{\"gridX\":1848,\"gridY\":96,\"type\":1},{\"gridX\":1896,\"gridY\":96,\"type\":2},{\"gridX\":1960,\"gridY\":96,\"type\":1},{\"gridX\":2008,\"gridY\":96,\"type\":1},{\"gridX\":2056,\"gridY\":96,\"type\":2},{\"gridX\":2120,\"gridY\":96,\"type\":1},{\"gridX\":2168,\"gridY\":96,\"type\":1},{\"gridX\":2216,\"gridY\":96,\"type\":2},{\"gridX\":2280,\"gridY\":96,\"type\":1},{\"gridX\":2328,\"gridY\":96,\"type\":1},{\"gridX\":2376,\"gridY\":96,\"type\":2},{\"gridX\":2440,\"gridY\":96,\"type\":1},{\"gridX\":2488,\"gridY\":96,\"type\":1},{\"gridX\":2540,\"gridY\":96,\"type\":2},{\"gridX\":2604,\"gridY\":96,\"type\":1},{\"gridX\":2932,\"gridY\":96,\"type\":1},{\"gridX\":2788,\"gridY\":96,\"type\":0},{\"gridX\":2992,\"gridY\":96,\"type\":0},{\"gridX\":144,\"gridY\":0,\"type\":5},{\"gridX\":192,\"gridY\":0,\"type\":4},{\"gridX\":220,\"gridY\":0,\"type\":4},{\"gridX\":284,\"gridY\":0,\"type\":5},{\"gridX\":352,\"gridY\":0,\"type\":4},{\"gridX\":504,\"gridY\":0,\"type\":5},{\"gridX\":1476,\"gridY\":0,\"type\":5},{\"gridX\":1512,\"gridY\":0,\"type\":4},{\"gridX\":1572,\"gridY\":0,\"type\":4},{\"gridX\":1680,\"gridY\":0,\"type\":4},{\"gridX\":1720,\"gridY\":0,\"type\":4},{\"gridX\":1764,\"gridY\":0,\"type\":4},{\"gridX\":1780,\"gridY\":0,\"type\":5},{\"gridX\":3040,\"gridY\":96,\"type\":2},{\"gridX\":3108,\"gridY\":96,\"type\":1},{\"gridX\":3156,\"gridY\":96,\"type\":1},{\"gridX\":3204,\"gridY\":96,\"type\":2},{\"gridX\":3268,\"gridY\":96,\"type\":1},{\"gridX\":3316,\"gridY\":96,\"type\":1},{\"gridX\":3364,\"gridY\":96,\"type\":2},{\"gridX\":3428,\"gridY\":96,\"type\":1},{\"gridX\":3476,\"gridY\":96,\"type\":1},{\"gridX\":3524,\"gridY\":96,\"type\":2},{\"gridX\":3588,\"gridY\":96,\"type\":1},{\"gridX\":3636,\"gridY\":96,\"type\":1},{\"gridX\":3684,\"gridY\":96,\"type\":2},{\"gridX\":3748,\"gridY\":96,\"type\":2},{\"gridX\":3820,\"gridY\":96,\"type\":1},{\"gridX\":3876,\"gridY\":96,\"type\":1},{\"gridX\":3924,\"gridY\":96,\"type\":2},{\"gridX\":3988,\"gridY\":96,\"type\":1},{\"gridX\":4036,\"gridY\":96,\"type\":2},{\"gridX\":4100,\"gridY\":96,\"type\":1},{\"gridX\":4148,\"gridY\":96,\"type\":2},{\"gridX\":4212,\"gridY\":96,\"type\":1},{\"gridX\":4260,\"gridY\":96,\"type\":2},{\"gridX\":4324,\"gridY\":96,\"type\":1},{\"gridX\":4364,\"gridY\":96,\"type\":1},{\"gridX\":4404,\"gridY\":96,\"type\":2},{\"gridX\":5088,\"gridY\":96,\"type\":1},{\"gridX\":4464,\"gridY\":96,\"type\":0},{\"gridX\":5128,\"gridY\":96,\"type\":2},{\"gridX\":5188,\"gridY\":96,\"type\":0},{\"gridX\":4504,\"gridY\":96,\"type\":2},{\"gridX\":5224,\"gridY\":96,\"type\":1},{\"gridX\":5268,\"gridY\":96,\"type\":0},{\"gridX\":5300,\"gridY\":96,\"type\":0},{\"gridX\":5332,\"gridY\":96,\"type\":0},{\"gridX\":1964,\"gridY\":0,\"type\":4},{\"gridX\":2032,\"gridY\":0,\"type\":4},{\"gridX\":1980,\"gridY\":0,\"type\":5},{\"gridX\":2064,\"gridY\":0,\"type\":4},{\"gridX\":2124,\"gridY\":0,\"type\":4},{\"gridX\":2140,\"gridY\":0,\"type\":5},{\"gridX\":2164,\"gridY\":0,\"type\":5},{\"gridX\":2210,\"gridY\":0,\"type\":4},{\"gridX\":2292,\"gridY\":0,\"type\":5},{\"gridX\":2736,\"gridY\":0,\"type\":5},{\"gridX\":2764,\"gridY\":0,\"type\":5},{\"gridX\":2844,\"gridY\":0,\"type\":5},{\"gridX\":2876,\"gridY\":0,\"type\":4},{\"gridX\":2904,\"gridY\":0,\"type\":5},{\"gridX\":2964,\"gridY\":0,\"type\":5},{\"gridX\":3004,\"gridY\":0,\"type\":4},{\"gridX\":3068,\"gridY\":0,\"type\":4},{\"gridX\":3140,\"gridY\":0,\"type\":5},{\"gridX\":3416,\"gridY\":0,\"type\":5},{\"gridX\":3396,\"gridY\":0,\"type\":4},{\"gridX\":3512,\"gridY\":0,\"type\":5},{\"gridX\":3532,\"gridY\":0,\"type\":4},{\"gridX\":3616,\"gridY\":0,\"type\":4},{\"gridX\":3660,\"gridY\":0,\"type\":4},{\"gridX\":3712,\"gridY\":0,\"type\":5},{\"gridX\":3820,\"gridY\":0,\"type\":5},{\"gridX\":3844,\"gridY\":0,\"type\":4},{\"gridX\":3864,\"gridY\":0,\"type\":4},{\"gridX\":3876,\"gridY\":0,\"type\":5},{\"gridX\":3940,\"gridY\":0,\"type\":5},{\"gridX\":3976,\"gridY\":0,\"type\":5},{\"gridX\":3924,\"gridY\":0,\"type\":4},{\"gridX\":4068,\"gridY\":0,\"type\":5},{\"gridX\":4092,\"gridY\":0,\"type\":4},{\"gridX\":4128,\"gridY\":0,\"type\":5},{\"gridX\":4196,\"gridY\":0,\"type\":4},{\"gridX\":4236,\"gridY\":0,\"type\":5},{\"gridX\":4268,\"gridY\":0,\"type\":4},{\"gridX\":4220,\"gridY\":0,\"type\":4},{\"gridX\":1324,\"gridY\":96,\"type\":2},{\"gridX\":1392,\"gridY\":96,\"type\":0},{\"gridX\":2648,\"gridY\":96,\"type\":2},{\"gridX\":2712,\"gridY\":96,\"type\":1},{\"gridX\":2756,\"gridY\":96,\"type\":0},{\"gridX\":4568,\"gridY\":96,\"type\":1},{\"gridX\":4616,\"gridY\":96,\"type\":1},{\"gridX\":4664,\"gridY\":96,\"type\":0},{\"gridX\":4700,\"gridY\":96,\"type\":1},{\"gridX\":4748,\"gridY\":96,\"type\":2},{\"gridX\":4812,\"gridY\":96,\"type\":1},{\"gridX\":4936,\"gridY\":96,\"type\":2},{\"gridX\":4996,\"gridY\":96,\"type\":0},{\"gridX\":5028,\"gridY\":96,\"type\":1},{\"gridX\":4856,\"gridY\":96,\"type\":1},{\"gridX\":4900,\"gridY\":96,\"type\":0},{\"gridX\":4300,\"gridY\":0,\"type\":5},{\"gridX\":4380,\"gridY\":0,\"type\":5},{\"gridX\":4428,\"gridY\":0,\"type\":4},{\"gridX\":4476,\"gridY\":0,\"type\":4},{\"gridX\":4506,\"gridY\":0,\"type\":5},{\"gridX\":4536,\"gridY\":0,\"type\":5},{\"gridX\":4564,\"gridY\":0,\"type\":4},{\"gridX\":4632,\"gridY\":0,\"type\":5},{\"gridX\":4664,\"gridY\":0,\"type\":4},{\"gridX\":4696,\"gridY\":0,\"type\":4},{\"gridX\":4720,\"gridY\":0,\"type\":5},{\"gridX\":4746,\"gridY\":0,\"type\":4},{\"gridX\":4760,\"gridY\":0,\"type\":5},{\"gridX\":4836,\"gridY\":0,\"type\":5},{\"gridX\":4812,\"gridY\":0,\"type\":4},{\"gridX\":4868,\"gridY\":0,\"type\":4},{\"gridX\":4944,\"gridY\":0,\"type\":4},{\"gridX\":4960,\"gridY\":0,\"type\":5},{\"gridX\":5016,\"gridY\":0,\"type\":5},{\"gridX\":5044,\"gridY\":0,\"type\":4},{\"gridX\":5056,\"gridY\":0,\"type\":4},{\"gridX\":1100,\"gridY\":0,\"type\":4},{\"gridX\":1120,\"gridY\":0,\"type\":5},{\"gridX\":1179,\"gridY\":0,\"type\":4},{\"gridX\":1156,\"gridY\":0,\"type\":5},{\"gridX\":1232,\"gridY\":0,\"type\":4},{\"gridX\":1252,\"gridY\":0,\"type\":5},{\"gridX\":1276,\"gridY\":0,\"type\":5},{\"gridX\":1356,\"gridY\":0,\"type\":5},{\"gridX\":1380,\"gridY\":0,\"type\":4},{\"gridX\":3340,\"gridY\":0,\"type\":5},{\"gridX\":396,\"gridY\":0,\"type\":4},{\"gridX\":464,\"gridY\":0,\"type\":5},{\"gridX\":296,\"gridY\":96,\"type\":2},{\"gridX\":356,\"gridY\":96,\"type\":1},{\"gridX\":400,\"gridY\":96,\"type\":0},{\"gridX\":1592,\"gridY\":0,\"type\":5},{\"gridX\":1660,\"gridY\":0,\"type\":4},{\"gridX\":1616,\"gridY\":96,\"type\":1},{\"gridX\":1660,\"gridY\":96,\"type\":0},{\"gridX\":2824,\"gridY\":96,\"type\":0},{\"gridX\":304" << "4,\"gridY\":0,\"type\":4}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":0},{\"gridX\":5064,\"gridY\":0,\"type\":1},{\"gridX\":4872,\"gridY\":0,\"type\":2},{\"gridX\":1672,\"gridY\":0,\"type\":2},{\"gridX\":3016,\"gridY\":0,\"type\":2},{\"gridX\":648,\"gridY\":0,\"type\":3},{\"gridX\":776,\"gridY\":0,\"type\":3},{\"gridX\":904,\"gridY\":0,\"type\":3},{\"gridX\":1032,\"gridY\":0,\"type\":3},{\"gridX\":1160,\"gridY\":0,\"type\":3},{\"gridX\":1416,\"gridY\":0,\"type\":3},{\"gridX\":1544,\"gridY\":0,\"type\":3},{\"gridX\":1736,\"gridY\":0,\"type\":3},{\"gridX\":1864,\"gridY\":0,\"type\":3},{\"gridX\":520,\"gridY\":0,\"type\":3},{\"gridX\":1992,\"gridY\":0,\"type\":3},{\"gridX\":2120,\"gridY\":0,\"type\":3},{\"gridX\":2376,\"gridY\":0,\"type\":3},{\"gridX\":2760,\"gridY\":0,\"type\":3},{\"gridX\":2888,\"gridY\":0,\"type\":3},{\"gridX\":3208,\"gridY\":0,\"type\":3},{\"gridX\":3336,\"gridY\":0,\"type\":3},{\"gridX\":3464,\"gridY\":0,\"type\":3},{\"gridX\":3592,\"gridY\":0,\"type\":3},{\"gridX\":3720,\"gridY\":0,\"type\":3},{\"gridX\":3848,\"gridY\":0,\"type\":3},{\"gridX\":3976,\"gridY\":0,\"type\":3},{\"gridX\":4104,\"gridY\":0,\"type\":3},{\"gridX\":4232,\"gridY\":0,\"type\":3},{\"gridX\":4360,\"gridY\":0,\"type\":3},{\"gridX\":8,\"gridY\":0,\"type\":3},{\"gridX\":136,\"gridY\":0,\"type\":3},{\"gridX\":264,\"gridY\":0,\"type\":3},{\"gridX\":2504,\"gridY\":0,\"type\":3},{\"gridX\":4488,\"gridY\":0,\"type\":3},{\"gridX\":2248,\"gridY\":0,\"type\":3},{\"gridX\":1288,\"gridY\":0,\"type\":3},{\"gridX\":2632,\"gridY\":0,\"type\":3},{\"gridX\":3080,\"gridY\":0,\"type\":3},{\"gridX\":392,\"gridY\":0,\"type\":3},{\"gridX\":4616,\"gridY\":0,\"type\":3},{\"gridX\":4744,\"gridY\":0,\"type\":3},{\"gridX\":4936,\"gridY\":0,\"type\":3},{\"gridX\":660,\"gridY\":50,\"type\":10},{\"gridX\":4368,\"gridY\":50,\"type\":10},{\"gridX\":140,\"gridY\":50,\"type\":10},{\"gridX\":1928,\"gridY\":50,\"type\":10},{\"gridX\":3296,\"gridY\":50,\"type\":10},{\"gridX\":4624,\"gridY\":50,\"type\":10},{\"gridX\":3488,\"gridY\":50,\"type\":10},{\"gridX\":3800,\"gridY\":50,\"type\":10},{\"gridX\":4936,\"gridY\":50,\"type\":10},{\"gridX\":4056,\"gridY\":50,\"type\":10},{\"gridX\":1472,\"gridY\":50,\"type\":10},{\"gridX\":5008,\"gridY\":50,\"type\":10},{\"gridX\":1092,\"gridY\":50,\"type\":10},{\"gridX\":2720,\"gridY\":50,\"type\":10},{\"gridX\":1348,\"gridY\":50,\"type\":10},{\"gridX\":2840,\"gridY\":50,\"type\":10},{\"gridX\":4944,\"gridY\":50,\"type\":11},{\"gridX\":5040,\"gridY\":50,\"type\":11},{\"gridX\":5016,\"gridY\":50,\"type\":11},{\"gridX\":1356,\"gridY\":50,\"type\":11},{\"gridX\":3304,\"gridY\":50,\"type\":12},{\"gridX\":3368,\"gridY\":50,\"type\":12},{\"gridX\":3808,\"gridY\":50,\"type\":12},{\"gridX\":3872,\"gridY\":50,\"type\":12},{\"gridX\":3936,\"gridY\":50,\"type\":12},{\"gridX\":1228,\"gridY\":50,\"type\":12},{\"gridX\":4504,\"gridY\":50,\"type\":12},{\"gridX\":4376,\"gridY\":50,\"type\":12},{\"gridX\":4440,\"gridY\":50,\"type\":12},{\"gridX\":1608,\"gridY\":48,\"type\":12},{\"gridX\":2728,\"gridY\":50,\"type\":12},{\"gridX\":2976,\"gridY\":48,\"type\":12},{\"gridX\":924,\"gridY\":50,\"type\":13},{\"gridX\":3040,\"gridY\":50,\"type\":13},{\"gridX\":668,\"gridY\":50,\"type\":13},{\"gridX\":1936,\"gridY\":50,\"type\":13},{\"gridX\":148,\"gridY\":50,\"type\":13},{\"gridX\":3496,\"gridY\":50,\"type\":13},{\"gridX\":1100,\"gridY\":50,\"type\":13},{\"gridX\":1480,\"gridY\":50,\"type\":13},{\"gridX\":2192,\"gridY\":50,\"type\":13},{\"gridX\":4192,\"gridY\":50,\"type\":13},{\"gridX\":2848,\"gridY\":50,\"type\":13},{\"gridX\":4064,\"gridY\":50,\"type\":13},{\"gridX\":276,\"gridY\":48,\"type\":13},{\"gridX\":2064,\"gridY\":50,\"type\":13},{\"gridX\":1672,\"gridY\":50,\"type\":13},{\"gridX\":404,\"gridY\":50,\"type\":13},{\"gridX\":3624,\"gridY\":50,\"type\":13},{\"gridX\":796,\"gridY\":50,\"type\":13},{\"gridX\":4632,\"gridY\":50,\"type\":13},{\"gridX\":4760,\"gridY\":50,\"type\":13},{\"gridX\":4000,\"gridY\":50,\"type\":14},{\"gridX\":1388,\"gridY\":50,\"type\":14},{\"gridX\":1292,\"gridY\":50,\"type\":14},{\"gridX\":4976,\"gridY\":50,\"type\":14},{\"gridX\":4568,\"gridY\":50,\"type\":14},{\"gridX\":3432,\"gridY\":50,\"type\":14},{\"gridX\":3292,\"gridY\":48,\"type\":15},{\"gridX\":656,\"gridY\":48,\"type\":15},{\"gridX\":1468,\"gridY\":48,\"type\":15},{\"gridX\":1924,\"gridY\":48,\"type\":15},{\"gridX\":4052,\"gridY\":48,\"type\":15},{\"gridX\":2836,\"gridY\":48,\"type\":15},{\"gridX\":3868,\"gridY\":48,\"type\":15},{\"gridX\":4620,\"gridY\":48,\"type\":15},{\"gridX\":2716,\"gridY\":48,\"type\":15},{\"gridX\":3484,\"gridY\":48,\"type\":15},{\"gridX\":136,\"gridY\":48,\"type\":15},{\"gridX\":4432,\"gridY\":48,\"type\":15},{\"gridX\":5061,\"gridY\":48,\"type\":15},{\"gridX\":3300,\"gridY\":48,\"type\":17},{\"gridX\":3876,\"gridY\":48,\"type\":17},{\"gridX\":4440,\"gridY\":48,\"type\":17},{\"gridX\":2724,\"gridY\":48,\"type\":17},{\"gridX\":1604,\"gridY\":48,\"type\":17},{\"gridX\":2972,\"gridY\":48,\"type\":17},{\"gridX\":920,\"gridY\":48,\"type\":18},{\"gridX\":2188,\"gridY\":48,\"type\":18},{\"gridX\":3492,\"gridY\":48,\"type\":18},{\"gridX\":3620,\"gridY\":48,\"type\":18},{\"gridX\":4060,\"gridY\":48,\"type\":18},{\"gridX\":4188,\"gridY\":48,\"type\":18},{\"gridX\":272,\"gridY\":48,\"type\":18},{\"gridX\":1476,\"gridY\":48,\"type\":18},{\"gridX\":4628,\"gridY\":48,\"type\":18},{\"gridX\":4756,\"gridY\":48,\"type\":18},{\"gridX\":1668,\"gridY\":48,\"type\":18},{\"gridX\":664,\"gridY\":48,\"type\":18},{\"gridX\":144,\"gridY\":48,\"type\":18},{\"gridX\":792,\"gridY\":48,\"type\":18},{\"gridX\":2844,\"gridY\":48,\"type\":18},{\"gridX\":3036,\"gridY\":48,\"type\":18},{\"gridX\":1932,\"gridY\":48,\"type\":18},{\"gridX\":400,\"gridY\":48,\"type\":18},{\"gridX\":2060,\"gridY\":48,\"type\":18},{\"gridX\":3164,\"gridY\":48,\"type\":19},{\"gridX\":2316,\"gridY\":48,\"type\":19},{\"gridX\":1048,\"gridY\":48,\"type\":19},{\"gridX\":528,\"gridY\":48,\"type\":19},{\"gridX\":3940,\"gridY\":48,\"type\":19},{\"gridX\":4316,\"gridY\":48,\"type\":19},{\"gridX\":3364,\"gridY\":48,\"type\":19},{\"gridX\":3748,\"gridY\":48,\"type\":19},{\"gridX\":5068,\"gridY\":48,\"type\":19},{\"gridX\":1796,\"gridY\":48,\"type\":19},{\"gridX\":2788,\"gridY\":48,\"type\":19},{\"gridX\":4504,\"gridY\":48,\"type\":19},{\"gridX\":4884,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":4872,\"gridY\":90,\"type\":22},{\"gridX\":1672,\"gridY\":88,\"type\":22},{\"gridX\":3016,\"gridY\":88,\"type\":22},{\"gridX\":3464,\"gridY\":90,\"type\":23},{\"gridX\":3592,\"gridY\":90,\"type\":23},{\"gridX\":4360,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":3720,\"gridY\":90,\"type\":23},{\"gridX\":264,\"gridY\":90,\"type\":23},{\"gridX\":520,\"gridY\":90,\"type\":23},{\"gridX\":776,\"gridY\":90,\"type\":23},{\"gridX\":2888,\"gridY\":90,\"type\":23},{\"gridX\":3080,\"gridY\":90,\"type\":23},{\"gridX\":1544,\"gridY\":90,\"type\":23},{\"gridX\":1864,\"gridY\":90,\"type\":23},{\"gridX\":904,\"gridY\":90,\"type\":23},{\"gridX\":3336,\"gridY\":90,\"type\":23},{\"gridX\":3208,\"gridY\":90,\"type\":23},{\"gridX\":4488,\"gridY\":90,\"type\":23},{\"gridX\":2760,\"gridY\":90,\"type\":23},{\"gridX\":648,\"gridY\":90,\"type\":23},{\"gridX\":2120,\"gridY\":90,\"type\":23},{\"gridX\":1416,\"gridY\":90,\"type\":23},{\"gridX\":2632,\"gridY\":90,\"type\":23},{\"gridX\":1160,\"gridY\":90,\"type\":23},{\"gridX\":1032,\"gridY\":90,\"type\":23},{\"gridX\":392,\"gridY\":88,\"type\":23},{\"gridX\":1736,\"gridY\":90,\"type\":23},{\"gridX\":3848,\"gridY\":90,\"type\":23},{\"gridX\":1992,\"gridY\":90,\"type\":23},{\"gridX\":3976,\"gridY\":90,\"type\":23},{\"gridX\":4104,\"gridY\":90,\"type\":23},{\"gridX\":2376,\"gridY\":90,\"type\":23},{\"gridX\":4936,\"gridY\":90,\"type\":23},{\"gridX\":4232,\"gridY\":90,\"type\":23},{\"gridX\":4744,\"gridY\":90,\"type\":23},{\"gridX\":1288,\"gridY\":90,\"type\":23},{\"gridX\":2504,\"gridY\":90,\"type\":23},{\"gridX\":4616,\"gridY\":90,\"type\":23},{\"gridX\":2248,\"gridY\"" << ":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":5088,\"gridY\":0,\"type\":25},{\"gridX\":5096,\"gridY\":0,\"type\":28},{\"gridX\":5224,\"gridY\":0,\"type\":28},{\"gridX\":5352,\"gridY\":0,\"type\":28},{\"gridX\":5480,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[{\"gridX\":5064,\"gridY\":88,\"type\":0}],\"holes\":[{\"gridX\":120,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":184,\"gridY\":128,\"type\":0},{\"gridX\":188,\"gridY\":128,\"type\":1},{\"gridX\":202,\"gridY\":128,\"type\":1},{\"gridX\":244,\"gridY\":128,\"type\":2},{\"gridX\":660,\"gridY\":75,\"type\":27},{\"gridX\":660,\"gridY\":84,\"type\":30},{\"gridX\":230,\"gridY\":128,\"type\":1},{\"gridX\":216,\"gridY\":128,\"type\":1},{\"gridX\":616,\"gridY\":48,\"type\":34},{\"gridX\":632,\"gridY\":60,\"type\":34},{\"gridX\":644,\"gridY\":52,\"type\":34},{\"gridX\":552,\"gridY\":56,\"type\":34},{\"gridX\":540,\"gridY\":48,\"type\":34},{\"gridX\":564,\"gridY\":48,\"type\":34},{\"gridX\":140,\"gridY\":84,\"type\":30},{\"gridX\":140,\"gridY\":75,\"type\":27},{\"gridX\":176,\"gridY\":84,\"type\":30},{\"gridX\":176,\"gridY\":75,\"type\":27},{\"gridX\":212,\"gridY\":84,\"type\":30},{\"gridX\":212,\"gridY\":75,\"type\":27},{\"gridX\":1520,\"gridY\":96,\"type\":36},{\"gridX\":1472,\"gridY\":75,\"type\":27},{\"gridX\":1472,\"gridY\":84,\"type\":30},{\"gridX\":2352,\"gridY\":48,\"type\":3},{\"gridX\":2356,\"gridY\":48,\"type\":4},{\"gridX\":2370,\"gridY\":48,\"type\":4},{\"gridX\":2384,\"gridY\":48,\"type\":5},{\"gridX\":2452,\"gridY\":48,\"type\":3},{\"gridX\":2456,\"gridY\":48,\"type\":4},{\"gridX\":1128,\"gridY\":70,\"type\":3},{\"gridX\":1132,\"gridY\":70,\"type\":4},{\"gridX\":1146,\"gridY\":70,\"type\":5},{\"gridX\":1208,\"gridY\":70,\"type\":3},{\"gridX\":1212,\"gridY\":70,\"type\":4},{\"gridX\":1226,\"gridY\":70,\"type\":5},{\"gridX\":1808,\"gridY\":48,\"type\":34},{\"gridX\":1832,\"gridY\":48,\"type\":34},{\"gridX\":1820,\"gridY\":56,\"type\":34},{\"gridX\":1884,\"gridY\":48,\"type\":34},{\"gridX\":1896,\"gridY\":64,\"type\":34},{\"gridX\":1908,\"gridY\":56,\"type\":34},{\"gridX\":2470,\"gridY\":48,\"type\":4},{\"gridX\":2484,\"gridY\":48,\"type\":5},{\"gridX\":1464,\"gridY\":59,\"type\":32},{\"gridX\":1928,\"gridY\":84,\"type\":30},{\"gridX\":1928,\"gridY\":75,\"type\":27},{\"gridX\":2552,\"gridY\":48,\"type\":3},{\"gridX\":2556,\"gridY\":48,\"type\":4},{\"gridX\":2570,\"gridY\":48,\"type\":4},{\"gridX\":2584,\"gridY\":48,\"type\":5},{\"gridX\":2884,\"gridY\":96,\"type\":36},{\"gridX\":2840,\"gridY\":75,\"type\":27},{\"gridX\":2840,\"gridY\":84,\"type\":30},{\"gridX\":3176,\"gridY\":48,\"type\":34},{\"gridX\":3200,\"gridY\":48,\"type\":34},{\"gridX\":3188,\"gridY\":56,\"type\":34},{\"gridX\":3252,\"gridY\":48,\"type\":34},{\"gridX\":3268,\"gridY\":60,\"type\":34},{\"gridX\":3280,\"gridY\":52,\"type\":34},{\"gridX\":3296,\"gridY\":75,\"type\":27},{\"gridX\":3296,\"gridY\":84,\"type\":30},{\"gridX\":3488,\"gridY\":84,\"type\":30},{\"gridX\":3488,\"gridY\":75,\"type\":27},{\"gridX\":3864,\"gridY\":58,\"type\":32},{\"gridX\":3872,\"gridY\":84,\"type\":30},{\"gridX\":3872,\"gridY\":75,\"type\":27},{\"gridX\":5164,\"gridY\":96,\"type\":12},{\"gridX\":1044,\"gridY\":24,\"type\":3},{\"gridX\":1062,\"gridY\":24,\"type\":5},{\"gridX\":1048,\"gridY\":24,\"type\":4},{\"gridX\":2652,\"gridY\":48,\"type\":3},{\"gridX\":2656,\"gridY\":48,\"type\":4},{\"gridX\":2670,\"gridY\":48,\"type\":5},{\"gridX\":5076,\"gridY\":7,\"type\":20},{\"gridX\":3496,\"gridY\":44,\"type\":30},{\"gridX\":3532,\"gridY\":44,\"type\":30},{\"gridX\":3568,\"gridY\":44,\"type\":30},{\"gridX\":3604,\"gridY\":44,\"type\":30},{\"gridX\":3640,\"gridY\":44,\"type\":30},{\"gridX\":3676,\"gridY\":44,\"type\":30},{\"gridX\":3712,\"gridY\":44,\"type\":30},{\"gridX\":3972,\"gridY\":44,\"type\":30},{\"gridX\":4064,\"gridY\":44,\"type\":30},{\"gridX\":4056,\"gridY\":84,\"type\":30},{\"gridX\":4056,\"gridY\":75,\"type\":27},{\"gridX\":4100,\"gridY\":44,\"type\":30},{\"gridX\":4136,\"gridY\":44,\"type\":30},{\"gridX\":4172,\"gridY\":44,\"type\":30},{\"gridX\":4208,\"gridY\":44,\"type\":30},{\"gridX\":4244,\"gridY\":44,\"type\":30},{\"gridX\":4280,\"gridY\":44,\"type\":30},{\"gridX\":4540,\"gridY\":44,\"type\":30},{\"gridX\":4436,\"gridY\":84,\"type\":30},{\"gridX\":4436,\"gridY\":75,\"type\":27},{\"gridX\":4428,\"gridY\":58,\"type\":32},{\"gridX\":4624,\"gridY\":84,\"type\":30},{\"gridX\":4624,\"gridY\":75,\"type\":27},{\"gridX\":4632,\"gridY\":44,\"type\":30},{\"gridX\":4668,\"gridY\":44,\"type\":30},{\"gridX\":4704,\"gridY\":44,\"type\":30},{\"gridX\":4740,\"gridY\":44,\"type\":30},{\"gridX\":4776,\"gridY\":44,\"type\":30},{\"gridX\":4812,\"gridY\":44,\"type\":30},{\"gridX\":4848,\"gridY\":44,\"type\":30},{\"gridX\":3745,\"gridY\":44,\"type\":28},{\"gridX\":4313,\"gridY\":44,\"type\":28},{\"gridX\":4881,\"gridY\":44,\"type\":28},{\"gridX\":260,\"gridY\":96,\"type\":36}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[{\"gridX\":664,\"gridY\":7,\"type\":45},{\"gridX\":577,\"gridY\":66,\"type\":44},{\"gridX\":1932,\"gridY\":7,\"type\":45},{\"gridX\":1845,\"gridY\":66,\"type\":44},{\"gridX\":3416,\"gridY\":55,\"type\":43},{\"gridX\":3300,\"gridY\":7,\"type\":45},{\"gridX\":3213,\"gridY\":66,\"type\":44},{\"gridX\":3800,\"gridY\":55,\"type\":42},{\"gridX\":3984,\"gridY\":55,\"type\":43},{\"gridX\":4368,\"gridY\":55,\"type\":42},{\"gridX\":4552,\"gridY\":55,\"type\":43},{\"gridX\":4936,\"gridY\":55,\"type\":42}],\"countHissWithMinas\":[{\"gridX\":180,\"gridY\":133}],\"endBossSnakes\":[{\"gridX\":196,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":1064,\"gridY\":36,\"type\":0},{\"gridX\":1068,\"gridY\":44,\"type\":0},{\"gridX\":640,\"gridY\":16,\"type\":0},{\"gridX\":648,\"gridY\":16,\"type\":0},{\"gridX\":632,\"gridY\":16,\"type\":0},{\"gridX\":260,\"gridY\":16,\"type\":0},{\"gridX\":252,\"gridY\":16,\"type\":0},{\"gridX\":244,\"gridY\":16,\"type\":0},{\"gridX\":440,\"gridY\":16,\"type\":0},{\"gridX\":424,\"gridY\":16,\"type\":0},{\"gridX\":432,\"gridY\":16,\"type\":0},{\"gridX\":804,\"gridY\":16,\"type\":0},{\"gridX\":960,\"gridY\":16,\"type\":0},{\"gridX\":812,\"gridY\":16,\"type\":0},{\"gridX\":820,\"gridY\":16,\"type\":0},{\"gridX\":952,\"gridY\":16,\"type\":0},{\"gridX\":944,\"gridY\":16,\"type\":0},{\"gridX\":1128,\"gridY\":80,\"type\":0},{\"gridX\":1136,\"gridY\":80,\"type\":0},{\"gridX\":1144,\"gridY\":80,\"type\":0},{\"gridX\":1172,\"gridY\":64,\"type\":0},{\"gridX\":1180,\"gridY\":64,\"type\":0},{\"gridX\":1188,\"gridY\":64,\"type\":0},{\"gridX\":1208,\"gridY\":80,\"type\":0},{\"gridX\":1216,\"gridY\":80,\"type\":0},{\"gridX\":1224,\"gridY\":80,\"type\":0},{\"gridX\":1252,\"gridY\":64,\"type\":0},{\"gridX\":1260,\"gridY\":64,\"type\":0},{\"gridX\":1268,\"gridY\":64,\"type\":0},{\"gridX\":1456,\"gridY\":76,\"type\":1},{\"gridX\":2788,\"gridY\":80,\"type\":1},{\"gridX\":1540,\"gridY\":16,\"type\":0},{\"gridX\":1548,\"gridY\":16,\"type\":0},{\"gridX\":1556,\"gridY\":16,\"type\":0},{\"gridX\":1624,\"gridY\":16,\"type\":0},{\"gridX\":1640,\"gridY\":16,\"type\":0},{\"gridX\":1632,\"gridY\":16,\"type\":0},{\"gridX\":1736,\"gridY\":16,\"type\":0},{\"gridX\":1744,\"gridY\":16,\"type\":0},{\"gridX\":1752,\"gridY\":16,\"type\":0},{\"gridX\":1900,\"gridY\":16,\"type\":0},{\"gridX\":1908,\"gridY\":16,\"type\":0},{\"gridX\":1916,\"gridY\":16,\"type\":0},{\"gridX\":2356,\"gridY\":60,\"type\":0},{\"gridX\":2368,\"gridY\":60,\"type\":0},{\"gridX\":2380,\"gridY\":60,\"type\":0},{\"gridX\":2456,\"gridY\":60,\"type\":0},{\"gridX\":2468,\"gridY\":60,\"type\":0},{\"gridX\":2480,\"gridY\":60,\"type\":0},{\"gridX\":2556,\"gridY\":60,\"type\":0},{\"gridX\":2568,\"gridY\":60,\"type\":0},{\"gridX\":2580,\"gridY\":60,\"type\":0},{\"gridX\":1072,\"gridY\":52,\"type\":0},{\"gridX\":2012,\"gridY\":16,\"type\":0},{\"gridX\":2004,\"gridY\":16,\"type\":0},{\"gridX\":2020,\"gridY\":16,\"type\":0},{\"gridX\":2096,\"gridY\":16,\"type\":0},{\"gridX\":2104,\"gridY\":16,\"type\":0},{\"gridX\":2112,\"gridY\":16,\"type\"" << ":0},{\"gridX\":2244,\"gridY\":16,\"type\":0},{\"gridX\":2252,\"gridY\":16,\"type\":0},{\"gridX\":2260,\"gridY\":16,\"type\":0},{\"gridX\":2408,\"gridY\":44,\"type\":0},{\"gridX\":2416,\"gridY\":40,\"type\":0},{\"gridX\":2424,\"gridY\":44,\"type\":0},{\"gridX\":2508,\"gridY\":44,\"type\":0},{\"gridX\":2516,\"gridY\":40,\"type\":0},{\"gridX\":2524,\"gridY\":44,\"type\":0},{\"gridX\":2936,\"gridY\":16,\"type\":0},{\"gridX\":2944,\"gridY\":16,\"type\":0},{\"gridX\":2952,\"gridY\":16,\"type\":0},{\"gridX\":3284,\"gridY\":16,\"type\":0},{\"gridX\":3276,\"gridY\":16,\"type\":0},{\"gridX\":3268,\"gridY\":16,\"type\":0},{\"gridX\":3572,\"gridY\":16,\"type\":0},{\"gridX\":3580,\"gridY\":16,\"type\":0},{\"gridX\":3588,\"gridY\":16,\"type\":0},{\"gridX\":3916,\"gridY\":16,\"type\":0},{\"gridX\":3908,\"gridY\":16,\"type\":0},{\"gridX\":3900,\"gridY\":16,\"type\":0},{\"gridX\":4172,\"gridY\":16,\"type\":0},{\"gridX\":4156,\"gridY\":16,\"type\":0},{\"gridX\":4164,\"gridY\":16,\"type\":0},{\"gridX\":4468,\"gridY\":16,\"type\":0},{\"gridX\":4460,\"gridY\":16,\"type\":0},{\"gridX\":4452,\"gridY\":16,\"type\":0},{\"gridX\":1312,\"gridY\":48,\"type\":0},{\"gridX\":1320,\"gridY\":44,\"type\":0},{\"gridX\":1328,\"gridY\":48,\"type\":0},{\"gridX\":2608,\"gridY\":44,\"type\":0},{\"gridX\":2616,\"gridY\":40,\"type\":0},{\"gridX\":2624,\"gridY\":44,\"type\":0},{\"gridX\":2652,\"gridY\":60,\"type\":0},{\"gridX\":2660,\"gridY\":60,\"type\":0},{\"gridX\":2668,\"gridY\":60,\"type\":0},{\"gridX\":5068,\"gridY\":80,\"type\":1},{\"gridX\":3104,\"gridY\":16,\"type\":0},{\"gridX\":3112,\"gridY\":16,\"type\":0},{\"gridX\":3120,\"gridY\":16,\"type\":0},{\"gridX\":1076,\"gridY\":60,\"type\":0},{\"gridX\":4784,\"gridY\":16,\"type\":0},{\"gridX\":4792,\"gridY\":16,\"type\":0},{\"gridX\":4800,\"gridY\":16,\"type\":0}],\"jons\":[{\"gridX\":16,\"gridY\":96}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level21 *instance = new Chapter1Level21(text.c_str());
    
    return instance;
}
