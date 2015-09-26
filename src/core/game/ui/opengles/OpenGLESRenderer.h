//
//  OpenGLESRenderer.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__OpenGLESRenderer__
#define __nosfuratu__OpenGLESRenderer__

#include "Renderer.h"

class OpenGLESRenderer : public Renderer
{
public:
    OpenGLESRenderer();

	virtual void cleanUp();

protected:
    virtual void updateMatrix(float left, float right, float bottom, float top);
    
    virtual void bindToOffscreenFramebuffer();
    
    virtual void beginFrame();
    
    virtual void clearFrameBufferWithColor(float r, float g, float b, float a);
    
    virtual void bindToScreenFramebuffer();
    
    virtual void endFrame();
    
    virtual GpuProgramWrapper& getFramebufferToScreenGpuProgramWrapper();
    
private:
    TextureWrapper* loadTexture(const char* textureName);
};

#endif /* defined(__nosfuratu__OpenGLESRenderer__) */