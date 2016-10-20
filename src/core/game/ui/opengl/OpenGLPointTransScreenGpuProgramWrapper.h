//
//  OpenGLPointTransScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLPointTransScreenGpuProgramWrapper__
#define __noctisgames__OpenGLPointTransScreenGpuProgramWrapper__

#include "PointTransitionGpuProgramWrapper.h"
#include "PointTransitionProgram.h"

class OpenGLPointTransScreenGpuProgramWrapper : public PointTransitionGpuProgramWrapper
{
public:
    OpenGLPointTransScreenGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    PointTransitionProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLPointTransScreenGpuProgramWrapper__) */
