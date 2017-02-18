//
//  MainScreenTitle.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainScreenTitle__
#define __nosfuratu__MainScreenTitle__

#include "MainScreen.h"

#include "GameButton.h"
#include "TitlePanel.h"
#include "MainScreenState.h"
#include "RTTI.h"

#include <memory>

class Title : public MainScreenState
{
    RTTI_DECL;
    
public:
    static Title* getInstance();
    
    virtual void enter(MainScreen* gs);
    
    virtual void execute(MainScreen* gs);
    
    virtual void exit(MainScreen* gs);
    
    TitlePanel* getTitlePanel();
    
    GameButton* getLevelEditorButton();
    
private:
    std::unique_ptr<TitlePanel> m_panel;
    std::unique_ptr<GameButton> m_levelEditorButton;
    bool m_isRequestingNextState;
    bool m_isRequestingLevelEditor;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Title();
    Title(const Title&);
    Title& operator=(const Title&);
};

#endif /* defined(__nosfuratu__MainScreenTitle__) */
