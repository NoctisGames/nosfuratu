//
//  Rock.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Rock__
#define __nosfuratu__Rock__

#include "DestructiblePhysicalEntity.h"
#include "Color.h"

#define ROCK_WIDTH 4.491228070175438f
#define ROCK_HEIGHT 4.305025996533795f

typedef enum
{
    RockType_Rock,
    RockType_CrackedRock
} RockType;

class Rock : public DestructiblePhysicalEntity
{
public:
    static Rock* create(float x, float y, int type);
    
    Rock(float x, float y, float width = ROCK_WIDTH, float height = ROCK_HEIGHT, bool isCracked = false, RockType type = RockType_Rock);
    
    virtual void update(float deltaTime);
    
    virtual void triggerHit();
    
    virtual void updateBounds();
    
    Color getColor();
    
    bool isCracked();
    
    bool isBlowingUp();
    
    RockType getEnumType();
    
    int getType();
    
private:
    RockType m_type;
    Color m_color;
    bool m_isCracked;
    bool m_isBlowingUp;
};

class CrackedRock : public Rock
{
public:
    CrackedRock(float x, float y, float width = ROCK_WIDTH, float height = ROCK_HEIGHT) : Rock(x, y, width, height, true, RockType_CrackedRock) {}
};

#endif /* defined(__nosfuratu__Rock__) */
