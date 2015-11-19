//
//  Direct3DRenderer.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/1/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __NosFURatu__Direct3DRenderer__
#define __NosFURatu__Direct3DRenderer__

#include "Renderer.h"

class Direct3DRenderer : public Renderer
{
public:
	Direct3DRenderer();

protected:
	virtual TextureWrapper* loadTexture(const char* textureName, int repeatS);

	virtual void updateMatrix(float left, float right, float bottom, float top);

	virtual void bindToOffscreenFramebuffer();

	virtual void beginFrame();

	virtual void clearFrameBufferWithColor(float r, float g, float b, float a);

	virtual void bindToScreenFramebuffer();

	virtual void endFrame();

	virtual GpuProgramWrapper& getFramebufferToScreenGpuProgramWrapper();

	virtual bool isLoaded();
    
    virtual void destroyTexture(TextureWrapper& textureWrapper);

private:
	int m_iNumTexturesLoaded;

	void loadTexture(LPCWSTR szFile, ID3D11ShaderResourceView **shaderResourceView);
};

#endif /* defined(__NosFURatu__Direct3DRenderer__) */
