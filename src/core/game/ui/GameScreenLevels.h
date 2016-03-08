//
//  GameScreenLevels.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
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
    Level(int level = 1337, const char* m_json = nullptr);
    Level(const Level&);
    Level& operator=(const Level&);
};

class Chapter1Level1 : public Level
{
public:
    static Chapter1Level1* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level1(const char* json) : Level(1, json) {};
    Chapter1Level1(const Chapter1Level1&);
    Chapter1Level1& operator=(const Chapter1Level1&);
};

class Chapter1Level2 : public Level
{
public:
    static Chapter1Level2* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level2(const char* json) : Level(2, json) {};
    Chapter1Level2(const Chapter1Level2&);
    Chapter1Level2& operator=(const Chapter1Level2&);
};

class Chapter1Level3 : public Level
{
public:
    static Chapter1Level3* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level3(const char* json) : Level(3, json) {};
    Chapter1Level3(const Chapter1Level3&);
    Chapter1Level3& operator=(const Chapter1Level3&);
};

class Chapter1Level4 : public Level
{
public:
    static Chapter1Level4* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level4(const char* json) : Level(4, json) {};
    Chapter1Level4(const Chapter1Level4&);
    Chapter1Level4& operator=(const Chapter1Level4&);
};

class Chapter1Level5 : public Level
{
public:
    static Chapter1Level5* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level5(const char* json) : Level(5, json) {};
    Chapter1Level5(const Chapter1Level5&);
    Chapter1Level5& operator=(const Chapter1Level5&);
};

class Chapter1Level6 : public Level
{
public:
    static Chapter1Level6* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level6(const char* json) : Level(6, json) {};
    Chapter1Level6(const Chapter1Level6&);
    Chapter1Level6& operator=(const Chapter1Level6&);
};

class Chapter1Level7 : public Level
{
public:
    static Chapter1Level7* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level7(const char* json) : Level(7, json) {};
    Chapter1Level7(const Chapter1Level7&);
    Chapter1Level7& operator=(const Chapter1Level7&);
};

class Chapter1Level8 : public Level
{
public:
    static Chapter1Level8* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level8(const char* json) : Level(8, json) {};
    Chapter1Level8(const Chapter1Level8&);
    Chapter1Level8& operator=(const Chapter1Level8&);
};

class Chapter1Level9 : public Level
{
public:
    static Chapter1Level9* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level9(const char* json) : Level(9, json) {};
    Chapter1Level9(const Chapter1Level9&);
    Chapter1Level9& operator=(const Chapter1Level9&);
};

class Chapter1Level10 : public Level
{
public:
    static Chapter1Level10* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level10(const char* json) : Level(10, json) {};
    Chapter1Level10(const Chapter1Level10&);
    Chapter1Level10& operator=(const Chapter1Level10&);
};

#endif /* defined(__nosfuratu__GameScreenLevels__) */
