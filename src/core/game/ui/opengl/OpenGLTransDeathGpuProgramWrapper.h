//
//  OpenGLTransDeathGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLTransDeathGpuProgramWrapper__
#define __noctisgames__OpenGLTransDeathGpuProgramWrapper__

#include "TransDeathGpuProgramWrapper.h"
#include "TransDeathProgram.h"

class OpenGLTransDeathGpuProgramWrapper : public TransDeathGpuProgramWrapper
{
public:
    OpenGLTransDeathGpuProgramWrapper(bool isTransIn);
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    TransDeathProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLTransDeathGpuProgramWrapper__) */
