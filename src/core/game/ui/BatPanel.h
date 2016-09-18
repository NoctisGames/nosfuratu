//
//  BatPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 4/4/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__BatPanel__
#define __nosfuratu__BatPanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Color.h"

class Game;
class GameScreen;

typedef enum
{
    BatInstructionType_None,
    BatInstructionType_Tap,
    BatInstructionType_TapHold,
    BatInstructionType_SwipeDown,
    BatInstructionType_SwipeRight
} BatInstructionType;

typedef enum
{
    BatGoalType_None,
    BatGoalType_Jump,
    BatGoalType_DoubleJump,
    BatGoalType_Vampire,
    BatGoalType_Drill,
    BatGoalType_Stomp,
    BatGoalType_Dash,
} BatGoalType;

class BatInstruction : public PhysicalEntity
{
public:
    BatInstruction() : PhysicalEntity(1337, 1337, 4.376953125f, 3.462890625f), m_color(1, 1, 1, 1) { }
    
    virtual void update(float deltaTime);
    
    void open(float x, float y)
    {
        m_position->set(x, y);
        
        m_fStateTime = 0;
    }
    
    Color& getColor() { return m_color; }
    
private:
    Color m_color;
};

class Bat : public PhysicalEntity
{
public:
    Bat() : PhysicalEntity(1337, 1337, 1.44140625f, 1.388671875f) { }
    
    virtual void update(float deltaTime);
    
    void naviPoof(float x, float y)
    {
        m_position->set(x, y);
        
        m_fStateTime = 0;
    }
};

class BatPanel
{
public:
    BatPanel(Game* game, int world, int level);
    
    virtual void update(GameScreen* gs);
    
private:
    std::unique_ptr<Bat> m_bat;
    std::unique_ptr<BatInstruction> m_batInstruction;
    Game* m_game;
    BatGoalType m_type;
    bool m_isRequestingInput;
};

#endif /* defined(__nosfuratu__BatPanel__) */
