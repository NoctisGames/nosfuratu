//
//  PhysicalEntity.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "PhysicalEntity.h"
#include "macros.h"

PhysicalEntity::PhysicalEntity(float x, float y, float width, float height) : Entity()
{
    m_position = std::unique_ptr<Vector2D>(new Vector2D(x, y));
    m_velocity = std::unique_ptr<Vector2D>(new Vector2D());
    m_acceleration = std::unique_ptr<Vector2D>(new Vector2D());
    m_bounds = std::unique_ptr<Rectangle>(new Rectangle(x - width / 2, y - height / 2, width, height));

    m_fWidth = width;
    m_fHeight = height;
    m_fAngle = 0;
}

void PhysicalEntity::update(float deltaTime)
{
    Entity::update(deltaTime);
    
    m_velocity->add(m_acceleration->getX() * deltaTime, m_acceleration->getY() * deltaTime);
    m_position->add(m_velocity->getX() * deltaTime, m_velocity->getY() * deltaTime);
    
    updateBounds();
}

void PhysicalEntity::resetBounds(float width, float height)
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - width / 2, m_position->getY() - height / 2);
    m_bounds->setWidth(width);
    m_bounds->setHeight(height);
}

void PhysicalEntity::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - m_bounds->getWidth() / 2, m_position->getY() - m_bounds->getHeight() / 2);
}

void PhysicalEntity::boxIn(float size)
{
    if (m_fWidth > m_fHeight)
    {
        m_fHeight /= m_fWidth;
        m_fHeight *= size;
        m_fWidth = size;
    }
    else
    {
        m_fWidth /= m_fHeight;
        m_fWidth *= size;
        m_fHeight = size;
    }
}

Vector2D& PhysicalEntity::getPosition()
{
    return *m_position;
}

Vector2D& PhysicalEntity::getVelocity()
{
    return *m_velocity;
}

Vector2D& PhysicalEntity::getAcceleration()
{
    return *m_acceleration;
}

Rectangle& PhysicalEntity::getBounds()
{
    return *m_bounds;
}

const float& PhysicalEntity::getWidth()
{
    return m_fWidth;
}

const float& PhysicalEntity::getHeight()
{
    return m_fHeight;
}

float PhysicalEntity::getAngle()
{
    return m_fAngle;
}

PhysicalEntity PhysicalEntity::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    
    return PhysicalEntity(x, y, width, height);
}

void PhysicalEntity::serialize(rapidjson::Writer<rapidjson::StringBuffer>& w)
{
    w.StartObject();
    w.String(xKey);
    w.Double(m_position->getX());
    w.String(ykey);
    w.Double(m_position->getY());
    w.String(widthKey);
    w.Double(getWidth());
    w.String(heightKey);
    w.Double(getHeight());
    
    serializeAdditionalParams(w);
    
    w.EndObject();
}

void PhysicalEntity::serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w)
{
    UNUSED(w);
}