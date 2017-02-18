//
//  OpenGLFramebufferToScreenGpuProgramWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLFramebufferToScreenGpuProgramWrapper__
#define __noctisgames__OpenGLFramebufferToScreenGpuProgramWrapper__

#include "GpuProgramWrapper.h"

#include "OpenGLFramebufferToScreenProgram.h"

class OpenGLFramebufferToScreenGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLFramebufferToScreenGpuProgramWrapper();
    
    virtual ~OpenGLFramebufferToScreenGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLFramebufferToScreenProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLFramebufferToScreenGpuProgramWrapper__) */
