//
//  OpenGLESShockwaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESShockwaveTextureGpuProgramWrapper__
#define __gowengamedev__OpenGLESShockwaveTextureGpuProgramWrapper__

#include "ShockwaveTextureGpuProgramWrapper.h"
#include "ShockwaveTextureProgram.h"

class OpenGLESShockwaveTextureGpuProgramWrapper : public ShockwaveTextureGpuProgramWrapper
{
public:
    OpenGLESShockwaveTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    ShockwaveTextureProgramStruct m_program;
};

#endif /* defined(__gowengamedev__OpenGLESShockwaveTextureGpuProgramWrapper__) */
