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
#include "NGAudioEngine.h"
#include "NGSTDUtil.h"
#include "NGRect.h"

#include <math.h>

#define END_BOSS_SNAKE_DEFAULT_MAX_SPEED VAMP_DEFAULT_MAX_SPEED + 1.4f
#define END_BOSS_SNAKE_DEFAULT_ACCELERATION VAMP_DEFAULT_ACCELERATION

EndBossSnake* EndBossSnake::create(int gridX, int gridY, int type)
{
    return new EndBossSnake(gridX, gridY);
}

EndBossSnake::EndBossSnake(int gridX, int gridY) : GridLockedPhysicalEntity(gridX, gridY, 54, 34, 0.25f, 0.12f, 0.5f, 0.88f),
m_game(nullptr),
m_state(EndBossSnakeState_Sleeping),
m_color(1, 1, 1, 1),
m_fTimeSinceLastVelocityCheck(0),
m_iDamage(0),
m_type(-1),
m_hasPlayedChargeSound(false)
{
    float x = m_position.getX();
    float y = m_position.getY();
    float w = m_fWidth;
    float h = m_fHeight;
	float l = x - w / 2;
	float b = y - (h / 2);
    
    m_snakeSkin = new SnakeSkin(x, y, w, h, this);
    m_snakeEye = new SnakeEye(0.52083333333333 * w + l, 0.65073529411765 * h + b, this);
    m_snakeTonque = new SnakeTonque(x, y, this);
    m_snakeBody = new SnakeBody(x, y, h, this);
	m_snakeHeadImpact = new SnakeHeadImpact(x, y, this);
    m_snakeSpirit = new SnakeSpirit(x, y, this);
}

