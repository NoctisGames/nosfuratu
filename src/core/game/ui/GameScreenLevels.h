//
//  GameScreenLevels.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__GameScreenLevels__
#define __nosfuratu__GameScreenLevels__

#include "State.h"
#include "BackButton.h"
#include "LevelEditorButton.h"

#include <memory>

class GameScreen;
class Game;

class Level : public State<GameScreen>
{
public:
    static Level* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    void setSourceGame(Game* game);
    
    Game& getGame();
    
    BackButton& getBackButton();
    
protected:
    const char* m_json;
    std::unique_ptr<Game> m_game;
    Game* m_sourceGame;
    std::unique_ptr<BackButton> m_backButton;
    float m_fStateTime;
    bool m_isReleasingShockwave;
    float m_fShockwaveCenterX;
    float m_fShockwaveCenterY;
    float m_fShockwaveElapsedTime;
    bool m_hasShownOpeningSequence;
    bool m_hasOpeningSequenceCompleted;
    bool m_activateRadialBlur;
    bool m_hasSwiped;
    bool m_showDeathTransOut;
    
    bool handleOpeningSequenceTouchInput(GameScreen* gs);
    bool handleTouchInput(GameScreen* gs);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Level(const char* m_json);
    Level(const Level&);
    Level& operator=(const Level&);
};

class Chapter1Level1 : public Level
{
public:
    static Chapter1Level1* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level1(const char* json) : Level(json) {};
    Chapter1Level1(const Chapter1Level1&);
    Chapter1Level1& operator=(const Chapter1Level1&);
};

class Chapter1Level2 : public Level
{
public:
    static Chapter1Level2* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level2(const char* json) : Level(json) {};
    Chapter1Level2(const Chapter1Level2&);
    Chapter1Level2& operator=(const Chapter1Level2&);
};

class Chapter1Level3 : public Level
{
public:
    static Chapter1Level3* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level3(const char* json) : Level(json) {};
    Chapter1Level3(const Chapter1Level3&);
    Chapter1Level3& operator=(const Chapter1Level3&);
};

#endif /* defined(__nosfuratu__GameScreenLevels__) */
