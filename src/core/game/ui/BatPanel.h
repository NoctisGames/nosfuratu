//
//  BatPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 4/4/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__BatPanel__
#define __nosfuratu__BatPanel__

#include "PhysicalEntity.h"

#include "BatInstructionType.h"
#include "BatGoalType.h"

#include "RTTI.h"

class MainScreen;
class Game;
class Jon;
class Vector2D;

class Bat;
class BatInstruction;

class BatPanel
{
    RTTI_DECL;
    
public:
    BatPanel();
    
    virtual ~BatPanel();
    
    virtual void update(MainScreen* ms);
    
    void config(Game* game, int world, int level);
    
    void configWithoutUi(Game* game, int world, int level);
    
    void config(Game* game, BatGoalType type);
    
    void configWithoutUi(Game* game, BatGoalType type);
    
    void reset();
    
    bool isRequestingInput();
    
    Bat* getBat();
    
    BatInstruction* getBatInstruction();
    
private:
    Bat* m_bat;
    BatInstruction* m_batInstruction;
    Game* m_game;
    BatGoalType m_type;
    float m_fJonX;
    float m_fJonVelocityX;
    bool m_isRequestingInput;
    bool m_isAcknowledgedPart1;
    bool m_isAcknowledgedPart2;
    bool m_isAcknowledgedPart3;
    bool m_isAcknowledgedPart4;
    bool m_isAcknowledgedPart5;
    bool m_hasTriggeredRequestedAction;
    bool m_hasSwiped;
    
    void enableAbilityAndReset();
    
    void updateJump(MainScreen* ms);
    
    void updateDoubleJump(MainScreen* ms);
    
    void updateVampire(MainScreen* ms);
    
    void updateDrill(MainScreen* ms);
    
    void updateDrillToDamageOwl(MainScreen* ms);
    
    void updateStomp(MainScreen* ms);
    
    void updateDash(MainScreen* ms);
    
    void showBatNearJon(Jon& jon);
    
    void showBatInstruction(BatInstructionType type);
};

class BatInstruction : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    BatInstruction();
    
    virtual void update(float deltaTime);
    
    void open(BatInstructionType type, float x, float y);
    
    void close();
    
    void reset();
    
    BatInstructionType getType();
    
    bool isOpening();
    
    bool isOpen();
    
private:
    BatInstructionType m_type;
    bool m_isClosing;
    bool m_isOpening;
    bool m_isOpen;
};

class Bat : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    Bat();
    
    virtual ~Bat();
    
    virtual void update(float deltaTime);
    
    void naviPoof(float x, float y);
    
    void moveTo(float x, float y);
    
    void reset();
    
    bool isInPosition();
    
private:
    Vector2D* m_target;
    bool m_isInPosition;
};

#endif /* defined(__nosfuratu__BatPanel__) */
