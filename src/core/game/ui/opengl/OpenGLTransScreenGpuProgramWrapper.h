//
//  OpenGLTransScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLTransScreenGpuProgramWrapper__
#define __nosfuratu__OpenGLTransScreenGpuProgramWrapper__

#include "TransitionGpuProgramWrapper.h"

#include "OpenGLTransitionProgram.h"

class OpenGLTransScreenGpuProgramWrapper : public TransitionGpuProgramWrapper
{
public:
    OpenGLTransScreenGpuProgramWrapper();
    
    virtual ~OpenGLTransScreenGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLTransitionProgramStruct m_program;
};

#endif /* defined(__nosfuratu__OpenGLTransScreenGpuProgramWrapper__) */
