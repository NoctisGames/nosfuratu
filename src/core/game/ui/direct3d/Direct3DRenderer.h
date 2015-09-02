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

	virtual void clearScreenWithColor(float r, float g, float b, float a);

	virtual void beginFrame();

	virtual void endFrame();

	virtual void cleanUp();

private:
	void loadTexture(LPCWSTR szFile, ID3D11ShaderResourceView **shaderResourceView);
};

#endif /* defined(__NosFURatu__Direct3DRenderer__) */
