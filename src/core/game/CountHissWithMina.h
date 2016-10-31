//
//  CountHissWithMina.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/26/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__CountHissWithMina__
#define __nosfuratu__CountHissWithMina__

#include "GridLockedPhysicalEntity.h"

class CountHissWithMina : public GridLockedPhysicalEntity
{
public:
    static CountHissWithMina* create(int gridX, int gridY, int type);
    
    CountHissWithMina(int gridX, int gridY);
    
    virtual void update(float deltaTime);
    
    bool isMoving();
    
    void beginMovement();
    
    bool isFacingLeft();
    
    void faceLeft();
    
    int getType();
    
private:
    int m_type;
    bool m_isMoving;
    bool m_isFacingLeft;
};

#endif /* defined(__nosfuratu__CountHissWithMina__) */
