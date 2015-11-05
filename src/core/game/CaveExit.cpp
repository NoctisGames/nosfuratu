//
//  CaveExit.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 11/3/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "CaveExit.h"
#include "EntityUtils.h"

CaveExit* CaveExit::create(float x, float y, int type)
{
    return new CaveExit(x);
}

CaveExit::CaveExit(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    m_CaveExitCovers.push_back(std::unique_ptr<CaveExitCover>(new CaveExitCover(x, y, width, height)));
    
    resetBounds(width * 0.56f, height);
}

void CaveExit::update(float deltaTime)
{
    EntityUtils::updateAndClean(m_CaveExitCovers, deltaTime);
    
    for (std::vector<std::unique_ptr<CaveExitCover>>::iterator i = m_CaveExitCovers.begin(); i != m_CaveExitCovers.end(); i++)
    {
        (*i)->getPosition().set(getPosition());
        (*i)->updateBounds();
    }
}

void CaveExit::updateBounds()
{
    m_bounds->setHeight(getHeight());
    
    PhysicalEntity::updateBounds();
    
    m_bounds->setHeight(getHeight() * 0.42f);
}

void CaveExit::triggerEruption()
{
    if (m_CaveExitCovers.size() > 0)
    {
        m_CaveExitCovers.at(0)->triggerHit();
    }
}

bool CaveExit::hasCover()
{
    return m_CaveExitCovers.size() > 0;
}

std::vector<std::unique_ptr<CaveExitCover>>& CaveExit::getCaveExitCovers()
{
    return m_CaveExitCovers;
}

int CaveExit::getType()
{
    return -1;
}