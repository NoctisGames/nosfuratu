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
#include "MathUtil.h"

#include <math.h>

#define END_BOSS_SNAKE_DEFAULT_MAX_SPEED VAMP_DEFAULT_MAX_SPEED + 1.4f
#define END_BOSS_SNAKE_DEFAULT_ACCELERATION VAMP_DEFAULT_ACCELERATION

EndBossSnake::EndBossSnake(int gridX, int gridY) : GridLockedPhysicalEntity(gridX, gridY, 54, 34, 0.25f, 0.12f, 0.5f, 0.88f),
m_game(nullptr),
m_state(EndBossSnakeState_Sleeping),
m_color(1, 1, 1, 1),
m_fTimeSinceLastVelocityCheck(0),
m_iDamage(0),
m_type(-1),
m_hasPlayedChargeSound(false)
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
    m_snakeSpirit = std::unique_ptr<SnakeSpirit>(new SnakeSpirit(x, y, this));
}

void EndBossSnake::update(float deltaTime)
{
    GridLockedPhysicalEntity::update(deltaTime);
    
    for (std::vector<EndBossSnake *>::iterator i = m_afterImages.begin(); i != m_afterImages.end(); )
    {
        (*i)->m_color.green -= deltaTime * 3;
        (*i)->m_color.blue += deltaTime * 3;
        (*i)->m_color.alpha -= deltaTime * 3;
        
        if ((*i)->m_color.alpha < 0.0)
        {
            delete *i;
            i = m_afterImages.erase(i);
        }
        else
        {
            i++;
        }
    }
    
    m_snakeSkin->update(deltaTime);
    m_snakeEye->update(deltaTime);
    m_snakeTonque->update(deltaTime);
    m_snakeBody->update(deltaTime);
    m_snakeHeadImpact->update(deltaTime);
    m_snakeSpirit->update(deltaTime);
    
    switch (m_state)
    {
        case EndBossSnakeState_ChargingLeft:
        case EndBossSnakeState_ChargingRight:
        {
            m_fTimeSinceLastVelocityCheck += deltaTime;
            if (m_fTimeSinceLastVelocityCheck > 0.05f)
            {
                m_fTimeSinceLastVelocityCheck = 0;
                
                EndBossSnake* afterImage = new EndBossSnake(getGridX(), getGridY());
                afterImage->m_velocity->set(*m_velocity);
                afterImage->m_position->set(*m_position);
                afterImage->m_state = m_state;
                
                afterImage->m_color = m_color;
                
                afterImage->m_fHeight = m_fHeight;
                afterImage->m_fWidth = m_fWidth;
                afterImage->m_fStateTime = m_fStateTime;
                
                afterImage->m_color.green *= 0.7f;
                afterImage->m_color.blue *= 1.3f;
                afterImage->m_color.alpha *= 0.35f;
                afterImage->m_color.alpha = clamp(afterImage->m_color.alpha, 1, 0);
                
                // bye bye!
                afterImage->m_snakeSkin->getPosition().setY(1337);
                afterImage->m_snakeEye->getPosition().setY(1337);
                afterImage->m_snakeHeadImpact->getPosition().setY(1337);
                
                if (m_snakeTonque->isMouthOpen())
                {
                    afterImage->m_snakeTonque->onMouthOpen();
                    afterImage->m_snakeTonque->update(m_snakeTonque->getStateTime());
                }
                
                afterImage->m_snakeBody->update(0);
                afterImage->m_snakeBody->getColor().red = m_color.red;
                afterImage->m_snakeBody->getColor().green *= 0.7f;
                afterImage->m_snakeBody->getColor().blue *= 1.3f;
                afterImage->m_snakeBody->getColor().alpha *= 0.35f;
                afterImage->m_snakeBody->getColor().alpha = clamp(afterImage->m_color.alpha, 1, 0);
                
                m_afterImages.push_back(afterImage);
            }
            
            if (!m_hasPlayedChargeSound)
            {
                Rectangle& camBounds = *m_game->getCameraBounds();
                
                if (camBounds.getWidth() > CAM_WIDTH)
                {
                    return;
                }
                
                if (OverlapTester::doRectanglesOverlap(camBounds, getMainBounds()))
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_CHARGE);
                    
                    m_hasPlayedChargeSound = true;
                }
            }
        }
            break;
        default:
            break;
    }

	switch (m_state)
	{
		case EndBossSnakeState_Sleeping:
			break;
		case EndBossSnakeState_Awakening:
		{
			if (m_fStateTime > 1.3f)
			{
				setState(EndBossSnakeState_OpeningMouthLeft);

				return;
			}
		}
			break;
		case EndBossSnakeState_OpeningMouthLeft:
		{
			if (m_iDamage > 0)
			{
				Jon& jon = m_game->getJon();

				if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds())
					|| OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
				{
					jon.kill();
				}
			}

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
			if (m_iDamage > 0)
			{
				Jon& jon = m_game->getJon();

				if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds())
					|| OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
				{
					jon.kill();
				}
			}

			if (m_fStateTime > 0.3f)
			{
				m_velocity->setX(-1 * (END_BOSS_SNAKE_DEFAULT_MAX_SPEED * 8));
                
                m_hasPlayedChargeSound = false;

				setState(EndBossSnakeState_ChargingLeft);

				return;
			}
		}
			break;
		case EndBossSnakeState_ChargingLeft:
		{
			if (m_position->getY() < 3) // Make sure Snake is on Cave Floor (opening sequence completed)
			{
				Jon& jon = m_game->getJon();

				if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds())
					|| OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
				{
					jon.kill();
				}
			}
		}
			break;
		case EndBossSnakeState_Waiting:
		{
			if (m_fStateTime > 3.2f)
			{
				Jon& jon = m_game->getJon();
				Vector2D target = Vector2D(jon.getPosition().getX(), jon.getPosition().getY());
				if (target.dist(getMainBounds().getRight(), getMainBounds().getBottom()) < 11.0f)
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
		case EndBossSnakeState_Pursuing:
		{
			Jon& jon = m_game->getJon();
			Vector2D target = Vector2D(jon.getPosition().getX(), jon.getPosition().getY());

			if (target.dist(getMainBounds().getRight(), getMainBounds().getBottom()) < 11.0f)
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
                m_velocity->setX(END_BOSS_SNAKE_DEFAULT_MAX_SPEED * 8);
                
                m_hasPlayedChargeSound = false;

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
			m_color.red += 1 * deltaTime * 2;

			if (m_color.red > (1.0f + 1 * m_iDamage))
			{
				m_color.red = 1.0f + 1 * m_iDamage;
			}

			m_snakeBody->getColor().red = m_color.red;
			
			if (m_fStateTime > 3.2f)
			{
				Jon& jon = m_game->getJon();
				Vector2D target = Vector2D(jon.getPosition().getX(), jon.getPosition().getY());
				if (target.dist(getMainBounds().getRight(), getMainBounds().getBottom()) < 11.0f)
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
				m_position->setX(jon.getPosition().getX() + CAM_WIDTH * 1.1f);
				
				Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_CHARGE_CUE);

				setState(EndBossSnakeState_OpeningMouthLeft);

				getSnakeBody().update(0);

				return;
			}
		}
			break;
		case EndBossSnakeState_Dying:
		{
            if (m_velocity->getX() < 0)
            {
                m_velocity->setX(0);
                m_acceleration->setX(0);
            }
            
			if (m_fStateTime > 4.3f)
			{
				setState(EndBossSnakeState_DeadSpiritReleasing);
                
                m_snakeSpirit->onDeath();

				return;
			}
		}
			break;
		case EndBossSnakeState_DeadSpiritReleasing:
        {
            if (m_fStateTime > 1.2f)
            {
                setState(EndBossSnakeState_Dead);
                
                return;
            }
        }
			break;
		case EndBossSnakeState_Dead:
			break;
	}
}

