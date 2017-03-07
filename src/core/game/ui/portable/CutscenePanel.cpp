//
//  CutscenePanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 8/2/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#include "CutscenePanel.h"

#include "NGRect.h"

#include "Vector2D.h"
#include "EntityUtils.h"
#include "VectorUtil.h"

#include <assert.h>

CutscenePanel::CutscenePanel(CutscenePanelType type, float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_camBounds(new NGRect(0, 0, CAM_WIDTH, CAM_HEIGHT)), m_type(type), m_color(1, 1, 1, 1), m_isReadyForNextPanel(false)
{
    // Empty
}

CutscenePanel::~CutscenePanel()
{
    delete m_camBounds;
    
    VectorUtil::cleanUpVectorOfPointers(m_cutsceneEffects);
}

void CutscenePanel::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    EntityUtils::updateAndClean(getCutsceneEffects(), deltaTime);
}

CutscenePanel* CutscenePanel::getNextPanel()
{
    return nullptr;
}

std::vector<CutsceneEffect*>& CutscenePanel::getCutsceneEffects()
{
    return m_cutsceneEffects;
}

CutscenePanelType CutscenePanel::getType()
{
    return m_type;
}

Color& CutscenePanel::getColor()
{
    return m_color;
}

NGRect& CutscenePanel::getCamBounds()
{
    return *m_camBounds;
}

bool CutscenePanel::isReadyForNextPanel()
{
    return m_isReadyForNextPanel;
}

CutscenePanelOpeningOne::CutscenePanelOpeningOne() : CutscenePanel(CutscenePanelType_Opening_One, CAM_WIDTH / 2, CAM_HEIGHT * 2 / 3, CAM_WIDTH, CAM_HEIGHT * 4 / 3), shadowX(0), shadowY(0), shadowW(0), shadowH(0), m_hasSpawnedShadow(false)
{
    float baseCamB = CAM_HEIGHT * 0.83333333333333f;
    
    float camL = 0;
    float camB = baseCamB;
    float camW = CAM_WIDTH / 2;
    float camH = CAM_HEIGHT / 2;
    
    m_camBounds->getLowerLeft().set(camL, camB);
    m_camBounds->setWidth(camL + camW);
    m_camBounds->setHeight(camB + camH);
    
    m_color.alpha = 0;
}

void CutscenePanelOpeningOne::update(float deltaTime)
{
    CutscenePanel::update(deltaTime);
    
    m_color.alpha += deltaTime;
    
    if (m_color.alpha > 1)
    {
        m_color.alpha = 1;
    }
    
    float baseCamB = CAM_HEIGHT * 0.83333333333333f;
    
    float camL = 0;
    float camB = baseCamB - m_fStateTime / 6 * baseCamB;
    float camW = CAM_WIDTH / 2 * m_fStateTime / 6 + CAM_WIDTH / 2;
    float camH = CAM_HEIGHT / 2 * m_fStateTime / 6 + CAM_HEIGHT / 2;
    
    if (m_fStateTime > 6.5f)
    {
        if (m_cutsceneEffects.size() > 0)
        {
            shadowX = m_cutsceneEffects.at(0)->getPosition().getX();
            shadowY = m_cutsceneEffects.at(0)->getPosition().getY();
            shadowW = m_cutsceneEffects.at(0)->getWidth();
            shadowH = m_cutsceneEffects.at(0)->getHeight();
        }
        
        m_isReadyForNextPanel = true;
    }
    
    if (m_fStateTime > 6)
    {
        camL = 0;
        camB = 0;
        camW = CAM_WIDTH;
        camH = CAM_HEIGHT;
    }
    else if (m_fStateTime > 4.5f && !m_hasSpawnedShadow)
    {
        m_cutsceneEffects.push_back(CutsceneEffect::create(CutsceneEffectType_Shadow_One));
        
        m_hasSpawnedShadow = true;
    }
    
    m_camBounds->getLowerLeft().set(camL, camB);
    m_camBounds->setWidth(camL + camW);
    m_camBounds->setHeight(camB + camH);
}

CutscenePanel* CutscenePanelOpeningOne::getNextPanel()
{
    CutscenePanelOpeningTwo* ret = new CutscenePanelOpeningTwo(shadowX, shadowY, shadowW, shadowH);
    
    return ret;
}

CutscenePanelOpeningTwo::CutscenePanelOpeningTwo(float shadowX, float shadowY, float shadowWidth, float shadowHeight) : CutscenePanel(CutscenePanelType_Opening_Two)
{
    m_cutsceneEffects.push_back(CutsceneEffect::create(CutsceneEffectType_Shadow_One));
    
    m_cutsceneEffects.at(0)->getPosition().set(shadowX, shadowY);
    m_cutsceneEffects.at(0)->setWidth(shadowWidth);
    m_cutsceneEffects.at(0)->setHeight(shadowHeight);
}

void CutscenePanelOpeningTwo::update(float deltaTime)
{
    CutscenePanel::update(deltaTime);
    
    if (m_fStateTime > 0.8f)
    {
        m_isReadyForNextPanel = true;
    }
}

