//
//  EndBossSnake.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/24/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "EndBossSnake.h"
#include "Game.h"
#include "OverlapTester.h"
#include "Assets.h"
#include "Jon.h"
#include "macros.h"
#include "GameConstants.h"

#include <math.h>

#define END_BOSS_SNAKE_DEFAULT_MAX_SPEED VAMP_DEFAULT_MAX_SPEED + 1.4f
#define END_BOSS_SNAKE_DEFAULT_ACCELERATION VAMP_DEFAULT_ACCELERATION

EndBossSnake::EndBossSnake(int gridX, int gridY) : GridLockedPhysicalEntity(gridX, gridY, 54, 34, 0.25f, 0.12f, 0.5f, 0.88f),
m_game(nullptr),
m_state(EndBossSnakeState_Sleeping),
m_color(1, 1, 1, 1),
m_iDamage(0),
m_type(-1)
{
    float x = m_position->getX();
    float y = m_position->getY();
    float w = m_fWidth;
    float h = m_fHeight;
	float l = x - w / 2;
	float b = y - (h / 2);
    
    m_snakeSkin = std::unique_ptr<SnakeSkin>(new SnakeSkin(x, y, w, h, this));
    m_snakeEye = std::unique_ptr<SnakeEye>(new SnakeEye(0.52083333333333 * w + l, 0.65073529411765 * h + b, this));
    m_snakeTonque = std::unique_ptr<SnakeTonque>(new SnakeTonque(x, y, this));
    m_snakeBody = std::unique_ptr<SnakeBody>(new SnakeBody(x, y, h, this));
	m_snakeHeadImpact = std::unique_ptr<SnakeHeadImpact>(new SnakeHeadImpact(x, y, this));
}

void EndBossSnake::update(float deltaTime)
{
    GridLockedPhysicalEntity::update(deltaTime);
    
    m_snakeSkin->update(deltaTime);
    m_snakeEye->update(deltaTime);
    m_snakeTonque->update(deltaTime);
    m_snakeBody->update(deltaTime);
	m_snakeHeadImpact->update(deltaTime);

	switch (m_state)
	{
		case EndBossSnakeState_Sleeping:
			break;
		case EndBossSnakeState_Awakening:
		{
			if (m_fStateTime > 0.3f)
			{
				setState(EndBossSnakeState_OpeningMouthLeft);

				return;
			}
		}
			break;
		case EndBossSnakeState_OpeningMouthLeft:
		{
			if (m_fStateTime > 0.3f)
			{
				m_snakeTonque->onMouthOpen();

				setState(EndBossSnakeState_OpenMouthLeft);

				return;
			}
		}
			break;
		case EndBossSnakeState_OpenMouthLeft:
		{
			if (m_fStateTime > 0.3f)
			{
				m_velocity->setX(-1 * (END_BOSS_SNAKE_DEFAULT_MAX_SPEED * 8));

				Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_CHARGE);

				setState(EndBossSnakeState_ChargingLeft);

				return;
			}
		}
			break;
		case EndBossSnakeState_ChargingLeft:
			break;
		case EndBossSnakeState_Pursuing:
		{
			Jon& jon = m_game->getJon();
			Vector2D target = Vector2D(jon.getPosition().getX(), jon.getPosition().getY());

			if (target.dist(getMainBounds().getRight(), getMainBounds().getBottom()) < 10.0f)
			{
				Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_MOUTH_OPEN);

				setState(EndBossSnakeState_OpeningMouthRight);

				return;
			}

			if (m_velocity->getX() > END_BOSS_SNAKE_DEFAULT_MAX_SPEED
				&& jon.getAbilityState() != ABILITY_DASH)
			{
				m_velocity->setX(END_BOSS_SNAKE_DEFAULT_MAX_SPEED);
			}

			if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds())
				|| OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
			{
				jon.kill();
			}
		}
			break;
		case EndBossSnakeState_OpeningMouthRight:
		{
			if (m_fStateTime > 0.3f)
			{
				m_snakeTonque->onMouthOpen();

				setState(EndBossSnakeState_OpenMouthRight);

				return;
			}

			Jon& jon = m_game->getJon();

			if (m_velocity->getX() > END_BOSS_SNAKE_DEFAULT_MAX_SPEED
				&& jon.getAbilityState() != ABILITY_DASH)
			{
				m_velocity->setX(END_BOSS_SNAKE_DEFAULT_MAX_SPEED);
			}

			if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds())
				|| OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
			{
				jon.kill();
			}
		}
			break;
		case EndBossSnakeState_OpenMouthRight:
		{
			if (m_fStateTime > 0.3f)
			{
				Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_CHARGE);

				m_velocity->setX(END_BOSS_SNAKE_DEFAULT_MAX_SPEED * 8);

				setState(EndBossSnakeState_ChargingRight);

				return;
			}

			Jon& jon = m_game->getJon();

			if (m_velocity->getX() > END_BOSS_SNAKE_DEFAULT_MAX_SPEED
				&& jon.getAbilityState() != ABILITY_DASH)
			{
				m_velocity->setX(END_BOSS_SNAKE_DEFAULT_MAX_SPEED);
			}

			if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds())
				|| OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
			{
				jon.kill();
			}
		}
			break;
		case EndBossSnakeState_Damaged:
		{
			m_color.red += 0.5f * deltaTime * 2;

			if (m_color.red > (1.0f + 0.5f * m_iDamage))
			{
				m_color.red = 1.0f + 0.5f * m_iDamage;
			}

			m_snakeBody->getColor().red = m_color.red;
			
			if (m_fStateTime > 4)
			{
				Jon& jon = m_game->getJon();
				Vector2D target = Vector2D(jon.getPosition().getX(), jon.getPosition().getY());
				if (target.dist(getMainBounds().getRight(), getMainBounds().getBottom()) < 10.0f)
				{
					Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_MOUTH_OPEN);
				}
				else
				{
					Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_CHARGE_CUE);
				}

				setState(EndBossSnakeState_OpeningMouthRight);

				return;
			}
		}
			break;
		case EndBossSnakeState_ChargingRight:
		{
			Jon& jon = m_game->getJon();

			if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds())
				|| OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
			{
				jon.kill();
			}

			Rectangle& camBounds = *m_game->getCameraBounds();

			if (camBounds.getWidth() > CAM_WIDTH)
			{
				return;
			}

			if (getSnakeBody().getMainBounds().getLeft() > camBounds.getRight())
			{
				m_position->setX(jon.getPosition().getX() + CAM_WIDTH * 2);
				
				Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_CHARGE_CUE);

				setState(EndBossSnakeState_OpeningMouthLeft);

				return;
			}
		}
			break;
		case EndBossSnakeState_Dying:
		{
			if (m_fStateTime > 4.3f)
			{
				setState(EndBossSnakeState_Dead);
				m_fStateTime = 5;

				return;
			}
		}
			break;
		case EndBossSnakeState_Dead:
			break;
	}
}

