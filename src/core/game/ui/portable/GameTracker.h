//
//  GameTracker.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/18/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameTracker__
#define __nosfuratu__GameTracker__

#include "Text.h"

#include "RTTI.h"

#include <vector>

class DelayText : public Text
{
    RTTI_DECL;
    
public:
    DelayText(float delay, std::string text, float x, float y, float width, float height);
    
    virtual void update(float deltaTime);
    
private:
    float m_fDelay;
};

class GameTracker
{
    RTTI_DECL;
    
public:
	static GameTracker * getInstance();
    
    void update(float deltaTime);
    
    void reset();
    
    void onScored(int score);
    
    void config(float scoreX, float scoreY, float fontWidth, float fontHeight);
    
    std::vector<DelayText *>& getTexts();

private:
    std::vector<DelayText *> m_texts;
    float m_scoreX;
    float m_scoreY;
    float m_fontWidth;
    float m_fontHeight;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    GameTracker();
    GameTracker(const GameTracker&);
    GameTracker& operator=(const GameTracker&);
};

#endif /* defined(__nosfuratu__GameTracker__) */
