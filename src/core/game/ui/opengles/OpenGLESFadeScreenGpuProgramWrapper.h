//
//  OpenGLESFadeScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLESFadeScreenGpuProgramWrapper__
#define __noctisgames__OpenGLESFadeScreenGpuProgramWrapper__

#include "TransitionGpuProgramWrapper.h"
#include "TransitionProgram.h"

class OpenGLESFadeScreenGpuProgramWrapper : public TransitionGpuProgramWrapper
{
public:
    OpenGLESFadeScreenGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    TransitionProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLESFadeScreenGpuProgramWrapper__) */
