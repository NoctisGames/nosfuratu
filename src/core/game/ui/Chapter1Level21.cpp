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

		m_endBossSnake->begin();
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

        if (jon.getPosition().getX() > m_hole->getPosition().getX()
            && !m_hasTriggeredMusicLoopIntro)
        {
            jon.getAcceleration().set(0, 0);
            jon.getVelocity().set(0, 0);
            jon.setIdle(true);
            
            jon.getPosition().setX(m_hole->getPosition().getX());
            jon.updateBounds();
            
            if (Assets::getInstance()->isMusicEnabled())
            {
                Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_LOOP_INTRO);
            }
            
            m_hasTriggeredMusicLoopIntro = true;
            
            return;
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

				m_endBossSnake->begin();

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
			 || m_endBossSnake->getState() == EndBossSnakeState_Waiting
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
            
            if (!jon.isIdle())
            {
                jon.setIdle(true);
                jon.setUserActionPrevented(true);
            }
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
                jon.setUserActionPrevented(false);
			}

			if (!FlagUtil::isFlagSet(m_iBestLevelStatsFlag, FLAG_LEVEL_COMPLETE))
			{
				m_batPanel->config(m_game.get(), BatGoalType_Dash);
                
                jon.setUserActionPrevented(true);
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
    
    ss << "{\"world\":1,\"level\":21,\"midgrounds\":[{\"gridX\":1460,\"gridY\":0,\"type\":5},{\"gridX\":1508,\"gridY\":0,\"type\":4},{\"gridX\":1548,\"gridY\":0,\"type\":5},{\"gridX\":1644,\"gridY\":0,\"type\":5},{\"gridX\":1600,\"gridY\":0,\"type\":4},{\"gridX\":1696,\"gridY\":0,\"type\":4},{\"gridX\":1744,\"gridY\":0,\"type\":5},{\"gridX\":1796,\"gridY\":0,\"type\":4},{\"gridX\":0,\"gridY\":96,\"type\":2},{\"gridX\":56,\"gridY\":96,\"type\":1},{\"gridX\":96,\"gridY\":96,\"type\":0},{\"gridX\":136,\"gridY\":96,\"type\":1},{\"gridX\":1204,\"gridY\":96,\"type\":1},{\"gridX\":1248,\"gridY\":96,\"type\":1},{\"gridX\":1292,\"gridY\":96,\"type\":2},{\"gridX\":1352,\"gridY\":96,\"type\":1},{\"gridX\":1396,\"gridY\":96,\"type\":1},{\"gridX\":1440,\"gridY\":96,\"type\":2},{\"gridX\":1500,\"gridY\":96,\"type\":1},{\"gridX\":1544,\"gridY\":96,\"type\":1},{\"gridX\":1588,\"gridY\":96,\"type\":2},{\"gridX\":1648,\"gridY\":96,\"type\":1},{\"gridX\":1692,\"gridY\":96,\"type\":1},{\"gridX\":1736,\"gridY\":96,\"type\":2},{\"gridX\":1800,\"gridY\":96,\"type\":1},{\"gridX\":1844,\"gridY\":96,\"type\":1},{\"gridX\":1888,\"gridY\":96,\"type\":2},{\"gridX\":1948,\"gridY\":96,\"type\":1},{\"gridX\":1996,\"gridY\":96,\"type\":1},{\"gridX\":2044,\"gridY\":96,\"type\":0},{\"gridX\":2324,\"gridY\":96,\"type\":2},{\"gridX\":2204,\"gridY\":96,\"type\":1},{\"gridX\":2464,\"gridY\":96,\"type\":1},{\"gridX\":2504,\"gridY\":96,\"type\":2},{\"gridX\":2568,\"gridY\":96,\"type\":1},{\"gridX\":2616,\"gridY\":96,\"type\":1},{\"gridX\":2664,\"gridY\":96,\"type\":2},{\"gridX\":2728,\"gridY\":96,\"type\":1},{\"gridX\":2776,\"gridY\":96,\"type\":1},{\"gridX\":2824,\"gridY\":96,\"type\":2},{\"gridX\":2888,\"gridY\":96,\"type\":1},{\"gridX\":2936,\"gridY\":96,\"type\":1},{\"gridX\":2984,\"gridY\":96,\"type\":2},{\"gridX\":3048,\"gridY\":96,\"type\":1},{\"gridX\":3096,\"gridY\":96,\"type\":1},{\"gridX\":3144,\"gridY\":96,\"type\":2},{\"gridX\":3208,\"gridY\":96,\"type\":1},{\"gridX\":3256,\"gridY\":96,\"type\":1},{\"gridX\":3308,\"gridY\":96,\"type\":2},{\"gridX\":3372,\"gridY\":96,\"type\":1},{\"gridX\":3700,\"gridY\":96,\"type\":1},{\"gridX\":3556,\"gridY\":96,\"type\":0},{\"gridX\":3760,\"gridY\":96,\"type\":0},{\"gridX\":144,\"gridY\":0,\"type\":5},{\"gridX\":188,\"gridY\":0,\"type\":4},{\"gridX\":216,\"gridY\":0,\"type\":4},{\"gridX\":1068,\"gridY\":0,\"type\":5},{\"gridX\":1120,\"gridY\":0,\"type\":4},{\"gridX\":1272,\"gridY\":0,\"type\":5},{\"gridX\":2244,\"gridY\":0,\"type\":5},{\"gridX\":2280,\"gridY\":0,\"type\":4},{\"gridX\":2364,\"gridY\":0,\"type\":4},{\"gridX\":2448,\"gridY\":0,\"type\":4},{\"gridX\":2504,\"gridY\":0,\"type\":4},{\"gridX\":2532,\"gridY\":0,\"type\":4},{\"gridX\":2548,\"gridY\":0,\"type\":5},{\"gridX\":3808,\"gridY\":96,\"type\":2},{\"gridX\":3876,\"gridY\":96,\"type\":1},{\"gridX\":3924,\"gridY\":96,\"type\":1},{\"gridX\":3972,\"gridY\":96,\"type\":2},{\"gridX\":4036,\"gridY\":96,\"type\":1},{\"gridX\":4084,\"gridY\":96,\"type\":1},{\"gridX\":4132,\"gridY\":96,\"type\":2},{\"gridX\":4196,\"gridY\":96,\"type\":1},{\"gridX\":4244,\"gridY\":96,\"type\":1},{\"gridX\":4292,\"gridY\":96,\"type\":2},{\"gridX\":4356,\"gridY\":96,\"type\":1},{\"gridX\":4404,\"gridY\":96,\"type\":1},{\"gridX\":4452,\"gridY\":96,\"type\":2},{\"gridX\":4516,\"gridY\":96,\"type\":2},{\"gridX\":4588,\"gridY\":96,\"type\":1},{\"gridX\":4644,\"gridY\":96,\"type\":1},{\"gridX\":4692,\"gridY\":96,\"type\":2},{\"gridX\":4756,\"gridY\":96,\"type\":1},{\"gridX\":4804,\"gridY\":96,\"type\":2},{\"gridX\":4868,\"gridY\":96,\"type\":1},{\"gridX\":4916,\"gridY\":96,\"type\":2},{\"gridX\":4980,\"gridY\":96,\"type\":1},{\"gridX\":5028,\"gridY\":96,\"type\":2},{\"gridX\":5092,\"gridY\":96,\"type\":1},{\"gridX\":5132,\"gridY\":96,\"type\":1},{\"gridX\":5172,\"gridY\":96,\"type\":2},{\"gridX\":5856,\"gridY\":96,\"type\":1},{\"gridX\":5232,\"gridY\":96,\"type\":0},{\"gridX\":5896,\"gridY\":96,\"type\":2},{\"gridX\":5956,\"gridY\":96,\"type\":0},{\"gridX\":5272,\"gridY\":96,\"type\":2},{\"gridX\":5992,\"gridY\":96,\"type\":1},{\"gridX\":6036,\"gridY\":96,\"type\":0},{\"gridX\":6068,\"gridY\":96,\"type\":0},{\"gridX\":6100,\"gridY\":96,\"type\":0},{\"gridX\":2732,\"gridY\":0,\"type\":4},{\"gridX\":2800,\"gridY\":0,\"type\":4},{\"gridX\":2756,\"gridY\":0,\"type\":5},{\"gridX\":2832,\"gridY\":0,\"type\":4},{\"gridX\":2892,\"gridY\":0,\"type\":4},{\"gridX\":2908,\"gridY\":0,\"type\":5},{\"gridX\":2932,\"gridY\":0,\"type\":5},{\"gridX\":2978,\"gridY\":0,\"type\":4},{\"gridX\":3060,\"gridY\":0,\"type\":5},{\"gridX\":3504,\"gridY\":0,\"type\":5},{\"gridX\":3532,\"gridY\":0,\"type\":5},{\"gridX\":3612,\"gridY\":0,\"type\":5},{\"gridX\":3644,\"gridY\":0,\"type\":4},{\"gridX\":3688,\"gridY\":0,\"type\":5},{\"gridX\":3732,\"gridY\":0,\"type\":5},{\"gridX\":3772,\"gridY\":0,\"type\":4},{\"gridX\":3836,\"gridY\":0,\"type\":4},{\"gridX\":3908,\"gridY\":0,\"type\":5},{\"gridX\":4184,\"gridY\":0,\"type\":5},{\"gridX\":4164,\"gridY\":0,\"type\":4},{\"gridX\":4280,\"gridY\":0,\"type\":5},{\"gridX\":4300,\"gridY\":0,\"type\":4},{\"gridX\":4384,\"gridY\":0,\"type\":4},{\"gridX\":4428,\"gridY\":0,\"type\":4},{\"gridX\":4480,\"gridY\":0,\"type\":5},{\"gridX\":4588,\"gridY\":0,\"type\":5},{\"gridX\":4612,\"gridY\":0,\"type\":4},{\"gridX\":4632,\"gridY\":0,\"type\":4},{\"gridX\":4644,\"gridY\":0,\"type\":5},{\"gridX\":4708,\"gridY\":0,\"type\":5},{\"gridX\":4744,\"gridY\":0,\"type\":5},{\"gridX\":4692,\"gridY\":0,\"type\":4},{\"gridX\":4836,\"gridY\":0,\"type\":5},{\"gridX\":4860,\"gridY\":0,\"type\":4},{\"gridX\":4896,\"gridY\":0,\"type\":5},{\"gridX\":4964,\"gridY\":0,\"type\":4},{\"gridX\":5004,\"gridY\":0,\"type\":5},{\"gridX\":5036,\"gridY\":0,\"type\":4},{\"gridX\":4988,\"gridY\":0,\"type\":4},{\"gridX\":2092,\"gridY\":96,\"type\":2},{\"gridX\":2160,\"gridY\":96,\"type\":0},{\"gridX\":3416,\"gridY\":96,\"type\":2},{\"gridX\":3480,\"gridY\":96,\"type\":1},{\"gridX\":3524,\"gridY\":96,\"type\":0},{\"gridX\":5336,\"gridY\":96,\"type\":1},{\"gridX\":5384,\"gridY\":96,\"type\":1},{\"gridX\":5432,\"gridY\":96,\"type\":0},{\"gridX\":5468,\"gridY\":96,\"type\":1},{\"gridX\":5516,\"gridY\":96,\"type\":2},{\"gridX\":5580,\"gridY\":96,\"type\":1},{\"gridX\":5704,\"gridY\":96,\"type\":2},{\"gridX\":5764,\"gridY\":96,\"type\":0},{\"gridX\":5796,\"gridY\":96,\"type\":1},{\"gridX\":5624,\"gridY\":96,\"type\":1},{\"gridX\":5668,\"gridY\":96,\"type\":0},{\"gridX\":5068,\"gridY\":0,\"type\":5},{\"gridX\":5148,\"gridY\":0,\"type\":5},{\"gridX\":5196,\"gridY\":0,\"type\":4},{\"gridX\":5244,\"gridY\":0,\"type\":4},{\"gridX\":5274,\"gridY\":0,\"type\":5},{\"gridX\":5304,\"gridY\":0,\"type\":5},{\"gridX\":5332,\"gridY\":0,\"type\":4},{\"gridX\":5400,\"gridY\":0,\"type\":5},{\"gridX\":5432,\"gridY\":0,\"type\":4},{\"gridX\":5464,\"gridY\":0,\"type\":4},{\"gridX\":5488,\"gridY\":0,\"type\":5},{\"gridX\":5514,\"gridY\":0,\"type\":4},{\"gridX\":5528,\"gridY\":0,\"type\":5},{\"gridX\":5604,\"gridY\":0,\"type\":5},{\"gridX\":5580,\"gridY\":0,\"type\":4},{\"gridX\":5636,\"gridY\":0,\"type\":4},{\"gridX\":5712,\"gridY\":0,\"type\":4},{\"gridX\":5728,\"gridY\":0,\"type\":5},{\"gridX\":5784,\"gridY\":0,\"type\":5},{\"gridX\":5812,\"gridY\":0,\"type\":4},{\"gridX\":5824,\"gridY\":0,\"type\":4},{\"gridX\":1868,\"gridY\":0,\"type\":4},{\"gridX\":1888,\"gridY\":0,\"type\":5},{\"gridX\":1947,\"gridY\":0,\"type\":4},{\"gridX\":1924,\"gridY\":0,\"type\":5},{\"gridX\":1976,\"gridY\":0,\"type\":4},{\"gridX\":1988,\"gridY\":0,\"type\":5},{\"gridX\":2008,\"gridY\":0,\"type\":5},{\"gridX\":2092,\"gridY\":0,\"type\":5},{\"gridX\":2112,\"gridY\":0,\"type\":4},{\"gridX\":4108,\"gridY\":0,\"type\":5},{\"gridX\":1164,\"gridY\":0,\"type\":4},{\"gridX\":1232,\"gridY\":0,\"type\":5},{\"gridX\":1064,\"gridY\":96,\"type\":2},{\"gridX\":1124,\"gridY\":96,\"type\":1},{\"gridX\":1168,\"gridY\":96,\"type\":0},{\"gridX\":2328,\"gridY\":0,\"type\":5},{\"gridX\":2412,\"gridY\":0,\"type\":4},{\"gridX\":2384,\"gridY\":96,\"type\":1},{\"gridX\":2428,\"gridY\":96,\"type\":0},{\"gridX\":3592,\"gridY\":96" << ",\"type\":0},{\"gridX\":3812,\"gridY\":0,\"type\":4},{\"gridX\":248,\"gridY\":96,\"type\":2},{\"gridX\":312,\"gridY\":96,\"type\":1},{\"gridX\":360,\"gridY\":96,\"type\":0},{\"gridX\":396,\"gridY\":96,\"type\":1},{\"gridX\":444,\"gridY\":96,\"type\":2},{\"gridX\":508,\"gridY\":96,\"type\":1},{\"gridX\":556,\"gridY\":96,\"type\":0},{\"gridX\":596,\"gridY\":96,\"type\":1},{\"gridX\":644,\"gridY\":96,\"type\":2},{\"gridX\":708,\"gridY\":96,\"type\":1},{\"gridX\":756,\"gridY\":96,\"type\":0},{\"gridX\":792,\"gridY\":96,\"type\":2},{\"gridX\":852,\"gridY\":96,\"type\":1},{\"gridX\":892,\"gridY\":96,\"type\":0},{\"gridX\":924,\"gridY\":96,\"type\":0},{\"gridX\":956,\"gridY\":96,\"type\":1},{\"gridX\":256,\"gridY\":0,\"type\":5},{\"gridX\":324,\"gridY\":0,\"type\":4},{\"gridX\":360,\"gridY\":0,\"type\":5},{\"gridX\":396,\"gridY\":0,\"type\":5},{\"gridX\":440,\"gridY\":0,\"type\":4},{\"gridX\":516,\"gridY\":0,\"type\":4},{\"gridX\":496,\"gridY\":0,\"type\":5},{\"gridX\":984,\"gridY\":0,\"type\":4},{\"gridX\":1008,\"gridY\":0,\"type\":5}],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":0},{\"gridX\":5832,\"gridY\":0,\"type\":1},{\"gridX\":5640,\"gridY\":0,\"type\":2},{\"gridX\":2440,\"gridY\":0,\"type\":2},{\"gridX\":3784,\"gridY\":0,\"type\":2},{\"gridX\":1416,\"gridY\":0,\"type\":3},{\"gridX\":1544,\"gridY\":0,\"type\":3},{\"gridX\":1672,\"gridY\":0,\"type\":3},{\"gridX\":1800,\"gridY\":0,\"type\":3},{\"gridX\":1928,\"gridY\":0,\"type\":3},{\"gridX\":2184,\"gridY\":0,\"type\":3},{\"gridX\":2312,\"gridY\":0,\"type\":3},{\"gridX\":2504,\"gridY\":0,\"type\":3},{\"gridX\":2632,\"gridY\":0,\"type\":3},{\"gridX\":1288,\"gridY\":0,\"type\":3},{\"gridX\":2760,\"gridY\":0,\"type\":3},{\"gridX\":2888,\"gridY\":0,\"type\":3},{\"gridX\":3144,\"gridY\":0,\"type\":3},{\"gridX\":3528,\"gridY\":0,\"type\":3},{\"gridX\":3656,\"gridY\":0,\"type\":3},{\"gridX\":3976,\"gridY\":0,\"type\":3},{\"gridX\":4104,\"gridY\":0,\"type\":3},{\"gridX\":4232,\"gridY\":0,\"type\":3},{\"gridX\":4360,\"gridY\":0,\"type\":3},{\"gridX\":4488,\"gridY\":0,\"type\":3},{\"gridX\":4616,\"gridY\":0,\"type\":3},{\"gridX\":4744,\"gridY\":0,\"type\":3},{\"gridX\":4872,\"gridY\":0,\"type\":3},{\"gridX\":5000,\"gridY\":0,\"type\":3},{\"gridX\":5128,\"gridY\":0,\"type\":3},{\"gridX\":8,\"gridY\":0,\"type\":3},{\"gridX\":136,\"gridY\":0,\"type\":3},{\"gridX\":264,\"gridY\":0,\"type\":3},{\"gridX\":3272,\"gridY\":0,\"type\":3},{\"gridX\":5256,\"gridY\":0,\"type\":3},{\"gridX\":3016,\"gridY\":0,\"type\":3},{\"gridX\":2056,\"gridY\":0,\"type\":3},{\"gridX\":3400,\"gridY\":0,\"type\":3},{\"gridX\":3848,\"gridY\":0,\"type\":3},{\"gridX\":1160,\"gridY\":0,\"type\":3},{\"gridX\":5384,\"gridY\":0,\"type\":3},{\"gridX\":5512,\"gridY\":0,\"type\":3},{\"gridX\":5704,\"gridY\":0,\"type\":3},{\"gridX\":392,\"gridY\":0,\"type\":3},{\"gridX\":520,\"gridY\":0,\"type\":3},{\"gridX\":648,\"gridY\":0,\"type\":3},{\"gridX\":776,\"gridY\":0,\"type\":3},{\"gridX\":904,\"gridY\":0,\"type\":3},{\"gridX\":1032,\"gridY\":0,\"type\":3},{\"gridX\":4568,\"gridY\":50,\"type\":10},{\"gridX\":5704,\"gridY\":50,\"type\":10},{\"gridX\":4824,\"gridY\":50,\"type\":10},{\"gridX\":2240,\"gridY\":50,\"type\":10},{\"gridX\":5776,\"gridY\":50,\"type\":10},{\"gridX\":1860,\"gridY\":50,\"type\":10},{\"gridX\":3488,\"gridY\":50,\"type\":10},{\"gridX\":2084,\"gridY\":50,\"type\":10},{\"gridX\":3608,\"gridY\":50,\"type\":10},{\"gridX\":1428,\"gridY\":50,\"type\":10},{\"gridX\":5136,\"gridY\":50,\"type\":10},{\"gridX\":140,\"gridY\":50,\"type\":10},{\"gridX\":2696,\"gridY\":50,\"type\":10},{\"gridX\":4064,\"gridY\":50,\"type\":10},{\"gridX\":5392,\"gridY\":50,\"type\":10},{\"gridX\":4256,\"gridY\":50,\"type\":10},{\"gridX\":632,\"gridY\":50,\"type\":10},{\"gridX\":972,\"gridY\":50,\"type\":10},{\"gridX\":852,\"gridY\":50,\"type\":10},{\"gridX\":5784,\"gridY\":50,\"type\":11},{\"gridX\":2092,\"gridY\":50,\"type\":11},{\"gridX\":5712,\"gridY\":50,\"type\":11},{\"gridX\":5808,\"gridY\":50,\"type\":11},{\"gridX\":1996,\"gridY\":50,\"type\":11},{\"gridX\":4072,\"gridY\":50,\"type\":12},{\"gridX\":4136,\"gridY\":50,\"type\":12},{\"gridX\":4576,\"gridY\":50,\"type\":12},{\"gridX\":4640,\"gridY\":50,\"type\":12},{\"gridX\":4704,\"gridY\":50,\"type\":12},{\"gridX\":5272,\"gridY\":50,\"type\":12},{\"gridX\":5144,\"gridY\":50,\"type\":12},{\"gridX\":2376,\"gridY\":50,\"type\":12},{\"gridX\":3496,\"gridY\":50,\"type\":12},{\"gridX\":5208,\"gridY\":50,\"type\":12},{\"gridX\":980,\"gridY\":50,\"type\":12},{\"gridX\":860,\"gridY\":50,\"type\":12},{\"gridX\":3744,\"gridY\":50,\"type\":12},{\"gridX\":1436,\"gridY\":50,\"type\":13},{\"gridX\":1692,\"gridY\":50,\"type\":13},{\"gridX\":148,\"gridY\":50,\"type\":13},{\"gridX\":3808,\"gridY\":50,\"type\":13},{\"gridX\":4960,\"gridY\":50,\"type\":13},{\"gridX\":2248,\"gridY\":50,\"type\":13},{\"gridX\":4832,\"gridY\":50,\"type\":13},{\"gridX\":1044,\"gridY\":50,\"type\":13},{\"gridX\":2832,\"gridY\":50,\"type\":13},{\"gridX\":2440,\"gridY\":50,\"type\":13},{\"gridX\":5400,\"gridY\":50,\"type\":13},{\"gridX\":5528,\"gridY\":50,\"type\":13},{\"gridX\":276,\"gridY\":50,\"type\":13},{\"gridX\":404,\"gridY\":50,\"type\":13},{\"gridX\":2704,\"gridY\":50,\"type\":13},{\"gridX\":640,\"gridY\":50,\"type\":13},{\"gridX\":4264,\"gridY\":50,\"type\":13},{\"gridX\":1868,\"gridY\":50,\"type\":13},{\"gridX\":2960,\"gridY\":50,\"type\":13},{\"gridX\":3616,\"gridY\":50,\"type\":13},{\"gridX\":1172,\"gridY\":50,\"type\":13},{\"gridX\":4392,\"gridY\":50,\"type\":13},{\"gridX\":1564,\"gridY\":50,\"type\":13},{\"gridX\":5744,\"gridY\":50,\"type\":14},{\"gridX\":5336,\"gridY\":50,\"type\":14},{\"gridX\":2028,\"gridY\":50,\"type\":14},{\"gridX\":4200,\"gridY\":50,\"type\":14},{\"gridX\":924,\"gridY\":50,\"type\":14},{\"gridX\":4768,\"gridY\":50,\"type\":14},{\"gridX\":2124,\"gridY\":50,\"type\":14},{\"gridX\":768,\"gridY\":50,\"type\":14},{\"gridX\":4252,\"gridY\":48,\"type\":15},{\"gridX\":136,\"gridY\":48,\"type\":15},{\"gridX\":5200,\"gridY\":48,\"type\":15},{\"gridX\":5829,\"gridY\":48,\"type\":15},{\"gridX\":4060,\"gridY\":48,\"type\":15},{\"gridX\":1424,\"gridY\":48,\"type\":15},{\"gridX\":2236,\"gridY\":48,\"type\":15},{\"gridX\":968,\"gridY\":48,\"type\":15},{\"gridX\":2692,\"gridY\":48,\"type\":15},{\"gridX\":4820,\"gridY\":48,\"type\":15},{\"gridX\":3604,\"gridY\":48,\"type\":15},{\"gridX\":4636,\"gridY\":48,\"type\":15},{\"gridX\":5388,\"gridY\":48,\"type\":15},{\"gridX\":3484,\"gridY\":48,\"type\":15},{\"gridX\":4644,\"gridY\":48,\"type\":17},{\"gridX\":5208,\"gridY\":48,\"type\":17},{\"gridX\":3492,\"gridY\":48,\"type\":17},{\"gridX\":2372,\"gridY\":48,\"type\":17},{\"gridX\":3740,\"gridY\":48,\"type\":17},{\"gridX\":4068,\"gridY\":48,\"type\":17},{\"gridX\":976,\"gridY\":48,\"type\":17},{\"gridX\":4388,\"gridY\":48,\"type\":18},{\"gridX\":4828,\"gridY\":48,\"type\":18},{\"gridX\":4956,\"gridY\":48,\"type\":18},{\"gridX\":1040,\"gridY\":48,\"type\":18},{\"gridX\":2244,\"gridY\":48,\"type\":18},{\"gridX\":5396,\"gridY\":48,\"type\":18},{\"gridX\":5524,\"gridY\":48,\"type\":18},{\"gridX\":2436,\"gridY\":48,\"type\":18},{\"gridX\":4260,\"gridY\":48,\"type\":18},{\"gridX\":400,\"gridY\":48,\"type\":18},{\"gridX\":1688,\"gridY\":48,\"type\":18},{\"gridX\":3612,\"gridY\":48,\"type\":18},{\"gridX\":3804,\"gridY\":48,\"type\":18},{\"gridX\":1432,\"gridY\":48,\"type\":18},{\"gridX\":144,\"gridY\":48,\"type\":18},{\"gridX\":1560,\"gridY\":48,\"type\":18},{\"gridX\":2956,\"gridY\":48,\"type\":18},{\"gridX\":272,\"gridY\":48,\"type\":18},{\"gridX\":2700,\"gridY\":48,\"type\":18},{\"gridX\":1168,\"gridY\":48,\"type\":18},{\"gridX\":2828,\"gridY\":48,\"type\":18},{\"gridX\":3556,\"gridY\":48,\"type\":19},{\"gridX\":528,\"gridY\":48,\"type\":19},{\"gridX\":4132,\"gridY\":48,\"type\":19},{\"gridX\":3084,\"gridY\":48,\"type\":19},{\"gridX\":1816,\"gridY\":48,\"type\":19},{\"gridX\":1296,\"gridY\":48,\"type\":19},{\"gridX\":4708,\"gridY\":48,\"type\":19},{\"gridX\":3932,\"gridY\":48,\"type\":19},{\"gridX\":4516,\"gridY\":48,\"type\":19},{\"gridX\":5272" << ",\"gridY\":48,\"type\":19},{\"gridX\":5652,\"gridY\":48,\"type\":19},{\"gridX\":5084,\"gridY\":48,\"type\":19},{\"gridX\":5836,\"gridY\":48,\"type\":19},{\"gridX\":2564,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":3784,\"gridY\":90,\"type\":22},{\"gridX\":5640,\"gridY\":90,\"type\":22},{\"gridX\":2440,\"gridY\":90,\"type\":22},{\"gridX\":1544,\"gridY\":90,\"type\":23},{\"gridX\":3656,\"gridY\":90,\"type\":23},{\"gridX\":3848,\"gridY\":90,\"type\":23},{\"gridX\":2312,\"gridY\":90,\"type\":23},{\"gridX\":2632,\"gridY\":90,\"type\":23},{\"gridX\":1672,\"gridY\":90,\"type\":23},{\"gridX\":4104,\"gridY\":90,\"type\":23},{\"gridX\":3976,\"gridY\":90,\"type\":23},{\"gridX\":5256,\"gridY\":90,\"type\":23},{\"gridX\":3528,\"gridY\":90,\"type\":23},{\"gridX\":1416,\"gridY\":90,\"type\":23},{\"gridX\":1160,\"gridY\":90,\"type\":23},{\"gridX\":2504,\"gridY\":90,\"type\":23},{\"gridX\":4616,\"gridY\":90,\"type\":23},{\"gridX\":2760,\"gridY\":90,\"type\":23},{\"gridX\":4744,\"gridY\":90,\"type\":23},{\"gridX\":4232,\"gridY\":90,\"type\":23},{\"gridX\":4360,\"gridY\":90,\"type\":23},{\"gridX\":5128,\"gridY\":90,\"type\":23},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":4488,\"gridY\":90,\"type\":23},{\"gridX\":2056,\"gridY\":90,\"type\":23},{\"gridX\":264,\"gridY\":90,\"type\":23},{\"gridX\":3272,\"gridY\":90,\"type\":23},{\"gridX\":5384,\"gridY\":90,\"type\":23},{\"gridX\":3016,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":90,\"type\":23},{\"gridX\":2888,\"gridY\":90,\"type\":23},{\"gridX\":2184,\"gridY\":90,\"type\":23},{\"gridX\":3400,\"gridY\":90,\"type\":23},{\"gridX\":1928,\"gridY\":90,\"type\":23},{\"gridX\":1800,\"gridY\":90,\"type\":23},{\"gridX\":392,\"gridY\":90,\"type\":23},{\"gridX\":520,\"gridY\":90,\"type\":23},{\"gridX\":648,\"gridY\":90,\"type\":23},{\"gridX\":776,\"gridY\":90,\"type\":23},{\"gridX\":904,\"gridY\":90,\"type\":23},{\"gridX\":1032,\"gridY\":90,\"type\":23},{\"gridX\":4872,\"gridY\":90,\"type\":23},{\"gridX\":1288,\"gridY\":90,\"type\":23},{\"gridX\":3144,\"gridY\":90,\"type\":23},{\"gridX\":5704,\"gridY\":90,\"type\":23},{\"gridX\":5000,\"gridY\":90,\"type\":23},{\"gridX\":5512,\"gridY\":90,\"type\":23},{\"gridX\":5856,\"gridY\":0,\"type\":25},{\"gridX\":5864,\"gridY\":0,\"type\":28},{\"gridX\":5992,\"gridY\":0,\"type\":28},{\"gridX\":6120,\"gridY\":0,\"type\":28},{\"gridX\":6248,\"gridY\":0,\"type\":29}],\"pits\":[],\"exitGrounds\":[{\"gridX\":5832,\"gridY\":88,\"type\":0}],\"holes\":[{\"gridX\":120,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":184,\"gridY\":128,\"type\":0},{\"gridX\":188,\"gridY\":128,\"type\":1},{\"gridX\":202,\"gridY\":128,\"type\":1},{\"gridX\":244,\"gridY\":128,\"type\":2},{\"gridX\":1428,\"gridY\":75,\"type\":27},{\"gridX\":1428,\"gridY\":84,\"type\":30},{\"gridX\":230,\"gridY\":128,\"type\":1},{\"gridX\":216,\"gridY\":128,\"type\":1},{\"gridX\":1384,\"gridY\":48,\"type\":34},{\"gridX\":1400,\"gridY\":60,\"type\":34},{\"gridX\":1412,\"gridY\":52,\"type\":34},{\"gridX\":1320,\"gridY\":56,\"type\":34},{\"gridX\":1308,\"gridY\":48,\"type\":34},{\"gridX\":1332,\"gridY\":48,\"type\":34},{\"gridX\":140,\"gridY\":84,\"type\":30},{\"gridX\":140,\"gridY\":75,\"type\":27},{\"gridX\":176,\"gridY\":84,\"type\":30},{\"gridX\":176,\"gridY\":75,\"type\":27},{\"gridX\":212,\"gridY\":84,\"type\":30},{\"gridX\":212,\"gridY\":75,\"type\":27},{\"gridX\":2288,\"gridY\":96,\"type\":36},{\"gridX\":2240,\"gridY\":75,\"type\":27},{\"gridX\":2240,\"gridY\":84,\"type\":30},{\"gridX\":3120,\"gridY\":48,\"type\":3},{\"gridX\":3124,\"gridY\":48,\"type\":4},{\"gridX\":3138,\"gridY\":48,\"type\":4},{\"gridX\":3152,\"gridY\":48,\"type\":5},{\"gridX\":3220,\"gridY\":48,\"type\":3},{\"gridX\":3224,\"gridY\":48,\"type\":4},{\"gridX\":1896,\"gridY\":70,\"type\":3},{\"gridX\":1900,\"gridY\":70,\"type\":4},{\"gridX\":1914,\"gridY\":70,\"type\":5},{\"gridX\":1976,\"gridY\":70,\"type\":3},{\"gridX\":1980,\"gridY\":70,\"type\":4},{\"gridX\":1994,\"gridY\":70,\"type\":5},{\"gridX\":2576,\"gridY\":48,\"type\":34},{\"gridX\":2600,\"gridY\":48,\"type\":34},{\"gridX\":2588,\"gridY\":56,\"type\":34},{\"gridX\":2652,\"gridY\":48,\"type\":34},{\"gridX\":2664,\"gridY\":64,\"type\":34},{\"gridX\":2676,\"gridY\":56,\"type\":34},{\"gridX\":3238,\"gridY\":48,\"type\":4},{\"gridX\":3252,\"gridY\":48,\"type\":5},{\"gridX\":2232,\"gridY\":59,\"type\":32},{\"gridX\":2696,\"gridY\":84,\"type\":30},{\"gridX\":2696,\"gridY\":75,\"type\":27},{\"gridX\":3320,\"gridY\":48,\"type\":3},{\"gridX\":3324,\"gridY\":48,\"type\":4},{\"gridX\":3338,\"gridY\":48,\"type\":4},{\"gridX\":3352,\"gridY\":48,\"type\":5},{\"gridX\":3652,\"gridY\":96,\"type\":36},{\"gridX\":3608,\"gridY\":75,\"type\":27},{\"gridX\":3608,\"gridY\":84,\"type\":30},{\"gridX\":3944,\"gridY\":48,\"type\":34},{\"gridX\":3968,\"gridY\":48,\"type\":34},{\"gridX\":3956,\"gridY\":56,\"type\":34},{\"gridX\":4020,\"gridY\":48,\"type\":34},{\"gridX\":4036,\"gridY\":60,\"type\":34},{\"gridX\":4048,\"gridY\":52,\"type\":34},{\"gridX\":4064,\"gridY\":75,\"type\":27},{\"gridX\":4064,\"gridY\":84,\"type\":30},{\"gridX\":4256,\"gridY\":84,\"type\":30},{\"gridX\":4256,\"gridY\":75,\"type\":27},{\"gridX\":4632,\"gridY\":58,\"type\":32},{\"gridX\":4640,\"gridY\":84,\"type\":30},{\"gridX\":4640,\"gridY\":75,\"type\":27},{\"gridX\":5932,\"gridY\":96,\"type\":12},{\"gridX\":1812,\"gridY\":24,\"type\":3},{\"gridX\":1830,\"gridY\":24,\"type\":5},{\"gridX\":1816,\"gridY\":24,\"type\":4},{\"gridX\":3420,\"gridY\":48,\"type\":3},{\"gridX\":3424,\"gridY\":48,\"type\":4},{\"gridX\":3438,\"gridY\":48,\"type\":5},{\"gridX\":5844,\"gridY\":7,\"type\":20},{\"gridX\":4264,\"gridY\":44,\"type\":30},{\"gridX\":4300,\"gridY\":44,\"type\":30},{\"gridX\":4336,\"gridY\":44,\"type\":30},{\"gridX\":4372,\"gridY\":44,\"type\":30},{\"gridX\":4408,\"gridY\":44,\"type\":30},{\"gridX\":4444,\"gridY\":44,\"type\":30},{\"gridX\":4480,\"gridY\":44,\"type\":30},{\"gridX\":4740,\"gridY\":44,\"type\":30},{\"gridX\":4832,\"gridY\":44,\"type\":30},{\"gridX\":4824,\"gridY\":84,\"type\":30},{\"gridX\":4824,\"gridY\":75,\"type\":27},{\"gridX\":4868,\"gridY\":44,\"type\":30},{\"gridX\":4904,\"gridY\":44,\"type\":30},{\"gridX\":4940,\"gridY\":44,\"type\":30},{\"gridX\":4976,\"gridY\":44,\"type\":30},{\"gridX\":5012,\"gridY\":44,\"type\":30},{\"gridX\":5048,\"gridY\":44,\"type\":30},{\"gridX\":5308,\"gridY\":44,\"type\":30},{\"gridX\":5204,\"gridY\":84,\"type\":30},{\"gridX\":5204,\"gridY\":75,\"type\":27},{\"gridX\":5196,\"gridY\":58,\"type\":32},{\"gridX\":5392,\"gridY\":84,\"type\":30},{\"gridX\":5392,\"gridY\":75,\"type\":27},{\"gridX\":5400,\"gridY\":44,\"type\":30},{\"gridX\":5436,\"gridY\":44,\"type\":30},{\"gridX\":5472,\"gridY\":44,\"type\":30},{\"gridX\":5508,\"gridY\":44,\"type\":30},{\"gridX\":5544,\"gridY\":44,\"type\":30},{\"gridX\":5580,\"gridY\":44,\"type\":30},{\"gridX\":5616,\"gridY\":44,\"type\":30},{\"gridX\":4513,\"gridY\":44,\"type\":28},{\"gridX\":5081,\"gridY\":44,\"type\":28},{\"gridX\":5649,\"gridY\":44,\"type\":28},{\"gridX\":552,\"gridY\":24,\"type\":3},{\"gridX\":556,\"gridY\":24,\"type\":4},{\"gridX\":570,\"gridY\":24,\"type\":4},{\"gridX\":584,\"gridY\":24,\"type\":5},{\"gridX\":804,\"gridY\":38,\"type\":4},{\"gridX\":800,\"gridY\":38,\"type\":3},{\"gridX\":818,\"gridY\":38,\"type\":5},{\"gridX\":972,\"gridY\":75,\"type\":27},{\"gridX\":972,\"gridY\":84,\"type\":30},{\"gridX\":1028,\"gridY\":96,\"type\":36},{\"gridX\":628,\"gridY\":84,\"type\":30},{\"gridX\":752,\"gridY\":84,\"type\":30},{\"gridX\":848,\"gridY\":84,\"type\":30},{\"gridX\":908,\"gridY\":84,\"type\":30},{\"gridX\":3480,\"gridY\":59,\"type\":32},{\"gridX\":3174,\"gridY\":84,\"type\":30},{\"gridX\":3274,\"gridY\":84,\"type\":30},{\"gridX\":3374,\"gridY\":84,\"type\":30}],\"midBossForegroundObjects\":[],\"endBossForegroundObjects\":[{\"gridX\":1432,\"gridY\":7,\"type\":45},{\"gridX\":1345,\"gridY\":66,\"type\":44},{\"gridX\":2700,\"gridY\":7,\"type\":45},{\"gridX\":2613,\"gridY\":66,\"type\":44},{\"gridX\":4184,\"gridY\":55" << ",\"type\":43},{\"gridX\":4068,\"gridY\":7,\"type\":45},{\"gridX\":3981,\"gridY\":66,\"type\":44},{\"gridX\":4568,\"gridY\":55,\"type\":42},{\"gridX\":4752,\"gridY\":55,\"type\":43},{\"gridX\":5136,\"gridY\":55,\"type\":42},{\"gridX\":5320,\"gridY\":55,\"type\":43},{\"gridX\":5704,\"gridY\":55,\"type\":42}],\"countHissWithMinas\":[{\"gridX\":180,\"gridY\":133}],\"endBossSnakes\":[{\"gridX\":196,\"gridY\":96}],\"enemies\":[],\"collectibles\":[{\"gridX\":1828,\"gridY\":40,\"type\":0},{\"gridX\":1836,\"gridY\":48,\"type\":0},{\"gridX\":1408,\"gridY\":16,\"type\":0},{\"gridX\":1416,\"gridY\":16,\"type\":0},{\"gridX\":1400,\"gridY\":16,\"type\":0},{\"gridX\":300,\"gridY\":16,\"type\":0},{\"gridX\":284,\"gridY\":16,\"type\":0},{\"gridX\":292,\"gridY\":16,\"type\":0},{\"gridX\":1208,\"gridY\":16,\"type\":0},{\"gridX\":1192,\"gridY\":16,\"type\":0},{\"gridX\":1200,\"gridY\":16,\"type\":0},{\"gridX\":1572,\"gridY\":16,\"type\":0},{\"gridX\":1728,\"gridY\":16,\"type\":0},{\"gridX\":1580,\"gridY\":16,\"type\":0},{\"gridX\":1588,\"gridY\":16,\"type\":0},{\"gridX\":1720,\"gridY\":16,\"type\":0},{\"gridX\":1712,\"gridY\":16,\"type\":0},{\"gridX\":1868,\"gridY\":64,\"type\":0},{\"gridX\":1908,\"gridY\":80,\"type\":0},{\"gridX\":1944,\"gridY\":64,\"type\":0},{\"gridX\":1980,\"gridY\":80,\"type\":0},{\"gridX\":2224,\"gridY\":76,\"type\":1},{\"gridX\":3556,\"gridY\":80,\"type\":1},{\"gridX\":2308,\"gridY\":16,\"type\":0},{\"gridX\":2316,\"gridY\":16,\"type\":0},{\"gridX\":2300,\"gridY\":16,\"type\":0},{\"gridX\":2472,\"gridY\":16,\"type\":0},{\"gridX\":2480,\"gridY\":16,\"type\":0},{\"gridX\":2488,\"gridY\":16,\"type\":0},{\"gridX\":2668,\"gridY\":16,\"type\":0},{\"gridX\":2676,\"gridY\":16,\"type\":0},{\"gridX\":2684,\"gridY\":16,\"type\":0},{\"gridX\":3128,\"gridY\":60,\"type\":0},{\"gridX\":3136,\"gridY\":60,\"type\":0},{\"gridX\":3144,\"gridY\":60,\"type\":0},{\"gridX\":3228,\"gridY\":60,\"type\":0},{\"gridX\":3236,\"gridY\":60,\"type\":0},{\"gridX\":3244,\"gridY\":60,\"type\":0},{\"gridX\":3328,\"gridY\":60,\"type\":0},{\"gridX\":3336,\"gridY\":60,\"type\":0},{\"gridX\":3344,\"gridY\":60,\"type\":0},{\"gridX\":2864,\"gridY\":16,\"type\":0},{\"gridX\":2872,\"gridY\":16,\"type\":0},{\"gridX\":2880,\"gridY\":16,\"type\":0},{\"gridX\":3032,\"gridY\":16,\"type\":0},{\"gridX\":3040,\"gridY\":16,\"type\":0},{\"gridX\":3048,\"gridY\":16,\"type\":0},{\"gridX\":3176,\"gridY\":44,\"type\":0},{\"gridX\":3184,\"gridY\":40,\"type\":0},{\"gridX\":3192,\"gridY\":44,\"type\":0},{\"gridX\":3276,\"gridY\":44,\"type\":0},{\"gridX\":3284,\"gridY\":40,\"type\":0},{\"gridX\":3292,\"gridY\":44,\"type\":0},{\"gridX\":3664,\"gridY\":16,\"type\":0},{\"gridX\":3672,\"gridY\":16,\"type\":0},{\"gridX\":3680,\"gridY\":16,\"type\":0},{\"gridX\":4052,\"gridY\":16,\"type\":0},{\"gridX\":4044,\"gridY\":16,\"type\":0},{\"gridX\":4036,\"gridY\":16,\"type\":0},{\"gridX\":4340,\"gridY\":16,\"type\":0},{\"gridX\":4348,\"gridY\":16,\"type\":0},{\"gridX\":4356,\"gridY\":16,\"type\":0},{\"gridX\":4684,\"gridY\":16,\"type\":0},{\"gridX\":4676,\"gridY\":16,\"type\":0},{\"gridX\":4668,\"gridY\":16,\"type\":0},{\"gridX\":4940,\"gridY\":16,\"type\":0},{\"gridX\":4924,\"gridY\":16,\"type\":0},{\"gridX\":4932,\"gridY\":16,\"type\":0},{\"gridX\":5236,\"gridY\":16,\"type\":0},{\"gridX\":5228,\"gridY\":16,\"type\":0},{\"gridX\":5220,\"gridY\":16,\"type\":0},{\"gridX\":2048,\"gridY\":48,\"type\":0},{\"gridX\":2056,\"gridY\":44,\"type\":0},{\"gridX\":2064,\"gridY\":48,\"type\":0},{\"gridX\":3376,\"gridY\":44,\"type\":0},{\"gridX\":3384,\"gridY\":40,\"type\":0},{\"gridX\":3392,\"gridY\":44,\"type\":0},{\"gridX\":5836,\"gridY\":80,\"type\":1},{\"gridX\":3872,\"gridY\":16,\"type\":0},{\"gridX\":3880,\"gridY\":16,\"type\":0},{\"gridX\":3888,\"gridY\":16,\"type\":0},{\"gridX\":5552,\"gridY\":16,\"type\":0},{\"gridX\":5560,\"gridY\":16,\"type\":0},{\"gridX\":5568,\"gridY\":16,\"type\":0},{\"gridX\":468,\"gridY\":16,\"type\":0},{\"gridX\":476,\"gridY\":16,\"type\":0},{\"gridX\":484,\"gridY\":16,\"type\":0},{\"gridX\":688,\"gridY\":64,\"type\":0},{\"gridX\":704,\"gridY\":64,\"type\":0},{\"gridX\":696,\"gridY\":64,\"type\":0},{\"gridX\":808,\"gridY\":52,\"type\":0},{\"gridX\":880,\"gridY\":64,\"type\":0},{\"gridX\":888,\"gridY\":64,\"type\":0},{\"gridX\":896,\"gridY\":64,\"type\":0},{\"gridX\":600,\"gridY\":52,\"type\":0},{\"gridX\":592,\"gridY\":48,\"type\":0},{\"gridX\":608,\"gridY\":56,\"type\":0},{\"gridX\":2096,\"gridY\":64,\"type\":0},{\"gridX\":2104,\"gridY\":64,\"type\":0},{\"gridX\":2112,\"gridY\":64,\"type\":0},{\"gridX\":1040,\"gridY\":20,\"type\":0},{\"gridX\":1048,\"gridY\":20,\"type\":0},{\"gridX\":1056,\"gridY\":20,\"type\":0}],\"jons\":[{\"gridX\":12,\"gridY\":104}],\"extraForegroundObjects\":[],\"markers\":[]}";
    
    static std::string text = ss.str();
    
    static Chapter1Level21 *instance = new Chapter1Level21(text.c_str());
    
    return instance;
}