CutscenePanel* CutscenePanelOpeningTwo::getNextPanel()
{
    return new CutscenePanelOpeningThree();
}

CutscenePanelOpeningThree::CutscenePanelOpeningThree() : CutscenePanel(CutscenePanelType_Opening_Three), m_hasSpawnedPow(false)
{
    m_cutsceneEffects.push_back(CutsceneEffect::create(CutsceneEffectType_Shadow_Two));
}

void CutscenePanelOpeningThree::update(float deltaTime)
{
    CutscenePanel::update(deltaTime);
    
    if (m_fStateTime > 2)
    {
        m_isReadyForNextPanel = true;
    }
    else if (m_fStateTime > 1 && !m_hasSpawnedPow)
    {
        m_cutsceneEffects.push_back(CutsceneEffect::create(CutsceneEffectType_POW));
        
        m_hasSpawnedPow = true;
    }
}

CutscenePanel* CutscenePanelOpeningThree::getNextPanel()
{
    return new CutscenePanelOpeningFour();
}

CutscenePanelOpeningFour::CutscenePanelOpeningFour() : CutscenePanel(CutscenePanelType_Opening_Four)
{
    float camL = 0;
    float camB = 0;
    float camW = CAM_WIDTH;
    float camH = CAM_HEIGHT;
    
    m_camBounds->getLowerLeft().set(camL, camB);
    m_camBounds->setWidth(camL + camW);
    m_camBounds->setHeight(camB + camH);
}

void CutscenePanelOpeningFour::update(float deltaTime)
{
    CutscenePanel::update(deltaTime);
    
    float camL = m_fStateTime / 1.5f * CAM_WIDTH / 16;
    float camB = m_fStateTime / 1.5f * CAM_HEIGHT / 16;
    float camW = CAM_WIDTH - m_fStateTime / 1.5f * CAM_WIDTH * 1 / 8;
    float camH = CAM_HEIGHT - m_fStateTime / 1.5f * CAM_HEIGHT * 1 / 8;
    
    if (m_fStateTime > 1.5f)
    {
        camL = CAM_WIDTH / 16;
        camB = CAM_HEIGHT / 16;
        camW = CAM_WIDTH * 7 / 8;
        camH = CAM_HEIGHT * 7 / 8;
        
        m_color.alpha = 0;
    }
    else if (m_fStateTime > 1.25f)
    {
        m_isReadyForNextPanel = true;
    }
    
    if (m_fStateTime > 0.75f)
    {
        m_color.alpha -= deltaTime * 1.33333333333333f;
        
        if (m_color.alpha < 0)
        {
            m_color.alpha = 0;
        }
    }
    
    m_camBounds->getLowerLeft().set(camL, camB);
    m_camBounds->setWidth(camL + camW);
    m_camBounds->setHeight(camB + camH);
}

CutscenePanel* CutscenePanelOpeningFour::getNextPanel()
{
    return new CutscenePanelOpeningFive();
}

CutscenePanelOpeningFive::CutscenePanelOpeningFive() : CutscenePanel(CutscenePanelType_Opening_Five)
{
    m_color.alpha = 0.5f;
    
    float camL = 0;
    float camB = 0;
    float camW = CAM_WIDTH;
    float camH = CAM_HEIGHT;
    
    m_camBounds->getLowerLeft().set(camL, camB);
    m_camBounds->setWidth(camL + camW);
    m_camBounds->setHeight(camB + camH);
}

void CutscenePanelOpeningFive::update(float deltaTime)
{
    CutscenePanel::update(deltaTime);
    
    float camL = 0;
    float camB = 0;
    float camW = CAM_WIDTH - m_fStateTime / 3 * CAM_WIDTH * 1 / 3;
    float camH = CAM_HEIGHT - m_fStateTime / 3 * CAM_HEIGHT * 1 / 3;
    
    if (m_fStateTime > 3)
    {
        camL = 0;
        camB = 0;
        camW = CAM_WIDTH * 2 / 3;
        camH = CAM_HEIGHT * 2 / 3;
    }
    else if (m_fStateTime > 2.5f)
    {
        m_isReadyForNextPanel = true;
        
        m_color.alpha -= deltaTime / 2;
        
        if (m_color.alpha < 0)
        {
            m_color.alpha = 0;
        }
    }
    else
    {
        m_color.alpha += deltaTime / 2;
        
        if (m_color.alpha > 1)
        {
            m_color.alpha = 1;
        }
    }
    
    m_camBounds->getLowerLeft().set(camL, camB);
    m_camBounds->setWidth(camL + camW);
    m_camBounds->setHeight(camB + camH);
}

CutscenePanel* CutscenePanelOpeningFive::getNextPanel()
{
    return new CutscenePanelOpeningSix();
}

CutscenePanelOpeningSix::CutscenePanelOpeningSix() : CutscenePanel(CutscenePanelType_Opening_Six, CAM_WIDTH / 2, CAM_HEIGHT * 2 / 3, CAM_WIDTH, CAM_HEIGHT * 4 / 3)
{
    m_color.alpha = 0;
    
    float camL = CAM_WIDTH / 4;
    float camB = CAM_HEIGHT / 4;
    float camW = CAM_WIDTH / 2;
    float camH = CAM_HEIGHT / 2;
    
    m_camBounds->getLowerLeft().set(camL, camB);
    m_camBounds->setWidth(camL + camW);
    m_camBounds->setHeight(camB + camH);
}

