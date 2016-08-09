//
//  CutscenePanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 8/2/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#include "CutscenePanel.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"

#include <assert.h>

CutscenePanel* CutscenePanel::create(CutscenePanelType type)
{
    switch (type)
    {
        case CutscenePanelType_Opening_One:
            return new CutscenePanel(type, CAM_WIDTH / 2, CAM_HEIGHT * 2 / 3, CAM_WIDTH, CAM_HEIGHT * 4 / 3);
        case CutscenePanelType_Opening_Two:
            return new CutscenePanel(type);
        case CutscenePanelType_Opening_Three:
            return new CutscenePanel(type);
        case CutscenePanelType_Opening_Four:
            return new CutscenePanel(type);
        case CutscenePanelType_Opening_Five:
            return new CutscenePanel(type);
        case CutscenePanelType_Opening_Six:
            return new CutscenePanel(type, CAM_WIDTH / 2, CAM_HEIGHT / 2, CAM_WIDTH, CAM_HEIGHT * 4 / 3);
        case CutscenePanelType_Opening_Seven:
            return new CutscenePanel(type);
        default:
            break;
    }
    
    assert(false);
}

CutscenePanel::CutscenePanel(CutscenePanelType type, float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_type(type)
{
    // Empty
}

void CutscenePanel::update(float deltaTime)
{
    PhysicalEntity::Entity::update(deltaTime);
    
    // TODO, zoom in, zoom out, pan, etc.
}

CutscenePanelType CutscenePanel::getType()
{
    return m_type;
}