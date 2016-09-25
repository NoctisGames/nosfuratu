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

#include <math.h>

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
    
    m_snakeSkin = std::unique_ptr<SnakeSkin>(new SnakeSkin(x, y, w, h, this));
    m_snakeEye = std::unique_ptr<SnakeEye>(new SnakeEye(x, y, this));
    m_snakeTonque = std::unique_ptr<SnakeTonque>(new SnakeTonque(x, y, this));
    m_snakeBody = std::unique_ptr<SnakeBody>(new SnakeBody(x, y, h, this));
}

void EndBossSnake::update(float deltaTime)
{
    GridLockedPhysicalEntity::update(deltaTime);
    
    m_snakeSkin->update(deltaTime);
    m_snakeEye->update(deltaTime);
    m_snakeTonque->update(deltaTime);
    m_snakeBody->update(deltaTime);
}

void EndBossSnake::triggerHit()
{
    m_iDamage++;
    if (m_iDamage < 3)
    {
        setState(EndBossSnakeState_Damaged);
        
        m_snakeSkin->onDamageTaken();
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_DAMAGED);
    }
    else
    {
        setState(EndBossSnakeState_Dying);
        
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_END_BOSS_SNAKE_DEATH);
    }
    
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
            float itemTop = getMainBounds().getTop();
            
            entity->getPosition().setY(itemTop + entity->getMainBounds().getHeight() / 2 * .99f);
            entity->updateBounds();
            
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
    EndBossSnake *snake = m_endBossSnake;
    float snakeX = snake->getPosition().getX();
    float snakeY = snake->getPosition().getY();
    float snakeW = snake->getWidth();
    float snakeH = snake->getHeight();
    
    float snakeLeft = snakeX - snakeW / 2;
    float snakeBottom = snakeY - (snakeH / 2);
    
    m_position->set(0.52083333333333 * snakeW + snakeLeft, 0.65073529411765 * snakeH + snakeBottom);
    
    m_fStateTime = 0;
    
    m_isWakingUp = true;
}

EndBossSnake& SnakeEye::getEndBossSnake()
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
    
    m_isShowing = true;
}

EndBossSnake& SnakeSkin::getEndBossSnake()
{
    return *m_endBossSnake;
}

void SnakeTonque::onMouthOpen()
{
    EndBossSnake *snake = m_endBossSnake;
    float snakeX = snake->getPosition().getX();
    float snakeY = snake->getPosition().getY();
    float snakeW = snake->getWidth();
    float snakeH = snake->getHeight();
    
    float snakeBottom = snakeY - (snakeH / 2);
    
    if (snake->getState() == EndBossSnakeState_OpeningMouthLeft
        || snake->getState() == EndBossSnakeState_ChargingLeft)
    {
        float snakeLeft = snakeX - snakeW / 2;
        m_position->set(snakeLeft + getWidth() / 2, snakeBottom + getHeight() * 2);
    }
    else if (snake->getState() == EndBossSnakeState_OpeningMouthRight
             || snake->getState() == EndBossSnakeState_ChargingRight)
    {
        float snakeRight = snakeX + snakeW / 2;
        m_position->set(snakeRight - getWidth() / 2, snakeBottom + getHeight() * 2);
    }
    
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
             || snake->getState() == EndBossSnakeState_ChargingLeft)
    {
        float snakeRight = snakeX + snakeW / 2;
        m_position->set(snakeRight - spacing + getWidth() / 2, snakeY);
    }
}

EndBossSnake& SnakeBody::getEndBossSnake()
{
    return *m_endBossSnake;
}