void EndBossSnake::begin()
{
	setState(EndBossSnakeState_Waiting);

	m_snakeTonque->onMouthClose();

	if (m_game->getJons().size() > 0)
	{
		Jon& jon = m_game->getJon();

		m_velocity->setX(0);
		m_acceleration->setX(0);

		m_position->setX(jon.getPosition().getX() - CAM_WIDTH);
		m_position->setY(2.80124998f);
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

        m_velocity->setX(VAMP_DEFAULT_MAX_SPEED);
		m_acceleration->setX(END_BOSS_SNAKE_DEFAULT_ACCELERATION);

		m_position->setX(jon.getPosition().getX() - CAM_WIDTH * 1.25f);
		m_position->setY(2.80124998f);
	}
}

void EndBossSnake::triggerHit()
{
    m_iDamage++;
    if (m_iDamage < 3)
    {
        m_velocity->setX(0);
        m_acceleration->setX(0);
        
        setState(EndBossSnakeState_Damaged);
        
        m_snakeSkin->onDamageTaken();
		m_snakeHeadImpact->onDamageTaken();
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_DAMAGED);
    }
    else
    {
		m_velocity->setX(8);
		m_acceleration->setX(-10);

		m_iDamage = 3;
		m_color.red = 3.0f;
		m_snakeBody->getColor().red = m_color.red;

		setState(EndBossSnakeState_Dying);

		m_snakeBody->onDeath();

		Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_DEATH);
    }

	m_snakeTonque->onMouthClose();
}

