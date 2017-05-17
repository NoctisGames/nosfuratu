//
//  MainScreenLevelEditor.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainScreenLevelEditor__
#define __nosfuratu__MainScreenLevelEditor__

#include "MainScreenState.h"

#include "RTTI.h"

#include <vector>

class MainScreen;
class Game;
class LevelEditorActionsPanel;
class LevelEditorEntitiesPanel;
class TrashCan;
class LevelSelectorPanel;
class OffsetPanel;
class ConfirmResetPanel;
class ConfirmExitPanel;
class GridLockedPhysicalEntity;
class PhysicalEntity;

class MainScreenLevelEditor : public MainScreenState
{
    RTTI_DECL;
    
public:
    static MainScreenLevelEditor* getInstance();
    
    virtual void enter(MainScreen* ms);
    
    virtual void execute(MainScreen* ms);
    
    virtual void exit(MainScreen* ms);
    
    virtual void initRenderer(MainScreen* ms);
    
    const char* save();
    
    Game& getGame();
    
    LevelEditorActionsPanel* getLevelEditorActionsPanel();
    
    LevelEditorEntitiesPanel* getLevelEditorEntitiesPanel();
    
    TrashCan* getTrashCan();
    
    LevelSelectorPanel* getLevelSelectorPanel();

	OffsetPanel* getOffsetPanel();

	ConfirmResetPanel* getConfirmResetPanel();

	ConfirmExitPanel* getConfirmExitPanel();

	const char* getMessage();
    
private:
    Game* m_game;
    LevelEditorActionsPanel* m_levelEditorActionsPanel;
    LevelEditorEntitiesPanel* m_levelEditorEntitiesPanel;
    TrashCan* m_trashCan;
    LevelSelectorPanel* m_levelSelectorPanel;
    OffsetPanel* m_offsetPanel;
    ConfirmResetPanel* m_confirmResetPanel;
    ConfirmExitPanel* m_confirmExitPanel;
    std::vector<GridLockedPhysicalEntity*> m_gameEntities;
    PhysicalEntity* m_lastAddedEntity;
    GridLockedPhysicalEntity* m_draggingEntity;
    GridLockedPhysicalEntity* m_attachToEntity;
	const char* m_message;
	float m_fMessageTime;
    float m_fDraggingEntityOriginalY;
    int m_iWorld;
    int m_iLevel;
	int m_iLastOffset;
    bool m_isVerticalChangeAllowed;
    bool m_allowPlaceOn;
    bool m_allowPlaceUnder;
    
    void handleInput(MainScreen* ms);
    
    void resetEntities(bool clearLastAddedEntity);
    
    void loadIfNecessary(MainScreen* ms);
    
    bool isLevelValid(MainScreen* ms, bool isSaving = false);
    
    void setMessage(const char *message);
    
    void saveLevel(int world, int level);
    void loadLevel(int world, int level, MainScreen* ms);
    
    void load(const char* json, MainScreen* ms);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    MainScreenLevelEditor();
    ~MainScreenLevelEditor();
    MainScreenLevelEditor(const MainScreenLevelEditor&);
    MainScreenLevelEditor& operator=(const MainScreenLevelEditor&);
};

#endif /* defined(__nosfuratu__MainScreenLevelEditor__) */
