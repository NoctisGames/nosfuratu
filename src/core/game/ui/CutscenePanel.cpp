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
#include "EntityUtils.h"

#include <assert.h>

CutsceneEffect* CutsceneEffect::create(CutsceneEffectType type)
{
    switch (type)
    {
        case CutsceneEffectType_Shadow_One:
            return new CutsceneEffectShadowOne();
        case CutsceneEffectType_Shadow_Two:
            return new CutsceneEffectShadowTwo();
        case CutsceneEffectType_POW:
            return new CutsceneEffectPOW();
        default:
            break;
    }
    
    assert(false);
}

CutsceneEffect::CutsceneEffect(float x, float y, float width, float height, CutsceneEffectType type) : PhysicalEntity(x, y, width, height), m_type(type), m_color(1, 1, 1, 1)
{
    // Empty
}

CutsceneEffectType CutsceneEffect::getType()
{
    return m_type;
}

CutscenePanel::CutscenePanel(CutscenePanelType type, float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_type(type), m_color(1, 1, 1, 1), m_isReadyForNextPanel(false)
{
    m_camBounds = std::unique_ptr<Rectangle>(new Rectangle(0, 0, CAM_WIDTH, CAM_HEIGHT));
}

void CutscenePanel::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    EntityUtils::updateAndClean(getCutsceneEffects(), deltaTime);
}

void CutscenePanel::onCleanUp()
{
    if (m_cutsceneEffects.size() > 0)
    {
        EntityUtils::cleanUpVectorOfPointers(m_cutsceneEffects);
    }
}
