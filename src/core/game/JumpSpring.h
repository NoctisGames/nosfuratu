//
//  JumpSpring.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__JumpSpring__
#define __nosfuratu__JumpSpring__

#include "PhysicalEntity.h"
#include "JumpSpringType.h"
#include "EntityAnchor.h"

#include <vector>

class JumpSpring : public PhysicalEntity
{
public:
    static void create(std::vector<JumpSpring>& items, float x, JumpSpringType type);
    
    JumpSpring(float x, float y, float width, float height, JumpSpringType type);
    
    virtual void update(float deltaTime);
    
    void trigger();
    
    JumpSpringType getJumpSpringType();
    
    bool isTriggered();
    
    static JumpSpring deserialize(rapidjson::Value& v);
    
    virtual void serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w);
    
private:
    JumpSpringType m_jumpSpringType;
    bool m_isTriggered;
};

#endif /* defined(__nosfuratu__JumpSpring__) */
