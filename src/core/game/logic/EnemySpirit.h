//
//  EnemySpirit.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__EnemySpirit__
#define __nosfuratu__EnemySpirit__

#include "PhysicalEntity.h"
#include "RTTI.h"

typedef enum
{
    EnemySpiritType_None,
    EnemySpiritType_Snake,
    EnemySpiritType_Sparrow
} EnemySpiritType;

class EnemySpirit : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    static EnemySpirit* create(float x, float y, EnemySpiritType type);
    
    EnemySpirit(float x, float y, float width, float height, EnemySpiritType type);
    
    virtual void update(float deltaTime);
    
    EnemySpiritType getType();
    
private:
    EnemySpiritType m_type;
};

#endif /* defined(__nosfuratu__EnemySpirit__) */
