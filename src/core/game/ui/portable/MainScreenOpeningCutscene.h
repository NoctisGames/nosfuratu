//
//  MainScreenOpeningCutscene.h
//  nosfuratu
//
//  Created by Stephen Gowen on 8/2/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainScreenOpeningCutscene__
#define __nosfuratu__MainScreenOpeningCutscene__

#include "MainScreen.h"

#include "CutscenePanel.h"

#include "RTTI.h"

#include <vector>

class OpeningCutscene : public MainScreenState
{
    RTTI_DECL;
    
public:
    static OpeningCutscene* getInstance();
    
    virtual void enter(MainScreen* ms);
    
    virtual void execute(MainScreen* ms);
    
    virtual void exit(MainScreen* ms);
    
    virtual void initRenderer(MainScreen* ms);
    
    std::vector<CutscenePanel*>& getCutscenePanels();
    
private:
    std::vector<CutscenePanel*> m_cutscenePanels;
    int m_currentPanelIndex;
    bool m_isRequestingNextState;
    bool m_needsReset;
    
    CutscenePanelType cutscenePanelTypeForIndex(int index);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    OpeningCutscene();
    ~OpeningCutscene();
    OpeningCutscene(const OpeningCutscene&);
    OpeningCutscene& operator=(const OpeningCutscene&);
};

#endif /* defined(__nosfuratu__MainScreenOpeningCutscene__) */
