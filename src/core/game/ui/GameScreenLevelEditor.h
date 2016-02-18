//
//  GameScreenLevelEditor.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameScreenLevelEditor__
#define __nosfuratu__GameScreenLevelEditor__

#include "State.h"
#include "LevelEditorActionsPanel.h"
#include "LevelEditorEntitiesPanel.h"
#include "TrashCan.h"
#include "LevelSelectorPanel.h"

#include <memory>

class GameScreen;

class GameScreenLevelEditor : public State<GameScreen>
{
public:
    static GameScreenLevelEditor* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    const char* save();
    
    void load(const char* json);
    
    Game& getGame();
    
    LevelEditorActionsPanel& getLevelEditorActionsPanel();
    
    LevelEditorEntitiesPanel& getLevelEditorEntitiesPanel();
    
    TrashCan& getTrashCan();
    
    LevelSelectorPanel& getLevelSelectorPanel();
    
private:
    std::unique_ptr<Game> m_game;
    std::unique_ptr<LevelEditorActionsPanel> m_levelEditorActionsPanel;
    std::unique_ptr<LevelEditorEntitiesPanel> m_levelEditorEntitiesPanel;
    std::unique_ptr<TrashCan> m_trashCan;
    std::unique_ptr<LevelSelectorPanel> m_levelSelectorPanel;
    std::vector<GridLockedPhysicalEntity*> m_gameEntities;
    PhysicalEntity* m_lastAddedEntity;
    GridLockedPhysicalEntity* m_draggingEntity;
    GridLockedPhysicalEntity* m_attachToEntity;
    float m_fDraggingEntityOriginalY;
    bool m_isVerticalChangeAllowed;
    bool m_allowPlaceOn;
    bool m_allowPlaceUnder;
    
    void handleTouchInput(GameScreen* gs);
    
    void resetEntities(bool clearLastAddedEntity);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    GameScreenLevelEditor();
    GameScreenLevelEditor(const GameScreenLevelEditor&);
    GameScreenLevelEditor& operator=(const GameScreenLevelEditor&);
};

#endif /* defined(__nosfuratu__GameScreenLevelEditor__) */
