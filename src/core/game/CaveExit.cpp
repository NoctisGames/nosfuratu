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
    CaveExit* pCaveExit;
    
    switch ((CaveExitType)type)
    {
        case CaveExitType_End:
            pCaveExit = new CaveExitEnd(x);
            break;
        case CaveExitType_Mid:
        default:
            pCaveExit = new CaveExitMid(x);
            break;
    }
    
    return pCaveExit;
}

CaveExit::CaveExit(float x, float y, float width, float height, CaveExitType type) : PhysicalEntity(x, y, width, height), m_type(type)
{
    m_caveExitCovers.push_back(std::unique_ptr<CaveExitCover>(new CaveExitCover(x, y, width, height)));
    
    updateBounds();
}

void CaveExit::update(float deltaTime)
{
    EntityUtils::updateAndClean(m_caveExitCovers, deltaTime);
    
    for (std::vector<std::unique_ptr<CaveExitCover>>::iterator i = m_caveExitCovers.begin(); i != m_caveExitCovers.end(); i++)
    {
        (*i)->getPosition().set(getPosition());
        (*i)->updateBounds();
    }
}

void CaveExit::updateBounds()
{
    m_bounds->setHeight(getHeight());
    
    PhysicalEntity::updateBounds();
    
    m_bounds->setHeight(getHeight() * 0.39583333333333f);
}

void CaveExit::triggerEruption()
{
    if (m_caveExitCovers.size() > 0)
    {
        m_caveExitCovers.at(0)->triggerHit();
    }
}

bool CaveExit::hasCover()
{
    return m_caveExitCovers.size() > 0;
}

std::vector<std::unique_ptr<CaveExitCover>>& CaveExit::getCaveExitCovers()
{
    return m_caveExitCovers;
}

CaveExitType CaveExit::getEnumType()
{
    return m_type;
}

int CaveExit::getType()
{
    return m_type;
}