EndBossSnake::~EndBossSnake()
{
    NGSTDUtil::cleanUpVectorOfPointers(m_afterImages);
    
    delete m_snakeSkin;
    delete m_snakeEye;
    delete m_snakeTonque;
    delete m_snakeBody;
    delete m_snakeHeadImpact;
    delete m_snakeSpirit;
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
                afterImage->m_velocity.set(m_velocity);
                afterImage->m_position.set(m_position);
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
                NGRect& camBounds = *m_game->getCameraBounds();
                
                if (camBounds.getWidth() > CAM_WIDTH)
                {
                    return;
                }
                
                if (OverlapTester::doNGRectsOverlap(camBounds, getMainBounds()))
                {
                    NG_AUDIO_ENGINE->playSound(SOUND_END_BOSS_SNAKE_CHARGE);
                    
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

				if (OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getMainBounds())
					|| OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
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

				if (OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getMainBounds())
					|| OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
				{
					jon.kill();
				}
			}

			if (m_fStateTime > 0.3f)
			{
				m_velocity.setX(-1 * (END_BOSS_SNAKE_DEFAULT_MAX_SPEED * 8));
                
                m_hasPlayedChargeSound = false;

				setState(EndBossSnakeState_ChargingLeft);

				return;
			}
		}
			break;
		case EndBossSnakeState_ChargingLeft:
		{
			if (m_position.getY() < 3) // Make sure Snake is on Cave Floor (opening sequence completed)
			{
				Jon& jon = m_game->getJon();

				if (OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getMainBounds())
					|| OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
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
					NG_AUDIO_ENGINE->playSound(SOUND_END_BOSS_SNAKE_MOUTH_OPEN);
				}
				else
				{
					NG_AUDIO_ENGINE->playSound(SOUND_END_BOSS_SNAKE_CHARGE_CUE);
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
				NG_AUDIO_ENGINE->playSound(SOUND_END_BOSS_SNAKE_MOUTH_OPEN);

				setState(EndBossSnakeState_OpeningMouthRight);

				return;
			}

			if (m_velocity.getX() > END_BOSS_SNAKE_DEFAULT_MAX_SPEED
				&& jon.getAbilityState() != ABILITY_DASH)
			{
				m_velocity.setX(END_BOSS_SNAKE_DEFAULT_MAX_SPEED);
			}

			if (OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getMainBounds())
				|| OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
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

			if (m_velocity.getX() > END_BOSS_SNAKE_DEFAULT_MAX_SPEED
				&& jon.getAbilityState() != ABILITY_DASH)
			{
				m_velocity.setX(END_BOSS_SNAKE_DEFAULT_MAX_SPEED);
			}

			if (OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getMainBounds())
				|| OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
			{
				jon.kill();
			}
		}
			break;
		case EndBossSnakeState_OpenMouthRight:
		{
			if (m_fStateTime > 0.3f)
			{
                m_velocity.setX(END_BOSS_SNAKE_DEFAULT_MAX_SPEED * 8);
                
                m_hasPlayedChargeSound = false;

				setState(EndBossSnakeState_ChargingRight);

				return;
			}

			Jon& jon = m_game->getJon();

			if (m_velocity.getX() > END_BOSS_SNAKE_DEFAULT_MAX_SPEED
				&& jon.getAbilityState() != ABILITY_DASH)
			{
				m_velocity.setX(END_BOSS_SNAKE_DEFAULT_MAX_SPEED);
			}

			if (OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getMainBounds())
				|| OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
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
					NG_AUDIO_ENGINE->playSound(SOUND_END_BOSS_SNAKE_MOUTH_OPEN);
				}
				else
				{
					NG_AUDIO_ENGINE->playSound(SOUND_END_BOSS_SNAKE_CHARGE_CUE);
				}

				setState(EndBossSnakeState_OpeningMouthRight);

				return;
			}
		}
			break;
		case EndBossSnakeState_ChargingRight:
		{
			Jon& jon = m_game->getJon();

			if (OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getMainBounds())
				|| OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getSnakeBody().getMainBounds()))
			{
				jon.kill();
			}

			NGRect& camBounds = *m_game->getCameraBounds();

			if (camBounds.getWidth() > CAM_WIDTH)
			{
				return;
			}

			if (getSnakeBody().getMainBounds().getLeft() > camBounds.getRight())
			{
				m_position.setX(jon.getPosition().getX() + CAM_WIDTH * 1.1f);
				
				NG_AUDIO_ENGINE->playSound(SOUND_END_BOSS_SNAKE_CHARGE_CUE);

				setState(EndBossSnakeState_OpeningMouthLeft);

				getSnakeBody().update(0);

				return;
			}
		}
			break;
		case EndBossSnakeState_Dying:
		{
            if (m_velocity.getX() < 0)
            {
                m_velocity.setX(0);
                m_acceleration.setX(0);
            }
            
            Jon& jon = m_game->getJon();
            if (getMainBounds().getRight() > (jon.getMainBounds().getLeft() - 1))
            {
                m_velocity.setX(0);
                m_acceleration.setX(0);
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

		m_velocity.setX(0);
		m_acceleration.setX(0);

		m_position.setX(jon.getPosition().getX() - CAM_WIDTH);
		m_position.setY(2.80124998f);
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

        m_velocity.setX(VAMP_DEFAULT_MAX_SPEED);
		m_acceleration.setX(END_BOSS_SNAKE_DEFAULT_ACCELERATION);

		m_position.setX(jon.getPosition().getX() - CAM_WIDTH * 1.25f);
		m_position.setY(2.80124998f);
	}
}

void EndBossSnake::triggerHit()
{
    m_iDamage++;
    if (m_iDamage < 3)
    {
        m_velocity.setX(0);
        m_acceleration.setX(0);
        
        setState(EndBossSnakeState_Damaged);
        
        m_snakeSkin->onDamageTaken();
		m_snakeHeadImpact->onDamageTaken();
        
        NG_AUDIO_ENGINE->playSound(SOUND_END_BOSS_SNAKE_DAMAGED);
    }
    else
    {
		m_velocity.setX(8);
		m_acceleration.setX(-8);

		m_iDamage = 3;
		m_color.red = 3.0f;
		m_snakeBody->getColor().red = m_color.red;

		setState(EndBossSnakeState_Dying);

		m_snakeBody->onDeath();

		NG_AUDIO_ENGINE->playSound(SOUND_END_BOSS_SNAKE_DEATH);
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
        if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), getMainBounds()))
        {
            return true;
        }
    }
    
    return false;
}

