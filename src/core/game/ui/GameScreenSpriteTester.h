//
//  GameScreenSpriteTester.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameScreenSpriteTester__
#define __nosfuratu__GameScreenSpriteTester__

#include "GameScreen.h"

#include "SpriteTesterActionsPanel.h"
#include "SpriteTesterEntitiesPanel.h"
#include "TrashCan.h"
#include "TextureSelectorPanel.h"
#include "ConfirmResetPanel.h"
#include "ConfirmExitPanel.h"
#include "RendererType.h"
#include "GameScreenState.h"
#include "UnknownEntity.h"
#include "RTTI.h"

#include <memory>

class GameScreenSpriteTester : public GameScreenState
{
    RTTI_DECL;
    
public:
    static GameScreenSpriteTester* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    virtual void initRenderer(GameScreen* gs);
    
    std::vector<UnknownEntity*>& getEntities();
    
    UnknownEntity* getLastAddedEntity();
    
    UnknownEntity* getDraggingEntity();
    
    SpriteTesterActionsPanel* getSpriteTesterActionsPanel();
    
    SpriteTesterEntitiesPanel* getSpriteTesterEntitiesPanel();
    
    TrashCan* getTrashCan();
    
    TextureSelectorPanel* getTextureSelectorPanel();

	ConfirmResetPanel* getConfirmResetPanel();

	ConfirmExitPanel* getConfirmExitPanel();
    
private:
    std::unique_ptr<SpriteTesterActionsPanel> m_spriteTesterActionsPanel;
    std::unique_ptr<SpriteTesterEntitiesPanel> m_spriteTesterEntitiesPanel;
    std::unique_ptr<TrashCan> m_trashCan;
    std::unique_ptr<TextureSelectorPanel> m_textureSelectorPanel;
    std::unique_ptr<ConfirmResetPanel> m_confirmResetPanel;
    std::unique_ptr<ConfirmExitPanel> m_confirmExitPanel;
    std::vector<UnknownEntity*> m_gameEntities;
    UnknownEntity* m_lastAddedEntity;
    UnknownEntity* m_draggingEntity;
    
    void handleTouchInput(GameScreen* gs);
    
    void resetEntities();
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    GameScreenSpriteTester();
    GameScreenSpriteTester(const GameScreenSpriteTester&);
    GameScreenSpriteTester& operator=(const GameScreenSpriteTester&);
};

#endif /* defined(__nosfuratu__GameScreenSpriteTester__) */
