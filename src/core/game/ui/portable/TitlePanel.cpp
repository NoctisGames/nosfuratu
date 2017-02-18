//
//  TitlePanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/8/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#include "TitlePanel.h"

#include "NGRect.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "Assets.h"
#include "SoundManager.h"

#include <cstdlib>
#include <ctime>

TitlePanel::TitlePanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_fTimeBetweenStrikes(1.80f), m_isLightningStriking(false)
{
    m_bigCloud = std::unique_ptr<BigCloud>(new BigCloud());
    m_smallCloud = std::unique_ptr<SmallCloud>(new SmallCloud());
    m_lightning = std::unique_ptr<Lightning>(new Lightning());
    m_nosfuratuLogoLightEffect = std::unique_ptr<NosfuratuLogoLightEffect>(new NosfuratuLogoLightEffect());
    m_nosfuratuLogo = std::unique_ptr<NosfuratuLogo>(new NosfuratuLogo());
    m_castleLightEffect = std::unique_ptr<CastleLightEffect>(new CastleLightEffect());
    m_castle = std::unique_ptr<Castle>(new Castle());
    
    srand (static_cast <unsigned> (time(0)));
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
            
            if (MAIN_ASSETS->isMusicEnabled())
            {
                int rn = rand() % 2;
                SOUND_MANAGER->addSoundIdToPlayQueue(rn == 1 ? SOUND_TITLE_LIGHTNING_1 : SOUND_TITLE_LIGHTNING_2);
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

RTTI_IMPL(BigCloud, PhysicalEntity);
RTTI_IMPL(SmallCloud, PhysicalEntity);
RTTI_IMPL(Lightning, PhysicalEntity);
RTTI_IMPL(NosfuratuLogoLightEffect, PhysicalEntity);
RTTI_IMPL(NosfuratuLogo, PhysicalEntity);
RTTI_IMPL(CastleLightEffect, PhysicalEntity);
RTTI_IMPL(Castle, PhysicalEntity);
RTTI_IMPL(TitlePanel, PhysicalEntity);
