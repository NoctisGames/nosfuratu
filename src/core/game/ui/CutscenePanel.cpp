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

CutsceneEffect::CutsceneEffect(float x, float y, float width, float height, CutsceneEffectType type) : PhysicalEntity(x, y, width, height), m_type(type)
{
    // Empty
}

void CutsceneEffect::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    // TODO, zoom in, zoom out, pan, etc.
}

CutsceneEffectType CutsceneEffect::getType()
{
    return m_type;
}

CutscenePanel* CutscenePanel::create(CutscenePanelType type)
{
    switch (type)
    {
        case CutscenePanelType_Opening_One:
            return new CutscenePanelOpeningOne();
        case CutscenePanelType_Opening_Two:
            return new CutscenePanelOpeningTwo();
        case CutscenePanelType_Opening_Three:
            return new CutscenePanel(type);
        case CutscenePanelType_Opening_Four:
            return new CutscenePanel(type);
        case CutscenePanelType_Opening_Five:
            return new CutscenePanel(type);
        case CutscenePanelType_Opening_Six:
            return new CutscenePanelOpeningSix();
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
    PhysicalEntity::update(deltaTime);
    
    EntityUtils::updateAndClean(getCutsceneEffects(), deltaTime);
    
    // TODO, zoom in, zoom out, pan, etc.
    
    if (m_fStateTime > 3.165f)
    {
        m_isRequestingDeletion = true;
    }
}

std::vector<CutsceneEffect *>& CutscenePanel::getCutsceneEffects()
{
    return m_cutsceneEffects;
}

CutscenePanelType CutscenePanel::getType()
{
    return m_type;
}
