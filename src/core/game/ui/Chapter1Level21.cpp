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
    
    if (m_game->getEndBossSnakes().size() > 0)
    {
        m_endBossSnake = m_game->getEndBossSnakeP();
    }
    
    m_game->getCountHissWithMina().faceLeft();

	Jon& jon = m_game->getJon(); 
	if (m_hasTriggeredMusicLoop)
	{
		jon.getPosition().set(m_fCheckPointX, m_fCheckPointY);

		m_game->setStateTime(m_fGameStateTime);

		jon.setUserActionPrevented(false);
		jon.becomeVampire();

		if (m_endBossSnake)
		{
			m_endBossSnake->beginPursuit();
		}
	}
	else
	{
		jon.setUserActionPrevented(true);
	}
}

void Chapter1Level21::exit(GameScreen* gs)
{
	m_endBossSnake = nullptr;
    m_hole = nullptr;

	m_fGameStateTime = 0;
	m_fCheckPointX = 0;
	m_fCheckPointY = 0;
	m_isChaseCamActivated = false;
	m_hasTriggeredMusicLoopIntro = false;
	m_hasTriggeredSnakeAwaken = false;
	m_hasTriggeredMusicLoop = false;
    
    Level::exit(gs);
}

void Chapter1Level21::beginOpeningSequence(GameScreen* gs)
{
	Jon& jon = m_game->getJon();
	jon.setAllowedToMove(false);
	jon.beginWarmingUp();

	updateCamera(gs, 0, false, true);

	EntityUtils::updateBackgrounds(m_game->getBackgroundUppers(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundMids(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundLowers(), gs->m_renderer->getCameraPosition(), 0);
	EntityUtils::updateBackgrounds(m_game->getBackgroundMidgroundCovers(), gs->m_renderer->getCameraPosition(), 0);

	m_hasShownOpeningSequence = true;
}

void Chapter1Level21::handleOpeningSequence(GameScreen* gs)
{
	Jon& jon = m_game->getJon();
	jon.setAllowedToMove(true);
	jon.getVelocity().setX(RABBIT_DEFAULT_MAX_SPEED);

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
    
    if (m_endBossSnake->getState() == EndBossSnakeState_Sleeping)
    {
        if (jon.getPosition().getX() > m_hole->getPosition().getX())
        {
			jon.getAcceleration().set(0, 0);
			jon.getVelocity().set(0, 0);
			jon.setIdle(true);
			jon.setUserActionPrevented(true);

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

			if (m_fGameStateTime > 4)
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
		m_fGameStateTime += gs->m_fDeltaTime;

		if (m_hasTriggeredSnakeAwaken
			&& !m_hasTriggeredMusicLoop)
		{
			m_fGameStateTime += gs->m_fDeltaTime;

			if (m_fGameStateTime > 7.15f)
			{
				Assets::getInstance()->setMusicId(MUSIC_PLAY_END_BOSS_LOOP);

				m_hasTriggeredMusicLoop = true;
			}
		}

		if (jon.isVampire()
			&& jon.getPhysicalState() == PHYSICAL_GROUNDED)
		{
			jon.setIdle(false);
			jon.setUserActionPrevented(false);

			m_fCheckPointX = jon.getPosition().getX();
			m_fCheckPointY = jon.getPosition().getY();
			m_fCheckPointStateTime = m_game->getStateTime();

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
	else if (m_endBossSnake->getState() == EndBossSnakeState_Pursuing
		|| m_endBossSnake->getState() == EndBossSnakeState_OpeningMouthRight
		|| m_endBossSnake->getState() == EndBossSnakeState_OpenMouthRight
		|| m_endBossSnake->getState() == EndBossSnakeState_ChargingRight)
	{
		m_isChaseCamActivated = true;
	}
}

void Chapter1Level21::updateCamera(GameScreen* gs, float paddingX, bool ignoreY, bool instant)
{
    if (m_isChaseCamActivated)
    {
		if (m_endBossSnake->getState() == EndBossSnakeState_Damaged
			|| m_endBossSnake->getState() == EndBossSnakeState_ChargingRight)
		{
			paddingX = 5;
		}

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
m_isChaseCamActivated(false),
m_hasTriggeredMusicLoopIntro(false),
m_hasTriggeredSnakeAwaken(false),
m_hasTriggeredMusicLoop(false)
{
    // Empty
}

/// Chapter 1 Level 21 ///

Chapter1Level21 * Chapter1Level21::getInstance()
{
    static Chapter1Level21 *instance = new Chapter1Level21("{\"world\":1,\"level\":21,\"midgrounds\":[],\"grounds\":[{\"gridX\":0,\"gridY\":0,\"type\":0},{\"gridX\":8,\"gridY\":0,\"type\":3},{\"gridX\":136,\"gridY\":0,\"type\":3},{\"gridX\":264,\"gridY\":0,\"type\":3},{\"gridX\":392,\"gridY\":0,\"type\":3},{\"gridX\":520,\"gridY\":0,\"type\":3},{\"gridX\":648,\"gridY\":0,\"type\":3},{\"gridX\":776,\"gridY\":0,\"type\":3},{\"gridX\":904,\"gridY\":0,\"type\":3},{\"gridX\":1032,\"gridY\":0,\"type\":3},{\"gridX\":1160,\"gridY\":0,\"type\":3},{\"gridX\":1288,\"gridY\":0,\"type\":3},{\"gridX\":1416,\"gridY\":0,\"type\":3},{\"gridX\":148,\"gridY\":48,\"type\":10},{\"gridX\":156,\"gridY\":48,\"type\":13},{\"gridX\":284,\"gridY\":48,\"type\":13},{\"gridX\":412,\"gridY\":48,\"type\":13},{\"gridX\":540,\"gridY\":48,\"type\":13},{\"gridX\":144,\"gridY\":48,\"type\":15},{\"gridX\":152,\"gridY\":48,\"type\":18},{\"gridX\":280,\"gridY\":48,\"type\":18},{\"gridX\":408,\"gridY\":48,\"type\":18},{\"gridX\":536,\"gridY\":48,\"type\":18},{\"gridX\":664,\"gridY\":48,\"type\":19},{\"gridX\":0,\"gridY\":90,\"type\":20},{\"gridX\":8,\"gridY\":90,\"type\":23},{\"gridX\":136,\"gridY\":88,\"type\":23},{\"gridX\":264,\"gridY\":88,\"type\":23},{\"gridX\":392,\"gridY\":88,\"type\":23},{\"gridX\":520,\"gridY\":88,\"type\":23},{\"gridX\":648,\"gridY\":88,\"type\":23},{\"gridX\":776,\"gridY\":88,\"type\":23},{\"gridX\":904,\"gridY\":88,\"type\":23},{\"gridX\":1032,\"gridY\":88,\"type\":23},{\"gridX\":1160,\"gridY\":88,\"type\":23},{\"gridX\":1288,\"gridY\":88,\"type\":23},{\"gridX\":1416,\"gridY\":88,\"type\":23}],\"pits\":[],\"exitGrounds\":[],\"holes\":[{\"gridX\":128,\"gridY\":80,\"type\":3}],\"foregroundObjects\":[{\"gridX\":200,\"gridY\":132,\"type\":0},{\"gridX\":204,\"gridY\":132,\"type\":1},{\"gridX\":218,\"gridY\":132,\"type\":1},{\"gridX\":232,\"gridY\":132,\"type\":2},{\"gridX\":1536,\"gridY\":96,\"type\":12},{\"gridX\":148,\"gridY\":75,\"type\":27},{\"gridX\":148,\"gridY\":84,\"type\":30},{\"gridX\":184,\"gridY\":75,\"type\":27},{\"gridX\":184,\"gridY\":84,\"type\":30},{\"gridX\":220,\"gridY\":75,\"type\":27},{\"gridX\":220,\"gridY\":84,\"type\":30}],\"midBossForegroundObjects\":[],\"countHissWithMinas\":[{\"gridX\":200,\"gridY\":137}],\"enemies\":[],\"collectibles\":[],\"jons\":[{\"gridX\":24,\"gridY\":108}],\"extraForegroundObjects\":[],\"markers\":[]}");
    
    return instance;
}
