//
//  OpenGLFramebufferRadialBlurGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/29/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLFramebufferRadialBlurGpuProgramWrapper__
#define __nosfuratu__OpenGLFramebufferRadialBlurGpuProgramWrapper__

#include "FramebufferRadialBlurGpuProgramWrapper.h"

class OpenGLFramebufferRadialBlurGpuProgram;

class OpenGLFramebufferRadialBlurGpuProgramWrapper : public FramebufferRadialBlurGpuProgramWrapper
{
public:
    OpenGLFramebufferRadialBlurGpuProgramWrapper();
    
    virtual ~OpenGLFramebufferRadialBlurGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLFramebufferRadialBlurGpuProgram* m_program;
};

#endif /* defined(__nosfuratu__OpenGLFramebufferRadialBlurGpuProgramWrapper__) */
