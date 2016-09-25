//
//  EndBossSnake.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/24/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__EndBossSnake__
#define __nosfuratu__EndBossSnake__

#include "GridLockedPhysicalEntity.h"

#include "Color.h"

class Game;

class SnakeSkin : public PhysicalEntity
{
public:
    SnakeSkin(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_color(1, 1, 1, 1), m_isShowing(false)
    {
        // Empty
    }
    
    virtual void update(float deltaTime)
    {
        m_fStateTime += deltaTime;
    }
    
    void onDamageTaken(float x, float y)
    {
        m_position->set(x, y);
        
        m_fStateTime = 0;
        
        m_isShowing = true;
    }
    
    Color& getColor() { return m_color; }
    
private:
    Color m_color;
    bool m_isShowing;
};

typedef enum
{
    EndBossSnakeState_Sleeping,
    EndBossSnakeState_Awakening,
    EndBossSnakeState_OpeningMouthLeft,
    EndBossSnakeState_ChargingLeft,
    EndBossSnakeState_Pursuing,
    EndBossSnakeState_Damaged,
    EndBossSnakeState_OpeningMouthRight,
    EndBossSnakeState_ChargingRight,
    EndBossSnakeState_Dying,
    EndBossSnakeState_Dead
} EndBossSnakeState;

class EndBossSnake : public GridLockedPhysicalEntity
{
public:
    static EndBossSnake* create(int gridX, int gridY, int type)
    {
        return new EndBossSnake(gridX, gridY);
    }
    
    EndBossSnake(int gridX, int gridY, int gridWidth = 54, int gridHeight = 34);
    
    virtual void update(float deltaTime);
    
    void setGame(Game* game) { m_game = game; }
    
    EndBossSnakeState getState() { return m_state; }
    
    Color getColor() { return m_color; }
    
    int getDamage() { return m_iDamage; }
    
    int getType() { return m_type; }
    
private:
    Game* m_game;
    EndBossSnakeState m_state;
    Color m_color;
    int m_iDamage;
    int m_type;
    
    void setState(EndBossSnakeState state);
};

#endif /* defined(__nosfuratu__EndBossSnake__) */
