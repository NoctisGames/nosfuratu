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
#include "BatPanel.h"

#include <memory>

class GameScreen;
class Game;
class MidBossOwl;
class ForegroundObject;

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
    std::unique_ptr<BatPanel> m_batPanel;
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
    bool m_exitLoop;
    bool m_hasCompletedLevel;
    
    virtual void update(GameScreen* gs);
    
    virtual void render(GameScreen* gs);
    
    virtual void updateCamera(GameScreen* gs, bool instant = false);
    
    virtual void additionalRenderingBeforeHud(GameScreen* gs);
    
    virtual bool isInSlowMotionMode();
    
    bool handleOpeningSequenceTouchInput(GameScreen* gs);
    bool handleTouchInput(GameScreen* gs);
    
    void openBatPanelWithType(BatPanelType batPanelType);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Level(const char* m_json = nullptr);
    Level(const Level&);
    Level& operator=(const Level&);
    
private:
    BatPanelType getBatPanelType();
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

class Chapter1Level4 : public Level
{
public:
    static Chapter1Level4* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level4(const char* json) : Level(json) {};
    Chapter1Level4(const Chapter1Level4&);
    Chapter1Level4& operator=(const Chapter1Level4&);
};

class Chapter1Level5 : public Level
{
public:
    static Chapter1Level5* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level5(const char* json) : Level(json) {};
    Chapter1Level5(const Chapter1Level5&);
    Chapter1Level5& operator=(const Chapter1Level5&);
};

class Chapter1Level6 : public Level
{
public:
    static Chapter1Level6* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level6(const char* json) : Level(json) {};
    Chapter1Level6(const Chapter1Level6&);
    Chapter1Level6& operator=(const Chapter1Level6&);
};

class Chapter1Level7 : public Level
{
public:
    static Chapter1Level7* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level7(const char* json) : Level(json) {};
    Chapter1Level7(const Chapter1Level7&);
    Chapter1Level7& operator=(const Chapter1Level7&);
};

class Chapter1Level8 : public Level
{
public:
    static Chapter1Level8* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level8(const char* json) : Level(json) {};
    Chapter1Level8(const Chapter1Level8&);
    Chapter1Level8& operator=(const Chapter1Level8&);
};

class Chapter1Level9 : public Level
{
public:
    static Chapter1Level9* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level9(const char* json) : Level(json) {};
    Chapter1Level9(const Chapter1Level9&);
    Chapter1Level9& operator=(const Chapter1Level9&);
};

class Chapter1Level10 : public Level
{
public:
    static Chapter1Level10* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
private:
    std::unique_ptr<MidBossOwl> m_midBossOwl;
    ForegroundObject* m_perchTree;
    float m_fJonY;
    float m_fGameStateTime;
    float m_fIdleWaitTime;
    float m_fMusicVolume;
    int m_iNumAttempts;
    int m_iLastKnownOwlDamage;
    int m_iLastKnownJonNumBoosts;
    bool m_isIdleWaitingForOwl;
    bool m_hasTriggeredMidBossMusicLoopIntro;
    bool m_hasTriggeredMidBossMusicLoop;
    bool m_isChaseCamActivated;
    bool m_hasTriggeredBurrow;
    
    virtual void update(GameScreen* gs);
    
    virtual void updateCamera(GameScreen* gs, bool instant = false);
    
    virtual void additionalRenderingBeforeHud(GameScreen* gs);
    
    virtual bool isInSlowMotionMode();
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level10(const char* json);
    Chapter1Level10(const Chapter1Level10&);
    Chapter1Level10& operator=(const Chapter1Level10&);
};

class Chapter1Level11 : public Level
{
public:
    static Chapter1Level11* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level11(const char* json) : Level(json) {};
    Chapter1Level11(const Chapter1Level11&);
    Chapter1Level11& operator=(const Chapter1Level11&);
};

class Chapter1Level12 : public Level
{
public:
    static Chapter1Level12* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level12(const char* json) : Level(json) {};
    Chapter1Level12(const Chapter1Level12&);
    Chapter1Level12& operator=(const Chapter1Level12&);
};

class Chapter1Level13 : public Level
{
public:
    static Chapter1Level13* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level13(const char* json) : Level(json) {};
    Chapter1Level13(const Chapter1Level13&);
    Chapter1Level13& operator=(const Chapter1Level13&);
};

class Chapter1Level14 : public Level
{
public:
    static Chapter1Level14* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level14(const char* json) : Level(json) {};
    Chapter1Level14(const Chapter1Level14&);
    Chapter1Level14& operator=(const Chapter1Level14&);
};

class Chapter1Level15 : public Level
{
public:
    static Chapter1Level15* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level15(const char* json) : Level(json) {};
    Chapter1Level15(const Chapter1Level15&);
    Chapter1Level15& operator=(const Chapter1Level15&);
};

class Chapter1Level16 : public Level
{
public:
    static Chapter1Level16* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level16(const char* json) : Level(json) {};
    Chapter1Level16(const Chapter1Level16&);
    Chapter1Level16& operator=(const Chapter1Level16&);
};

class Chapter1Level17 : public Level
{
public:
    static Chapter1Level17* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level17(const char* json) : Level(json) {};
    Chapter1Level17(const Chapter1Level17&);
    Chapter1Level17& operator=(const Chapter1Level17&);
};

class Chapter1Level18 : public Level
{
public:
    static Chapter1Level18* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level18(const char* json) : Level(json) {};
    Chapter1Level18(const Chapter1Level18&);
    Chapter1Level18& operator=(const Chapter1Level18&);
};

class Chapter1Level19 : public Level
{
public:
    static Chapter1Level19* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level19(const char* json) : Level(json) {};
    Chapter1Level19(const Chapter1Level19&);
    Chapter1Level19& operator=(const Chapter1Level19&);
};

class Chapter1Level20 : public Level
{
public:
    static Chapter1Level20* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level20(const char* json) : Level(json) {};
    Chapter1Level20(const Chapter1Level20&);
    Chapter1Level20& operator=(const Chapter1Level20&);
};

class Chapter1Level21 : public Level
{
public:
    static Chapter1Level21* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level21(const char* json) : Level(json) {};
    Chapter1Level21(const Chapter1Level21&);
    Chapter1Level21& operator=(const Chapter1Level21&);
};

#endif /* defined(__nosfuratu__GameScreenLevels__) */
