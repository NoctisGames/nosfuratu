//
//  GameScreenStateMachine.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/4/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__GameScreenStateMachine__
#define __noctisgames__GameScreenStateMachine__

#include <cassert>

#include "GameScreenState.h"

class GameScreenStateMachine
{
public:
    
    GameScreenStateMachine(GameScreen* owner) : m_owner(owner), m_currentState(nullptr), m_previousState(nullptr), m_globalState(nullptr)
    {
        // Empty
    }
    
    virtual ~GameScreenStateMachine()
    {
        // Empty
    }
    
    void setCurrentState(GameScreenState* state)
    {
        m_currentState = state;
    }
    
    void setGlobalState(GameScreenState* state)
    {
        m_globalState = state;
    }
    
    void setPreviousState(GameScreenState* state)
    {
        m_previousState = state;
    }
    
    void execute() const
    {
        if (m_globalState)
        {
            m_globalState->execute(m_owner);
        }
        
        if (m_currentState)
        {
            m_currentState->execute(m_owner);
        }
    }
    
    void changeState(GameScreenState* newState)
    {
        assert(newState && "<GameScreenStateMachine::ChangeState>:trying to assign null state to current");
        
        m_previousState = m_currentState;
        
        m_currentState->exit(m_owner);
        
        m_currentState = newState;
        
        m_currentState->enter(m_owner);
    }
    
    void revertToPreviousState()
    {
        changeState(m_previousState);
    }
    
    bool isInState(const GameScreenState* st)const
    {
        return m_currentState == st;
    }
    
    GameScreenState* getCurrentState() const
    {
        return m_currentState;
    }
    
    GameScreenState* getGlobalState() const
    {
        return m_globalState;
    }
    
    GameScreenState* getPreviousState() const
    {
        return m_previousState;
    }
    
private:
    GameScreen* m_owner;
    
    GameScreenState* m_currentState;
    GameScreenState* m_previousState;
    GameScreenState* m_globalState;
};

#endif /* defined(__noctisgames__GameScreenStateMachine__) */
