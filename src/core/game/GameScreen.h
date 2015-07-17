//
//  GameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <memory>
#include <vector>

#include "GameState.h"

class TouchEvent;
class Vector2D;
class Renderer;

class GameScreen
{
public:
	GameScreen();
    
    virtual void touchToWorld(TouchEvent &touchEvent) = 0;
    
    virtual void platformResume() = 0;
    
    virtual void platformPause() = 0;
    
    virtual bool handleOnBackPressed() = 0;
    
    void init();

	void onResume();
	
	void onPause();
    
	void update(float deltaTime, std::vector<TouchEvent> &touchEvents);
    
    void present();
    
    int getState();
    
    void clearState();

protected:
    std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<Vector2D> m_touchPoint;
	Game_State m_gameState;
    int m_iDeviceScreenWidth;
	int m_iDeviceScreenHeight;
	int m_iScreenState;
};

#endif /* GAME_SCREEN_H */