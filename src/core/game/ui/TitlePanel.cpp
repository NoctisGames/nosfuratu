//
//  TitlePanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/8/16.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "TitlePanel.h"

#include "NGRect.h"
#include "Vector2D.h"
#include "Assets.h"
#include "NGAudioEngine.h"

#include <cstdlib>
#include <ctime>

TitlePanel::TitlePanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height),
m_bigCloud(new BigCloud()),
m_smallCloud(new SmallCloud()),
m_lightning(new Lightning()),
m_nosfuratuLogoLightEffect(new NosfuratuLogoLightEffect()),
m_nosfuratuLogo(new NosfuratuLogo()),
m_castleLightEffect(new CastleLightEffect()),
m_castle(new Castle()),
m_fTimeBetweenStrikes(1.80f),
m_isLightningStriking(false)
{
    srand (static_cast <unsigned> (time(0)));
}

TitlePanel::~TitlePanel()
{
    delete m_bigCloud;
    delete m_smallCloud;
    delete m_lightning;
    delete m_nosfuratuLogoLightEffect;
    delete m_nosfuratuLogo;
    delete m_castleLightEffect;
    delete m_castle;
}

void TitlePanel::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    m_bigCloud->update(deltaTime);
    m_smallCloud->update(deltaTime);
    m_castle->update(deltaTime);
    
    if (m_isLightningStriking)
    {
        if (m_fStateTime > 0.60f)
        {
            m_fStateTime = 0;
            m_isLightningStriking = false;
            m_fTimeBetweenStrikes = (rand() % 21) * 0.2f + 0.6f;
        }
    }
    else
    {
        if (m_fStateTime > m_fTimeBetweenStrikes)
        {
            m_fStateTime = 0;
            m_isLightningStriking = true;
            
            if (!NG_AUDIO_ENGINE->isMusicDisabled())
            {
                int rn = rand() % 2;
                NG_AUDIO_ENGINE->playSound(rn == 1 ? SOUND_ID_TITLE_LIGHTNING_1 : SOUND_ID_TITLE_LIGHTNING_2);
            }
        }
    }
    
    m_lightning->setStateTime(m_fStateTime);
    m_nosfuratuLogoLightEffect->setStateTime(m_fStateTime);
    m_castleLightEffect->setStateTime(m_fStateTime);
}

bool TitlePanel::isLightningStriking()
{
    return m_isLightningStriking;
}

BigCloud* TitlePanel::getBigCloud()
{
    return m_bigCloud;
}

SmallCloud* TitlePanel::getSmallCloud()
{
    return m_smallCloud;
}

Lightning* TitlePanel::getLightning()
{
    return m_lightning;
}

NosfuratuLogoLightEffect* TitlePanel::getNosfuratuLogoLightEffect()
{
    return m_nosfuratuLogoLightEffect;
}

NosfuratuLogo* TitlePanel::getNosfuratuLogo()
{
    return m_nosfuratuLogo;
}

CastleLightEffect* TitlePanel::getCastleLightEffect()
{
    return m_castleLightEffect;
}

Castle* TitlePanel::getCastle()
{
    return m_castle;
}

BigCloud::BigCloud(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    m_velocity.setX(-1.5f);
}

void BigCloud::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (getMainBounds().getRight() < -CAM_WIDTH * 0.5f)
    {
        m_velocity.setX(1.6f);
    }
    else if (getMainBounds().getLeft() > CAM_WIDTH)
    {
        m_velocity.setX(-1.6f);
    }
}

SmallCloud::SmallCloud(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    m_velocity.setX(2.5f);
}

void SmallCloud::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (getMainBounds().getRight() < -CAM_WIDTH)
    {
        m_velocity.setX(2.4f);
    }
    else if (getMainBounds().getLeft() > CAM_WIDTH * 2)
    {
        m_velocity.setX(-2.4f);
    }
}

Lightning::Lightning(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

NosfuratuLogoLightEffect::NosfuratuLogoLightEffect(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

NosfuratuLogo::NosfuratuLogo(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

CastleLightEffect::CastleLightEffect(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

Castle::Castle(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

RTTI_IMPL(TitlePanel, PhysicalEntity);
RTTI_IMPL(BigCloud, PhysicalEntity);
RTTI_IMPL(SmallCloud, PhysicalEntity);
RTTI_IMPL(Lightning, PhysicalEntity);
RTTI_IMPL(NosfuratuLogoLightEffect, PhysicalEntity);
RTTI_IMPL(NosfuratuLogo, PhysicalEntity);
RTTI_IMPL(CastleLightEffect, PhysicalEntity);
RTTI_IMPL(Castle, PhysicalEntity);
