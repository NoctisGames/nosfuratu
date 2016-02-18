//
//  OpenGLESTransScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLESTransScreenGpuProgramWrapper__
#define __noctisgames__OpenGLESTransScreenGpuProgramWrapper__

#include "TransitionGpuProgramWrapper.h"
#include "TransitionProgram.h"

class OpenGLESTransScreenGpuProgramWrapper : public TransitionGpuProgramWrapper
{
public:
    OpenGLESTransScreenGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    TransitionProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLESTransScreenGpuProgramWrapper__) */
