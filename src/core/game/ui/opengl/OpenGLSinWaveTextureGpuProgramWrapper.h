//
//  OpenGLSinWaveTextureGpuProgramWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLSinWaveTextureGpuProgramWrapper__
#define __noctisgames__OpenGLSinWaveTextureGpuProgramWrapper__

#include "SinWaveTextureGpuProgramWrapper.h"
#include "OpenGLSinWaveTextureProgram.h"

class OpenGLSinWaveTextureGpuProgramWrapper : public SinWaveTextureGpuProgramWrapper
{
public:
    OpenGLSinWaveTextureGpuProgramWrapper();
    
    virtual ~OpenGLSinWaveTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLSinWaveTextureProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLSinWaveTextureGpuProgramWrapper__) */
