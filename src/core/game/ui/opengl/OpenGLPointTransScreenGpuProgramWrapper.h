//
//  OpenGLPointTransScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLPointTransScreenGpuProgramWrapper__
#define __nosfuratu__OpenGLPointTransScreenGpuProgramWrapper__

#include "PointTransitionGpuProgramWrapper.h"

class OpenGLPointTransitionProgram;

class OpenGLPointTransScreenGpuProgramWrapper : public PointTransitionGpuProgramWrapper
{
public:
    OpenGLPointTransScreenGpuProgramWrapper();
    
    virtual ~OpenGLPointTransScreenGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLPointTransitionProgram* m_program;
};

#endif /* defined(__nosfuratu__OpenGLPointTransScreenGpuProgramWrapper__) */