void EndBossSnake::awaken()
{
	setState(EndBossSnakeState_Awakening);

	m_snakeEye->onAwaken();
}

void EndBossSnake::beginPursuit()
{
	setState(EndBossSnakeState_Pursuing);

	m_snakeTonque->onMouthClose();

	if (m_game->getJons().size() > 0)
	{
		Jon& jon = m_game->getJon();

		m_velocity->setX(0);
		m_acceleration->setX(END_BOSS_SNAKE_DEFAULT_ACCELERATION);

		m_position->setX(jon.getPosition().getX() - CAM_WIDTH * 1.2f);
		m_position->setY(2.80124998f);
	}
}

void EndBossSnake::triggerHit()
{
    m_iDamage++;
    if (m_iDamage < 3)
    {
        setState(EndBossSnakeState_Damaged);
        
        m_snakeSkin->onDamageTaken();
		m_snakeHeadImpact->onDamageTaken();
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_DAMAGED);
    }
    else
    {
        setState(EndBossSnakeState_Dying);
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_DEATH);
    }

	m_snakeTonque->onMouthClose();
    
    m_velocity->setX(0);
    m_acceleration->setX(0);
}

void EndBossSnake::setState(EndBossSnakeState state)
{
    m_state = state;
    m_fStateTime = 0.0f;
}

bool EndBossSnake::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    float entityVelocityY = entity->getVelocity().getY();
    
    if (entityVelocityY <= 0
        && entity->getPosition().getX() > getMainBounds().getLeft()
        && (entity->getMainBounds().getBottom() + 0.01f) > getMainBounds().getBottom())
    {
        if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), getMainBounds()))
        {
			Jon *jon;
			if ((jon = dynamic_cast<Jon *>(entity)))
			{
				// Do Nothing
			}
			else
			{
				float itemTop = getMainBounds().getTop();

				entity->getPosition().setY(itemTop + entity->getMainBounds().getHeight() / 2 * .99f);
				entity->updateBounds();
			}
            
            return true;
        }
    }
    
    return false;
}

void SnakeEye::update(float deltaTime)
{
	if (m_isWakingUp)
	{
		m_fStateTime += deltaTime;

		if (m_fStateTime > 0.3f)
		{
			m_isShowing = false;
			m_isWakingUp = false;
		}
	}
}

void SnakeEye::onAwaken()
{
	m_fStateTime = 0;
    
    m_isWakingUp = true;
}

EndBossSnake& SnakeEye::getEndBossSnake()
{
    return *m_endBossSnake;
}

void SnakeHeadImpact::update(float deltaTime)
{
	if (m_isShowing)
	{
		EndBossSnake *snake = m_endBossSnake;
		float snakeX = snake->getPosition().getX();
		float snakeY = snake->getPosition().getY();

		m_position->set(snakeX, snakeY + 1);

		m_fStateTime += deltaTime;
		if (m_fStateTime > 0.8f)
		{
			m_color.alpha -= deltaTime * 2;

			if (m_color.alpha < 0)
			{
				m_color.alpha = 0;

				m_isShowing = false;
			}
		}
	}
}

