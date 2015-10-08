//
//  PhysicalEntity.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__PhysicalEntity__
#define __gowengamedev__PhysicalEntity__

#include "Entity.h"
#include "Vector2D.h"
#include "Rectangle.h"

#include <memory>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define xKey "x"
#define ykey "y"
#define widthKey "width"
#define heightKey "height"

class PhysicalEntity : public Entity
{
public:
    PhysicalEntity(float x, float y, float width, float height);
    
    virtual void update(float deltaTime);

    virtual void resetBounds(float width, float height);

    virtual void updateBounds();
    
    void boxIn(float size);

    Vector2D& getPosition();
    
    Vector2D& getVelocity();
    
    Vector2D& getAcceleration();

    Rectangle& getBounds();

    const float& getWidth();

    const float& getHeight();
    
    float getAngle();
    
    static PhysicalEntity deserialize(rapidjson::Value& v);
    
    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& w);
    
    virtual void serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w);
    
protected:
    std::unique_ptr<Vector2D> m_position;
    std::unique_ptr<Vector2D> m_velocity;
    std::unique_ptr<Vector2D> m_acceleration;
    std::unique_ptr<Rectangle> m_bounds;
    float m_fWidth;
    float m_fHeight;
    float m_fAngle;
};

#endif /* defined(__gowengamedev__PhysicalEntity__) */
