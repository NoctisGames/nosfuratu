//
//  OpenGLTransDeathGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLTransDeathGpuProgramWrapper__
#define __nosfuratu__OpenGLTransDeathGpuProgramWrapper__

#include "TransDeathGpuProgramWrapper.h"
#include "OpenGLTransDeathProgram.h"

class OpenGLTransDeathGpuProgramWrapper : public TransDeathGpuProgramWrapper
{
public:
    OpenGLTransDeathGpuProgramWrapper(bool isTransIn);
    
    virtual ~OpenGLTransDeathGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLTransDeathProgramStruct m_program;
};

#endif /* defined(__nosfuratu__OpenGLTransDeathGpuProgramWrapper__) */
