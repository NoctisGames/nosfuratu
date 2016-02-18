//
//  Entity.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 8/3/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Entity__
#define __noctisgames__Entity__

class Entity
{
public:
    Entity();
    
    virtual void update(float deltaTime);
    
    virtual void onDeletion();
    
    int getID();
    
    float getStateTime();
    
    void requestDeletion();
    
    bool isRequestingDeletion();
    
protected:
    float m_fStateTime;
    bool m_isRequestingDeletion;
    
private:
    static int getUniqueEntityID();
    
    int m_ID;
};

#endif /* defined(__noctisgames__Entity__) */
