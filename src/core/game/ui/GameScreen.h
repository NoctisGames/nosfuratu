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

#include "TouchType.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "SpriteBatcher.h"
#include "Rectangle.h"
#include "Renderer.h"
#include "Circle.h"
#include "Assets.h"
#include "GameConstants.h"
#include "Game.h"
#include "Jon.h"
#include "StateMachine.h"

class GameScreen
{
public:
    std::unique_ptr<StateMachine<GameScreen>> m_stateMachine;
    std::vector<TouchEvent> m_touchEvents;
    std::vector<TouchEvent> m_touchEventsPool;
    std::vector<TouchEvent> m_touchEventsBuffer;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Vector2D> m_touchPoint;
    std::unique_ptr<Vector2D> m_touchPointDown;
    std::unique_ptr<Game> m_game;
    float m_fDeltaTime;
    
	GameScreen(bool isLevelEditor = false);
    
    virtual void touchToWorld(TouchEvent &touchEvent) = 0;
    
    virtual void init();

	virtual void onResume();
	
	virtual void onPause();
    
	void update(float deltaTime);
    
    void render();
    
    int getState();
    
    void clearState();
    
    short getCurrentMusicId();
    
    short getCurrentSoundId();
    
    void onTouch(Touch_Type type, float raw_touch_x, float raw_touch_y);
    
    void processTouchEvents();
    
private:
    TouchEvent newTouchEvent();
    
    void addTouchEventForType(Touch_Type touchType, float x, float y);
    
    void handleTouchInput();
};

#endif /* GAME_SCREEN_H */