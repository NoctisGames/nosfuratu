//
//  Rock.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Rock__
#define __nosfuratu__Rock__

#include "DestructiblePhysicalEntity.h"
#include "EntityAnchor.h"
#include "Color.h"

#include <vector>

#define ROCK_WIDTH 4.491228070175438f
#define ROCK_HEIGHT 4.305025996533795f

class Rock : public DestructiblePhysicalEntity
{
public:
    static void create(std::vector<Rock>& items, float x, EntityAnchor anchor = EntityAnchor::ANCHOR_GROUND);
    
    Rock(float x, float y, float width = ROCK_WIDTH, float height = ROCK_HEIGHT, bool isCracked = false);
    
    virtual void update(float deltaTime);
    
    virtual void triggerHit();
    
    virtual void updateBounds();
    
    Color getColor();
    
    bool isCracked();
    
    bool isBlowingUp();
    
    static Rock deserialize(rapidjson::Value& v);
    
    virtual void serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w);
    
private:
    Color m_color;
    bool m_isCracked;
    bool m_isBlowingUp;
};

#endif /* defined(__nosfuratu__Rock__) */
