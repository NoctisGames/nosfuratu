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

#endif /* defined(__nosfuratu__LevelEditor__) */
