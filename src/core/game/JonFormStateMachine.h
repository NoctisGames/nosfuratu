//
//  JonFormStateMachine.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/4/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__JonFormStateMachine__
#define __noctisgames__JonFormStateMachine__

#include <cassert>

#include "JonFormState.h"

class JonFormStateMachine
{
public:
    
    JonFormStateMachine(Jon* owner) : m_owner(owner), m_currentState(nullptr), m_previousState(nullptr), m_globalState(nullptr)
    {
        // Empty
    }
    
    virtual ~JonFormStateMachine()
    {
        // Empty
    }
    
    void setCurrentState(JonFormState* state)
    {
        m_currentState = state;
    }
    
    void setGlobalState(JonFormState* state)
    {
        m_globalState = state;
    }
    
    void setPreviousState(JonFormState* state)
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
    
    void changeState(JonFormState* newState)
    {
        assert(newState && "<JonFormStateMachine::ChangeState>:trying to assign null state to current");
        
        m_previousState = m_currentState;
        
        m_currentState->exit(m_owner);
        
        m_currentState = newState;
        
        m_currentState->enter(m_owner);
    }
    
    void revertToPreviousState()
    {
        changeState(m_previousState);
    }
    
    bool isInState(const JonFormState* st)const
    {
        return m_currentState == st;
    }
    
    JonFormState* getCurrentState() const
    {
        return m_currentState;
    }
    
    JonFormState* getGlobalState() const
    {
        return m_globalState;
    }
    
    JonFormState* getPreviousState() const
    {
        return m_previousState;
    }
    
private:
    Jon* m_owner;
    
    JonFormState* m_currentState;
    JonFormState* m_previousState;
    JonFormState* m_globalState;
};

#endif /* defined(__noctisgames__JonFormStateMachine__) */
