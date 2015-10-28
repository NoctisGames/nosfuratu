//
//  GameScreenStates.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__GameScreenStates__
#define __nosfuratu__GameScreenStates__

#include "State.h"
#include "BackButton.h"
#include "LevelEditorActionsPanel.h"
#include "LevelEditorEntitiesPanel.h"
#include "TrashCan.h"
#include "LevelSelectorPanel.h"

#include <memory>

class GameScreen;

class Title : public State<GameScreen>
{
public:
    static Title* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Title() {};
    Title(const Title&);
    Title& operator=(const Title&);
};

class GamePlay : public State<GameScreen>
{
public:
    static GamePlay* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
private:
    std::unique_ptr<Game> m_game;
    bool m_hasShownOpeningSequence;
    bool m_hasOpeningSequenceCompleted;
    
    void handleTouchInput(GameScreen* gs);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    GamePlay();
    GamePlay(const GamePlay&);
    GamePlay& operator=(const GamePlay&);
};

class TestLevel : public State<GameScreen>
{
public:
    static TestLevel* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    void setSourceGame(Game* game);
    
private:
    std::unique_ptr<Game> m_game;
    Game* m_sourceGame;
    std::unique_ptr<BackButton> m_backButton;
    bool m_hasShownOpeningSequence;
    bool m_hasOpeningSequenceCompleted;
    
    bool handleTouchInput(GameScreen* gs);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    TestLevel();
    TestLevel(const GamePlay&);
    TestLevel& operator=(const GamePlay&);
};

class LevelEditor : public State<GameScreen>
{
public:
    static LevelEditor* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    const char* save();
    
    void load(const char* json);
    
private:
    std::unique_ptr<Game> m_game;
    std::unique_ptr<LevelEditorActionsPanel> m_levelEditorActionsPanel;
    std::unique_ptr<LevelEditorEntitiesPanel> m_levelEditorEntitiesPanel;
    std::unique_ptr<TrashCan> m_trashCan;
    std::unique_ptr<LevelSelectorPanel> m_levelSelectorPanel;
    std::vector<PhysicalEntity*> m_gameEntities;
    std::vector<PhysicalEntity*> m_addedEntities;
    PhysicalEntity* m_lastAddedEntity;
    PhysicalEntity* m_draggingEntity;
    PhysicalEntity* m_attachToEntity;
    float m_fDraggingEntityOriginalY;
    bool m_isVerticalChangeAllowed;
    bool m_useYCorrection;
    bool m_allowAttachment;
    bool m_allowPlaceOn;
    float m_fYOffset;
    
    void handleTouchInput(GameScreen* gs);
    
    void resetEntities(bool clearLastAddedEntity);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    LevelEditor();
    LevelEditor(const LevelEditor&);
    LevelEditor& operator=(const LevelEditor&);
};

#endif /* defined(__nosfuratu__GameScreenStates__) */
