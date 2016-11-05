//
//  PhysicalEntity.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__PhysicalEntity__
#define __noctisgames__PhysicalEntity__

#include "Entity.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "RTTI.h"

#include <memory>
#include <vector>

class PhysicalEntity : public Entity
{
    RTTI_DECL;
    
public:
    PhysicalEntity(float x, float y, float width, float height);
    
    virtual void update(float deltaTime);

    virtual void resetBounds(float width, float height);

    virtual void updateBounds();

	virtual void placeOn(float itemTopY);

    Vector2D& getPosition();
    
    Vector2D& getVelocity();
    
    Vector2D& getAcceleration();

    std::vector<std::unique_ptr<Rectangle>>& getBounds();
    
    Rectangle& getMainBounds();
    
    const float& getWidth();
    
    void setWidth(float width);
    
    const float& getHeight();
    
    void setHeight(float height);
    
    float getAngle();
    
    void log();
    
protected:
    std::unique_ptr<Vector2D> m_position;
    std::unique_ptr<Vector2D> m_velocity;
    std::unique_ptr<Vector2D> m_acceleration;
	std::vector<std::unique_ptr<Rectangle>> m_bounds;
    float m_fWidth;
    float m_fHeight;
    float m_fAngle;
};

#endif /* defined(__noctisgames__PhysicalEntity__) */
