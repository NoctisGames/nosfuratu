//
//  Rock.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "Rock.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "EntityUtils.h"

#define isCrackedKey "isCracked"

void Rock::create(std::vector<Rock>& items, float x, EntityAnchor anchor)
{
    items.push_back(Rock(x, 0));
    
    EntityUtils::applyAnchor(items.at(items.size() - 1), anchor, ROCK_HEIGHT * -0.21739130434783f);
    
    items.at(items.size() - 1).updateBounds();
}

Rock::Rock(float x, float y, float width, float height, bool isCracked) : DestructiblePhysicalEntity(x, y, width, height), m_color(1, 1, 1, 1), m_isCracked(isCracked), m_isBlowingUp(false)
{
    updateBounds();
}

void Rock::update(float deltaTime)
{
    Entity::update(deltaTime);
    
    if (m_isBlowingUp)
    {
        updateBounds();
        
        if (m_fStateTime > 0.32f)
        {
            m_color.alpha = 1.96f - m_fStateTime * 3;
            
            if (m_color.alpha < 0)
            {
                m_color.alpha = 0;
                m_isDestroyed = true;
            }
        }
    }
}

void Rock::triggerHit()
{
    if (m_isCracked)
    {
        m_isBlowingUp = true;
        m_fStateTime = 0;
    }
    
    m_isCracked = true;
}

void Rock::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.35416666666667f, getHeight() * 0.21739130434783);
    
    m_bounds->setWidth(getWidth() * 0.27604166666667f);
    m_bounds->setHeight(getHeight() * 0.26086956521739f);
    
    if (m_isBlowingUp && m_fStateTime > 0.06f)
    {
        // Bye bye!
        lowerLeft.setY(1337);
    }
}

Color Rock::getColor()
{
    return m_color;
}

bool Rock::isCracked()
{
    return m_isCracked;
}

bool Rock::isBlowingUp()
{
    return m_isBlowingUp;
}

Rock Rock::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    bool isCracked = v[isCrackedKey].GetBool();
    
    return Rock(x, y, width, height, isCracked);
}

void Rock::serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w)
{
    w.String(isCrackedKey);
    w.Bool(isCracked());
}