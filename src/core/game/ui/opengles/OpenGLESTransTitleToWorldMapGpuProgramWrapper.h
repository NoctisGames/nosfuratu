//
//  OpenGLESTransTitleToWorldMapGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESTransTitleToWorldMapGpuProgramWrapper__
#define __gowengamedev__OpenGLESTransTitleToWorldMapGpuProgramWrapper__

#include "TransitionGpuProgramWrapper.h"
#include "TransitionProgram.h"

class OpenGLESTransTitleToWorldMapGpuProgramWrapper : public TransitionGpuProgramWrapper
{
public:
    OpenGLESTransTitleToWorldMapGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    TransitionProgramStruct m_program;
};

#endif /* defined(__gowengamedev__OpenGLESTransTitleToWorldMapGpuProgramWrapper__) */
