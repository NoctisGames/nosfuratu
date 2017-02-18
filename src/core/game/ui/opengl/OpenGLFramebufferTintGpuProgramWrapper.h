//
//  OpenGLFramebufferTintGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/23/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLFramebufferTintGpuProgramWrapper__
#define __nosfuratu__OpenGLFramebufferTintGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "OpenGLFramebufferToScreenProgram.h"

class OpenGLFramebufferTintGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLFramebufferTintGpuProgramWrapper();
    
    virtual ~OpenGLFramebufferTintGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLFramebufferToScreenProgramStruct m_program;
};

#endif /* defined(__nosfuratu__OpenGLFramebufferTintGpuProgramWrapper__) */