int EndBossSnake::getEntityLandingPriority()
{
    return 0;
}

Game* EndBossSnake::getGame()
{
    return m_game;
}

void EndBossSnake::setGame(Game* game)
{
    m_game = game;
}

std::vector<EndBossSnake *>& EndBossSnake::getAfterImages()
{
    return m_afterImages;
}

SnakeSkin& EndBossSnake::getSnakeSkin()
{
    return *m_snakeSkin;
}

SnakeEye& EndBossSnake::getSnakeEye()
{
    return *m_snakeEye;
}

SnakeTonque& EndBossSnake::getSnakeTonque()
{
    return *m_snakeTonque;
}

SnakeBody& EndBossSnake::getSnakeBody()
{
    return *m_snakeBody;
}

SnakeHeadImpact& EndBossSnake::getSnakeHeadImpact()
{
    return *m_snakeHeadImpact;
}

SnakeSpirit& EndBossSnake::getSnakeSpirit()
{
    return *m_snakeSpirit;
}

EndBossSnakeState EndBossSnake::getState()
{
    return m_state;
}

Color EndBossSnake::getColor()
{
    return m_color;
}

int EndBossSnake::getDamage()
{
    return m_iDamage;
}

int EndBossSnake::getType()
{
    return m_type;
}

SnakeSpirit::SnakeSpirit(float x, float y, EndBossSnake* endBossSnake) : PhysicalEntity(x, y, 51.25f * GRID_CELL_SIZE, 22.5f * GRID_CELL_SIZE), m_endBossSnake(endBossSnake), m_color(1, 1, 1, 1), m_isShowing(false)
{
    // Empty
}

