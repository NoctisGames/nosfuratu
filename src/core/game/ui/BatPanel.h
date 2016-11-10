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
#include "macros.h"
#include "Assets.h"
#include "RTTI.h"

#include <math.h>

class Game;
class GameScreen;
class Jon;

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
	BatGoalType_DrillToDamageOwl,
    BatGoalType_Stomp,
    BatGoalType_Dash,
} BatGoalType;

class BatInstruction : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    BatInstruction() : PhysicalEntity(1337, 1337, 4.376953125f, 3.462890625f), m_type(BatInstructionType_None), m_color(1, 1, 1, 1), m_isClosing(false), m_isOpening(false), m_isOpen(false) { }
    
    virtual void update(float deltaTime)
    {
        PhysicalEntity::update(deltaTime);
        
        if (m_isOpening)
        {
            if (m_fStateTime > 0.3f)
            {
                m_fStateTime = 0;
                m_isOpening = false;
                m_isOpen = true;
            }
        }
        else if (m_isClosing)
        {
            m_color.alpha -= deltaTime * 2;
            
            if (m_color.alpha < 0)
            {
                m_isClosing = false;
                m_isOpen = false;
                
                m_position->set(1337, 1337);
            }
        }
    }
    
    void open(BatInstructionType type, float x, float y)
    {
        m_type = type;
        
        m_position->set(x, y);
        
        m_fStateTime = 0;
        m_isOpening = true;
        m_isOpen = false;
        
        m_color.alpha = 1;
    }
    
    void close()
    {
        m_fStateTime = 0;
        m_isOpening = false;
        m_isOpen = true;
        m_isClosing = true;
        
        m_color.alpha = 1;
    }
    
    void reset()
    {
        m_type = BatInstructionType_None;
        
        m_position->set(1337, 1337);
        
        m_fStateTime = 0;
        
        m_isClosing = false;
        m_isOpening = false;
        m_isOpen = false;
        
        m_color.alpha = 1;
    }
    
    BatInstructionType getType() { return m_type; }
    
    Color& getColor() { return m_color; }
    
    bool isOpening() { return m_isOpening; }
    
    bool isOpen() { return m_isOpen; }
    
private:
    BatInstructionType m_type;
    Color m_color;
    bool m_isClosing;
    bool m_isOpening;
    bool m_isOpen;
};

class Bat : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    Bat() : PhysicalEntity(1337, 1337, 1.44140625f, 1.388671875f)
    {
        m_target = std::unique_ptr<Vector2D>(new Vector2D());
        m_isInPosition = false;
    }
    
    virtual void update(float deltaTime)
    {
        PhysicalEntity::update(deltaTime);
        
        if (!m_isInPosition)
        {
            m_isInPosition = !(m_target->dist(*m_position) > 0.25f);
            
            if (m_isInPosition)
            {
                m_velocity->set(0, 0);
            }
        }
    }
    
    void naviPoof(float x, float y)
    {
        m_position->set(x, y);
        m_target->set(x, y);
        
        m_fStateTime = 0;
        m_isInPosition = true;
        
        ASSETS->addSoundIdToPlayQueue(SOUND_BAT_POOF);
    }
    
    void moveTo(float x, float y)
    {
        m_target->set(x, y);
        
        if (m_target->dist(*m_position) > 6)
        {
            naviPoof(x, y);
            
            return;
        }
        
        float angle = m_target->cpy().sub(m_position->getX(), m_position->getY()).angle();
        float radians = DEGREES_TO_RADIANS(angle);
        
        m_velocity->set(cosf(radians) * 6, sinf(radians) * 6);
        
        m_isInPosition = false;
    }
    
    void reset()
    {
        m_position->set(1337, 1337);
        m_target->set(0, 0);
        
        m_fStateTime = 0;
        m_isInPosition = false;
    }
    
    bool isInPosition() { return m_isInPosition && m_fStateTime > 0.80f; }

private:
    std::unique_ptr<Vector2D> m_target;
    bool m_isInPosition;
};

class BatPanel
{
    RTTI_DECL;
    
public:
    BatPanel();
    
    virtual void update(GameScreen* gs);
    
    void config(Game* game, int world, int level);
    
    void configWithoutUi(Game* game, int world, int level);
    
    void config(Game* game, BatGoalType type);
    
    void configWithoutUi(Game* game, BatGoalType type);
    
    void reset();
    
    bool isRequestingInput() { return m_isRequestingInput; }
    
    Bat* getBat() { return m_bat.get(); }
    
    BatInstruction* getBatInstruction() { return m_batInstruction.get(); }
    
private:
    std::unique_ptr<Bat> m_bat;
    std::unique_ptr<BatInstruction> m_batInstruction;
    Game* m_game;
    BatGoalType m_type;
    float m_fJonX;
    bool m_isRequestingInput;
    bool m_isAcknowledgedPart1;
    bool m_isAcknowledgedPart2;
    bool m_isAcknowledgedPart3;
    bool m_isAcknowledgedPart4;
    bool m_isAcknowledgedPart5;
    bool m_hasTriggeredRequestedAction;
    bool m_hasSwiped;
    
    void enableAbilityAndReset();
    
    void updateJump(GameScreen* gs);
    
    void updateDoubleJump(GameScreen* gs);
    
    void updateVampire(GameScreen* gs);
    
    void updateDrill(GameScreen* gs);
    
    void updateDrillToDamageOwl(GameScreen* gs);
    
    void updateStomp(GameScreen* gs);
    
    void updateDash(GameScreen* gs);
    
    void showBatNearJon(Jon& jon);
    
    void showBatInstruction(BatInstructionType type);
    
    void handleTouchInput(GameScreen* gs);
};

#endif /* defined(__nosfuratu__BatPanel__) */
