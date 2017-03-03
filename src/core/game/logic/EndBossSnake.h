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

#include "EndBossSnakeState.h"
#include "Color.h"

#include "RTTI.h"

#include <vector>

class Game;

class SnakeSkin;
class SnakeEye;
class SnakeTonque;
class SnakeBody;
class SnakeHeadImpact;
class SnakeSpirit;

class EndBossSnake : public GridLockedPhysicalEntity
{
    RTTI_DECL;
    
public:
    static EndBossSnake* create(int gridX, int gridY, int type);
    
    EndBossSnake(int gridX, int gridY);
    
    virtual ~EndBossSnake();
    
    virtual void update(float deltaTime);
    
	void begin();

	void awaken();

	void beginPursuit();

    void triggerHit();

	void checkPointKill();
    
    virtual bool isEntityLanding(PhysicalEntity* entity, float deltaTime);
    
    virtual int getEntityLandingPriority();
    
    Game* getGame();
    void setGame(Game* game);
    
    std::vector<EndBossSnake *>& getAfterImages();
    SnakeSkin& getSnakeSkin();
    SnakeEye& getSnakeEye();
    SnakeTonque& getSnakeTonque();
    SnakeBody& getSnakeBody();
    SnakeHeadImpact& getSnakeHeadImpact();
    SnakeSpirit& getSnakeSpirit();
    
    EndBossSnakeState getState();
    
    Color getColor();
    
    int getDamage();
    
    int getType();
    
private:
    std::vector<EndBossSnake *> m_afterImages;
    SnakeSkin* m_snakeSkin;
    SnakeEye* m_snakeEye;
    SnakeTonque* m_snakeTonque;
    SnakeBody* m_snakeBody;
	SnakeHeadImpact* m_snakeHeadImpact;
    SnakeSpirit* m_snakeSpirit;
    Game* m_game;
    EndBossSnakeState m_state;
    Color m_color;
    float m_fTimeSinceLastVelocityCheck;
    int m_iDamage;
    int m_type;
    bool m_hasPlayedChargeSound;
    
    void setState(EndBossSnakeState state);
};

class SnakeSpirit : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    SnakeSpirit(float x, float y, EndBossSnake* endBossSnake);
    
    virtual void update(float deltaTime);
    
    void onDeath();
    
    EndBossSnake& getEndBossSnake();
    
    Color& getColor();
    
private:
    EndBossSnake* m_endBossSnake;
    Color m_color;
    bool m_isShowing;
};

class SnakeHeadImpact : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    SnakeHeadImpact(float x, float y, EndBossSnake* endBossSnake);
    
    virtual void update(float deltaTime);
    
    void onDamageTaken();
    
    EndBossSnake& getEndBossSnake();
    
    Color& getColor();
    
private:
    EndBossSnake* m_endBossSnake;
    Color m_color;
    bool m_isShowing;
};

class SnakeSkin : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    SnakeSkin(float x, float y, float width, float height, EndBossSnake* endBossSnake);
    
    virtual void update(float deltaTime);
    
    void onDamageTaken();
    
    EndBossSnake& getEndBossSnake();
    
    Color& getColor();
    
private:
    EndBossSnake* m_endBossSnake;
    Color m_color;
    bool m_isShowing;
};

class SnakeEye : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    SnakeEye(float x, float y, EndBossSnake* endBossSnake);
    
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
    RTTI_DECL;
    
public:
    SnakeTonque(float x, float y, EndBossSnake* endBossSnake);
    
    virtual void update(float deltaTime);
    
    void onMouthOpen();
    
    void onMouthClose();
    
    EndBossSnake& getEndBossSnake();
    
    bool isMouthOpen();
    
private:
    EndBossSnake* m_endBossSnake;
    bool m_isMouthOpen;
};

class SnakeBody : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    SnakeBody(float x, float y, float height, EndBossSnake* endBossSnake);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    void onDeath();
    
    EndBossSnake& getEndBossSnake();
    
    Color& getColor();
    
private:
    EndBossSnake* m_endBossSnake;
    Color m_color;
    bool m_isDead;
};

#endif /* defined(__nosfuratu__EndBossSnake__) */