void SnakeSpirit::update(float deltaTime)
{
    if (m_isShowing)
    {
        EndBossSnake *snake = m_endBossSnake;
        float snakeX = snake->getPosition().getX();
        float snakeY = snake->getPosition().getY();
        
        m_position.set(snakeX + getWidth() / 2, snakeY);
        
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
    
    m_position.set(snakeX + m_fWidth / 2, snakeY);
    
    m_fStateTime = 0;
    
    m_color.alpha = 1;
    
    m_isShowing = true;
    
    NG_AUDIO_ENGINE->playSound(SOUND_ABSORB_DASH_ABILITY);
}

EndBossSnake& SnakeSpirit::getEndBossSnake()
{
    return *m_endBossSnake;
}

Color& SnakeSpirit::getColor()
{
    return m_color;
}

SnakeHeadImpact::SnakeHeadImpact(float x, float y, EndBossSnake* endBossSnake) : PhysicalEntity(x, y, 32 * GRID_CELL_SIZE, 32 * GRID_CELL_SIZE), m_endBossSnake(endBossSnake), m_color(1, 1, 1, 1), m_isShowing(false)
{
    // Empty
}

void SnakeHeadImpact::update(float deltaTime)
{
	if (m_isShowing)
	{
		EndBossSnake *snake = m_endBossSnake;
		float snakeX = snake->getPosition().getX();
		float snakeY = snake->getPosition().getY();

		m_position.set(snakeX, snakeY + 1);

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

	m_position.set(snakeX, snakeY + 1);

	m_fStateTime = 0;

	m_color.alpha = 1;

	m_isShowing = true;
}

EndBossSnake& SnakeHeadImpact::getEndBossSnake()
{
    return *m_endBossSnake;
}

Color& SnakeHeadImpact::getColor()
{
    return m_color;
}

SnakeSkin::SnakeSkin(float x, float y, float width, float height, EndBossSnake* endBossSnake) : PhysicalEntity(x, y, width, height), m_endBossSnake(endBossSnake), m_color(1, 1, 1, 1), m_isShowing(false)
{
    // Empty
}

void SnakeSkin::update(float deltaTime)
{
    if (m_isShowing)
    {
        EndBossSnake *snake = m_endBossSnake;
        float snakeX = snake->getPosition().getX();
        float snakeY = snake->getPosition().getY();
        
        m_position.set(snakeX, snakeY);
        
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
    
    m_position.set(snakeX, snakeY);
    
    m_fStateTime = 0;
    
    m_color.alpha = 1;

	if (snake->getDamage() > 1)
	{
		m_color.red += 1 * (snake->getDamage() - 1);
	}
    
    m_isShowing = true;
}

EndBossSnake& SnakeSkin::getEndBossSnake()
{
    return *m_endBossSnake;
}

Color& SnakeSkin::getColor()
{
    return m_color;
}

SnakeEye::SnakeEye(float x, float y, EndBossSnake* endBossSnake) : PhysicalEntity(x, y, 1.037109375f, 1.037109375f), m_endBossSnake(endBossSnake), m_isWakingUp(false), m_isShowing(true)
{
    // Empty
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

SnakeTonque::SnakeTonque(float x, float y, EndBossSnake* endBossSnake) : PhysicalEntity(x, y, 4.869140625f, 0.685546875f), m_endBossSnake(endBossSnake), m_isMouthOpen(false)
{
    // Empty
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
		m_position.set(snakeLeft + getWidth() / 2, snakeBottom + getHeight() * 2);
	}
	else if (snake->getState() == EndBossSnakeState_OpeningMouthRight
		|| snake->getState() == EndBossSnakeState_OpenMouthRight
		|| snake->getState() == EndBossSnakeState_ChargingRight)
	{
		float snakeRight = snakeX + snakeW / 2;
		m_position.set(snakeRight - getWidth() / 2, snakeBottom + getHeight() * 2);
	}
}

void SnakeTonque::onMouthOpen()
{
	update(0);

	m_fStateTime = 0;
    
    m_isMouthOpen = true;
}

void SnakeTonque::onMouthClose()
{
    m_isMouthOpen = false;
}

EndBossSnake& SnakeTonque::getEndBossSnake()
{
    return *m_endBossSnake;
}

bool SnakeTonque::isMouthOpen()
{
    return m_isMouthOpen;
}

SnakeBody::SnakeBody(float x, float y, float height, EndBossSnake* endBossSnake) : PhysicalEntity(x, y, 30.65625f, height), m_endBossSnake(endBossSnake), m_color(1, 1, 1, 1), m_isDead(false)
{
    update(0);
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
        m_position.set(snakeLeft + spacing - getWidth() / 2, snakeY);
    }
    else if (snake->getState() == EndBossSnakeState_Sleeping
		|| snake->getState() == EndBossSnakeState_Awakening
		|| snake->getState() == EndBossSnakeState_OpeningMouthLeft
		|| snake->getState() == EndBossSnakeState_OpenMouthLeft
		|| snake->getState() == EndBossSnakeState_ChargingLeft)
    {
        float snakeRight = snakeX + snakeW / 2;
        m_position.set(snakeRight - spacing + getWidth() / 2, snakeY);
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

void SnakeBody::updateBounds()
{
    getMainBounds().setWidth(getWidth());
    getMainBounds().setHeight(getHeight());
    
    PhysicalEntity::updateBounds();
    
    getMainBounds().getLowerLeft().add(getWidth() * 0.15f, getHeight() * 0.0f);
    getMainBounds().setWidth(getWidth() * 0.70f);
    getMainBounds().setHeight(getHeight() * 0.50f);
}

void SnakeBody::onDeath()
{
    update(0);
	m_isDead = true;
}

EndBossSnake& SnakeBody::getEndBossSnake()
{
    return *m_endBossSnake;
}

Color& SnakeBody::getColor()
{
    return m_color;
}

RTTI_IMPL(EndBossSnake, GridLockedPhysicalEntity);
RTTI_IMPL(SnakeSpirit, PhysicalEntity);
RTTI_IMPL(SnakeHeadImpact, PhysicalEntity);
RTTI_IMPL(SnakeSkin, PhysicalEntity);
RTTI_IMPL(SnakeEye, PhysicalEntity);
RTTI_IMPL(SnakeTonque, PhysicalEntity);
RTTI_IMPL(SnakeBody, PhysicalEntity);
