//
//  OpenGLESFramebufferToScreenGpuProgramWrapper.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESFramebufferToScreenGpuProgramWrapper__
#define __gowengamedev__OpenGLESFramebufferToScreenGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "FramebufferToScreenProgram.h"

class OpenGLESFramebufferToScreenGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLESFramebufferToScreenGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    FramebufferToScreenProgramStruct m_program;
};

#endif /* defined(__gowengamedev__OpenGLESFramebufferToScreenGpuProgramWrapper__) */
