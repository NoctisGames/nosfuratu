//
//  MidBossOwl.h
//  nosfuratu
//
//  Created by Stephen Gowen on 4/1/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MidBossOwl__
#define __nosfuratu__MidBossOwl__

#include "PhysicalEntity.h"

#include "RTTI.h"

class Game;

#define MID_BOSS_OWL_SLEEPING_WIDTH 5.607421875f
#define MID_BOSS_OWL_SLEEPING_HEIGHT 5.02734375f

#define MID_BOSS_OWL_NORMAL_WIDTH 4.5f
#define MID_BOSS_OWL_NORMAL_HEIGHT 4.5f

#define MID_BOSS_OWL_DYING_WIDTH 6.328125f
#define MID_BOSS_OWL_DYING_HEIGHT 7.03125f

typedef enum
{
    MidBossOwlState_Sleeping,
    MidBossOwlState_Awakening,
    MidBossOwlState_Screeching,
    MidBossOwlState_Pursuing,
    MidBossOwlState_SwoopingDown,
    MidBossOwlState_FlyingAwayAfterCatchingJon,
    MidBossOwlState_SlammingIntoTree,
    MidBossOwlState_FlyingOverTree,
    MidBossOwlState_Dying,
    MidBossOwlState_Dead
} MidBossOwlState;

class MidBossOwl : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    MidBossOwl(float x, float y, float width = MID_BOSS_OWL_SLEEPING_WIDTH, float height = MID_BOSS_OWL_SLEEPING_HEIGHT);
    
    virtual void update(float deltaTime);
    
    MidBossOwlState getState();
    
    void goBackToSleep();
    
    void awaken();
    
    void beginPursuit();
    
    void setGame(Game* game);
    
    int getDamage();
    
    bool didJonTransform() { return m_didJonTransform; }

	void givePlayerAFreeHit() { m_giveFreeHit = true; }
    
private:
    Game* m_game;
    float m_fTreeTopY;
    float m_fGroundTopYWithPadding;
    float m_fTimeUnderTreeTop;
    float m_fHighestSwoopSpeedX;
    int m_iDamage;
    bool m_didJonTransform;
	bool m_giveFreeHit;
    MidBossOwlState m_state;
    
    void setState(MidBossOwlState state);
};

#endif /* defined(__nosfuratu__MidBossOwl__) */
