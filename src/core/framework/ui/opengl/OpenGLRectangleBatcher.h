//
//  OpenGLRectangleBatcher.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 7/15/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLRectangleBatcher__
#define __noctisgames__OpenGLRectangleBatcher__

#include "RectangleBatcher.h"
#include "OpenGLManager.h"

class OpenGLRectangleBatcher : public RectangleBatcher
{
public:
    OpenGLRectangleBatcher(bool isFill = false);
    
    virtual void beginBatch();
    
    virtual void endBatch();
    
    virtual void renderRectangle(float x1, float y1, float x2, float y2, Color &color);
};

#endif /* defined(__noctisgames__OpenGLRectangleBatcher__) */
