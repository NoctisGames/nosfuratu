//
//  State.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 8/28/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__State__
#define __noctisgames__State__

#include "RTTI.h"

template <class entity_type>
class State
{
    RTTI_DECL;
    
public:
    virtual ~State()
    {
        // Empty
    }
    
    virtual void enter(entity_type*) = 0;
    
    virtual void execute(entity_type*) = 0;
    
    virtual void exit(entity_type*) = 0;
};

#endif /* defined(__noctisgames__State__) */
