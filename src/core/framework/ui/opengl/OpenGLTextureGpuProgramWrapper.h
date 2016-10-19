//
//  OpenGLTextureGpuProgramWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 8/27/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLTextureGpuProgramWrapper__
#define __noctisgames__OpenGLTextureGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "TextureProgram.h"

class OpenGLTextureGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    TextureProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLTextureGpuProgramWrapper__) */
