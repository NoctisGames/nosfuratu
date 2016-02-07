//
//  LevelEditor.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__LevelEditor__
#define __nosfuratu__LevelEditor__

#include "State.h"
#include "LevelEditorActionsPanel.h"
#include "LevelEditorEntitiesPanel.h"
#include "TrashCan.h"
#include "LevelSelectorPanel.h"

#include <memory>

class GameScreen;

class LevelEditor : public State<GameScreen>
{
public:
    static LevelEditor* getInstance();
    
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
    float m_fDraggingEntityOriginalY;
    bool m_isVerticalChangeAllowed;
    
    void handleTouchInput(GameScreen* gs);
    
    void resetEntities(bool clearLastAddedEntity);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    LevelEditor();
    LevelEditor(const LevelEditor&);
    LevelEditor& operator=(const LevelEditor&);
};

#endif /* defined(__nosfuratu__LevelEditor__) */