void SnakeHeadImpact::onDamageTaken()
{
	EndBossSnake *snake = m_endBossSnake;
	float snakeX = snake->getPosition().getX();
	float snakeY = snake->getPosition().getY();

	m_position->set(snakeX, snakeY + 4);

	m_fStateTime = 0;

	m_color.alpha = 1;

	m_isShowing = true;
}

EndBossSnake& SnakeHeadImpact::getEndBossSnake()
{
	return *m_endBossSnake;
}

void SnakeSkin::update(float deltaTime)
{
    if (m_isShowing)
    {
        EndBossSnake *snake = m_endBossSnake;
        float snakeX = snake->getPosition().getX();
        float snakeY = snake->getPosition().getY();
        
        m_position->set(snakeX, snakeY);
        
        m_fStateTime += deltaTime;
        if (m_fStateTime > 0.7f)
        {
            m_color.alpha -= deltaTime * 2;
            
            if (m_color.alpha < 0)
            {
                m_color.alpha = 0;
                
                m_isShowing = false;
            }
        }
    }
}

void SnakeSkin::onDamageTaken()
{
    EndBossSnake *snake = m_endBossSnake;
    float snakeX = snake->getPosition().getX();
    float snakeY = snake->getPosition().getY();
    
    m_position->set(snakeX, snakeY);
    
    m_fStateTime = 0;
    
    m_color.alpha = 1;

	if (snake->getDamage() > 1)
	{
		m_color.red += 0.5f * (snake->getDamage() - 1);
	}
    
    m_isShowing = true;
}

EndBossSnake& SnakeSkin::getEndBossSnake()
{
    return *m_endBossSnake;
}

void SnakeTonque::update(float deltaTime)
{
	if (m_isMouthOpen)
	{
		m_fStateTime += deltaTime;
	}

	EndBossSnake *snake = m_endBossSnake;
	float snakeX = snake->getPosition().getX();
	float snakeY = snake->getPosition().getY();
	float snakeW = snake->getWidth();
	float snakeH = snake->getHeight();

	float snakeBottom = snakeY - (snakeH / 2);

	if (snake->getState() == EndBossSnakeState_OpeningMouthLeft
		|| snake->getState() == EndBossSnakeState_OpenMouthLeft
		|| snake->getState() == EndBossSnakeState_ChargingLeft)
	{
		float snakeLeft = snakeX - snakeW / 2;
		m_position->set(snakeLeft + getWidth() / 2, snakeBottom + getHeight() * 2);
	}
	else if (snake->getState() == EndBossSnakeState_OpeningMouthRight
		|| snake->getState() == EndBossSnakeState_OpenMouthRight
		|| snake->getState() == EndBossSnakeState_ChargingRight)
	{
		float snakeRight = snakeX + snakeW / 2;
		m_position->set(snakeRight - getWidth() / 2, snakeBottom + getHeight() * 2);
	}
}

void SnakeTonque::onMouthOpen()
{
	update(0);

	m_fStateTime = 0;
    
    m_isMouthOpen = true;
}

EndBossSnake& SnakeTonque::getEndBossSnake()
{
    return *m_endBossSnake;
}

void SnakeBody::update(float deltaTime)
{
    EndBossSnake *snake = m_endBossSnake;
    float snakeX = snake->getPosition().getX();
    float snakeY = snake->getPosition().getY();
    float snakeW = snake->getWidth();
    
    float spacing = GRID_CELL_SIZE * 5;
    
    if (snake->getState() == EndBossSnakeState_Pursuing
        || snake->getState() == EndBossSnakeState_Damaged
        || snake->getState() == EndBossSnakeState_OpeningMouthRight
		|| snake->getState() == EndBossSnakeState_OpenMouthRight
        || snake->getState() == EndBossSnakeState_ChargingRight
        || snake->getState() == EndBossSnakeState_Dying
        || snake->getState() == EndBossSnakeState_Dead)
    {
        float snakeLeft = snakeX - snakeW / 2;
        m_position->set(snakeLeft + spacing - getWidth() / 2, snakeY);
    }
    else if (snake->getState() == EndBossSnakeState_Sleeping
		|| snake->getState() == EndBossSnakeState_Awakening
		|| snake->getState() == EndBossSnakeState_OpeningMouthLeft
		|| snake->getState() == EndBossSnakeState_OpenMouthLeft
		|| snake->getState() == EndBossSnakeState_ChargingLeft)
    {
        float snakeRight = snakeX + snakeW / 2;
        m_position->set(snakeRight - spacing + getWidth() / 2, snakeY);
    }

	updateBounds();
}

EndBossSnake& SnakeBody::getEndBossSnake()
{
    return *m_endBossSnake;
}