void EndBossSnake::checkPointKill()
{
	setState(EndBossSnakeState_Dead);
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

void SnakeSpirit::update(float deltaTime)
{
    if (m_isShowing)
    {
        EndBossSnake *snake = m_endBossSnake;
        float snakeX = snake->getPosition().getX();
        float snakeY = snake->getPosition().getY();
        
        m_position->set(snakeX + getWidth() / 2, snakeY);
        
        m_fStateTime += deltaTime;
        if (m_fStateTime > 1.2f)
        {
            m_color.alpha -= deltaTime * 2;
            
            if (m_color.alpha < 0)
            {
                m_color.alpha = 0;
                
                m_isShowing = false;
            }
        }
		else if (m_fStateTime > 0.6f)
		{
			if (!m_hasPlayedSound)
			{
				Assets::getInstance()->addSoundIdToPlayQueue(SOUND_ABSORB_DASH_ABILITY);

				m_hasPlayedSound = true;
			}
		}
    }
}

void SnakeSpirit::onDeath()
{
    EndBossSnake *snake = m_endBossSnake;
    float snakeX = snake->getPosition().getX();
    float snakeY = snake->getPosition().getY();
    
    Jon& jon = snake->getGame()->getJon();
    Vector2D target = Vector2D(jon.getMainBounds().getRight(), jon.getPosition().getY());
    m_fWidth = target.dist(snakeX, snakeY);
    
    m_position->set(snakeX + m_fWidth / 2, snakeY);
    
    m_fStateTime = 0;
    
    m_color.alpha = 1;
    
    m_isShowing = true;
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

	m_position->set(snakeX, snakeY + 1);

	m_fStateTime = 0;

	m_color.alpha = 1;

	m_isShowing = true;
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
		m_color.red += 1 * (snake->getDamage() - 1);
	}
    
    m_isShowing = true;
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

void SnakeBody::update(float deltaTime)
{
    EndBossSnake *snake = m_endBossSnake;
    float snakeX = snake->getPosition().getX();
    float snakeY = snake->getPosition().getY();
    float snakeW = snake->getWidth();
    
    float spacing = GRID_CELL_SIZE * 5;
    
    if (snake->getState() == EndBossSnakeState_Pursuing
		|| snake->getState() == EndBossSnakeState_Waiting
        || snake->getState() == EndBossSnakeState_Damaged
        || snake->getState() == EndBossSnakeState_OpeningMouthRight
		|| snake->getState() == EndBossSnakeState_OpenMouthRight
        || snake->getState() == EndBossSnakeState_ChargingRight
        || snake->getState() == EndBossSnakeState_Dying
        || snake->getState() == EndBossSnakeState_DeadSpiritReleasing
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

	if (m_isDead)
	{
		m_color.alpha -= deltaTime;
		if (m_color.alpha < 0)
		{
			m_color.alpha = 0;
		}
	}
}

void SnakeBody::onDeath()
{
    update(0);
	m_isDead = true;
}
