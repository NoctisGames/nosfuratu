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

#include <cstdlib>
#include <ctime>

CutscenePanel::CutscenePanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_fTimeBetweenStrikes(1.80f), m_isLightningStriking(false)
{
    srand (static_cast <unsigned> (time(0)));
}

void CutscenePanel::update(float deltaTime)
{
    PhysicalEntity::Entity::update(deltaTime);
    
    if (m_isLightningStriking)
    {
        if (m_fStateTime > 0.60f)
        {
            m_fStateTime -= 0.60f;
            m_isLightningStriking = false;
            m_fTimeBetweenStrikes = (rand() % 21) * 0.2f + 0.6f;
        }
    }
    else
    {
        if (m_fStateTime > m_fTimeBetweenStrikes)
        {
            m_fStateTime -= m_fTimeBetweenStrikes;
            m_isLightningStriking = true;
        }
    }
}

bool CutscenePanel::isLightningStriking()
{
    return m_isLightningStriking;
}