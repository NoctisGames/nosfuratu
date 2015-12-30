//
//  SnakeEnemy.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/27/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__SnakeEnemy__
#define __nosfuratu__SnakeEnemy__

#include "DestructiblePhysicalEntity.h"
#include "Color.h"
#include "SnakeSpirit.h"

#include <memory>
#include <vector>

class Game;

class SnakeEnemy : public DestructiblePhysicalEntity
{
public:
    SnakeEnemy(float x, float y, float width, float height);
    
    virtual void update(float deltaTime);
    
    virtual void triggerHit();
    
    virtual void onDeletion();
    
    std::vector<std::unique_ptr<SnakeSpirit>>& getSnakeSpirits();
    
    Color getColor();
    
    float getColorAdditive();
    
    bool isDying();
    
    bool isDead();
    
    bool hasKilledJon();
    
    virtual bool canBeHitHorizontally();
    
    virtual bool canBeLandedOnToKill();
    
    virtual bool canBeHitFromBelow();
    
    void setGame(Game* game);
    
    int getType();

protected:
    Game* m_game;
    bool m_isDying;
    bool m_hasKilledJon;
    
private:
    std::vector<std::unique_ptr<SnakeSpirit>> m_snakeSpirits;
    Color m_color;
    float m_fColorAdditive;
    float m_fSpiritStateTime;
    float m_fXOfDeath;
    float m_fYOfDeath;
    bool m_isDead;
};

#endif /* defined(__nosfuratu__SnakeEnemy__) */
