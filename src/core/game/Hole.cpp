//
//  Hole.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "Hole.h"
#include "EntityUtils.h"

Hole* Hole::create(float x, float y, int type)
{
    return new Hole(x);
}

Hole::Hole(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    m_holeCovers.push_back(std::unique_ptr<HoleCover>(new HoleCover(x, y, width, height)));
}

void Hole::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    EntityUtils::updateAndClean(m_holeCovers, deltaTime);
    
    for (std::vector<std::unique_ptr<HoleCover>>::iterator i = m_holeCovers.begin(); i != m_holeCovers.end(); i++)
    {
        (*i)->getPosition().set(getPosition());
        (*i)->updateBounds();
    }
}

void Hole::triggerBurrow()
{
    if (m_holeCovers.size() > 0)
    {
        m_holeCovers.at(0)->triggerHit();
    }
}

bool Hole::hasCover()
{
    return m_holeCovers.size() > 0;
}

std::vector<std::unique_ptr<HoleCover>>& Hole::getHoleCovers()
{
    return m_holeCovers;
}

int Hole::getType()
{
    return -1;
}