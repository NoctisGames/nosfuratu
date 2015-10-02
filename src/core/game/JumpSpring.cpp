//
//  JumpSpring.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "JumpSpring.h"
#include "EntityUtils.h"

#define jumpSpringTypeKey "jumpSpringType"

void JumpSpring::create(std::vector<JumpSpring>& items, float x, JumpSpringType type)
{
    EntityAnchor anchor;
    switch (type)
    {
        case JumpSpringType::JUMP_SPRING_IN_GRASS:
            items.push_back(JumpSpring(x, 0, 1.0058479532163742f, 1.0060658578856152f, type));
            anchor = EntityAnchor::ANCHOR_GROUND;
            break;
        case JumpSpringType::JUMP_SPRING_IN_CAVE:
        default:
            items.push_back(JumpSpring(x, 0, 1.2163742690058479f, 1.5441941074523398f, type));
            anchor = EntityAnchor::ANCHOR_CAVE;
            break;
    }
    
    EntityUtils::applyAnchor(items.at(items.size() - 1), anchor);
    
    items.at(items.size() - 1).updateBounds();
}

JumpSpring::JumpSpring(float x, float y, float width, float height, JumpSpringType type) : PhysicalEntity(x, y, width, height), m_jumpSpringType(type), m_isTriggered(false)
{
    resetBounds(width, height * 0.70f);
}

void JumpSpring::update(float deltaTime)
{
    if (m_isTriggered)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.20f)
        {
            m_isTriggered = false;
        }
    }
}

void JumpSpring::trigger()
{
    m_isTriggered = true;
}

JumpSpringType JumpSpring::getJumpSpringType()
{
    return m_jumpSpringType;
}

bool JumpSpring::isTriggered()
{
    return m_isTriggered;
}

JumpSpring JumpSpring::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    JumpSpringType type = (JumpSpringType) v[jumpSpringTypeKey].GetInt();
    
    return JumpSpring(x, y, width, height, type);
}

void JumpSpring::serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w)
{
    w.String(jumpSpringTypeKey);
    w.Int(getJumpSpringType());
}