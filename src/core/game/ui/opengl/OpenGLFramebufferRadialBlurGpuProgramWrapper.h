//
//  OpenGLFramebufferRadialBlurGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/29/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLFramebufferRadialBlurGpuProgramWrapper__
#define __noctisgames__OpenGLFramebufferRadialBlurGpuProgramWrapper__

#include "FramebufferRadialBlurGpuProgramWrapper.h"
#include "FramebufferToScreenProgram.h"

class OpenGLFramebufferRadialBlurGpuProgramWrapper : public FramebufferRadialBlurGpuProgramWrapper
{
public:
    OpenGLFramebufferRadialBlurGpuProgramWrapper();
    
    virtual ~OpenGLFramebufferRadialBlurGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    FramebufferToScreenProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLFramebufferRadialBlurGpuProgramWrapper__) */
