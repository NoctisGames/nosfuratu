//
//  GameScreenTitle.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameScreenTitle__
#define __nosfuratu__GameScreenTitle__

#include "State.h"
#include "GameButton.h"
#include "TitlePanel.h"

#include <memory>

class GameScreen;

class Title : public State<GameScreen>
{
public:
    static Title* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    TitlePanel* getTitlePanel();
    
    GameButton* getLevelEditorButton();
    
    GameButton* getToggleMusicButton();
    
    GameButton* getToggleSoundButton();

	void setIsDisplayingLevelEditorButton(bool isDisplayingLevelEditorButton);
    
private:
    std::unique_ptr<TitlePanel> m_panel;
    std::unique_ptr<GameButton> m_levelEditorButton;
    std::unique_ptr<GameButton> m_toggleMusic;
    std::unique_ptr<GameButton> m_toggleSound;
    bool m_isRequestingNextState;
    bool m_isRequestingLevelEditor;
	bool m_isDisplayingLevelEditorButton;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Title();
    Title(const Title&);
    Title& operator=(const Title&);
};

#endif /* defined(__nosfuratu__GameScreenTitle__) */
