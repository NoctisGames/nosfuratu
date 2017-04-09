//
//  GameButtonContainer.h
//  nosfuratu
//
//  Created by Stephen Gowen on 4/9/17.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameButtonContainer__
#define __nosfuratu__GameButtonContainer__

#include "PhysicalEntity.h"

#include "GameConstants.h"

#include "RTTI.h"

class MainScreen;
class GameButton;

#define GAME_BUTTON_CONTAINER_RC_UNHANDLED -1
#define GAME_BUTTON_CONTAINER_RC_HANDLED 0
#define GAME_BUTTON_CONTAINER_RC_SIGN_IN 1
#define GAME_BUTTON_CONTAINER_RC_SIGN_OUT 2
#define GAME_BUTTON_CONTAINER_RC_SHOW_LEADERBOARDS 3
#define GAME_BUTTON_CONTAINER_RC_SHOW_ACHIEVEMENTS 4

class GameButtonContainer : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    GameButtonContainer(float x = CAM_WIDTH - 1.518956801470588f / 2, float y = CAM_HEIGHT - (CAM_HEIGHT * 0.627450980392157f) / 2, float width = 1.518956801470588f, float height = CAM_HEIGHT * 0.627450980392157f);
    
    ~GameButtonContainer();
    
    virtual void update(float deltaTime);
    
    int handleInput();
    
    void config(MainScreen* screen);
    
    void open();
    
    void close();
    
    bool isOpening();
    
    bool isOpen();
    
    bool isClosed();
    
    GameButton* getGooglePlayController();
    GameButton* getGooglePlayAchievements();
    GameButton* getGooglePlayLeaderboards();
    GameButton* getGooglePlaySignInOut();
    
private:
    GameButton* m_googlePlayController;
    GameButton* m_googlePlayAchievements;
    GameButton* m_googlePlayLeaderboards;
    GameButton* m_googlePlaySignInOut;
    
    MainScreen* m_screen;
    bool m_isOpening;
};

#endif /* defined(__nosfuratu__GameButtonContainer__) */
