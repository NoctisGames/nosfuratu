//
//  CutscenePanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 8/2/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__CutscenePanel__
#define __nosfuratu__CutscenePanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"

typedef enum
{
    CutscenePanelType_Opening_One,
    CutscenePanelType_Opening_Two,
    CutscenePanelType_Opening_Three,
    CutscenePanelType_Opening_Four,
    CutscenePanelType_Opening_Five,
    CutscenePanelType_Opening_Six,
    CutscenePanelType_Opening_Seven
} CutscenePanelType;

class CutscenePanel : public PhysicalEntity
{
public:
    static CutscenePanel* create(CutscenePanelType type);
    
    CutscenePanel(CutscenePanelType type, float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH, float height = CAM_HEIGHT);
    
    virtual void update(float deltaTime);
    
    CutscenePanelType getType();
    
private:
    CutscenePanelType m_type;
};

#endif /* defined(__nosfuratu__CutscenePanel__) */
