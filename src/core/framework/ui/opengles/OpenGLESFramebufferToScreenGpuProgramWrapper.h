//
//  OpenGLESFramebufferToScreenGpuProgramWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLESFramebufferToScreenGpuProgramWrapper__
#define __noctisgames__OpenGLESFramebufferToScreenGpuProgramWrapper__

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

#endif /* defined(__noctisgames__OpenGLESFramebufferToScreenGpuProgramWrapper__) */
