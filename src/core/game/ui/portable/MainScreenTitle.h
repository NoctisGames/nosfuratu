//
//  MainScreenTitle.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainScreenTitle__
#define __nosfuratu__MainScreenTitle__

#include "MainScreenState.h"

#include "RTTI.h"

class MainScreen;
class TitlePanel;
class GameButton;

class Title : public MainScreenState
{
    RTTI_DECL;
    
public:
    static Title* getInstance();
    
    virtual void enter(MainScreen* ms);
    
    virtual void execute(MainScreen* ms);
    
    virtual void exit(MainScreen* ms);
    
    virtual void initRenderer(MainScreen* ms);
    
    TitlePanel* getTitlePanel();
    
    GameButton* getLevelEditorButton();
    
private:
    TitlePanel* m_panel;
    GameButton* m_levelEditorButton;
    float m_fStateTime;
    bool m_isRequestingNextState;
    bool m_isRequestingLevelEditor;
    float m_fCodeStateTime;
    int m_iResetCodeState;
    int m_iMapCodeState;
    int m_iSwampCodeState;
    int m_iDmCodeState;
    bool m_isLevelEditor;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Title();
    ~Title();
    Title(const Title&);
    Title& operator=(const Title&);
};

#endif /* defined(__nosfuratu__MainScreenTitle__) */
