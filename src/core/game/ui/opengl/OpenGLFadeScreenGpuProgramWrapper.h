//
//  OpenGLFadeScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLFadeScreenGpuProgramWrapper__
#define __nosfuratu__OpenGLFadeScreenGpuProgramWrapper__

#include "TransitionGpuProgramWrapper.h"

#include "OpenGLTransitionProgram.h"

class OpenGLFadeScreenGpuProgramWrapper : public TransitionGpuProgramWrapper
{
public:
    OpenGLFadeScreenGpuProgramWrapper();
    
    virtual ~OpenGLFadeScreenGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLTransitionProgramStruct m_program;
};

#endif /* defined(__nosfuratu__OpenGLFadeScreenGpuProgramWrapper__) */
