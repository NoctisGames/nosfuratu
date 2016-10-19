//
//  GameScreenOpeningCutscene.h
//  nosfuratu
//
//  Created by Stephen Gowen on 8/2/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameScreenOpeningCutscene__
#define __nosfuratu__GameScreenOpeningCutscene__

#include "GameScreen.h"

#include "CutscenePanel.h"

#include <vector>
#include <memory>

class OpeningCutscene : public GameScreenState
{
public:
    static OpeningCutscene* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    virtual void initRenderer(GameScreen* gs);
    
    std::vector<CutscenePanel*>& getCutscenePanels();
    
private:
    std::vector<CutscenePanel*> m_cutscenePanels;
    int m_currentPanelIndex;
    bool m_isRequestingNextState;
    bool m_needsReset;
    
    CutscenePanelType cutscenePanelTypeForIndex(int index);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    OpeningCutscene();
    OpeningCutscene(const OpeningCutscene&);
    OpeningCutscene& operator=(const OpeningCutscene&);
};

#endif /* defined(__nosfuratu__GameScreenOpeningCutscene__) */
