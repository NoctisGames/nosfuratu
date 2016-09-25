//
//  EndBossSnake.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/24/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__EndBossSnake__
#define __nosfuratu__EndBossSnake__

#include "PhysicalEntity.h"

class Game;

#define MID_BOSS_OWL_SLEEPING_WIDTH 5.607421875f
#define MID_BOSS_OWL_SLEEPING_HEIGHT 5.02734375f

#define MID_BOSS_OWL_NORMAL_WIDTH 4.5f
#define MID_BOSS_OWL_NORMAL_HEIGHT 4.5f

#define MID_BOSS_OWL_DYING_WIDTH 6.328125f
#define MID_BOSS_OWL_DYING_HEIGHT 7.03125f

typedef enum
{
    EndBossSnakeState_Sleeping,
    EndBossSnakeState_Awakening,
    EndBossSnakeState_Screeching,
    EndBossSnakeState_Pursuing,
    EndBossSnakeState_SwoopingDown,
    EndBossSnakeState_FlyingAwayAfterCatchingJon,
    EndBossSnakeState_SlammingIntoTree,
    EndBossSnakeState_FlyingOverTree,
    EndBossSnakeState_Dying,
    EndBossSnakeState_Dead
} EndBossSnakeState;

class EndBossSnake : public PhysicalEntity
{
public:
    EndBossSnake(float x, float y, float width = MID_BOSS_OWL_SLEEPING_WIDTH, float height = MID_BOSS_OWL_SLEEPING_HEIGHT);
    
    virtual void update(float deltaTime);
    
    EndBossSnakeState getState();
    
    void goBackToSleep();
    
    void awaken();
    
    void beginPursuit();
    
    void setGame(Game* game);
    
    int getDamage();
    
    void setState(EndBossSnakeState state);
    
    bool didJonTransform() { return m_didJonTransform; }

	void givePlayerAFreeHit() { m_giveFreeHit = true; }
    
private:
    Game* m_game;
    float m_fTreeTopY;
    float m_fGroundTopYWithPadding;
    float m_fTimeUnderTreeTop;
    int m_iDamage;
    bool m_didJonTransform;
	bool m_giveFreeHit;
    EndBossSnakeState m_state;
};

#endif /* defined(__nosfuratu__EndBossSnake__) */
