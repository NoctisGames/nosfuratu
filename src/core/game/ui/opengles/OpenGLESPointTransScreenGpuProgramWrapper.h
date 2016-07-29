//
//  OpenGLESPointTransScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLESPointTransScreenGpuProgramWrapper__
#define __noctisgames__OpenGLESPointTransScreenGpuProgramWrapper__

#include "PointTransitionGpuProgramWrapper.h"
#include "PointTransitionProgram.h"

class OpenGLESPointTransScreenGpuProgramWrapper : public PointTransitionGpuProgramWrapper
{
public:
    OpenGLESPointTransScreenGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    PointTransitionProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLESPointTransScreenGpuProgramWrapper__) */
