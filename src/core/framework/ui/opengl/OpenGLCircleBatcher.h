//
//  OpenGLCircleBatcher.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 11/15/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLCircleBatcher__
#define __noctisgames__OpenGLCircleBatcher__

#include "CircleBatcher.h"

class OpenGLCircleBatcher : public CircleBatcher
{
public:
    OpenGLCircleBatcher();
    
    virtual void renderCircle(Circle &circle, Color &c);
    
    virtual void renderPartialCircle(Circle &circle, int arcDegrees, Color &c);
    
    virtual void renderCircle(Circle &circle, Color &c, GpuProgramWrapper &gpuProgramWrapper);
    
    virtual void renderPartialCircle(Circle &circle, int arcDegrees, Color &c, GpuProgramWrapper &gpuProgramWrapper);
    
protected:
    virtual void endBatch(GpuProgramWrapper &gpuProgramWrapper);
};

#endif /* defined(__noctisgames__OpenGLCircleBatcher__) */
