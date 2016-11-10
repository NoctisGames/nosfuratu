//
//  GameScreenTitle.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameScreenTitle__
#define __nosfuratu__GameScreenTitle__

#include "GameScreen.h"

#include "GameButton.h"
#include "TitlePanel.h"
#include "GameScreenState.h"
#include "RTTI.h"

#include <memory>

class Title : public GameScreenState
{
    RTTI_DECL;
    
public:
    static Title* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    virtual void initRenderer(GameScreen* gs);
    
    TitlePanel* getTitlePanel();
    
    GameButton* getLevelEditorButton();
    
private:
    std::unique_ptr<TitlePanel> m_panel;
    std::unique_ptr<GameButton> m_levelEditorButton;
    std::unique_ptr<GameButton> m_spriteTesterButton;
    bool m_isRequestingNextState;
    bool m_isRequestingLevelEditor;
    bool m_isRequestingSpriteTester;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Title();
    Title(const Title&);
    Title& operator=(const Title&);
};

#endif /* defined(__nosfuratu__GameScreenTitle__) */
