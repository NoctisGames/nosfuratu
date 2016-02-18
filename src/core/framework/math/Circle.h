//
//  Circle.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 8/6/13.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Circle__
#define __noctisgames__Circle__

#include <memory>

class Vector2D;

class Circle
{
public:
	float m_fRadius;

    Circle(float x, float y, float radius);
    
	Vector2D& getCenter() const;

private:
	std::unique_ptr<Vector2D> m_center;
};

#endif /* defined(__noctisgames__Circle__) */