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
class EndBossSnake;

class SnakeSkin : public PhysicalEntity
{
public:
    SnakeSkin(float x, float y, float width, float height, EndBossSnake* endBossSnake) : PhysicalEntity(x, y, width, height), m_endBossSnake(endBossSnake), m_color(1, 1, 1, 1), m_isShowing(false)
    {
        // Empty
    }
    
    virtual void update(float deltaTime);
    
    void onDamageTaken();
    
    EndBossSnake& getEndBossSnake();
    
    Color& getColor() { return m_color; }
    
private:
    EndBossSnake* m_endBossSnake;
    Color m_color;
    bool m_isShowing;
};

class SnakeEye : public PhysicalEntity
{
public:
    SnakeEye(float x, float y, EndBossSnake* endBossSnake) : PhysicalEntity(x, y, 1.037109375f, 1.037109375f), m_endBossSnake(endBossSnake), m_isWakingUp(false), m_isShowing(true)
    {
        // Empty
    }
    
    virtual void update(float deltaTime);
    
    void onAwaken();
    
    EndBossSnake& getEndBossSnake();
    
private:
    EndBossSnake* m_endBossSnake;
    bool m_isWakingUp;
    bool m_isShowing;
};

class SnakeTonque : public PhysicalEntity
{
public:
    SnakeTonque(float x, float y, EndBossSnake* endBossSnake) : PhysicalEntity(x, y, 4.869140625f, 0.685546875f), m_endBossSnake(endBossSnake), m_isMouthOpen(false)
    {
        // Empty
    }
    
    virtual void update(float deltaTime)
    {
        if (m_isMouthOpen)
        {
            m_fStateTime += deltaTime;
        }
    }
    
    void onMouthOpen();
    
    void onMouthClose() { m_isMouthOpen = false; }
    
    EndBossSnake& getEndBossSnake();
    
    bool isMouthOpen() { return m_isMouthOpen; }
    
private:
    EndBossSnake* m_endBossSnake;
    bool m_isMouthOpen;
};

class SnakeBody : public PhysicalEntity
{
public:
    SnakeBody(float x, float y, float height, EndBossSnake* endBossSnake) : PhysicalEntity(x, y, 35.05078125f, height), m_endBossSnake(endBossSnake), m_color(1, 1, 1, 1)
    {
        // Empty
    }
    
    virtual void update(float deltaTime);
    
    EndBossSnake& getEndBossSnake();
    
    Color& getColor() { return m_color; }
    
private:
    EndBossSnake* m_endBossSnake;
    Color m_color;
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
    
    EndBossSnake(int gridX, int gridY);
    
    virtual void update(float deltaTime);
    
    void triggerHit();
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual int getEntityLandingPriority() { return 0; }
    
    void setGame(Game* game) { m_game = game; }
    
    SnakeSkin& getSnakeSkin() { return *m_snakeSkin; }
    SnakeEye& getSnakeEye() { return *m_snakeEye; }
    SnakeTonque& getSnakeTonque() { return *m_snakeTonque; }
    SnakeBody& getSnakeBody() { return *m_snakeBody; }
    
    EndBossSnakeState getState() { return m_state; }
    
    Color getColor() { return m_color; }
    
    int getDamage() { return m_iDamage; }
    
    int getType() { return m_type; }
    
private:
    std::unique_ptr<SnakeSkin> m_snakeSkin;
    std::unique_ptr<SnakeEye> m_snakeEye;
    std::unique_ptr<SnakeTonque> m_snakeTonque;
    std::unique_ptr<SnakeBody> m_snakeBody;
    Game* m_game;
    EndBossSnakeState m_state;
    Color m_color;
    int m_iDamage;
    int m_type;
    
    void setState(EndBossSnakeState state);
};

#endif /* defined(__nosfuratu__EndBossSnake__) */