void CutscenePanelOpeningSix::update(float deltaTime)
{
    CutscenePanel::update(deltaTime);
    
    float camL = CAM_WIDTH / 4 - m_fStateTime / 3.6f * CAM_WIDTH / 4;
    float camB = CAM_HEIGHT / 4 - m_fStateTime / 3.6f * CAM_HEIGHT / 4;
    float camW = CAM_WIDTH / 2 + m_fStateTime / 3.6f * CAM_WIDTH / 2;
    float camH = CAM_HEIGHT / 2 + m_fStateTime / 3.6f * CAM_HEIGHT / 2;
    
    if (m_fStateTime > 4.4f)
    {
        m_isReadyForNextPanel = true;
    }
    
    if (m_fStateTime > 3.6f)
    {
        camL = 0;
        camB = 0;
        camW = CAM_WIDTH;
        camH = CAM_HEIGHT;
        
        m_color.alpha -= deltaTime * 1.25f;
        
        if (m_color.alpha < 0)
        {
            m_color.alpha = 0;
        }
    }
    else
    {
        m_color.alpha += deltaTime / 2;
        
        if (m_color.alpha > 1)
        {
            m_color.alpha = 1;
        }
    }
    
    m_camBounds->getLowerLeft().set(camL, camB);
    m_camBounds->setWidth(camL + camW);
    m_camBounds->setHeight(camB + camH);
}

CutscenePanel* CutscenePanelOpeningSix::getNextPanel()
{
    return new CutscenePanelOpeningSeven();
}

CutscenePanelOpeningSeven::CutscenePanelOpeningSeven() : CutscenePanel(CutscenePanelType_Opening_Seven)
{
    m_color.alpha = 0;
}

void CutscenePanelOpeningSeven::update(float deltaTime)
{
    CutscenePanel::update(deltaTime);
    
    if (m_fStateTime > 4.8f)
    {
        m_isReadyForNextPanel = true;
    }
    else if (m_fStateTime > 2)
    {
        m_color.alpha += deltaTime / 2;
        
        if (m_color.alpha > 1)
        {
            m_color.alpha = 1;
        }
    }
}

CutscenePanel* CutscenePanelOpeningSeven::getNextPanel()
{
    return nullptr;
}

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

Color& CutsceneEffect::getColor()
{
    return m_color;
}

CutsceneEffectShadowOne::CutsceneEffectShadowOne() : CutsceneEffect(CAM_WIDTH, 0, CAM_WIDTH * 0.60375f, CAM_HEIGHT * 0.56f, CutsceneEffectType_Shadow_One)
{
    m_position.add(m_fWidth / 2, 0);
    m_position.sub(0, m_fHeight * 1.68f);
    
    m_velocity.set(-3, 3);
}

void CutsceneEffectShadowOne::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    m_fWidth += deltaTime * 1.6f;
    m_fHeight += deltaTime * 1.6f;
}

CutsceneEffectShadowTwo::CutsceneEffectShadowTwo() : CutsceneEffect(CAM_WIDTH / 2, 0, CAM_WIDTH, CAM_HEIGHT * 0.84444444444444f, CutsceneEffectType_Shadow_Two)
{
    m_velocity.set(-1.2f, CAM_HEIGHT * 0.84444444444444f / 4);
}

void CutsceneEffectShadowTwo::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    m_fWidth += deltaTime * 3;
    m_fHeight += deltaTime * 2;
}

CutsceneEffectPOW::CutsceneEffectPOW() : CutsceneEffect(CAM_WIDTH * 0.44f, CAM_HEIGHT * 0.56f, CAM_WIDTH * 1.22f, CAM_HEIGHT * 1.56888888888889f, CutsceneEffectType_POW)
{
    // Empty
}

void CutsceneEffectPOW::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    m_fWidth += deltaTime * 2.4f;
    m_fHeight += deltaTime * 2.4f;
}

RTTI_IMPL(CutscenePanel, PhysicalEntity);
RTTI_IMPL(CutscenePanelOpeningOne, CutscenePanel);
RTTI_IMPL(CutscenePanelOpeningTwo, CutscenePanel);
RTTI_IMPL(CutscenePanelOpeningThree, CutscenePanel);
RTTI_IMPL(CutscenePanelOpeningFour, CutscenePanel);
RTTI_IMPL(CutscenePanelOpeningFive, CutscenePanel);
RTTI_IMPL(CutscenePanelOpeningSix, CutscenePanel);
RTTI_IMPL(CutscenePanelOpeningSeven, CutscenePanel);
RTTI_IMPL(CutsceneEffect, PhysicalEntity);
RTTI_IMPL(CutsceneEffectShadowOne, CutsceneEffect);
RTTI_IMPL(CutsceneEffectShadowTwo, CutsceneEffect);
RTTI_IMPL(CutsceneEffectPOW, CutsceneEffect);
