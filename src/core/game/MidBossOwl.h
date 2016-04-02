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

class Game;

typedef enum
{
    MidBossOwlState_Sleeping,
    MidBossOwlState_Awakening,
    MidBossOwlState_Screeching,
    MidBossOwlState_Pursuing,
    MidBossOwlState_SlammingIntoTree,
    MidBossOwlState_Dying,
    MidBossOwlState_Dead,
} MidBossOwlState;

class MidBossOwl : public PhysicalEntity
{
public:
    MidBossOwl(float x, float y, float width = 4.5f, float height = 4.5f);
    
    virtual void update(float deltaTime);
    
    void goBackToSleep();
    
    void awaken();
    
    void beginPursuit();
    
    void setGame(Game* game);
    
private:
    Game* m_game;
    int m_iDamage;
    MidBossOwlState m_state;
    
    void setState(MidBossOwlState state);
};

#endif /* defined(__nosfuratu__MidBossOwl__) */
